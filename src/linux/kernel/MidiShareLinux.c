/*

  Copyright © Grame 1999

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
  grame@rd.grame.fr
  
  modification history:
   [08-09-99] fifo functions moved to lffifoh
              mem allocation functions moved to mem.c

*/

#include <linux/slab.h>

#include "msLoader.h" 
#include "msExtern.h"
#include "msSync.h" 
#include "msEvents.h" 


/*_________________________________________________________________________*/
/* Linux desc structure                                                  */
/*_________________________________________________________________________*/
typedef struct TMachine {
	struct tq_struct 	timerTask;  /* timer task */
	struct wait_queue*  stopQueue;  /* stop queue (for cleanup_module ) */
	struct timeval		time;		/* for timer management */
} TLinux, * TLinuxPtr;

typedef struct {
	fifo 				commands;		/* fifo of commands : task and alarms */
	struct wait_queue*  commandsQueue;	/* to be used by the user real-time thead */
	uchar wakeFlag;
	
} LinuxContext, * LinuxContextPtr;

static void intrpt_routine(void * arg);


#define GetCommand(c)	(&(((LinuxContextPtr)c)->commands))
#define GetCommandQueue(c)	(&(((LinuxContextPtr)c)->commandsQueue))

/*_________________________________________________________________________*/  
/* application specific part: context, alarm, tasks                        */
/*_________________________________________________________________________*/

/*_________________________________________________________________________*/
TApplContextPtr CreateApplContext ()
{
	LinuxContextPtr ptr = (LinuxContextPtr)kmalloc (sizeof(LinuxContext), GFP_KERNEL);
	fifoinit(&ptr->commands);
	ptr->commandsQueue = 0;
	ptr->wakeFlag = 0;
	return ptr;
}

/*_________________________________________________________________________*/
void DisposeApplContext (TApplContextPtr context)
{
	LinuxContextPtr linuxContext = (LinuxContextPtr)context;
	MidiEvPtr ev, next;
	
	if (context) {
		kfree (context);
		
		// free command fifo
		
		ev = (MidiEvPtr)fifoclear (&linuxContext->commands);
		while( ev) {
			next= Link(ev);
			lfpush (FreeList(Memory(gMem)), (cell *)ev);
			//MSFreeCell(ev,FreeList(Memory(gMem)));  // A REVOIR
			ev= next;
		}
	}
}

/*_________________________________________________________________________*/

static void wakeUp (TApplContextPtr context)
{
	LinuxContextPtr linuxContext = (LinuxContextPtr)context;
	
	if (linuxContext->wakeFlag==0){
		wake_up(GetCommandQueue(context));
		linuxContext->wakeFlag = 1;	
	}
}

/*_________________________________________________________________________*/
void CallApplAlarm (TApplContextPtr context, ApplAlarmPtr alarm, short refNum, long alarmCode)
{
	MidiEvPtr ev;
	
	/* put and applAlarm event in the application command fifo  */
	
	ev = MSNewEv(typeApplAlarm,FreeList(Memory(gMem)));  // A REVOIR

	if (ev) {
	  	MSSetField(ev,0,alarmCode);
	  	fifoput(GetCommand(context), (cell*)ev);
		wakeUp(context);
	}
	
}

/*_________________________________________________________________________*/
void CallRcvAlarm (TApplContextPtr context, RcvAlarmPtr alarm, short refNum)
{
	MidiEvPtr ev;
	
	/* put and rcvAlarm event in the application command fifo  */
	
	ev = MSNewEv(typeRcvAlarm,FreeList(Memory(gMem)));  // A REVOIR

	if (ev) {
		fifoput(GetCommand(context), (cell*)ev);
		wakeUp(context);
	}
	
}

/*_________________________________________________________________________*/
void CallQuit (TApplContextPtr context)
{
	wake_up(GetCommandQueue(context));
}


/*_________________________________________________________________________*/
void CallTaskCode  (TApplContextPtr context, MidiEvPtr ev)
{

	if (ev) {
		fifoput(GetCommand(context), (cell*)ev);
		wakeUp(context);
	}
}

/*_________________________________________________________________________*/
void CallDTaskCode  (TApplContextPtr context, MidiEvPtr e)
{

}


/*_________________________________________________________________________*/
/* synchronization specific part                                           */
/*_________________________________________________________________________*/
Boolean CompareAndSwap (void **adr, void *compareTo, void *swapWith) 
{
	return false;
}

/*__________________________________________________________________________*/
MutexResCode OpenMutex  (MutexRef ref)	{ return kSuccess; }
MutexResCode CloseMutex (MutexRef ref)	{ return kSuccess; }


/*_________________________________________________________________________*/
/* initialization part, including time task                                */
/*_________________________________________________________________________*/
static void InitMachine (TLinuxPtr machine)
{
	machine->timerTask.next = NULL; 				/* Next item in list - queue_task will do  this for us */
	machine->timerTask.sync = 0;    				/* A flag meaning we haven't been inserted into a task queue yet */
	machine->timerTask.routine = intrpt_routine;   	/* The function to run */
	machine->timerTask.data = gMem;        			/* the arg parameter to the interrupt routine : the MidiShare global data */
	machine->stopQueue = NULL;
}

/*__________________________________________________________________________*/

static void intrpt_routine(void * arg)
{
	TMSGlobalPtr g  = (TMSGlobalPtr) arg;
	TLinuxPtr machine = (TLinuxPtr) g->local;
	int i;
	
	/*
	long sec = machine->time.tv_sec;
	long micro = machine->time.tv_usec;
	int n;
	
	do_gettimeofday(&machine->time);
	if (sec == machine->time.tv_sec){
		n = (machine->time.tv_usec - micro)/1000;
	}else {
		n = (machine->time.tv_usec + 1000000 - micro)/1000;
	}
	*/
	
	for (i = 0; i<10; i++) {
		ClockHandler(g);
	}
	
	if (machine->stopQueue != NULL) 
    	wake_up(&machine->stopQueue);   
  	else {
    	queue_task(&machine->timerTask, &tq_timer);  
  	}
	
}


/*__________________________________________________________________________*/
void SpecialWakeUp (TMSGlobalPtr g)
{
	g->local = (void*)kmalloc (sizeof(TLinux), GFP_KERNEL);
	if (g->local) InitMachine (g->local);
	
}

void SpecialSleep  (TMSGlobalPtr g)
{
	if (g->local) kfree (g->local);
	g->local = 0;
}


/*__________________________________________________________________________*/
void OpenTimeInterrupts (TMSGlobalPtr g)
{
	TLinuxPtr machine = (TLinuxPtr) g->local;
	do_gettimeofday(&machine->time);
	queue_task(&machine->timerTask, &tq_timer); 
}

/*__________________________________________________________________________*/
void CloseTimeInterrupts(TMSGlobalPtr g)
{
	TLinuxPtr machine = (TLinuxPtr) g->local;
	sleep_on(&machine->stopQueue);
}

/*__________________________________________________________________________*/
MidiEvPtr MSGetCommand (short refNum, TClientsPtr g)
{
	if( CheckRefNum( g, refNum)) {
		TApplPtr appl = g->appls[refNum]; 
		LinuxContextPtr context = (LinuxContextPtr)appl->context;
		
		if (fifosize(&context->commands) == 0){
			context->wakeFlag=0;
			sleep_on(&context->commandsQueue);
		} 
		return (MidiEvPtr)fifoget(&context->commands);
	}
	return 0; 
}

/*__________________________________________________________________________*/
MidiEvPtr MSGetDTask (short refNum, TClientsPtr g)
{	
	if( CheckRefNum( g, refNum)) {
		TApplPtr appl = g->appls[refNum]; 
		return (MidiEvPtr)fifoget (&appl->dTasks);
	}
	return 0; 
}




