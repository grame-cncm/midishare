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
//	TScheduler.cpp		    
// ===========================================================================
//
// A TScheduler object allows to schedule Tasks which dates are in ticks, using a
// synchroniser object to convert dates in ticks in dates in millisecond.
//
// The scheduler maintains a list of pending tasks, rescheduling them if
// necessary after a Tempo change.


#include "TScheduler.h"
#include "UMidi.h"
#include "UTools.h"

/*--------------------------------------------------------------------------*/

TScheduler::TScheduler(TSynchroniserInterfacePtr synchro, TMidiApplPtr appl){ Init(synchro,appl); }

/*--------------------------------------------------------------------------*/

void TScheduler::Init(TSynchroniserInterfacePtr synchro, TMidiApplPtr appl)
{
	fSynchro = synchro;
	fMidiAppl = appl;
	fHeadTasks = 0;
		
	#if GENERATINGCFM
		fUPPScheduleTask = NewTaskPtr(ScheduleTask);
		fUPPReScheduleTask = NewTaskPtr(ReScheduleTask);
		fUPPExecuteTask = NewTaskPtr(ExecuteTask);
	#else
		fUPPScheduleTask = (TaskPtr)ScheduleTask;
		fUPPReScheduleTask = (TaskPtr)ReScheduleTask;
		fUPPExecuteTask = (TaskPtr)ExecuteTask;
	#endif
}

/*--------------------------------------------------------------------------*/

TScheduler::~TScheduler ()
{ 
	#if GENERATINGCFM
		if (fUPPScheduleTask) DisposeRoutineDescriptor (fUPPScheduleTask);
		if (fUPPReScheduleTask) DisposeRoutineDescriptor (fUPPReScheduleTask);
		if (fUPPExecuteTask) DisposeRoutineDescriptor (fUPPExecuteTask);
	#endif
	
	TTicksTaskPtr task = fHeadTasks;
	fHeadTasks = 0; 
	
	while (task) {
		task->Clear();
		task->SetScheduler(0); // No more link on the scheduler
		task = task->GetNextTask();
	}
}

/*--------------------------------------------------------------------------*/
// Re-schedule pending tasks after a Tempo change
/*--------------------------------------------------------------------------*/

void TScheduler::ReScheduleTasks() 
{
	fMidiAppl->NewMidiCall(fUPPReScheduleTask, MidiGetTime(),(long)this,0,0);
}

/*--------------------------------------------------------------------------*/
// Re-schedule pending tasks after a Tempo change : non protected
/*--------------------------------------------------------------------------*/

void TScheduler::ReScheduleTasksInt() 
{
	TTicksTaskPtr task = fHeadTasks;
	while (task) {
		ScheduleRealTime(task);
		task = task->GetNextTask();
	}
}

/*--------------------------------------------------------------------------*/
// Schedule a "Tick" task 
/*--------------------------------------------------------------------------*/

void TScheduler::ScheduleTickTask(TTicksTaskPtr task, ULONG date_ticks)
{	
	fMidiAppl->NewMidiCall(fUPPScheduleTask, MidiGetTime(),(long)this,(long)task,date_ticks);
}

/*--------------------------------------------------------------------------*/
// Schedule a "Tick" task : non protected
/*--------------------------------------------------------------------------*/

void TScheduler::ScheduleTickTaskInt(TTicksTaskPtr task, ULONG date_ticks)
{	
	task->SetDate(date_ticks);
	if (task->IsIdle ()) InsertTask(task); // Non inserted Task
	task->SetRunning();
	ScheduleRealTime(task);
}

/*--------------------------------------------------------------------------*/
// Internal functions
/*--------------------------------------------------------------------------*/

void TScheduler::ExecuteTaskInt (TTicksTaskPtr task, ULONG date_ms) 
{
	RemoveTask(task);
	task->Clear(); // Important
	
	if (task->IsRunning()) {
		task->SetIdle();
		task->Execute(fMidiAppl,date_ms);
	}else {   	   // Forgeted task 
		task->SetIdle();
	}
}

/*--------------------------------------------------------------------------*/

void TScheduler::ScheduleRealTime(TTicksTaskPtr task)
{
	if (fSynchro->IsSchedulable(task->GetDate())) {
		task->Clear(); // Important
		fMidiAppl->NewMidiTask(fUPPExecuteTask, fSynchro->ConvertTickToMs(task->GetDate()),(long)this,(long)task,0, &task->fTask);
	}
}		

/*--------------------------------------------------------------------------*/
// Real-Time callback
/*--------------------------------------------------------------------------*/

void MSALARMAPI ExecuteTask (ULONG date_ms, short refnum, long scheduler, long task, long a3) 
{
	((TSchedulerPtr)scheduler)->ExecuteTaskInt((TTicksTaskPtr)task,date_ms);
}

/*--------------------------------------------------------------------------*/

void MSALARMAPI ReScheduleTask (ULONG date_ms, short refnum, long scheduler, long a2, long a3) 
{
	((TSchedulerPtr)scheduler)->ReScheduleTasksInt(); 
}

/*--------------------------------------------------------------------------*/

void MSALARMAPI ScheduleTask (ULONG date_ms, short refnum, long scheduler, long task, long a3) 
{
	((TSchedulerPtr)scheduler)->ScheduleTickTaskInt((TTicksTaskPtr)task,a3);
}

/*----------------------------------------------------------------------------*/

void  TScheduler::RemoveTask(TTicksTaskPtr task)
{
	TTicksTaskPtr currTask = fHeadTasks;
	TTicksTaskPtr prevTask = 0;
	
	while (currTask) {				// Search for the specified Task
		if (currTask == task) {
									// Found it. Adjust linked list.
			if (prevTask) {
				prevTask->SetNextTask(currTask->GetNextTask());
			} else {
				fHeadTasks = currTask->GetNextTask();
			}
			task->SetNextTask(0);
			break;
			
		} else {					// Continue search
			prevTask = currTask;
			currTask = currTask->GetNextTask();
		}
	}
}

/*----------------------------------------------------------------------------*/

void  TScheduler::InsertTask(TTicksTaskPtr task)
{
	task->SetNextTask(fHeadTasks);
	task->SetScheduler(this);
	fHeadTasks = task;
} 		