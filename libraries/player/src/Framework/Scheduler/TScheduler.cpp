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
	fTaskIndex = 0;
	
	for (short line = 0 ; line<TableLength; line++) {fTaskTable[line] = 0;}
		
	#if GENERATINGCFM
		fUPPExecuteTask = NewTaskPtr(ExecuteTask);
	#else
		fUPPExecuteTask = (TaskPtr)ExecuteTask;
	#endif
}

/*--------------------------------------------------------------------------*/

TScheduler::~TScheduler ()
{ 
	#if GENERATINGCFM
		if (fUPPExecuteTask) DisposeRoutineDescriptor (fUPPExecuteTask);
	#endif
	
	TTicksTaskPtr task;
	
	for (short line =  0 ; line<TableLength; line++) {
		if (task = fTaskTable[line]) task->Kill();
	}
}

/*--------------------------------------------------------------------------*/
// Re-schedule pending tasks after a Tempo change
/*--------------------------------------------------------------------------*/

void TScheduler::ReScheduleTasks() 
{
	TTicksTaskPtr task;
	for (short line = 0 ; line<TableLength; line++) {
		if (task = fTaskTable[line]) ScheduleRealTime(task);
	}
}

/*--------------------------------------------------------------------------*/
// Schedule a "Tick" task 
/*--------------------------------------------------------------------------*/

void TScheduler::ScheduleTickTask(TTicksTaskPtr task, ULONG date_ticks)
{	
	Boolean res = true;
	
	if (task->IsIdle ()) res = InsertTask(task); // Non inserted Task
	if (res) {
		task->SetDate(date_ticks);
		task->SetRunning();
		ScheduleRealTime(task);
	}
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
		task->Kill(); // Important
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

/*----------------------------------------------------------------------------*/

void  TScheduler::RemoveTask(TTicksTaskPtr task){ fTaskTable[task->GetIndex()] = 0;}

/*----------------------------------------------------------------------------*/
/*
When inserted for the first time, each task obtain a unique index
*/

Boolean  TScheduler::InsertTask(TTicksTaskPtr task)
{
	// Warning : No more that TableLength tasks can be used
	
	if (CheckTaskTable()) {
		// If first time the task is inserted, set a new index
		if (task->GetIndex() < 0) task->SetIndex(fTaskIndex++); 
		fTaskTable[task->GetIndex()] = task;
		return true;
	}else {
		return false;
	}
 } 		


