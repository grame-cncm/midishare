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
//	TScoreIterator.cpp		    
// ===========================================================================
//
// An iterator on a TScore object
// 

#include "TScoreIterator.h"
#include "UTools.h"
#include "UDebug.h"

/*----------------------------------------------------------------------------*/

TScoreIterator::TScoreIterator():fScore(0),fCur(0),fCurDate(0),fFirst(false),fLast(false),fNextIt(0){} 

/*----------------------------------------------------------------------------*/

TScoreIterator::TScoreIterator(TScorePtr score):fScore(score),fNextIt(0) 
{
        assert(fScore);
	fScore->AttachIterator(this);
	Init();
}

/*----------------------------------------------------------------------------*/

TScoreIterator::TScoreIterator(TScorePtr score, TEventPtr cur):fScore(score),fNextIt(0) 
{ 
        assert(fScore);
	fScore->AttachIterator(this);
	Init(cur);
}

/*----------------------------------------------------------------------------*/

TScoreIterator::~TScoreIterator()
{
        assert(fScore);
	Detach();
	fScore->DetachIterator(this);
}

/*----------------------------------------------------------------------------*/

TEventPtr 	TScoreIterator::CurEv() 	{return fCur;}
MidiEvPtr 	TScoreIterator::CurMidiEv() 	{return fCur->MidiEvent();}
ULONG 		TScoreIterator::CurDate() 	{return fCur ? fCur->GetDate() : 0;}
Boolean 	TScoreIterator::IsFirstEv() 	{return fFirst;}
Boolean 	TScoreIterator::IsLastEv()  	{return fLast;}
		
/*----------------------------------------------------------------------------*/
// Set the cur pos on the first event
/*----------------------------------------------------------------------------*/
		
void TScoreIterator::Init()
{
	fCurDate = 0;
	fFirst = fLast = false;
	if (fScore->fLast) {  				// Non empty 
		fCur = fScore->fLast->fNext; 	// Set fCur on the first ev
	}else{
		fCur = 0;  // Empty score
	}
}

/*----------------------------------------------------------------------------*/
// Set the cur pos on the parameter event
/*----------------------------------------------------------------------------*/
		
void TScoreIterator::Init(TEventPtr cur) 
{
	fCurDate = cur->GetDate();
	fFirst = fLast =  false;
	fCur = cur;
}

/*----------------------------------------------------------------------------*/

void TScoreIterator::ItemsInserted(TEventPtr ev) {} 

/*----------------------------------------------------------------------------*/

void TScoreIterator::ItemsRemoved(TEventPtr ev) 
{
	if (fCur == ev) {
		
		if (ev == fScore->fLast) {  // If last event
			if (ev->fNext == ev){   // Only one event
				fCur = 0;
				return;
			}else
				fCur = fCur->fPrev;
		}else{
			fCur = fCur->fNext;
		}
		
		assert(fCur);
		fCurDate = fCur->GetDate();
	}
}

/*----------------------------------------------------------------------------*/

void TScoreIterator::Detach()
{
	fCurDate = 0;
	fCur = 0;    	
	fFirst = fLast = false;
}

/*------------------------------------------------------------------------------------*/
// Returns the current event and goes forward, returns NULL if the last event is reached
/*------------------------------------------------------------------------------------*/

TEventPtr TScoreIterator::NextEv() 
{
	TEventPtr ev = fCur; 
	
	if (fLast) return 0;
	
	if (ev) {
		fCurDate = ev->GetDate();
		if (ev == fScore->fLast) {
			fLast = true;
		}else{
			fFirst = false;
			fCur = fCur->fNext;
		}
	}
	
	return ev;
}

/*----------------------------------------------------------------------------------------*/
// Returns the current event and goes backward, returns NULL if the first event is reached
/*----------------------------------------------------------------------------------------*/

TEventPtr TScoreIterator::PrevEv() 
{
	TEventPtr ev = fCur; 
	
	if (fFirst) return 0;
	
	if (ev) {
		fCurDate = ev->GetDate();
		if (ev == fScore->fLast->fNext) {
			fFirst = true;
		}else{
			fLast = false;
			fCur = fCur->fPrev;
		}
	}
	
	return ev;
}

/*--------------------------------------------------------------------------------------------------------------*/
// Returns the current event and goes forward, returns NULL when the next event with a different date is reached
/*-------------------------------------------------------------------------------------------------------------*/

TEventPtr TScoreIterator::NextDateEv() 
{ 
	TEventPtr ev = fCur; 

	if (fLast) return 0;
	
	if (ev) {
		if (fCurDate != fCur->GetDate()){
			fCurDate = fCur->GetDate();
			return 0;
		}else 
			return NextEv();
	}else{
		return ev;
	}
}

/*--------------------------------------------------------------------------------------------------------------*/
// Returns the current event and goes backward, returns NULL when the next event with a different date is reached
/*--------------------------------------------------------------------------------------------------------------*/

TEventPtr TScoreIterator::PrevDateEv() 
{ 
	TEventPtr ev = fCur; 
	
	if (fFirst) return 0;
	
	if (ev) {
		if (fCurDate != fCur->GetDate()){
			fCurDate = fCur->GetDate();
			return 0;
		}else 
			return PrevEv();
	}else {
		
		return ev;
	}
}

/*----------------------------------------------------------------------------*/

TEventPtr TScoreIterator::SetPosTicks (ULONG date_ticks)
{
	if (!IsFirstEv() && (CurDate() >= date_ticks)) {
		SetPosTicksBackward(date_ticks);
		SetPosTicksForward(date_ticks);
	}else{
		SetPosTicksForward(date_ticks);
	}
	return CurEv();
}

/*----------------------------------------------------------------------------*/

void TScoreIterator::SetPosTicksBackward (ULONG date_ticks)
{
	do {PrevEv();} while (!IsFirstEv() && (CurDate() >= date_ticks));
}

/*----------------------------------------------------------------------------*/

void TScoreIterator::SetPosTicksForward (ULONG date_ticks)
{
	while (!IsLastEv() && (CurDate() < date_ticks)) {NextEv();}
}

