// ===========================================================================
// The Player Library is Copyright (c) Grame, Computer Music Research Laboratory 
// 1996-1999, and is distributed as Open Source software under the Artistic License;
// see the file "Artistic" that is included in the distribution for details.
//
// Grame : Computer Music Research Laboratory
// Web : http://www.grame.fr/Research
// E-mail : MidiShare@rd.grame.fr
// ===========================================================================


// ===========================================================================
//	TEventRecorder.cpp			    
// ===========================================================================
//
//	Recording management
// 

#include "TEventRecorder.h"
#include "TEventFactory.h"

/*----------------------------------------------------------------------------*/

TEventRecorder::TEventRecorder(TPlayerScorePtr score, 
							   TPlayerSynchroniserPtr synchro,
							   TRunningStatePtr state,
							   TEventDispatcherPtr successor)
							  :TEventDispatcher(successor)
{
	fRecFilter = MidiNewFilter();
	
	if (fRecFilter) {
		short i;
		
		for (i = 0; i<256; i++) {
			MidiAcceptType(fRecFilter,i,true);
			MidiAcceptPort(fRecFilter,i,true);
		}
	
		for (i = 0; i<16; i++) {
			MidiAcceptChan(fRecFilter,i,true);
		}
		
		// Configuration of the record filter 
				
		RejectBit(fRecFilter->evType,typeActiveSens);
		RejectBit(fRecFilter->evType,typeQuarterFrame);
		RejectBit(fRecFilter->evType,typeTempo);
		RejectBit(fRecFilter->evType,typeTimeSign);
		RejectBit(fRecFilter->evType,typeClock); 
		RejectBit(fRecFilter->evType,typeStart); 
		RejectBit(fRecFilter->evType,typeStop); 
		RejectBit(fRecFilter->evType,typeContinue); 
		RejectBit(fRecFilter->evType,typeSongPos); 
	}
	
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

void TEventRecorder::SetRecordFilter(FilterPtr filter) 
{ 
	short i;
	
	for (i =  0; i<32; i++ ) {
		fRecFilter->evType[i] = filter->evType[i];
		fRecFilter->port[i] = filter->port[i];
	}

	for (i =  0; i<2; i++ ) {
		fRecFilter->channel[i] = filter->channel[i];
	}
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
			if (IsRecording()){ // if recording don't pass the event to the EventReceiver
				MidiFreeEv(e);
			}else{
				TEventDispatcher::ReceiveEvents(e);
			}
			break;
	
		case typeLoopEnd:
		case typeClock:
		case typeStart:
		case typeStop:
		case typeContinue:
		case typeSongPos:
		case typeTempo:
			TEventDispatcher::ReceiveEvents(e);
			break;
	
		default:
			ReceiveDefaultEvent(e);
			break;
	}
}

/*--------------------------------------------------------------------------*/

void TEventRecorder::SetRecordTrack (short tracknum) 
{
	fRecordtrack = tracknum;
	if (IsErasing() && IsRecording()) EraseTrack(); 
}

/*--------------------------------------------------------------------------*/

void TEventRecorder::SetRecordMode (short state) 
{ 
	fRecordmode = state;
	if (IsErasing () && IsRecording()) EraseTrack(); 
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
	if (MidiSeqPtr tmp =  MidiNewSeq()) fScore->SetTrack(fRecordtrack,tmp);
}


