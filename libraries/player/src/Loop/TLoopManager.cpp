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

