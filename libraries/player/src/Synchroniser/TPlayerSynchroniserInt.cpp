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
//	TPlayerSynchroniserInt.cpp			    
// ===========================================================================
//
//	Synchronizer for kInternalSync and kSMPTESync mode: 
//

#include "TPlayerSynchroniser.h"
#include "UTools.h"
#include "UDebug.h"


/*--------------------------------------------------------------------------*/

 TPlayerSynchroniserInt::TPlayerSynchroniserInt (TScorePtr score, TSchedulerInterfacePtr scheduler, TRunningStatePtr state,ULONG tpq) 
	: TPlayerSynchroniser(scheduler,state,tpq)
{  
	fTempoTask = new TTempoTask (this);
	fIterator = new TScoreIterator(score);
	fTempoVisitor = new TTempoMapVisitor(tpq);
	fFollower = new TScoreFollower(fIterator,fTempoVisitor);
}


/*--------------------------------------------------------------------------*/
		
TPlayerSynchroniserInt::~TPlayerSynchroniserInt() 
{
 	delete(fTempoTask);
 	//The follower destroy fTempoVisitor and fIterator
 	delete(fFollower);
}

/*--------------------------------------------------------------------------*/

void TPlayerSynchroniserInt::PlaySlice () 
{
	TEventPtr cur; 
	if (!fIterator->IsLastEv()) {
	
		ULONG cur_tempo = fTempoVisitor->GetTempo();
	
		// For all events at the same date
		while (cur = fIterator->NextDateEv()){
			cur->Accept(fTempoVisitor,true);
		}
		
		// If Tempo has changed, update the Scheduler
		if (cur_tempo != fTempoVisitor->GetTempo()){ 
			fScheduler->ReScheduleTasksInt(); 
		}
		
		// Schedule the PlayTask for the date of the next events
		fScheduler->ScheduleTickTaskInt(fTempoTask, fIterator->CurDate());
	}
}

/*--------------------------------------------------------------------------*/

void TPlayerSynchroniserInt::Start ()
{ 
	fState->SetRunning(); // before  ScheduleTickTask
	fFollower->Init();
	fOffset = MidiGetTime();
	fScheduler->ScheduleTickTask(fTempoTask, fIterator->CurDate()); 
	
}	

/*--------------------------------------------------------------------------*/

void TPlayerSynchroniserInt::Stop () 
{ 
	fTempoTask->Forget();
	if (fState->IsRunning()) fTempoVisitor->UpdateMs(MidiGetTime() - fOffset);
	fState->SetIdle();
}

/*--------------------------------------------------------------------------*/

void TPlayerSynchroniserInt::Cont (ULONG date_ticks) 
{ 
	fState->SetRunning(); // before ScheduleTickTask
	SetPosTicks(date_ticks);
	fOffset = MidiGetTime() - fTempoVisitor->CurDateMs();
	fScheduler->ScheduleTickTask(fTempoTask, fIterator->CurDate()); 
}

/*--------------------------------------------------------------------------*/

Boolean TPlayerSynchroniserInt::IsSchedulable (ULONG date_ticks) {return fState->IsRunning();}

/*--------------------------------------------------------------------------*/

ULONG TPlayerSynchroniserInt::GetPosTicks () 
{
	if (fState->IsRunning()) fTempoVisitor->UpdateMs(MidiGetTime() - fOffset);
	return fTempoVisitor->CurDateTicks();
}

/*--------------------------------------------------------------------------*/

void  TPlayerSynchroniserInt::SetPosTicks (ULONG date_ticks)
{  
	fFollower->SetPosTicks(date_ticks);
}