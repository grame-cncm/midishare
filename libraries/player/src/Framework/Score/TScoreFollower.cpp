// ===========================================================================
// The Player Library is Copyright (c) Grame, Computer Music Research Laboratory 
// 1996-2003, and is distributed as Open Source software under the Artistic License;
// see the file "Artistic" that is included in the distribution for details.
//
// Grame : Computer Music Research Laboratory
// Web : http://www.grame.fr/Research
// E-mail : MidiShare@rd.grame.fr
// ===========================================================================


// ===========================================================================
//	TScoreFollower.cpp		    
// ===========================================================================
//
// Uses a TIterator to move a TVisitor on a TScore object.
// 

#include "TScoreFollower.h"

/*--------------------------------------------------------------------------*/

TScoreFollower::TScoreFollower(TScoreIteratorPtr iterator, TTempoMapVisitorPtr tempovisitor)
{
	fTempoVisitor = tempovisitor;
	fIterator = iterator;
} 	

/*--------------------------------------------------------------------------*/
	
TScoreFollower::TScoreFollower(TScorePtr score, ULONG tpq)
{
 	fTempoVisitor = new TTempoMapVisitor (tpq);
 	fIterator = new TScoreIterator(score);
}
 
/*--------------------------------------------------------------------------*/
 		 		
 TScoreFollower::~TScoreFollower () 
 {
 	delete(fTempoVisitor);
 	delete(fIterator);
}
 		
/*--------------------------------------------------------------------------*/

TEventPtr TScoreFollower::SetPosBBU (const TPos& pos)
{
	if (!fIterator->IsFirstEv() && fTempoVisitor->SupEq (fIterator->CurDate(), pos)){
		SetPosBBUBackward(pos);
		SetPosBBUForward(pos);
	}else{
		SetPosBBUForward(pos);
	}
	
	fTempoVisitor->UpdateBBU(pos);
	return fIterator->CurEv();
}
/*--------------------------------------------------------------------------*/

TEventPtr TScoreFollower::SetPosMs (ULONG date_ms)
{
	ULONG date_ten_micro = date_ms * 100;
	
	if (!fIterator->IsFirstEv() && (fTempoVisitor->SupEq(fIterator->CurDate(),date_ten_micro))) {
		SetPosMicroBackward(date_ten_micro);
		SetPosMicroForward(date_ten_micro);
	}else{
		SetPosMicroForward(date_ten_micro);
	}
	
	fTempoVisitor->UpdateTenMicroSec(date_ten_micro);
	return fIterator->CurEv();
}

/*--------------------------------------------------------------------------*/

TEventPtr TScoreFollower::SetPosTicks (ULONG date_ticks)
{
	if (!fIterator->IsFirstEv() && (fIterator->CurDate() >= date_ticks)) {
		SetPosTicksBackward(date_ticks);
		SetPosTicksForward(date_ticks);
	}else{
		SetPosTicksForward(date_ticks);
	}
	
	fTempoVisitor->UpdateTicks(date_ticks);
	return fIterator->CurEv();
}

/*--------------------------------------------------------------------------*/
/*!
 \brief Set the iterator pos on the first event which date is < date_ticks
 \param  date_ticks is the date in ticks.
*/

void TScoreFollower::SetPosTicksBackward (ULONG date_ticks)
{
	do {
		fIterator->CurEv()->Accept(fTempoVisitor,false); 
		fIterator->PrevEv(); 
	} while (!fIterator->IsFirstEv() && (fIterator->CurDate() >= date_ticks));
	
}

/*--------------------------------------------------------------------------*/
/*!
 \brief Set the iterator pos on the first event which date is > date_ticks
 \param  date_ticks is the date in ticks.
*/

void TScoreFollower::SetPosTicksForward (ULONG date_ticks)
{
	while (!fIterator->IsLastEv() && fIterator->CurDate() < date_ticks){ 
		fIterator->CurEv()->Accept(fTempoVisitor,true);
		fIterator->NextEv();
	}
}

/*--------------------------------------------------------------------------*/
/*!
 \brief Set the iterator pos on the first event which date is < date_ten_micro
 \param  date_ticks is the date in 10 microseconds.
*/

void TScoreFollower::SetPosMicroBackward  (ULONG date_ten_micro)
{
	do {
		fIterator->CurEv()->Accept(fTempoVisitor,false); 
		fIterator->PrevEv();
	} while (!fIterator->IsFirstEv() && (fTempoVisitor->SupEq(fIterator->CurDate(),date_ten_micro)));
	
}	

/*--------------------------------------------------------------------------*/
/*!
 \brief Set the iterator pos on the first event which date is > date_ten_micro
 \param  date_ticks is the date in 10 microseconds.
*/

void TScoreFollower::SetPosMicroForward  (ULONG date_ten_micro)
{
	while (!fIterator->IsLastEv() && fTempoVisitor->Inf(fIterator->CurDate(), date_ten_micro)){ 
		fIterator->CurEv()->Accept(fTempoVisitor,true);
		fIterator->NextEv();
	}
}

/*--------------------------------------------------------------------------*/
/*!
 \brief Set the iterator pos on the first event which date is < pos
 \param  pos is the date in BBU representation.
*/ 

void TScoreFollower::SetPosBBUBackward (const TPos& pos)
{
	do {
		fIterator->CurEv()->Accept(fTempoVisitor,false); 
		fIterator->PrevEv();
	} while (!fIterator->IsFirstEv() && fTempoVisitor->SupEq (fIterator->CurDate(), pos));
}

/*--------------------------------------------------------------------------*/
/*!
 \brief Set the iterator pos on the first event which date is > pos
 \param  pos is the date in BBU representation.
*/ 

void TScoreFollower::SetPosBBUForward (const TPos& pos)
{
	while (!fIterator->IsLastEv() && fTempoVisitor->Inf(fIterator->CurDate(), pos)){	
		fIterator->CurEv()->Accept(fTempoVisitor,true);
		fIterator->NextEv();
	}
}
	

	