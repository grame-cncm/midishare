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
//	TScheduler.h		    
// ===========================================================================
//
// A TScheduler object allows to schedule Tasks which dates are in ticks, using a
// synchroniser object to convert dates in ticks in dates in millisecond.
//
// The scheduler maintains a list of pending tasks, rescheduling them if
// necessary after a Tempo change.


#ifndef __TScheduler__
#define __TScheduler__

 
#include "TMidiAppl.h"
#include "TSynchroniserInterface.h"

//---------------------------
// Class TSchedulerInterface
//---------------------------

class TTicksTask;

class TSchedulerInterface{

	public:
	
		virtual ~TSchedulerInterface(){};
		
		virtual void Init(TSynchroniserInterfacePtr  synchro, TMidiApplPtr appl) = 0;
		virtual void ScheduleTickTask(TTicksTask* task, ULONG date_ticks) = 0;
		virtual void ReScheduleTasks() = 0;
};

typedef TSchedulerInterface FAR * TSchedulerInterfacePtr;

//-----------------------
// Class TScheduler 
//-----------------------

/*

The scheduler use a fixed length task table, this in order to avoid having to lock/unlock 
a shared data structure when several threads (normal thread and task execution thread)
access this data structure to insert/remove tasks.

Each task inserted in the scheduler will be associated with a unique number used to 
index the task in the tasktable.

This index number is set in the task data structure the first time the task is inserted
in the tasktable. (see InsertTask method)

In the context of the Player, there should be no more than 3 tasks in the tasktable:
- playtask for the tickplayer
- synchro task (when in kInternalSync mode)
- clock task (when in kClockSyncOut mode)

A more general data structure would have to be defined if the scheduler/ticktask classes
are used in a more demanding use.

*/

#define TableLength 8

class TScheduler :public TSchedulerInterface{

	friend class TTicksTask;

	private:
	
		ULONG 		fTaskIndex;
		TTicksTask* fTaskTable[TableLength];
		
		#if GENERATINGCFM
			UPPTaskPtr fUPPExecuteTask;
		#else
			TaskPtr fUPPExecuteTask;
		#endif
		
		void ScheduleRealTime(TTicksTask* task);
		void RemoveTask(TTicksTask* task);
		Boolean InsertTask(TTicksTask* task);
		Boolean CheckTaskTable() {return (fTaskIndex < TableLength);}
				
	public :
		TSynchroniserInterfacePtr  fSynchro;
		TMidiApplPtr 			   fMidiAppl;
		
		TScheduler() {}
		TScheduler(TSynchroniserInterfacePtr  synchro, TMidiApplPtr appl);
		void Init(TSynchroniserInterfacePtr  synchro, TMidiApplPtr appl);
		
 		virtual ~TScheduler();
 		
 		
 		void ScheduleTickTask(TTicksTask* task, ULONG date_ticks);	
 		void ReScheduleTasks();
 		
 		// Internal functions made public to be called from tasks
 		
 		void ExecuteTaskInt(TTicksTask* task, ULONG date);		
 		
   };

typedef TScheduler FAR * TSchedulerPtr;


//-----------------------
// Class TTicksTask 
//-----------------------
/*
* Task state : kTaskIdle, kTaskForget or kTaskRunning
*
* Transition  : kTaskIdle  ==> Schedule ==> kTaskRunning
*			  : kTaskForget  ==> Execute ==> kTaskIdle
*             : kTaskRunning  ==> Execute ==> kTaskIdle
*             : kTaskRunning  ==> Forget ==> kTaskForget
*             : kTaskForget  ==> Schedule ==> kTaskRunning
*             : kTaskIdle  ==> Forget ==> kTaskIdle (ATTENTION reste dans l'état kTaskIdle)
*/


class TTicksTask {
	
	
	friend class TScheduler;
	enum taskState { kTaskIdle = 0, kTaskRunning, kTaskForget};
		
	private:
	
		MidiEvPtr 	fTask;
		ULONG 		fDate_ticks;
		long		fIndex;
		Boolean		fStatus;
	
		void SetIdle() 		{fStatus = kTaskIdle;}
		void SetRunning() 	{fStatus = kTaskRunning;}
		
		void Clear () {fTask = 0;}
		void Kill () {MidiForgetTask(&fTask);}
		
		void  SetDate(ULONG date) {fDate_ticks = date;}
		ULONG GetDate() {return fDate_ticks;}
		long GetIndex() {return fIndex;}
		void  SetIndex(long index) {fIndex = index;}
			
	public:
	
		TTicksTask() 
		{
			fTask = 0; 
			fDate_ticks = 0;
			fStatus = kTaskIdle;
			fIndex = -1;
		}
		// A REVOIR (risque de conflit avec les taches temps réel)
		virtual ~TTicksTask() {MidiForgetTask(&fTask);}
		void Forget () { if (IsRunning ()) fStatus = kTaskForget;}
		
		Boolean IsRunning () 	{return (fStatus == kTaskRunning);}
		Boolean IsIdle () 	 	{return (fStatus == kTaskIdle);}
		
		
		virtual void Execute (TMidiAppl* , ULONG date){} // Must be implemented for concrete tasks
		
	
};

typedef TTicksTask FAR * TTicksTaskPtr;

//-----------------------------------
// Prototypes for Real-time callback
//-----------------------------------

void MSALARMAPI ScheduleTask (ULONG date, short refnum, long a1, long a2, long a3); 
void MSALARMAPI ReScheduleTask (ULONG date, short refnum, long a1, long a2, long a3); 
void MSALARMAPI ExecuteTask (ULONG date, short refnum, long a1, long a2, long a3); 

#endif