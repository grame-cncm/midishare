/*

  Copyright © Grame 2010

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
  research@grame.fr

*/

#include <CoreServices/CoreServices.h> //for file stuff
#include <AudioUnit/AudioUnit.h>
#include <AudioToolbox/AudioToolbox.h> //for AUGraph

#include "MidiShare.h"
#include "msAUDriver.h"
#include "msDriverState.h"

//#include <fstream>
//std::ofstream logfile("/Users/fober/Desktop/msAUDriver.log"); 

/* ----------------------------------*/
/* constants definitions             */
#define MidiShareDrvRef		127

#define AUSlotName "Audio Unit Synth"
#define kProfileName  "msAUDriver.ini"

/* ----------------------------------*/
/* data structures                   */
typedef struct {
	Boolean 	running;
	AUGraph		graph;
	AudioUnit	synth;	
} AUEnv, * AUEnvPtr;

typedef struct {
	short		refNum;
	TFilter		filter;
	SlotRefNum	slotRef;
	AUEnv		au;
} DriverData, * DriverDataPtr;

enum {
    kMidiMessage_ControlChange      = 0xB,
    kMidiMessage_ProgramChange      = 0xC,
    kMidiMessage_BankMSBControl     = 0,
    kMidiMessage_BankLSBControl     = 32,
    kMidiMessage_NoteOn             = 0x9
};


/* ----------------------------------*/
/* some macros                       */
#define Note(qt, i)	((qt)->chan[i].note)
#define SndNum(qt, i)	((qt)->chan[i].sndNum)
#define AU(data)	&(data->au)

/* ----------------------------------*/
/* functions declarations            */
static void PlayNote		(AUEnvPtr au, MidiEvPtr e, short vel);
static void PlayCtrlChange	(AUEnvPtr au, MidiEvPtr e);
static void PlayProgChange	(AUEnvPtr au, MidiEvPtr e);
static void PlayPitchWheel	(AUEnvPtr au, MidiEvPtr e);
static void PlayReset		(AUEnvPtr au);

static bool	CreateAUGraph (AUGraph &outGraph, AudioUnit &outSynth);
static void	SynthInit (AUEnvPtr au);
static void AUDriverStop (AUEnvPtr au);
static void FlushReceivedEvents (short r);
static void ReceiveEvents (short r);

DriverData gData;
static inline DriverDataPtr GetData ()	{ return &gData; }

/* -----------------------------------------------------------------------------*/
/* Driver required callbacks                                                    */
/* -----------------------------------------------------------------------------*/
static void WakeUp (short r)
{
	AUEnvPtr au = AU(GetData ());	  
	if (au->running) {
		MidiSetRcvAlarm (r, ReceiveEvents);
		MidiConnect (MidiShareDrvRef, r, true);
		MidiConnect (r, MidiShareDrvRef, true);
	}
	else MidiSetRcvAlarm (r, FlushReceivedEvents);
}

/* -----------------------------------------------------------------------------*/
static void Sleep (short r) {}

/* -----------------------------------------------------------------------------*/
/* MidiShare part                                                               */
/* -----------------------------------------------------------------------------*/
static void KeyOffTask (long date, short refNum, void* a1, void* a2, void* a3)
{
	PlayNote ((AUEnvPtr)a2, (MidiEvPtr)a1, 0);
	MidiFreeEv((MidiEvPtr)a1);
}

/* -----------------------------------------------------------------------------*/
static void FlushReceivedEvents (short r)
{
	MidiFlushEvs (r);
}

/* -----------------------------------------------------------------------------*/
static void ReceiveEvents (short r)
{
	AUEnvPtr au = AU(GetData ());
	MidiEvPtr e = MidiGetEv (r);

	while (e) {
		switch (EvType(e)) {
			case typeNote:
				PlayNote (au, e, Vel(e));
				MidiTask(KeyOffTask, MidiGetTime()+Dur(e), r, e, au, 0);
				goto loop;
			case typeKeyOn:		PlayNote (au, e, Vel(e));
				break;
			case typeKeyOff:	PlayNote (au, e, 0);
				break;
			case typeCtrlChange:	PlayCtrlChange (au, e);
				break;
			case typeProgChange:	PlayProgChange (au, e);
				break;
			case typePitchWheel:	PlayPitchWheel (au, e);
				break;
			case typeReset:		PlayReset (au);
				break;
			case typeRegParam:	PlayCtrlChange (au, e);
				break;
		}
		MidiFreeEv (e);
loop:
		e = MidiGetEv (r);
	}
}

/* -----------------------------------------------------------------------------*/
static void SetupFilter (MidiFilterPtr filter)
{
	short i;

	for (i = 0; i<256; i++) { 										
		MidiAcceptType(filter,i,false);
		MidiAcceptPort(filter,i,true);
	}
	for (i = 0; i<16; i++)
		MidiAcceptChan(filter,i,true);

	MidiAcceptType (filter, typeNote,true);
	MidiAcceptType (filter, typeKeyOn,true);
	MidiAcceptType (filter, typeKeyOff,true);
	MidiAcceptType (filter, typeCtrlChange,true);
	MidiAcceptType (filter, typeProgChange,true);
	MidiAcceptType (filter, typePitchWheel,true);
	MidiAcceptType (filter, typeReset,true);
	MidiAcceptType (filter, typeRegParam,true);
}

/* -----------------------------------------------------------------------------*/
Boolean SetUpMidi ()
{
	TDriverInfos infos = { AUDriverName, kAUDriverVersion, 0};
	short refNum; TDriverOperation op = { WakeUp, Sleep, 0, 0, 0 }; 
	DriverDataPtr data = GetData ();
	
 	if (MidiGetNamedAppl (AUDriverName) > 0) return true;
   	refNum = MidiRegisterDriver(&infos, &op);

	if (refNum == MIDIerrSpace) return false;
     
	data->refNum = refNum;
	data->slotRef = MidiAddSlot (refNum, AUSlotName, MidiOutputSlot);
        
	MidiSetRcvAlarm (refNum, FlushReceivedEvents);	
        	
	SetupFilter (&data->filter);
	MidiSetFilter (refNum, &data->filter);
	LoadSlot ("Output Slots", GetProfileFullName(kProfileName), AUDriverName);

//	MidiConnectSlot (0, data->slotRef, true);
        
	if (!CreateAUGraph (data->au.graph, data->au.synth)) {
		
		data->au.running = false;
		MidiUnregisterDriver(refNum);
		return false;
	}
	SynthInit (AU(data));
	data->au.running = true;
	return true;
}


/* -----------------------------------------------------------------------------*/
void CloseMidi ()
{
	DriverDataPtr data = GetData ();
	short ref = data->refNum;
	data->refNum = 0;
	if (ref > 0) {
		SaveSlot ("Output Slots", GetProfileFullName(kProfileName), AUDriverName);
		MidiUnregisterDriver (ref);
	}
        
	// Dispose QuickTime
	AUDriverStop (AU(data));
}

/* -----------------------------------------------------------------------------*/
/* Audio Unit part                                                               */
/* -----------------------------------------------------------------------------*/
static void SynthInit (AUEnvPtr au)
{
    MusicDeviceMIDIEvent (au->synth, kMidiMessage_ControlChange << 4 | 0, 
                                kMidiMessage_BankMSBControl, 0, 0/*sample offset*/);
 
    MusicDeviceMIDIEvent (au->synth, kMidiMessage_ProgramChange << 4 | 0, 
                                0/*prog change num*/, 0, 0/*sample offset*/);

	OSStatus result = AUGraphStart (au->graph);
	int count = 0;
	while (result == kAUGraphErr_CannotDoInCurrentContext) {
		result = AUGraphStart (au->graph);
		if (count++ > 10000) break;
	}
}

/* -----------------------------------------------------------------------------*/
static bool  CreateAUGraph (AUGraph &outGraph, AudioUnit &outSynth)
{
    OSStatus result;
    //create the nodes of the graph
    AUNode synthNode, limiterNode, outNode;
    
    AudioComponentDescription cd;
    cd.componentManufacturer = kAudioUnitManufacturer_Apple;
    cd.componentFlags = 0;
    cd.componentFlagsMask = 0;
 
    require_noerr (result = NewAUGraph (&outGraph), home);
 
    cd.componentType = kAudioUnitType_MusicDevice;
    cd.componentSubType = kAudioUnitSubType_DLSSynth;
 
    require_noerr (result = AUGraphAddNode (outGraph, &cd, &synthNode), home);
 
    cd.componentType = kAudioUnitType_Effect;
    cd.componentSubType = kAudioUnitSubType_PeakLimiter;  
 
    require_noerr (result = AUGraphAddNode (outGraph, &cd, &limiterNode), home);
 
    cd.componentType = kAudioUnitType_Output;
    cd.componentSubType = kAudioUnitSubType_DefaultOutput;  
    require_noerr (result = AUGraphAddNode (outGraph, &cd, &outNode), home);
    
    require_noerr (result = AUGraphOpen (outGraph), home);
    require_noerr (result = AUGraphConnectNodeInput (outGraph, synthNode, 0, limiterNode, 0), home);
    require_noerr (result = AUGraphConnectNodeInput (outGraph, limiterNode, 0, outNode, 0), home);
    
    // ok we're good to go - get the Synth Unit...
    require_noerr (result = AUGraphNodeInfo(outGraph, synthNode, 0, &outSynth), home);
    require_noerr (result = AUGraphInitialize (outGraph), home);
 
home:
    return result == 0;
}

/* -----------------------------------------------------------------------------*/
static void AUDriverStop (AUEnvPtr au)
{
	if (au->graph) {
        AUGraphStop (au->graph);	// stop playback - AUGraphDispose will do that for us but just showing you what to do
        DisposeAUGraph (au->graph);
    }
}

/* -----------------------------------------------------------------------------*/
static void PlayReset (AUEnvPtr au)
{
}

/* -----------------------------------------------------------------------------*/
static void PlayNote (AUEnvPtr au, MidiEvPtr e, short vel)
{ 
	UInt32 noteOnCommand =  kMidiMessage_NoteOn << 4 | Chan(e);
	MusicDeviceMIDIEvent (au->synth, noteOnCommand, Pitch(e), vel, 0); 
}

/* -----------------------------------------------------------------------------*/
static void PlayCtrlChange (AUEnvPtr au, MidiEvPtr e)
{ 
	MusicDeviceMIDIEvent (au->synth, kMidiMessage_ControlChange << 4 | Chan(e), 
						  MidiGetField(e, 0), MidiGetField (e, 1), 0/*sample offset*/);
}

/* -----------------------------------------------------------------------------*/
static void PlayProgChange (AUEnvPtr au, MidiEvPtr e)
{
	MusicDeviceMIDIEvent (au->synth, kMidiMessage_ProgramChange << 4 | Chan(e), MidiGetField (e, 0), 0, 0/*sample offset*/);
}

/* -----------------------------------------------------------------------------*/
static void PlayPitchWheel (AUEnvPtr au, MidiEvPtr e)
{
	MusicDeviceMIDIEvent (au->synth, kMidiMessage_ControlChange << 4 | Chan(e), 
						  MidiGetField(e, 0), MidiGetField (e, 1), 0/*sample offset*/);

//	short pitchWheel = ((MidiGetField (e, 1) << 7) + MidiGetField (e, 0)) - 8192;
//	NASetController (qt->allocator, Note(qt, Chan(e)), kControllerPitchBend, (pitchWheel * 2) >> 5);
}

/* -----------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

Boolean Start() { return SetUpMidi(); }
void Stop()		{ CloseMidi(); }

#ifdef __cplusplus
}
#endif
