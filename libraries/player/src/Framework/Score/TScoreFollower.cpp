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
//	TScoreFollower.cpp		    
// ===========================================================================
//
// Uses a TIterator to move a TVisitor on a TScore object.
// 

#include "TScoreFollower.h"

/*--------------------------------------------------------------------------*/

void  TScoreFollower::Init (){GetVisitor().Init(); GetIterator().Init();}

ULONG TScoreFollower::GetPosTicks() {return GetVisitor().CurDateTicks();}

/*--------------------------------------------------------------------------*/

TEventPtr TScoreFollower::SetPosBBU (const TPos& pos)
{
	if (!GetIterator().IsFirstEv() && GetVisitor().SupEq (GetIterator().CurDate(), pos)){
		SetPosBBUBackward(pos);
		SetPosBBUForward(pos);
	}else{
		SetPosBBUForward(pos);
	}
	
	GetVisitor().UpdateBBU(pos);
	return GetIterator().CurEv();
}
/*--------------------------------------------------------------------------*/

TEventPtr TScoreFollower::SetPosMs (ULONG date_ms)
{
	ULONG date_ten_micro = date_ms * 100;
	
	if (!GetIterator().IsFirstEv() && (GetVisitor().SupEq(GetIterator().CurDate(),date_ten_micro))) {
		SetPosMicroBackward(date_ten_micro);
		SetPosMicroForward(date_ten_micro);
	}else{
		SetPosMicroForward(date_ten_micro);
	}
	
	GetVisitor().UpdateTenMicroSec(date_ten_micro);
	return GetIterator().CurEv();
}

/*--------------------------------------------------------------------------*/

TEventPtr TScoreFollower::SetPosTicks (ULONG date_ticks)
{
	if (!GetIterator().IsFirstEv() && (GetIterator().CurDate() >= date_ticks)) {
		SetPosTicksBackward(date_ticks);
		SetPosTicksForward(date_ticks);
	}else{
		SetPosTicksForward(date_ticks);
	}
	
	GetVisitor().UpdateTicks(date_ticks);
	return GetIterator().CurEv();
}

/*--------------------------------------------------------------------------*/
/*!
 \brief Set the iterator pos on the first event which date is < date_ticks
 \param  date_ticks is the date in ticks.
*/

void TScoreFollower::SetPosTicksBackward (ULONG date_ticks)
{
	do {
		GetIterator().CurEv()->Accept(&GetVisitor(),false); 
		GetIterator().PrevEv(); 
	} while (!GetIterator().IsFirstEv() && (GetIterator().CurDate() >= date_ticks));
}

/*--------------------------------------------------------------------------*/
/*!
 \brief Set the iterator pos on the first event which date is > date_ticks
 \param  date_ticks is the date in ticks.
*/

void TScoreFollower::SetPosTicksForward (ULONG date_ticks)
{
	while (!GetIterator().IsLastEv() && GetIterator().CurDate() < date_ticks){ 
		GetIterator().CurEv()->Accept(&GetVisitor(),true);
		GetIterator().NextEv();
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
		GetIterator().CurEv()->Accept(&GetVisitor(),false); 
		GetIterator().PrevEv();
	} while (!GetIterator().IsFirstEv() && (GetVisitor().SupEq(GetIterator().CurDate(),date_ten_micro)));
}	

/*--------------------------------------------------------------------------*/
/*!
 \brief Set the iterator pos on the first event which date is > date_ten_micro
 \param  date_ticks is the date in 10 microseconds.
*/

void TScoreFollower::SetPosMicroForward  (ULONG date_ten_micro)
{
	while (!GetIterator().IsLastEv() && GetVisitor().Inf(GetIterator().CurDate(), date_ten_micro)){ 
		GetIterator().CurEv()->Accept(&GetVisitor(),true);
		GetIterator().NextEv();
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
		GetIterator().CurEv()->Accept(&GetVisitor(),false); 
		GetIterator().PrevEv();
	} while (!GetIterator().IsFirstEv() && GetVisitor().SupEq (GetIterator().CurDate(), pos));
}

/*--------------------------------------------------------------------------*/
/*!
 \brief Set the iterator pos on the first event which date is > pos
 \param  pos is the date in BBU representation.
*/ 

void TScoreFollower::SetPosBBUForward (const TPos& pos)
{
	while (!GetIterator().IsLastEv() && GetVisitor().Inf(GetIterator().CurDate(), pos)){	
		GetIterator().CurEv()->Accept(&GetVisitor(),true);
		GetIterator().NextEv();
	}
}
	

	