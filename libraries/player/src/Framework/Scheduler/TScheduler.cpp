/*

  Copyright © Grame 1996-2006

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

TScheduler::TScheduler(TSynchroniserInterfacePtr synchro, TMidiApplPtr appl) {Init(synchro,appl);}

/*--------------------------------------------------------------------------*/

void TScheduler::Init(TSynchroniserInterfacePtr synchro, TMidiApplPtr appl)
{
	fSynchro = synchro;
	fMidiAppl = appl;
	fTaskIndex = 0;
	
	for (short line = 0 ; line<TableLength; line++) {fTaskTable[line] = 0;}
		
#if defined (__Macintosh__) && defined (__MacOS9__)
	fUPPExecuteTask = NewTaskPtr(ExecuteTask);
#else
	fUPPExecuteTask = (TaskPtr)ExecuteTask;
#endif
}

/*--------------------------------------------------------------------------*/

TScheduler::~TScheduler()
{ 
#if defined (__Macintosh__) && defined (__MacOS9__)
	if (fUPPExecuteTask) DisposeRoutineDescriptor (fUPPExecuteTask);
#endif
	
	TTicksTaskPtr task;
	
	for (short line =  0 ; line<TableLength; line++) {
		if ((task = fTaskTable[line])) task->Kill();
	}
}

/*--------------------------------------------------------------------------*/
// Re-schedule pending tasks after a Tempo change
/*--------------------------------------------------------------------------*/

void TScheduler::ReScheduleTasks() 
{
	TTicksTaskPtr task;
	
	for (short line = 0 ; line<TableLength; line++) {
		if ((task = fTaskTable[line])) ScheduleRealTime(task);
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

void TScheduler::ExecuteTaskInt(TTicksTaskPtr task, ULONG date_ms) 
{
	RemoveTask(task);
	task->Clear(); // Important
	
	if (task->IsRunning()) {
		task->SetIdle();
		task->Execute(fMidiAppl,date_ms);
	} else {   	   // Forgeted task 
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

void MSALARMAPI ExecuteTask(ULONG date_ms, short refnum, long scheduler, long task, long a3) 
{
	((TSchedulerPtr)scheduler)->ExecuteTaskInt((TTicksTaskPtr)task,date_ms);
}

/*----------------------------------------------------------------------------*/

void  TScheduler::RemoveTask(TTicksTaskPtr task) { fTaskTable[task->GetIndex()] = 0;}

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
	} else {
		return false;
	}
 } 		


