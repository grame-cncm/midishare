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
//	TPlayerScore.cpp			    
// ===========================================================================
// The internal score : a PlayerScore contains at least one Tempo event, one 
// TimeSign event and ont EndScore event


#include "TPlayerScore.h"
#include "TScoreIterator.h"
#include "TEventFactory.h"

#include "UTools.h"
#include "UDebug.h"
#include "UMidi.h"
#include "UMath.h"



/*----------------------------------------------------------------------------*/

TEventPtr TPlayerScore::FirstEvent() { return fScoreBegin;}

/*----------------------------------------------------------------------------*/

TEventPtr TPlayerScore::LastEvent() { return fScoreEnd;}
/*----------------------------------------------------------------------------*/

ULONG TPlayerScore::GetLastDate() { return fScoreEnd->GetDate();}

/*----------------------------------------------------------------------------*/

TPlayerScore::TPlayerScore():TScore(){ Init();}

/*----------------------------------------------------------------------------*/

void TPlayerScore::Init()
{
	MidiEvPtr e2 = MidiNewEv(typeScoreEnd);
	MidiEvPtr e3 = MidiNewEv(typeTempo);
	MidiEvPtr e4 = MidiNewEv(typeTimeSign);
	if (e2 && e3 && e4) {
		Date(e3) = Date(e4) = 0;
		Date(e2) = kEndScoreOffset;
		TrackNum(e2) =  TrackNum(e3) = TrackNum(e4)  = kTempoMap;
		Tempo(e3) = kDefaultTempoEv;
		TSNum(e4) = kDefaultNum;  TSDenom(e4) = kDefaultDenom;
		TSClocks(e4) = kDefaultClocks; TS32nd(e4) = kDefaultN32;
		TEventPtr tempo = TEventFactory::GenericCreateEvent(e3);
		fScoreEnd = TEventFactory::GenericCreateEvent(e2);
		fScoreBegin = TEventFactory::GenericCreateEvent(e4);
		if (tempo && fScoreBegin  && fScoreEnd) {
			TScore::InsertAtBeginEvent(fScoreEnd);
			TScore::InsertAtBeginEvent(fScoreBegin);
			TScore::InsertAtBeginEvent(tempo);
		}
	}
	
}
	
/*----------------------------------------------------------------------------*/
// Insert at the beginning of the score
/*----------------------------------------------------------------------------*/

void TPlayerScore::InsertAtBeginEvent(TEventPtr ev) 
{
	assert(ev);
	TScore::InsertAfterEvent(fScoreBegin,ev);
}
	
/*----------------------------------------------------------------------------*/
// Insert at the end of the score
/*----------------------------------------------------------------------------*/

void TPlayerScore::InsertAtEndEvent(TEventPtr ev) 
{
	assert(ev);
	AdjustEndDate(ev->GetDate());
	TScore::InsertBeforeEvent(fScoreEnd,ev);
}

/*--------------------------------------------------------------------------*/

void TPlayerScore::RemoveEvent(TEventPtr ev)
{
	assert(ev);
	
	if (ev->fPrev && ev->fNext) { // If the event is inserted
	
		if (ev->fNext == fScoreEnd) 			  
			AdjustEndDate(ev->fPrev->GetDate());  
			
		TScore::RemoveEvent(ev);
	}
}

/*----------------------------------------------------------------------------*/

void TPlayerScore::InsertAfterEvent(TEventPtr ev1, TEventPtr ev2) 
{
	assert(ev1);
	assert(ev2);
	
	if (ev1 == fScoreEnd) {
		TScore::InsertBeforeEvent(ev1,ev2);
		AdjustEndDate(ev2->GetDate());  
	}else{
		TScore::InsertAfterEvent(ev1,ev2);
	}
}

/*----------------------------------------------------------------------------*/

void TPlayerScore::InsertBeforeEvent(TEventPtr ev1, TEventPtr ev2)
{
	assert(ev1);
	assert(ev2);
	
	if (ev1 == fScoreBegin) { 
		InsertAtBeginEvent(ev2);
	}else if (ev1 == fScoreEnd) {
		InsertAtEndEvent(ev2);
	}else {
		TScore::InsertBeforeEvent(ev1,ev2);
	}
}

/*----------------------------------------------------------------------------*/

short TPlayerScore::InsertEventsAtEnd(MidiEvPtr e1)
{
	TEventPtr next;
	MidiEvPtr e2;
	
	while (e1) {
		e2 = Link(e1); // Has to be done before the creation of the TEvent  
		if ((next = TEventFactory::GenericCreateEvent(e1))) {
			InsertAtEndEvent(next);
			e1 = e2;
		}else
			return kErrEvent;
	}
	return kNoErr;
}

/*----------------------------------------------------------------------------*/

short TPlayerScore::InsertEventsAtEnd(MidiEvPtr e1, short tracknum)
{
	TEventPtr next;
	MidiEvPtr e2;
	
	while (e1) {
		e2 = Link(e1); // Has to be done before the creation of the TEvent  
		if ((next = TEventFactory::GenericCreateEvent(e1))) {
			TrackNum(e1) = (unsigned char)tracknum;
			InsertAtEndEvent(next);
			e1 = e2;
		}else
			return kErrEvent;
	}
	return kNoErr;
}

/*----------------------------------------------------------------------------*/

short  TPlayerScore::InsertEventsIn(TScoreIteratorPtr it, MidiEvPtr* adr)
{
	TEventPtr cur,next;
	MidiEvPtr e2,e1 = *adr;
	
	while  ((cur = it->NextEv()) && e1) {
		while (e1 && (Date (e1) < cur->GetDate())) {
			e2 = Link(e1);  // Has to be done before the creation of the TEvent  
			if ((next = TEventFactory::GenericCreateEvent(e1))) {
				InsertBeforeEvent(cur,next);
				e1 = e2;
			}else{
				return kErrEvent;
			}
		}
	}
	*adr = e1;
	return kNoErr;
}

/*----------------------------------------------------------------------------*/

short TPlayerScore::InsertEventsIn(TScoreIteratorPtr it, MidiEvPtr* adr, short tracknum)
{
	TEventPtr cur,next;
	MidiEvPtr e2,e1 = *adr;
	
	while  ((cur = it->NextEv()) && e1) {
		while (e1 && (Date (e1) < cur->GetDate())) {
			e2 = Link(e1);  // Has to be done before the creation of the TEvent  
			if ((next = TEventFactory::GenericCreateEvent(e1))) {
				TrackNum(e1) = (unsigned char)tracknum;
				InsertBeforeEvent(cur,next);
				e1 = e2;
			}else{
				return kErrEvent;
			}
		}
	}
	*adr = e1;
	return kNoErr;
}

/*----------------------------------------------------------------------------*/

long TPlayerScore::SetAllTrack(MidiSeqPtr s) 
{
	MidiEvPtr e1,e2;
	
	DetachIterators(); 	// Iterators are detached 
	Clear();  			// Destruction of the score  
	Init();     		// Build the basic score
	
	
	e1 = UMidi::CheckEvType(s,typeTempo);
	e2 = UMidi::CheckEvType(s,typeTimeSign);
	
	if (e1) { // If the seq has a Tempo event at the begining
		((TTempoPtr)fScoreBegin->fPrev)->SetTempoForward(Tempo(e1));
		UMidi::RemoveEv(s,e1);
	}
	 
	if (e2) {  // If the seq has a TimeSign event at the begining
		((TTimeSignPtr)fScoreBegin)->SetFNum(TSNum(e2));
		((TTimeSignPtr)fScoreBegin)->SetFDenom(TSDenom(e2));
		((TTimeSignPtr)fScoreBegin)->SetFnClocks(TSClocks(e2));
		((TTimeSignPtr)fScoreBegin)->SetFn32nd(TS32nd(e2));
		UMidi::RemoveEv(s,e2);
	}
	
	e1 = FirstEv(s);
		
	long res = InsertEventsAtEnd(e1);
	if (res != kNoErr) return res;
	
	AttachIterators();  // Iterators are attached
	
	MidiFreeCell((MidiEvPtr)s);
	return kNoErr;
}

/*----------------------------------------------------------------------------*/

long TPlayerScore::SetTrack(short tracknum,MidiSeqPtr s)
{
	if (CheckTracknum(tracknum)) {
	
		MidiEvPtr e1 = FirstEv (s);
		
		DetachIterators(); 		// Iterators are detached 
		ClearTrack(tracknum);	// Clear the track
		
		// Insertion in the midle of the score
		TScoreIterator it(this);
		long res = InsertEventsIn(&it, &e1, tracknum);
		if (res != kNoErr) return res;
			
		// Insertion at the end of the score
		res = InsertEventsAtEnd(e1,tracknum);
		if (res != kNoErr) return res;
			
		AttachIterators();  // Iterators are attached
		
		MidiFreeCell((MidiEvPtr)s);
		return kNoErr;
		
	}else{
		return kErrSequencer;
	}
}

/*----------------------------------------------------------------------------*/

MidiSeqPtr TPlayerScore::GetAllTrack()
{
	CheckAndAllocateMemory();
	
	MidiSeqPtr dst = MidiNewSeq();
	MidiEvPtr e1;
	
	if (dst) {
	
		TScoreIterator it(this);
		TEventPtr cur;
	
		while ((cur = it.NextEv())) {
			e1 = cur->MidiEvent();
			if (IsScore (e1)){
				if (!UMidi::Copy_AddSeq(dst,e1)) {
					MidiFreeSeq(dst);
					return 0;
				}
			}
		}
	}
	return dst;
}

/*----------------------------------------------------------------------------*/

MidiSeqPtr TPlayerScore::GetTrack(short tracknum)
{
	if (!CheckTracknum (tracknum)) return 0;
	CheckAndAllocateMemory();
	
	MidiSeqPtr dst = MidiNewSeq();
	MidiEvPtr e1;
	
	if (dst) {
	
		TScoreIterator it(this);
		TEventPtr cur;
	
		while ((cur = it.NextEv())) {
			e1 = cur->MidiEvent();
			if (IsScore (e1) && (TrackNum(e1) == tracknum)){
				if (!UMidi::Copy_AddSeq(dst,e1)) {
					MidiFreeSeq(dst);
					return 0;
				}
			}
		}
	}	
	return dst;
}

/*----------------------------------------------------------------------------*/

long  TPlayerScore::InsertAllTrack(TEventPtr cur, MidiSeqPtr s)
{
	MidiEvPtr e1 = FirstEv (s);
	
	assert(cur);
	
	// Insertion in the middle of the score
	TScoreIterator it(this,cur);
	long res = InsertEventsIn(&it,&e1);
	if (res != kNoErr) return res;
	
	// Insertion at the end of the score
	res = InsertEventsAtEnd(e1);
	if (res != kNoErr) return res;
	
	MidiFreeCell((MidiEvPtr)s);
	return kNoErr;
}

/*----------------------------------------------------------------------------*/

long  TPlayerScore::InsertTrack(TEventPtr cur, short tracknum,MidiSeqPtr s)
{ 
	MidiEvPtr e1 = FirstEv (s);
	assert(cur);
	
	// Insertion in the middle of the score
	TScoreIterator it(this,cur);
	long res = InsertEventsIn(&it,&e1, tracknum);
	if (res != kNoErr) return res;
	
	// Insertion at the end of the score
	res = InsertEventsAtEnd(e1,tracknum);
	if (res != kNoErr) return res;
	
	MidiFreeCell((MidiEvPtr)s);
	return kNoErr;
}
	
/*--------------------------------------------------------------------------*/
// Internal Functions
/*--------------------------------------------------------------------------*/

void TPlayerScore::AdjustEndDate(ULONG date) {fScoreEnd->SetDate(date + kEndScoreOffset);}

/*----------------------------------------------------------------------------*/

void TPlayerScore::ClearTrack (short tracknum)
{
	MidiEvPtr e;
	TEventPtr cur;
	
	TScoreIterator1 it(this);
	
	while ((cur = it.NextEv())) {
		e = cur->MidiEvent();
		if (IsScore(e) && (TrackNum(e) == tracknum)){
			RemoveEvent(cur);
			delete(cur);
		}
	}
}

/*----------------------------------------------------------------------------*/

void TPlayerScore::CheckAndAllocateMemory() 
{
	ULONG l = Length() + kMemoryLimit;
	if (MidiFreeSpace() < l) MidiGrowSpace(l);
}
	
	