// ===========================================================================
// The Player Library is Copyright (c) Grame, Computer Music Research Laboratory 
// 1996-1999, and is distributed as Open Source software under the Artistic License;
// see the file "Artistic" that is included in the distribution for details.
//
// Grame : Computer Music Research Laboratory
// Web : http://www.grame.fr/Research
// E-mail : MidiShare@rd.grame.fr
//
// modifications history:
//   [11-12-99] SL - Use of global filter access functions 
// ===========================================================================


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
							  :TEventDispatcher(successor)
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
	fIterator = new TScoreIterator(fScore);
	fRecordtrack = kNoTrack;
	fRecordmode = kEraseOff;
}

/*----------------------------------------------------------------------------*/

TEventRecorder::~TEventRecorder()
{
	if (fRecFilter) MidiFreeFilter(fRecFilter);
	delete (fIterator);
}

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
	TEventPtr cur = fIterator->SetPosTicks(date_ticks);
	Date(e) = date_ticks;
	TrackNum(e) = (unsigned char)fRecordtrack;
	
	fScore->InsertBeforeEvent(cur, TEventFactory::GenericCreateEvent(e));
}

/*--------------------------------------------------------------------------*/

void TEventRecorder::EraseTrack() 
{
	MidiSeqPtr tmp;
	
	if (IsErasing () && IsRecording() && (tmp = MidiNewSeq()))
		fScore->SetTrack(fRecordtrack,tmp);
}


