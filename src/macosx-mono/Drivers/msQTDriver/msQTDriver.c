/*

  Copyright � Grame 2002

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
#include <QuickTime/QuickTimeMusic.h>

#include "MidiShare.h"
#include "msQTDriver.h"
#include "msDriverState.h"

/* ----------------------------------*/
/* constants definitions             */
#define MidiShareDrvRef		127
#define kChansCount 		16
#define kDrumKit 		16385
#define kDefaultSound 		1		 /* default sound (Piano) */

#define QTSlotName "QuickTime GM Synth"
#define kProfileName  "msQTDriver.ini"

/* ----------------------------------*/
/* data structures                   */
typedef struct {
	NoteChannel	note;
	short		sndNum;
} MidiChan;

typedef struct {
	Boolean 	running;
	NoteAllocator	allocator;
	MidiChan	chan[kChansCount];	
} QuickTimeEnv, * QuickTimeEnvPtr;

typedef struct {
	short		refNum;
	TFilter		filter;
	SlotRefNum	slotRef;
	QuickTimeEnv	qt;
} DriverData, * DriverDataPtr;

/* ----------------------------------*/
/* some macros                       */
#define Note(qt, i)	((qt)->chan[i].note)
#define SndNum(qt, i)	((qt)->chan[i].sndNum)
#define QTE(data)	&(data)->qt

/* ----------------------------------*/
/* functions declarations            */
static inline void PlayNote (QuickTimeEnvPtr qt, MidiEvPtr e, short vel)
{ NAPlayNote (qt->allocator, Note(qt, Chan(e)), Pitch(e), vel); }
static inline void PlayCtrlChange (QuickTimeEnvPtr qt, MidiEvPtr e)
{ NASetController(qt->allocator, Note(qt, Chan(e)), MidiGetField(e, 0), (MidiGetField (e, 1) << 8 )); }

static void PlayProgChange (QuickTimeEnvPtr qt, MidiEvPtr e);
static void PlayPitchWheel (QuickTimeEnvPtr qt, MidiEvPtr e);
static void PlayReset (QuickTimeEnvPtr qt);
static void QuickTimeInit (QuickTimeEnvPtr qt);
static void QuickTimeDispose (QuickTimeEnvPtr qt);
static Boolean QuickTimeWakeup (QuickTimeEnvPtr qt);
static Boolean SndChanInit (QuickTimeEnvPtr qt, short chan, short sound);
static void FlushReceivedEvents (short r);
static void ReceiveEvents (short r);

DriverData gData;
static inline DriverDataPtr GetData ()	{ return &gData; }

/* -----------------------------------------------------------------------------*/
/* Driver required callbacks                                                    */
/* -----------------------------------------------------------------------------*/
static void WakeUp (short r)
{
	QuickTimeEnvPtr qt = QTE(GetData ());
	  
	if (qt->running) {
		MidiSetRcvAlarm (r, ReceiveEvents);
		MidiConnect (MidiShareDrvRef, r, true);
		MidiConnect (r, MidiShareDrvRef, true);
	}else
		MidiSetRcvAlarm (r, FlushReceivedEvents);
}

/* -----------------------------------------------------------------------------*/
static void Sleep (short r) {}

/* -----------------------------------------------------------------------------*/
/* MidiShare part                                                               */
/* -----------------------------------------------------------------------------*/
static void KeyOffTask (long date, short refNum, long a1,long a2,long a3)
{
	PlayNote ((QuickTimeEnvPtr)a2, (MidiEvPtr)a1, 0);
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
	QuickTimeEnvPtr qt = QTE(GetData ());
	MidiEvPtr e = MidiGetEv (r);

	while (e) {
		switch (EvType(e)) {
			case typeNote:
				PlayNote (qt, e, Vel(e));
				MidiTask(KeyOffTask, MidiGetTime()+Dur(e), r, (long)e, (long)qt,0);
				goto loop;
			case typeKeyOn:		PlayNote (qt, e, Vel(e));
				break;
			case typeKeyOff:	PlayNote (qt, e, 0);
				break;
			case typeCtrlChange:	PlayCtrlChange (qt, e);
				break;
			case typeProgChange:	PlayProgChange (qt, e);
				break;
			case typePitchWheel:	PlayPitchWheel (qt, e);
				break;
			case typeReset:		PlayReset (qt);
				break;
			case typeRegParam:	PlayCtrlChange (qt, e);
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
	TDriverInfos infos = { QTDriverName, kQTDriverVersion, 0};
	short refNum; TDriverOperation op = { WakeUp, Sleep, 0, 0, 0 }; 
	DriverDataPtr data = GetData ();
        
  	if (MidiGetNamedAppl (QTDriverName) > 0) return true;
   	refNum = MidiRegisterDriver(&infos, &op);
      
	if (refNum == MIDIerrSpace) return false;
     
	data->refNum = refNum;
	data->slotRef = MidiAddSlot (refNum, QTSlotName, MidiOutputSlot);
        
	MidiSetRcvAlarm (refNum, FlushReceivedEvents);	
        	
	SetupFilter (&data->filter);
	MidiSetFilter (refNum, &data->filter);
	LoadSlot ("Output Slots", GetProfileFullName(kProfileName),QTDriverName);
        
	// Init QuickTime
	QuickTimeInit (QTE(data));
	if (!QuickTimeWakeup (QTE(data))) {
		MidiUnregisterDriver(refNum);
		return false;
	}else
		return true;
}

/* -----------------------------------------------------------------------------*/
void CloseMidi ()
{
	DriverDataPtr data = GetData ();
	short ref = data->refNum;
	data->refNum = 0;
	if (ref > 0) {
		SaveSlot ("Output Slots", GetProfileFullName(kProfileName),QTDriverName);
		MidiUnregisterDriver (ref);
	}
        
	// Dispose QuickTime
	QuickTimeDispose (QTE(data));
}

/* -----------------------------------------------------------------------------*/
/* QuickTime part                                                               */
/* -----------------------------------------------------------------------------*/
Boolean CheckQuickTime ()
{
	long reply;
	OSErr err = Gestalt (gestaltQuickTimeVersion, &reply);
	if ((err == noErr) && reply) {
		return true;
	}
	return false;
}

/* -----------------------------------------------------------------------------*/
static Boolean QuickTimeWakeup (QuickTimeEnvPtr qt)
{
	short i;
 
	qt->allocator = OpenDefaultComponent (kNoteAllocatorComponentType, 0);
	if (!qt->allocator)
 		goto err;
       

	for (i = 0; i < kChansCount; i++) {
		if (!SndChanInit (qt, i, SndNum(qt, i)))
			goto err;
	}
	qt->running = true;
	return true;
	
err:
	QuickTimeDispose (qt);
	qt->running = false;
	return false;	
}

/* -----------------------------------------------------------------------------*/
static Boolean SndChanInit (QuickTimeEnvPtr qt, short chan, short sound)
{
	ComponentResult err; NoteRequest nr;
        
	nr.info.flags = 0;
	nr.info.polyphony = 8;
	nr.info.typicalPolyphony = 0x00010000;
	err = NAStuffToneDescription (qt->allocator, sound, &nr.tone);
    
	if (err == noErr) {
		err = NANewNoteChannel (qt->allocator, &nr, &Note(qt, chan));
   	}
	return (err == noErr) && Note(qt, chan);
}

/* -----------------------------------------------------------------------------*/
static void QuickTimeInit (QuickTimeEnvPtr qt)
{
	short i;	
	qt->allocator = 0;
	for (i = 0; i < kChansCount; i++) {
		Note(qt, i) = 0;
		SndNum(qt, i) = (i == 9) ? kDrumKit : kDefaultSound;
	}
}

/* -----------------------------------------------------------------------------*/
static void QuickTimeDispose (QuickTimeEnvPtr qt)
{
	short i;	
	if (!qt->allocator) return;
	for (i = 0; i < kChansCount; i++) {
		if (Note(qt, i)) {
			NADisposeNoteChannel (qt->allocator, Note(qt, i));
			Note(qt, i) = 0;
		}
	}
	CloseComponent (qt->allocator);
	qt->allocator = 0;
	qt->running = false;
}

/* -----------------------------------------------------------------------------*/
static void PlayReset (QuickTimeEnvPtr qt)
{
	short i;
	for (i = 0; i < kChansCount; i++) {
		short sndNum = (i == 9) ? kDrumKit : kDefaultSound;
		NAResetNoteChannel (qt->allocator, Note(qt, i));
		NASetInstrumentNumberInterruptSafe (qt->allocator, Note(qt, i), sndNum);
		SndNum(qt, i) = sndNum;		
	}
}

/* -----------------------------------------------------------------------------*/
static void PlayProgChange (QuickTimeEnvPtr qt, MidiEvPtr e)
{
	if (Chan(e) != 9) {
		short sndNum = MidiGetField (e, 0) + 1; // MidiShare ProgChange go from 0 to 127, QuikTime from 1 to 128
		NASetInstrumentNumberInterruptSafe (qt->allocator, Note(qt, Chan(e)), sndNum);
		qt->chan[Chan(e)].sndNum = sndNum;
		NATask (qt->allocator);
	}
}

/* -----------------------------------------------------------------------------*/
static void PlayPitchWheel (QuickTimeEnvPtr qt, MidiEvPtr e)
{
	short pitchWheel = ((MidiGetField (e, 1) << 7) + MidiGetField (e, 0)) - 8192;
	NASetController (qt->allocator, Note(qt, Chan(e)), kControllerPitchBend, (pitchWheel * 2) >> 5);
}

/* -----------------------------------------------------------------------------*/
Boolean Start(){return SetUpMidi();}
void Stop(){CloseMidi();}
