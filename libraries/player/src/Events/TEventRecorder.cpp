/*

  Copyright © Grame 1996-2004

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


// ===========================================================================
//	TEventRecorder.cpp			    
// ===========================================================================
//
//	Recording management
// 

#include "TEventRecorder.h"
#include "TEventFactory.h"
#include <assert.h>

/*----------------------------------------------------------------------------*/

TEventRecorder::TEventRecorder(TPlayerScorePtr score, 
							   TPlayerSynchroniserPtr synchro,
							   TRunningStatePtr state,
							   TEventDispatcherPtr successor)
							  :TEventDispatcher(successor),fIterator(score)
{
	short i;
	
	fRecFilter = MidiNewFilter();
	assert(fRecFilter);
	
	for (i = 0; i<256; i++) {
		MidiAcceptType(fRecFilter,i,true);
		MidiAcceptPort(fRecFilter,i,true);
	}

	for (i = 0; i<16; i++) {
		MidiAcceptChan(fRecFilter,i,true);
	}
	
	// Configuration of the record filter 
	
	MidiAcceptType(fRecFilter,typeActiveSens,false);
	MidiAcceptType(fRecFilter,typeQuarterFrame,false);
	MidiAcceptType(fRecFilter,typeTempo,false);
	MidiAcceptType(fRecFilter,typeTimeSign,false);
	MidiAcceptType(fRecFilter,typeClock,false);
	MidiAcceptType(fRecFilter,typeStart,false);
	MidiAcceptType(fRecFilter,typeStop,false);
	MidiAcceptType(fRecFilter,typeContinue,false);
	MidiAcceptType(fRecFilter,typeSongPos,false);
	
	fSynchroniser = synchro;
	fScore = score;
	fState = state;
	fRecordtrack = kNoTrack;
	fRecordmode = kEraseOff;
}

/*----------------------------------------------------------------------------*/

TEventRecorder::~TEventRecorder(){if (fRecFilter) MidiFreeFilter(fRecFilter);}

/*--------------------------------------------------------------------------*/

void TEventRecorder::SetRecordFilter(MidiFilterPtr filter) 
{ 
	if (fRecFilter) MidiFreeFilter(fRecFilter);
	fRecFilter = filter; 
}

/*----------------------------------------------------------------------------*/

void TEventRecorder::ReceiveDefaultEvent(MidiEvPtr e)
{
	if (fState->IsRunning() 
		&& IsRecording()
		&& AcceptEv(e)){
		Insert(e);
	}else
		MidiFreeEv(e);
}

/*----------------------------------------------------------------------------*/

void TEventRecorder::ReceiveEvents(MidiEvPtr e)
{
	switch (EvType(e)) {
	
		case typeScoreEnd:
			if (IsRecording()) { 
				MidiFreeEv(e); // if recording don't pass the event to the EventReceiver
			}else{
				TEventDispatcher::ReceiveEvents(e);
			}
			break;
			
		default:
			if (IsRecording() && AcceptEv(e)) Insert(MidiCopyEv(e));
			TEventDispatcher::ReceiveEvents(e);
			break;
	}
}

/*--------------------------------------------------------------------------*/

void TEventRecorder::SetRecordTrack (short tracknum) 
{
	fRecordtrack = tracknum;
	EraseTrack();
}

/*--------------------------------------------------------------------------*/

void TEventRecorder::SetRecordMode (short state) 
{ 
	fRecordmode = state;
	EraseTrack();
}

/*--------------------------------------------------------------------------*/

void TEventRecorder::Insert(MidiEvPtr e) 
{
	// Set it's date to the current Tick date, and it's tracknum to the current recording tracknumber
	
	ULONG date_ticks = fSynchroniser->ConvertMsToTick(Date(e));
	TEventPtr cur = fIterator.SetPosTicks(date_ticks);
	Date(e) = date_ticks;
	TrackNum(e) = (unsigned char)fRecordtrack;
	
	// If there is an event at the insertion date, insert after
	if (date_ticks == fIterator.CurDate()) {
		fScore->InsertAfterEvent(cur, TEventFactory::GenericCreateEvent(e));
	// otherwise insert before the next event which date is > date_ticks
	}else {
		fScore->InsertBeforeEvent(cur, TEventFactory::GenericCreateEvent(e));
	}
}

/*--------------------------------------------------------------------------*/

void TEventRecorder::EraseTrack() 
{
	MidiSeqPtr tmp;
	
	if (IsErasing () && IsRecording() && (tmp = MidiNewSeq()))
		fScore->SetTrack(fRecordtrack,tmp);
}


