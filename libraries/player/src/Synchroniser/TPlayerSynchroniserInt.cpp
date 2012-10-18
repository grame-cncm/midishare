  /*

  Copyright (C) Grame 1996-2012

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
//	TPlayerSynchroniserInt.cpp			    
// ===========================================================================
//
//	Synchronizer for kInternalSync and kSMPTESync mode: 
//

#include "TPlayerSynchroniser.h"
#include "UTools.h"
#include "UDebug.h"

/*--------------------------------------------------------------------------*/

void TPlayerSynchroniserInt::PlaySlice() 
{
	TEventPtr cur; 
	if (!fIterator.IsLastEv()) {
	
		ULONG cur_tempo = fTempoVisitor.GetTempo();
	
		// For all events at the same date
		while ((cur = fIterator.NextDateEv())) {
			cur->Accept(&fTempoVisitor,true);
		}
		
		// If Tempo has changed, update the Scheduler
		if (cur_tempo != fTempoVisitor.GetTempo()) { 
			fScheduler->ReScheduleTasks(); 
		}
		
		// Schedule the PlayTask for the date of the next events
		fScheduler->ScheduleTickTask(&fTempoTask, fIterator.CurDate());
	}
}

/*--------------------------------------------------------------------------*/

void TPlayerSynchroniserInt::Init() {fFollower.Init();}

/*--------------------------------------------------------------------------*/

void TPlayerSynchroniserInt::Start()
{ 
	fState->SetRunning(); // before  ScheduleTickTask
	fOffset = MidiGetTime();
	fScheduler->ScheduleTickTask(&fTempoTask, fIterator.CurDate()); 
}	

/*--------------------------------------------------------------------------*/

void TPlayerSynchroniserInt::Stop() 
{ 
	fTempoTask.Forget();
	if (fState->IsRunning()) fTempoVisitor.UpdateMs(MidiGetTime() - fOffset);
	fState->SetIdle();
}

/*--------------------------------------------------------------------------*/

void TPlayerSynchroniserInt::Cont(ULONG date_ticks) 
{ 
	fState->SetRunning(); // before ScheduleTickTask
	SetPosTicks(date_ticks);
	fOffset = MidiGetTime() - fTempoVisitor.CurDateMs();
	fScheduler->ScheduleTickTask(&fTempoTask, fIterator.CurDate()); 
}

/*--------------------------------------------------------------------------*/

Boolean TPlayerSynchroniserInt::IsSchedulable(ULONG date_ticks) {return fState->IsRunning();}

/*--------------------------------------------------------------------------*/

ULONG TPlayerSynchroniserInt::GetPosTicks() 
{
	if (fState->IsRunning()) 
		fTempoVisitor.UpdateMs(MidiGetTime() - fOffset);
	return fTempoVisitor.CurDateTicks();
}

/*--------------------------------------------------------------------------*/

void  TPlayerSynchroniserInt::SetPosTicks(ULONG date_ticks)
{  
	fFollower.SetPosTicks(date_ticks);
}

/*--------------------------------------------------------------------------*/

void TTempoTask::Execute (TMidiApplPtr appl, ULONG date) {fSynchroniser->PlaySlice();}

