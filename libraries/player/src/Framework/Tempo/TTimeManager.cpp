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

TTimeManager::TTimeManager(TScorePtr score, ULONG tpq) : TScoreFollower(score, tpq)
{
 	fScore = score;
		
	// Prepare the score for fast forward/backward moves
	TTempoMapBuilder fTempomap;
 	UScoreManager::Apply (fScore, &fTempomap);
 	
}
 
/*--------------------------------------------------------------------------*/
 		
const TPos  TTimeManager::ConvertTickToBBU (ULONG date_tick) 
{ 
	SetPosTicks(date_tick);
	return fTempoVisitor->CurDateBBU();
}

/*--------------------------------------------------------------------------*/

ULONG   TTimeManager::ConvertBBUToTick(const TPos& pos) 
{ 
	SetPosBBU(pos);
	return fTempoVisitor->CurDateTicks();
}
	
/*--------------------------------------------------------------------------*/
	
ULONG  TTimeManager::ConvertTickToMs (ULONG date_tick) 
{ 
	SetPosTicks(date_tick);
	return fTempoVisitor->CurDateMicro()/100;
}

/*--------------------------------------------------------------------------*/

ULONG  TTimeManager::ConvertMsToTick (ULONG date_ms)   
{ 
	SetPosMs (date_ms);
	return fTempoVisitor->CurDateTicks();
}

/*--------------------------------------------------------------------------*/

ULONG TTimeManager::GetTempo (ULONG date_tick) 
{
	SetPosTicks(date_tick);
	return fTempoVisitor->GetTempo();
}

/*--------------------------------------------------------------------------*/

short TTimeManager::GetNum (ULONG date_tick) 
{
	SetPosTicks(date_tick);
	return fTempoVisitor->GetNum();
}

/*--------------------------------------------------------------------------*/

short TTimeManager::GetDenom (ULONG date_tick) 
{
	SetPosTicks(date_tick);
	return fTempoVisitor->GetDenom();
}

/*--------------------------------------------------------------------------*/

short TTimeManager::GetNClocks (ULONG date_tick) 
{
	SetPosTicks(date_tick);
	return fTempoVisitor->GetNClocks();
}

/*--------------------------------------------------------------------------*/

short TTimeManager::GetN32nd (ULONG date_tick) 
{
	SetPosTicks(date_tick);
	return fTempoVisitor->GetN32nd();
}

