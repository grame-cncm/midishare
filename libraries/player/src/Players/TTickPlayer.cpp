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
//	TTickPlayer.cpp			    
// ===========================================================================
//
//	Player in ticks 
//

#include "TTickPlayer.h"
#include "UTools.h"


/*--------------------------------------------------------------------------*/

TTickPlayer::TTickPlayer(TScorePtr score, TEventSenderInterfacePtr user, TSchedulerInterfacePtr scheduler)
{
	fSliceVisitor = new TSliceVisitor(user);
	fIterator = new TScoreIterator(score);
	fPlayTask = new TPlayTask(this);	
	fEventUser = user;
	fScheduler = scheduler;
}

/*--------------------------------------------------------------------------*/
		
TTickPlayer::~TTickPlayer() 
{
	delete(fSliceVisitor);
	delete(fIterator);
	delete(fPlayTask);
}

/*--------------------------------------------------------------------------*/

void TTickPlayer::PlaySliceForward () 
{
   TEventPtr cur;
	while (cur = fIterator->NextDateEv()) {
		cur->Accept(fSliceVisitor, true);
	}
}

/*--------------------------------------------------------------------------*/

void TTickPlayer::PlaySliceBackward () 
{
	TEventPtr cur;
	while ( cur = fIterator->PrevDateEv()) {
		cur->Accept(fSliceVisitor, false);
	}
}

/*--------------------------------------------------------------------------*/

void TTickPlayer::PlaySlice (ULONG date_ms) 
{
   TEventPtr cur;	
	if (fIterator->IsLastEv()) {
		fEventUser->CopyAndUseEvent(fIterator->CurEv(),date_ms);
	}else {
	
		// For all events at the same date
		while ( cur = fIterator->NextDateEv()) {fEventUser->CopyAndUseEvent(cur,date_ms);}
		
		// Schedule the PlayTask for the date of the next events in ticks
		fScheduler->ScheduleTickTaskInt(fPlayTask, fIterator->CurDate());
	}
}

/*--------------------------------------------------------------------------*/

void TTickPlayer::Start ()
{ 
	fIterator->Init();
	fScheduler->ScheduleTickTask(fPlayTask, fIterator->CurDate()); 
}

/*--------------------------------------------------------------------------*/

void TTickPlayer::Stop (){ fPlayTask->Forget(); }

/*--------------------------------------------------------------------------*/

void TTickPlayer::Cont (ULONG date_ticks) 
{ 
	fIterator->SetPosTicks(date_ticks);
	fScheduler->ScheduleTickTask(fPlayTask, fIterator->CurDate()); 
}

/*--------------------------------------------------------------------------*/

void TTickPlayer::SetPosTicks (ULONG date_ticks) { fIterator->SetPosTicks(date_ticks); }

/*--------------------------------------------------------------------------*/

ULONG TTickPlayer::GetPosTicks (){ return fIterator->CurDate(); }

