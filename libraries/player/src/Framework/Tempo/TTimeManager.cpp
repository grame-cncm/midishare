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
//	TTimeManager.cpp		    
// ===========================================================================
//
// Uses a TIterator object to move a TVisitor object on a TScore object.
// Provides time conversion functions.
// 

#include "TTimeManager.h"
#include "UScoreManager.h"
#include "TTempoMapBuilder.h"
#include "TEventFactory.h"

/*--------------------------------------------------------------------------*/

TTimeManager::TTimeManager(TScorePtr score, ULONG tpq) : TScoreObjFollower(score,tpq),fScore(score)
{
	// Prepare the score for fast forward/backward moves
	TTempoMapBuilder fTempomap;
 	UScoreManager::Apply (fScore, &fTempomap);
}
 
/*--------------------------------------------------------------------------*/
 		
const TPos  TTimeManager::ConvertTickToBBU(ULONG date_tick) 
{ 
	SetPosTicks(date_tick);
	return GetVisitor().CurDateBBU();
}

/*--------------------------------------------------------------------------*/

ULONG   TTimeManager::ConvertBBUToTick(const TPos& pos) 
{ 
	SetPosBBU(pos);
	return  GetVisitor().CurDateTicks();
}
	
/*--------------------------------------------------------------------------*/
	
ULONG  TTimeManager::ConvertTickToMs(ULONG date_tick) 
{ 
	SetPosTicks(date_tick);
	return GetVisitor().CurDateMicro()/100;
}

/*--------------------------------------------------------------------------*/

ULONG  TTimeManager::ConvertMsToTick(ULONG date_ms)   
{ 
	SetPosMs (date_ms);
	return GetVisitor().CurDateTicks();
}

/*--------------------------------------------------------------------------*/

ULONG TTimeManager::GetTempo(ULONG date_tick) 
{
	SetPosTicks(date_tick);
	return GetVisitor().GetTempo();
}

/*--------------------------------------------------------------------------*/

short TTimeManager::GetNum(ULONG date_tick) 
{
	SetPosTicks(date_tick);
	return GetVisitor().GetNum();
}

/*--------------------------------------------------------------------------*/

short TTimeManager::GetDenom(ULONG date_tick) 
{
	SetPosTicks(date_tick);
	return GetVisitor().GetDenom();
}

/*--------------------------------------------------------------------------*/

short TTimeManager::GetNClocks(ULONG date_tick) 
{
	SetPosTicks(date_tick);
	return GetVisitor().GetNClocks();
}

/*--------------------------------------------------------------------------*/

short TTimeManager::GetN32nd(ULONG date_tick) 
{
	SetPosTicks(date_tick);
	return GetVisitor().GetN32nd();
}

