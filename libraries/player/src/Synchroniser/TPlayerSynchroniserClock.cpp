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

void TPlayerSynchroniserClock::Init()
{ 
	fTempoVisitor.Init();
	fNextdate_ticks = 0;
	fClock_count = 0;
}		

/*--------------------------------------------------------------------------*/

void TPlayerSynchroniserClock::Start()
{ 
	fOffset = MidiGetTime();
	fState->SetWaiting();
}	

/*--------------------------------------------------------------------------*/

void TPlayerSynchroniserClock::Stop() {fState->SetIdle();}

/*--------------------------------------------------------------------------*/

void TPlayerSynchroniserClock::Cont(ULONG date_ticks) 
{ 
	fTempoVisitor.Init(); // Important  (Tempo Map must be re-initialized)
	fTempoVisitor.UpdateTicks(date_ticks);
	fClock_count = (ULONG)fClockConverter.ConvertTickToClock((float)date_ticks);
	fNextdate_ticks = date_ticks;
	fState->SetWaiting();
}

/*--------------------------------------------------------------------------*/

void TPlayerSynchroniserClock::RcvClock(ULONG date_ms)
{
	if (fState->IsRunning()) {
		RcvNextClock(date_ms);
	}else if (fState->IsWaiting()){
		RcvFirstClock(date_ms);
	}
}

/*--------------------------------------------------------------------------*/

void TPlayerSynchroniserClock::RcvFirstClock(ULONG date_ms)
{
	fState->SetRunning();
	fOffset = date_ms - fTempoVisitor.CurDateMs();
	fTempoVisitor.SetTempo(GetPosTicks(), kDefaultTempoEv);
 	NextClock(date_ms);
}

/*--------------------------------------------------------------------------*/

void TPlayerSynchroniserClock::RcvNextClock(ULONG date_ms)
{
 	fTempoVisitor.SetTempo(GetPosTicks(), fClockConverter.ConvertDeltaToTempo(date_ms - fRcv_clock));
 	fTempoVisitor.UpdateTicks(fNextdate_ticks);
 	NextClock(date_ms);
}

/*--------------------------------------------------------------------------*/

void TPlayerSynchroniserClock::NextClock(ULONG date_ms)
{
 	fRcv_clock = date_ms;
 	fNextdate_ticks = (ULONG)fClockConverter.ConvertClockToTick((float(++fClock_count)));
 	fScheduler->ReScheduleTasks();
}

/*--------------------------------------------------------------------------*/

Boolean TPlayerSynchroniserClock::IsSchedulable (ULONG date_ticks) {return (date_ticks < fNextdate_ticks);}

/*--------------------------------------------------------------------------*/

ULONG TPlayerSynchroniserClock::GetPosTicks() {return fTempoVisitor.CurDateTicks();}

/*--------------------------------------------------------------------------*/

void TPlayerSynchroniserClock::SetPosTicks (ULONG date_ticks)
{
	 fTempoVisitor.UpdateTicks((ULONG)fClockConverter.ConvertTickToTickAtPrevClock((float)date_ticks));
}
