/*

  Copyright © Grame 2001

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


#include <Gestalt.h>

#include <MidiShare.h>
#include <portaudio.h>
#include "pitchtracker.h"
#include "msPTDriver.h"
#include "SavingState.h"


/* ----------------------------------*/
/* constants definitions             */
#define MidiShareDrvRef		127

#define PTSlotName	"\pPitchTracker"
#define StateFile	"\pmsPTDriver State"	
enum { StateCreator = 'MsPT', StateType = 'stPT' };	

#define SAMPLE_RATE  (44100)
#define BUFFER_SIZE  512

#define PA_SAMPLE_TYPE  paInt16
typedef short SAMPLE;

/* ----------------------------------*/
/* data structures                   */

typedef struct {
	short				refNum;
	SlotRefNum			slotRef;
	pitchtrackerPtr 	pitchTracker;
	PortAudioStream *	stream;
	Boolean	*			autoQuit;
	Boolean	*			sflag;
	Boolean             connected;
	long*				tune;
	long*				buffers;
	long* 				noisein;
	long* 				noiseout;
	long* 				bendlength;
	long*				vol;
	long*				bend;
	long*				fftsize;
	long*				dynamic;
	long 				old_bendlength;
	long				old_vol;  // valeur du dernier volume envoyŽ
	long				old_bend; // valeur du dernier bend envoyŽ
	short	   		 	monoTable [BUFFER_SIZE];
} DriverData, * DriverDataPtr;

/* ----------------------------------*/
/* some macros                       */

/* ----------------------------------*/
/* functions declarations            */

DriverData gData;
static inline DriverDataPtr GetData ()	{ return &gData; }
static void AudioSleep (); 
static void AudioStart ();


/* ----------------------------------*/
/* utilities                      	 */
/* -----------------------------------------------------------------------------*/
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
	DriverData* data =  GetData ();
	int i;	
	SAMPLE* in = ((SAMPLE*)inputBuffer);
		
	if (data->refNum > 0 && data->connected) { 
	
		// Transform in a mono buffer : keep the left input
		for ( i = 0; i< framesPerBuffer; i++) data->monoTable[i] = in[2*i];
	
		// Update parameters
		if (*data->sflag) {
			*data->sflag = false;
			setFieldPitch(data->pitchTracker,NB_NOTE,*data->buffers);
			setFieldPitch(data->pitchTracker,TUNE,*data->tune);
			setFieldPitch(data->pitchTracker,TRESH_IN,*data->noisein);
			setFieldPitch(data->pitchTracker,TRESH_OUT,*data->noiseout);
			setFieldPitch(data->pitchTracker,LENGTH_BEND,*data->bendlength);
			setFieldPitch(data->pitchTracker,VOLUME,*data->dynamic);
			setFieldPitch(data->pitchTracker,SIZE_FFT,*data->fftsize);
			resetPitch(data->pitchTracker);
		}
		
		res = loadPitch(data->pitchTracker, data->monoTable);
		
		if (res.isEv == 1)
		{
	        if (res.Off != 0) MidiSendIm(data->refNum,KeyOn(res.Off,0));
	        if (res.On != 0) MidiSendIm(data->refNum,KeyOn(res.On,res.Vol));
	        
	        if ((res.Vol != 0) && (res.Vol != data->old_vol) && *data->vol) {
	        	MidiSendIm(data->refNum,CtrlChange(res.Vol));
	        	data->old_vol = res.Vol;
	        }
	        if ((res.Bend != 0) && (res.Bend != data->old_bend) && *data->bend) {
	        	MidiSendIm(data->refNum,PitchWheel(res.Bend));
	        	data->old_bend = res.Bend;
	        }
	    }
	    
	    // If bend state changed 
	    if (*data->bendlength != data->old_bendlength) 
	    {
	    	MidiSendIm(data->refNum,BendSensitivity(*data->bendlength));
	    	data->old_bendlength = *data->bendlength;
	    }
	}
	return 0;
}

/* -----------------------------------------------------------------------------*/
static pascal void ApplAlarm (long code)
{
	DriverData* data =  GetData ();
	
	MidiEvPtr tempo = MidiNewEv(typeTempo);
	Tempo(tempo) = code;
	MidiSendIm(data->refNum,tempo);
	
}

/* -----------------------------------------------------------------------------*/
static pascal void FlushReceivedEvents (short r){MidiFlushEvs (r);}


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
	DriverDataPtr data = GetData ();

	AudioSleep ();
	
	if (*data->autoQuit) {	
		CloseMidi ();
		doneFlag = true;
	}
}

/* -----------------------------------------------------------------------------*/
static void AudioStart () 
{
	PaError err;
	DriverDataPtr data = GetData ();
	err = Pa_StartStream( data->stream );
	if( err != paNoError) Pa_Terminate();
}


/* -----------------------------------------------------------------------------*/
 Boolean AudioWakeUp () 
{
	PaError err;
	DriverDataPtr data = GetData ();
	
	data->pitchTracker = createPitch(BUFFER_SIZE, *data->fftsize);
		
	err = Pa_Initialize();
	if( err != paNoError ) goto error;       
	
	err = Pa_OpenStream(
				&data->stream,
				Pa_GetDefaultInputDeviceID(),
				2,               // stereo input 
				PA_SAMPLE_TYPE,
				NULL,
				paNoDevice,
				0,
				PA_SAMPLE_TYPE,
				NULL,
				SAMPLE_RATE,
				BUFFER_SIZE,     //  frames per buffer 
				0,               //  number of buffers, if zero then use default minimum 
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
Boolean SetUpMidi (StatePtr state)
{
	TDriverInfos infos = { PTDriverName, 100, 0};
	TDriverOperation op = { 0, 0, 0, 0, 0 }; 
	UPPDriverPtr UPPDriverWakeUp = NewDriverPtr(WakeUp);
	UPPDriverPtr UPPDriverSleep = NewDriverPtr(Sleep);
	DriverDataPtr data = GetData ();
	short refNum; 
		
	op.wakeup = (DriverAlarmPtr)UPPDriverWakeUp;
	op.sleep = (DriverAlarmPtr)UPPDriverSleep;
	
	if (MidiGetNamedAppl (PTDriverName) > 0) {
		doneFlag = true;
		return true;
	}
	
	data->refNum = -1;
	data->autoQuit = &state->autoQuit;
	data->tune = &state->tune;
	data->buffers = &state->buffers;
	data->noisein = &state->noisein;
	data->noiseout = &state->noiseout;
	data->bendlength = &state->bendlength;
	data->vol = &state->vol;
	data->bend = &state->bend;
	data->fftsize = &state->fftsize;
	data->dynamic = &state->dynamic;
	data->sflag = &state->sflag;
	data->connected = true;
	data->old_bendlength = 0;
	data->old_vol = 0;
	data->old_bend = 0;
	
	refNum = MidiRegisterDriver(&infos, &op);
	if (refNum == MIDIerrSpace) return false;
	
	data->slotRef = MidiAddSlot (refNum, PTSlotName, MidiInputSlot);
	MidiSetRcvAlarm (refNum, NewRcvAlarmPtr(FlushReceivedEvents));	
	//MidiSetApplAlarm (refNum, NewApplAlarmPtr(ApplAlarm));	
	RestoreDriverState (refNum, StateFile);
	data->refNum = refNum;
	MidiSendIm(data->refNum,BendSensitivity(*data->bendlength));
	
	return true;
	
}

/* -----------------------------------------------------------------------------*/
void CloseMidi ()
{	
	DriverDataPtr data = GetData ();
	short ref = data->refNum;
	data->refNum = 0;
	if (ref > 0) {
		SaveDriverState (ref, StateFile, StateCreator, StateType);
		MidiUnregisterDriver (ref);
	}
}

/* -----------------------------------------------------------------------------*/
void DoIdle()
{
	short ref = GetData()->refNum;
	short n = MidiCountDTasks (ref);
	while (n--) { MidiExec1DTask (ref); }
}

