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
//	TPlayerSynchroniserClock.cpp			    
// ===========================================================================
//
//	Synchronizer for kClockSync mode: 
//  Reception of Start, Stop, Continue, Clock events will call the Start, Stop,
//  Cont, RcvClock methods.
//

#include "TPlayerSynchroniser.h"
#include "UTools.h"
#include "UDebug.h"

/*--------------------------------------------------------------------------*/

void TPlayerSynchroniserClock::Init ()
{ 
	fTempoVisitor->Init();
	fNextdate_ticks = 0;
	fClock_count = 0;
}	

/*--------------------------------------------------------------------------*/

void TPlayerSynchroniserClock::Start ()
{ 
	fOffset = MidiGetTime();
	fState->SetWaiting();
}	

/*--------------------------------------------------------------------------*/

void TPlayerSynchroniserClock::Stop () { fState->SetIdle(); }

/*--------------------------------------------------------------------------*/

void TPlayerSynchroniserClock::Cont (ULONG date_ticks) 
{ 
	fTempoVisitor->Init(); // Important  (Tempo Map must be re-initialized)
	fTempoVisitor->UpdateTicks(date_ticks);
	fClock_count = (ULONG)fClockConverter->ConvertTickToClock((float)date_ticks);
	fNextdate_ticks = date_ticks;
	fState->SetWaiting();
}

/*--------------------------------------------------------------------------*/

void TPlayerSynchroniserClock::RcvClock (ULONG date_ms)
{
	if (fState->IsRunning()) {
		RcvNextClock(date_ms);
	}else if (fState->IsWaiting()){
		RcvFirstClock(date_ms);
	}
}

/*--------------------------------------------------------------------------*/

void TPlayerSynchroniserClock::RcvFirstClock (ULONG date_ms)
{
	fState->SetRunning();
	fOffset = date_ms - fTempoVisitor->CurDateMs();
	fTempoVisitor->SetTempo(GetPosTicks(), kDefaultTempoEv);
 	NextClock(date_ms);
}

/*--------------------------------------------------------------------------*/

void TPlayerSynchroniserClock::RcvNextClock (ULONG date_ms)
{
 	fTempoVisitor->SetTempo(GetPosTicks(), fClockConverter->ConvertDeltaToTempo(date_ms - fRcv_clock));
 	fTempoVisitor->UpdateTicks(fNextdate_ticks);
 	NextClock(date_ms);
}

/*--------------------------------------------------------------------------*/

void TPlayerSynchroniserClock::NextClock (ULONG date_ms)
{
 	fRcv_clock = date_ms;
 	fNextdate_ticks = (ULONG)fClockConverter->ConvertClockToTick((float(++fClock_count)));
 	fScheduler->ReScheduleTasks();
}


/*--------------------------------------------------------------------------*/

Boolean TPlayerSynchroniserClock::IsSchedulable (ULONG date_ticks) {return (date_ticks < fNextdate_ticks);}

/*--------------------------------------------------------------------------*/

ULONG TPlayerSynchroniserClock::GetPosTicks () 
{
	return fTempoVisitor->CurDateTicks();
}

/*--------------------------------------------------------------------------*/

void TPlayerSynchroniserClock::SetPosTicks (ULONG date_ticks)
{
	 fTempoVisitor->UpdateTicks((ULONG)fClockConverter->ConvertTickToTickAtPrevClock((float)date_ticks));
}
