/*

  Copyright © Grame 2002

  This library is free software; you can redistribute it and modify it under 
  the terms of the GNU Library General Public License as published by the 
  Free Software Foundation version 2 of the License, or any later version.

  This library is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public License 
  for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

  Grame Research Laboratory, 9, rue du Garet 69001 Lyon - France
  grame@rd.grame.fr

*/


#include <Carbon/Carbon.h>
#include <MidiShare.h>
#include <portaudio.h>
#include "pitchtracker.h"
#include "msPTDriver.h"
#include "msDriverState.h"


/* ----------------------------------*/
/* constants definitions             */
#define MidiShareDrvRef		127

#define PTSlotName "PitchTracker"
#define kProfileName  "msPTDriver.ini"

#define SAMPLE_RATE  (44100)
#define BUFFER_SIZE  2048

#define PA_SAMPLE_TYPE  paFloat32
typedef float SAMPLE;

/* ----------------------------------*/
/* data structures                   */

typedef struct {
        short			refNum;
        SlotRefNum 		slotRef;
        pitchtrackerPtr 	pitchTracker;
        PortAudioStream *	stream;
        Boolean             	connected;
	Boolean			autoQuit;
	Boolean 		sflag;
	long			tune;
	long 			buffers;
	long 			noisein;
	long 			noiseout;
	long 			bendlength;
	long 			vol;
	long 			bend;
	long			fftsize;
	long			dynamic;
        long 			old_bendlength;
	long			old_vol;  // valeur du dernier volume envoyŽ
	long			old_bend; // valeur du dernier bend envoyŽ
	long			numInput;
	long			numOutput;
	SAMPLE	     		monoTable [BUFFER_SIZE];
	
} DriverData, * DriverDataPtr;

/* ----------------------------------*/
/* functions declarations            */

DriverData gData;
static inline DriverDataPtr GetData ()	{ return &gData; }
static void AudioSleep (); 
static void AudioStart ();

/* ---------------------------------------*/
/* utilities                      	  */
/* ---------------------------------------*/
static MidiEvPtr KeyOn(short pitch,short vel)
{
  MidiEvPtr e;

  	if ((e = MidiNewEv( typeKeyOn ))!=0)  
        {
            Chan(e) = 0;      
            Port(e) = 0;
            MidiSetField(e,0,pitch);
            MidiSetField(e,1,vel);
        }
  	return e;
};

/* -----------------------------------------------------------------------------*/
static MidiEvPtr CtrlChange(short val)
{
  	MidiEvPtr e;
  	if ((e = MidiNewEv(typeCtrlChange))!=0)
        {
            Chan(e) = 0;
            Port(e) = 0;
            MidiSetField(e,0,7);
            MidiSetField(e,1,val);
        }
 	return(e);
};

/* -----------------------------------------------------------------------------*/
static MidiEvPtr PitchWheel(short wheel)
{
	const int offset = 8192;
	const int min = -8192;
	const int max = 8191;
	MidiEvPtr e;

	wheel = (wheel>max) ? max : (wheel<min) ? min : wheel;

	if ( (e = MidiNewEv( typePitchWheel )) !=NULL)    
	{
	  Date(e) = 0;        
	  Chan(e) = 0;        
	  Port(e) = 0;
	  MidiSetField(e,0,(wheel+offset) & 0x7F);    	
	  MidiSetField(e,1,(wheel+offset)>>7 & 0x7F);    
	}
	return e;
}

/* -----------------------------------------------------------------------------*/
static MidiEvPtr BendSensitivity( short val )
{
    MidiEvPtr e;

    if ( (e = MidiNewEv( typeRegParam ))!= NULL)
    {
            Chan(e) = 0;        
            Port(e) = 0;
            MidiSetField(e,0,0);    
            MidiSetField(e,1,val<<7);
    }
    return e;
}


/* -----------------------------------------------------------------------------*/
static int inputCallback(void *inputBuffer, void *outputBuffer,
						unsigned long framesPerBuffer,
				        PaTimestamp outTime, void *userData )
{
	PitchEv res;
	DriverData* data = GetData ();
	SAMPLE* in = ((SAMPLE*)inputBuffer);
        int i;	
		
	if (data->refNum > 0 && data->connected) { 
      
                if (in == NULL) return 0;
                // Transform in a mono buffer : keep the left input
		for ( i = 0; i< framesPerBuffer; i++) data->monoTable[i] = in[data->numInput*i];
                
                res = loadPitch_float(data->pitchTracker, data->monoTable);
		
		if (res.isEv == 1){
                    if (res.Off != 0) MidiSendIm(data->refNum,KeyOn(res.Off,0));
                    if (res.On != 0) MidiSendIm(data->refNum,KeyOn(res.On,res.Vol));
	        
                    if ((res.Vol != 0) && (res.Vol != data->old_vol) && data->vol) {
                	MidiSendIm(data->refNum,CtrlChange(res.Vol));
	        	data->old_vol = res.Vol;
                    }
                    if ((res.Bend != 0) && (res.Bend != data->old_bend) && data->bend) {
	        	MidiSendIm(data->refNum,PitchWheel(res.Bend));
	        	data->old_bend = res.Bend;
                    }
                }
	    
                // If bend state changed 
                if (data->bendlength != data->old_bendlength) 
                {
                    MidiSendIm(data->refNum,BendSensitivity(data->bendlength));
                    data->old_bendlength = data->bendlength;
                }
                
	}
	return 0;
}


/* -----------------------------------------------------------------------------*/
static void FlushReceivedEvents (short r){MidiFlushEvs (r);}

/* -----------------------------------------------------------------------------*/
/* Driver required callbacks                                                    */
/* -----------------------------------------------------------------------------*/
static void WakeUp (short r)
{
	MidiConnect (MidiShareDrvRef, r, true);
	MidiConnect (r, MidiShareDrvRef, true);
	AudioStart ();
}

/* -----------------------------------------------------------------------------*/
static void Sleep (short r)
{
	AudioSleep ();
}

/* -----------------------------------------------------------------------------*/
static void AudioStart () 
{
	PaError err;
	DriverDataPtr data = GetData ();
        data->connected = true;
	err = Pa_StartStream(data->stream);
	if( err != paNoError) Pa_Terminate();
}

/* -----------------------------------------------------------------------------*/
 Boolean AudioWakeUp (int buffersize) 
{
	PaError err;
	DriverDataPtr data = GetData ();
	const PaDeviceInfo* info;
	
        data->pitchTracker = createPitch(buffersize, data->fftsize);
        if (!data->pitchTracker) goto error;
      	
	err = Pa_Initialize();
	if(err != paNoError) goto error;       
        
  	/*
	info = Pa_GetDeviceInfo (Pa_GetDefaultInputDeviceID());
	data->numInput = info->maxInputChannels;
	data->numOutput = info->maxOutputChannels;
	*/
	
	data->numInput = 2;
	data->numOutput = 2;
	
	err = Pa_OpenStream(&data->stream,
				Pa_GetDefaultInputDeviceID(),
				data->numInput ,
				PA_SAMPLE_TYPE,
				NULL,
				paNoDevice,
				0,
        			PA_SAMPLE_TYPE,
				NULL,
				SAMPLE_RATE,
				buffersize,     //  frames per buffer 
				0,           	//  number of buffers, if zero then use default minimum 
				paClipOff,       
				inputCallback,
				0 );
        
	if( err != paNoError ) goto error;
	return true;
	
error:
	Pa_Terminate();
	return false; 
}

/* -----------------------------------------------------------------------------*/
static void AudioSleep () 
{
 	DriverDataPtr data = GetData ();
	Pa_CloseStream(data->stream);
	Pa_Terminate();
	deletePitch(data->pitchTracker);
}  

/* -----------------------------------------------------------------------------*/
void LoadState()
{
        DriverDataPtr data = GetData ();
        
        data->fftsize = LoadConfigNum("Configuration", "FFT", GetProfileFullName(kProfileName),512);
        data->tune = LoadConfigNum("Configuration", "Tune", GetProfileFullName(kProfileName),440);
        data->buffers = LoadConfigNum("Configuration", "Buffers", GetProfileFullName(kProfileName),5);
        data->noisein = LoadConfigNum("Configuration", "LevelIn", GetProfileFullName(kProfileName),30);
        data->noiseout = LoadConfigNum("Configuration", "LevelOut", GetProfileFullName(kProfileName),20);
        data->vol = LoadConfigNum("Configuration", "Vol", GetProfileFullName(kProfileName),1);
        data->bend = LoadConfigNum("Configuration", "Bend", GetProfileFullName(kProfileName),1);
        data->bendlength = LoadConfigNum("Configuration", "BendLength", GetProfileFullName(kProfileName),0);
        data->dynamic = LoadConfigNum("Configuration", "Dynamic", GetProfileFullName(kProfileName),50);
}

/* -----------------------------------------------------------------------------*/
Boolean SetUpMidi ()
{
	TDriverInfos infos = { PTDriverName, kPTDriverVersion, 0};
	TDriverOperation op = { WakeUp, Sleep, 0, 0, 0 }; 
	DriverDataPtr data = GetData ();
	short refNum; 
	
	if (MidiGetNamedAppl (PTDriverName) > 0) return true;
	
	data->refNum = -1;
       
	data->connected = false;
	data->old_bendlength = 0;
	data->old_vol = 0;
	data->old_bend = 0;
	
	refNum = MidiRegisterDriver(&infos, &op);
	if (refNum == MIDIerrSpace) return false;
        data->slotRef = MidiAddSlot (refNum, PTSlotName, MidiInputSlot);
     
	MidiSetRcvAlarm (refNum, FlushReceivedEvents);	
        LoadSlot ("Input Slots", GetProfileFullName(kProfileName),PTDriverName);
            
        setFieldPitch(data->pitchTracker,NB_NOTE,data->buffers);
        setFieldPitch(data->pitchTracker,TUNE,data->tune);
        setFieldPitch(data->pitchTracker,TRESH_IN,data->noisein);
        setFieldPitch(data->pitchTracker,TRESH_OUT,data->noiseout);
        setFieldPitch(data->pitchTracker,LENGTH_BEND,data->bendlength);
        setFieldPitch(data->pitchTracker,VOLUME,data->dynamic);
 
        //MidiSetApplAlarm (refNum, NewApplAlarmPtr(ApplAlarm));	
       
	data->refNum = refNum;
    	MidiSendIm(data->refNum,BendSensitivity(data->bendlength));
	
	return true;
}

/* -----------------------------------------------------------------------------*/
void CloseMidi ()
{	
	DriverDataPtr data = GetData ();
	short ref = data->refNum;
	data->refNum = 0;
	if (ref > 0) {
                SaveSlot ("Input Slots", GetProfileFullName(kProfileName),PTDriverName);
                MidiUnregisterDriver (ref);
        }
}

/* -----------------------------------------------------------------------------*/
Boolean Start() 
{
    int buffersize = LoadBufferSize();
    LoadState();
   
    if (!AudioWakeUp(buffersize)) {
        printf("PitchTracker AudioWakeUp failed\n");
        return false;
    }
    
    if (!SetUpMidi()) {
        printf("PitchTracker MidiSetup failed\n");
        return false;
    }
    
    return true;
}

/* -----------------------------------------------------------------------------*/
void Stop() {CloseMidi(); }

