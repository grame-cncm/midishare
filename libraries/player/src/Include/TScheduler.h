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
		virtual void ScheduleTickTaskInt(TTicksTask* task, ULONG date_ticks) = 0;
		virtual void ReScheduleTasks() = 0;
		virtual void ReScheduleTasksInt() = 0;
};

typedef TSchedulerInterface FAR * TSchedulerInterfacePtr;

//-----------------------
// Class TScheduler 
//-----------------------

class TScheduler :public TSchedulerInterface{

	friend class TTicksTask;

	private:
	
		TTicksTask* fHeadTasks;
		
		#if GENERATINGCFM
			UPPTaskPtr fUPPScheduleTask;
			UPPTaskPtr fUPPReScheduleTask;
			UPPTaskPtr fUPPExecuteTask;
		#else
			TaskPtr fUPPScheduleTask;
			TaskPtr fUPPReScheduleTask;
			TaskPtr fUPPExecuteTask;
		#endif
		
		void ScheduleRealTime(TTicksTask* task);
		void RemoveTask(TTicksTask* task);
		void InsertTask(TTicksTask* task);
				
	public :
		TSynchroniserInterfacePtr  fSynchro;
		TMidiApplPtr 			   fMidiAppl;
		
		TScheduler() {}
		TScheduler(TSynchroniserInterfacePtr  synchro, TMidiApplPtr appl);
		void Init(TSynchroniserInterfacePtr  synchro, TMidiApplPtr appl);
		
 		virtual ~TScheduler();
 		
 		
 		void ScheduleTickTask(TTicksTask* task, ULONG date_ticks);	
 		void ScheduleTickTaskInt(TTicksTask* task, ULONG date_ticks);
 		void ReScheduleTasks();
 		void ReScheduleTasksInt();
 		
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
*             : kTaskIdle  ==> Forget ==> kTaskIdle (ATTENTION reste dans l'Žtat kTaskIdle)
*/


class TTicksTask {
	
	
	friend class TScheduler;
	enum taskState { kTaskIdle = 0, kTaskRunning, kTaskForget};
		
	private:
	
		MidiEvPtr 	fTask;
		TTicksTask* fNextTask;
		TScheduler* fScheduler;
		ULONG 		fDate_ticks;
		Boolean		fStatus;
		
		void		SetNextTask(TTicksTask* inTask){ fNextTask = inTask;}
		TTicksTask*	GetNextTask(){ return fNextTask;}
		
		void SetIdle() 		{fStatus = kTaskIdle;}
		void SetRunning() 	{fStatus = kTaskRunning;}
		
		Boolean IsRunning () {return (fStatus == kTaskRunning);}
		Boolean IsIdle () 	 {return (fStatus == kTaskIdle);}
	
		void Clear () {MidiForgetTask(&fTask);}
		void SetScheduler(TScheduler* sh)  {fScheduler = sh;}
		
		void SetDate(ULONG date) { fDate_ticks = date;}
		ULONG GetDate() { return fDate_ticks;}
		
	public:
	
		TTicksTask() 
		{
			fTask = 0; 
			fScheduler = 0; 
			fNextTask = 0; 
			fDate_ticks = 0;
			fStatus = kTaskIdle;
		}
		// A REVOIR (risque de conflit avec les taches temps rŽel)
		virtual ~TTicksTask() {MidiForgetTask(&fTask); if (fScheduler) fScheduler->RemoveTask(this);}
		void Forget () { if (IsRunning ()) fStatus = kTaskForget;}
		
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