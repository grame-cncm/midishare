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
//	TLoopManager.cpp		    
// ===========================================================================
//
//	Management of the Loop
// 

#include "TLoopManager.h"


/*--------------------------------------------------------------------------*/

long TLoopManager::SetLoopStartTicks(ULONG date_ticks) 
{
	if (date_ticks  > GetLoopEndTicks()){
		return  kErrSequencer;
	}else {
		fFollower.SetPosTicks(date_ticks);
		fLoopStart = date_ticks;
		return kNoErr;
	}
}

/*--------------------------------------------------------------------------*/

long TLoopManager::SetLoopStartBBU(const TPos& pos) 
{
	fFollower.SetPosBBU(pos);
	return SetLoopStartTicks (fFollower.GetPosTicks());
}

/*--------------------------------------------------------------------------*/

long TLoopManager::SetLoopStartMs(ULONG date_ms)
{
	fFollower.SetPosMs(date_ms);
	return SetLoopStartTicks (fFollower.GetPosTicks());
}
	 
/*--------------------------------------------------------------------------*/

long TLoopManager::SetLoopEndTicks(ULONG date_ticks) 
{
	date_ticks -= kLoopEndOffset; // To avoid glitch when looping 
	if (date_ticks < GetLoopStartTicks()) {
		return  kErrSequencer;
	} else {
		fLoopEndMarker.Remove(fScore);
		TEventPtr cur = fFollower.SetPosTicks(date_ticks);
		fLoopEndMarker.Insert(fScore,cur,date_ticks);
		return kNoErr;
	}
}

/*--------------------------------------------------------------------------*/

long TLoopManager::SetLoopEndBBU(const TPos& pos) 
{
	fFollower.SetPosBBU(pos);
	return SetLoopEndTicks (fFollower.GetPosTicks());
}

/*--------------------------------------------------------------------------*/

long TLoopManager::SetLoopEndMs(ULONG date_ms)
{
	fFollower.SetPosMs(date_ms);
	return SetLoopEndTicks (fFollower.GetPosTicks());
}

