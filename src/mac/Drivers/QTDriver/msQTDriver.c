/*

  Copyright © Grame 2000

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


#include <QuickTimeMusic.h>
#include <Gestalt.h>

#include "MidiShare.h"
#include "msQTDriver.h"
#include "SavingState.h"

/* ----------------------------------*/
/* constants definitions             */
#define MidiShareDrvRef		127
#define kChansCount 		16
#define kDrumKit 			16385
#define kDefaultSound 		1		 /* default sound (Piano) */

#define StateFile	"\pmsQuicktime Driver State"	
enum { StateCreator = 'MsQT', StateType = 'stQT' };	

/* ----------------------------------*/
/* data structures                   */
typedef struct {
	NoteChannel		note;
	short			sndNum;
} MidiChan;

typedef struct {
	Boolean 		clean;
	NoteAllocator	allocator;
	MidiChan		chan[kChansCount];	
} QuickTimeEnv, * QuickTimeEnvPtr;

typedef struct {
	short		refNum;
	TFilter		filter;
	SlotRefNum	slotRef;
	QuickTimeEnv	qt;
} DriverData, * DriverDataPtr;

/* ----------------------------------*/
/* some macros                       */
#define Note(qt, i)		((qt)->chan[i].note)
#define SndNum(qt, i)	((qt)->chan[i].sndNum)
#define QTE(data)		&(data)->qt

/* ----------------------------------*/
/* functions declarations            */
static inline void PlayNote (QuickTimeEnvPtr qt, MidiEvPtr e, short vel)
			{ NAPlayNote (qt->allocator, Note(qt, Chan(e)), Pitch(e), vel); }
static inline void PlayCtrlChange (QuickTimeEnvPtr qt, MidiEvPtr e)
			{ NASetController(qt->allocator, Note(qt, Chan(e)), MidiGetField(e, 0), 
			  (MidiGetField (e, 1) << 8 )); }

static void PlayProgChange (QuickTimeEnvPtr qt, MidiEvPtr e);
static void PlayPitchWheel (QuickTimeEnvPtr qt, MidiEvPtr e);
static void PlayReset (QuickTimeEnvPtr qt);
static void QuickTimeInit (QuickTimeEnvPtr qt);
static void QuickTimeDispose (QuickTimeEnvPtr qt);
static Boolean QuickTimeWakeup (QuickTimeEnvPtr qt);
static Boolean SndChanInit (QuickTimeEnvPtr qt, short chan, short sound);

DriverData gData;
static inline DriverDataPtr GetData ()	{ return &gData; }


/* -----------------------------------------------------------------------------*/
/* Driver required callbacks                                                    */
/* -----------------------------------------------------------------------------*/
static void WakeUp (short r)
{
	DriverDataPtr data = GetData ();
	QuickTimeInit (QTE(data));
	if (QuickTimeWakeup (QTE(data))) {
		MidiConnect (MidiShareDrvRef, r, true);
		MidiConnect (r, MidiShareDrvRef, true);
	}
}

/* -----------------------------------------------------------------------------*/
static void Sleep (short r)
{
#ifdef __BackgroundOnly__
	CloseMidi ();
	doneFlag = true;
#else
	DriverDataPtr data = GetData ();
	QuickTimeDispose (QTE(data));
#endif
}

/* -----------------------------------------------------------------------------*/
static void PStrCpy (unsigned char *src, unsigned char * dst)
{
	short i = src[0] + 1;
	while (i--)
		*dst++ = *src++;
}

/* -----------------------------------------------------------------------------*/
static Boolean SlotInfo (SlotRefNum slot, TSlotInfos * infos)
{
	Str32 name = "\pQuickTime GM Synth";
	DriverDataPtr data = GetData ();
	if (infos && (slot == data->slotRef)) {
		PStrCpy (name, infos->name);
		infos->direction = MidiOutputSlot;
		return true;
	}
	return false;
}

/* -----------------------------------------------------------------------------*/
/* MidiShare part                                                               */
/* -----------------------------------------------------------------------------*/
static pascal void KeyOffTask (long date, short refNum, long a1,long a2,long a3)
{
	PlayNote ((QuickTimeEnvPtr)a2, (MidiEvPtr)a1, 0);
}

/* -----------------------------------------------------------------------------*/
static pascal void ReceiveEvents (short r)
{
	QuickTimeEnvPtr qt = QTE(GetData ());
	MidiEvPtr copy, e = MidiGetEv(r);

	if (!qt->clean) {
		MidiFlushEvs (r);
	}
	else while (e) {
		switch (EvType(e)) {
			case typeNote:
				copy = MidiCopyEv (e);
				if (copy && MidiTask(KeyOffTask, MidiGetTime()+Dur(e), r, (long)copy, (long)qt,0))
					PlayNote (qt, e, Vel(e));
				break;
			case typeKeyOn:			PlayNote (qt, e, Vel(e));
				break;
			case typeKeyOff:		PlayNote (qt, e, 0);
				break;
			case typeCtrlChange:	PlayCtrlChange (qt, e);
				break;
			case typeProgChange:	PlayProgChange (qt, e);
				break;
			case typePitchWheel:	PlayPitchWheel (qt, e);
				break;
		}
		if (EvType(e) == typeReset) PlayReset (qt);
		MidiFreeEv (e);
		e = MidiGetEv (r);
	}
}

/* -----------------------------------------------------------------------------*/
static void SetupFilter (MidiFilterPtr filter)
{
	short i;

	for (i = 0; i<256; i++) { 										
		RejectBit(filter->evType,i);
		AcceptBit(filter->port,i);
	}
	for (i = 0; i<16; i++)
		AcceptBit(filter->channel,i);

	AcceptBit (filter->evType, typeNote);
	AcceptBit (filter->evType, typeKeyOn);
	AcceptBit (filter->evType, typeKeyOff);
	AcceptBit (filter->evType, typeCtrlChange);
	AcceptBit (filter->evType, typeProgChange);
	AcceptBit (filter->evType, typePitchWheel);
	AcceptBit (filter->evType, typeReset);
}

/* -----------------------------------------------------------------------------*/
Boolean SetUpMidi ()
{
	TDriverInfos infos = { QTDriverName, 100, 0};
	short refNum; TDriverOperation op; 
	DriverDataPtr data = GetData ();
	
	op.wakeup = WakeUp;
	op.sleep = Sleep;
	op.slotInfo = SlotInfo; 

	if (MidiGetNamedAppl (QTDriverName) > 0)  // still running
		return true;

	refNum = MidiRegisterDriver(&infos, &op);
	if (refNum == MIDIerrSpace)
		return false;
	
	data->refNum = refNum;
	data->slotRef = MidiAddSlot (refNum);
	MidiSetRcvAlarm (refNum, ReceiveEvents);		
	SetupFilter (&data->filter);
	MidiSetFilter (refNum, &data->filter);
	RestoreDriverState (refNum, StateFile);
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
	QuickTimeDispose (QTE(data));
}

/* -----------------------------------------------------------------------------*/
void DoIdle()
{
	short ref = GetData()->refNum;
	short n = MidiCountDTasks (ref);
	while (n--) {
		MidiExec1DTask (ref);
	}
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
	qt->clean = true;
	return true;
	
err:
	QuickTimeDispose (qt);
	return false;	
}

/* -----------------------------------------------------------------------------*/
static Boolean SndChanInit (QuickTimeEnvPtr qt, short chan, short sound)
{
	ComponentResult err; NoteRequest nr;
	
	nr.info.flags = 0;
	nr.info.reserved = 0;
	nr.info.polyphony = 16;
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
	qt->clean = false;
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
	qt->clean = false;
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
static pascal void QTTask (long date, short refNum, long a1,long a2,long a3)
{
	NATask ((NoteAllocator)a1);
}

/* -----------------------------------------------------------------------------*/
static void PlayProgChange (QuickTimeEnvPtr qt, MidiEvPtr e)
{
	if (Chan(e) != 9) {
		short sndNum = MidiGetField (e, 0);
		NASetInstrumentNumberInterruptSafe (qt->allocator, Note(qt, Chan(e)), sndNum);
		qt->chan[Chan(e)].sndNum = sndNum;
		MidiDTask (QTTask, MidiGetTime(), GetData()->refNum, (long)qt->allocator, 0, 0);
	}
}

/* -----------------------------------------------------------------------------*/
static void PlayPitchWheel (QuickTimeEnvPtr qt, MidiEvPtr e)
{
	short pitchWheel = ((MidiGetField (e, 1) << 7) + MidiGetField (e, 0)) - 8192;
	NASetController (qt->allocator, Note(qt, Chan(e)), kControllerPitchBend, (pitchWheel * 2) >> 5);
}
