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
//	TTickPlayer.cpp			    
// ===========================================================================
//
//	Player in ticks 
//

#include "TTickPlayer.h"
#include "UTools.h"

/*--------------------------------------------------------------------------*/

void TTickPlayer::PlaySliceForward() 
{
   	TEventPtr cur;
	while ((cur = fIterator.NextDateEv())) {
		cur->Accept(&fSliceVisitor, true);
	}
}

/*--------------------------------------------------------------------------*/

void TTickPlayer::PlaySliceBackward() 
{
	TEventPtr cur;
	while ((cur = fIterator.PrevDateEv())) {
		cur->Accept(&fSliceVisitor, false);
	}
}

/*--------------------------------------------------------------------------*/
void TTickPlayer::PlaySlice(ULONG date_ms) 
{
   TEventPtr cur;	
	if (fIterator.IsLastEv()) {
		fEventUser->CopyAndUseEvent(fIterator.CurEv(),date_ms);
	}else {
	
		// For all events at the same date
		while ((cur = fIterator.NextDateEv())) {fEventUser->CopyAndUseEvent(cur,date_ms);}
		
		// Schedule the PlayTask for the date of the next events in ticks
		fScheduler->ScheduleTickTask(&fPlayTask, fIterator.CurDate());
	}
}

/*--------------------------------------------------------------------------*/

void TTickPlayer::Init() {fIterator.Init();}

/*--------------------------------------------------------------------------*/

void TTickPlayer::Start() {fScheduler->ScheduleTickTask(&fPlayTask, fIterator.CurDate());}

/*--------------------------------------------------------------------------*/

void TTickPlayer::Stop() {fPlayTask.Forget();}

/*--------------------------------------------------------------------------*/

void TTickPlayer::Cont(ULONG date_ticks) 
{ 
	fIterator.SetPosTicks(date_ticks);
	fScheduler->ScheduleTickTask(&fPlayTask, fIterator.CurDate()); 
}

/*--------------------------------------------------------------------------*/

void TTickPlayer::SetPosTicks(ULONG date_ticks) {fIterator.SetPosTicks(date_ticks);}

/*--------------------------------------------------------------------------*/

ULONG TTickPlayer::GetPosTicks() {return fIterator.CurDate();}

/*--------------------------------------------------------------------------*/

void TPlayTask::Execute(TMidiApplPtr appl, ULONG date_ms) {fPlayer->PlaySlice(date_ms);}
