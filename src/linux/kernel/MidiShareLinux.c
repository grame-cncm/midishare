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
	      
   [19-02-01] SL - CallQuitAction removed, use of pthread_cancel in the library
   [22-06-01] SL - New CloseAll function to close remaining application associated with a file descriptor
             
*/
  
#ifdef MODVERSIONS
#include <linux/modversions.h>
#endif

/* centralizing compatibility issues between 2.0, 2.2, 2.4 */

#ifndef LINUX_VERSION_CODE
#  include <linux/version.h>
#endif

#ifndef VERSION_CODE
#  define VERSION_CODE(vers,rel,seq) ( ((vers)<<16) | ((rel)<<8) | (seq) )
#endif

/* remember about the current version */

#if LINUX_VERSION_CODE < VERSION_CODE(2,1,0)
#  define LINUX_20
#elif LINUX_VERSION_CODE < VERSION_CODE(2,3,0)
#  define LINUX_22
#else
#  define LINUX_24
# endif

/* wait queue macros */

#ifdef LINUX_24
#define NEW_WAIT_QUEUE  wait_queue_head_t  
#define INIT_WAIT_QUEUE(q) init_waitqueue_head(&q)
#else 
#define NEW_WAIT_QUEUE struct wait_queue* 
#define INIT_WAIT_QUEUE(q) (q) = NULL
#endif


#include <linux/slab.h>
#include <linux/poll.h>

#include "msLoader.h" 
#include "msExtern.h"
#include "msSync.h" 
#include "msEvents.h" 
#include "msAppFun.h" 
#include "msMem.h" 
/*_______________________________________________________________________*/
/* Linux desc structure                                                  */
/*_______________________________________________________________________*/

typedef struct TMachine {
	struct tq_struct 	timerTask;  /* timer task 						*/
	NEW_WAIT_QUEUE		stopQueue;	/* stop queue (for cleanup_module ) */
	struct timeval		time;		/* for timer management 			*/
	long				phase;	/* used to count ClockHandler calls	*/
	Boolean 			status;    	/* running mode  					*/
} TLinux, * TLinuxPtr;

typedef struct {
	fifo 			commands;	/* fifo of commands : task and alarms */
	NEW_WAIT_QUEUE		commandsQueue; 	/* to be used by the user real-time thread */
	Boolean 		wakeFlag;	/* used for wake up of the real-time thread */
	Boolean 		status;    	/* running mode : user or kernel */
	struct file*            file;		/* file descriptor */
	
} LinuxContext, * LinuxContextPtr;

static void TimerTask(void * arg);

/* 
  	The machine structure (TLinux structure) is globally allocated so that the TimerTask 
   	can still access it after CloseTimerInterrupt has been called.
   	The CloseAll function is called in a context where interruptible_sleep_on
   	does not suspend the calling code. Thus a synchronization bug between
	the TimerTask and the cleanup code (MidiShareSleep) was occuring.
*/

static TLinux  gMachine ;



#define GetCommand(c)		(&(((LinuxContextPtr)c)->commands))
#define GetCommandQueue(c)	(&(((LinuxContextPtr)c)->commandsQueue))


/*_________________________________________________________________________*/  
/* application specific part: context, alarm, tasks                        */
/*_________________________________________________________________________*/

void FlushCommandFifo(TApplContextPtr context)
{
	LinuxContextPtr linuxContext = (LinuxContextPtr)context;
	MidiEvPtr ev, next;
	
	/* free command fifo */
		
	ev = (MidiEvPtr)fifoclear (&linuxContext->commands);
	while(ev) {
		next= Link(ev);
		MSFreeEv(ev,FreeList(Memory(gMem))); 
		ev= next;
	}
}

/*_________________________________________________________________________*/

TApplContextPtr CreateApplContext ()
{
	LinuxContextPtr ptr = (LinuxContextPtr)kmalloc(sizeof(LinuxContext), GFP_KERNEL);
	fifoinit(&ptr->commands);
	INIT_WAIT_QUEUE(ptr->commandsQueue);
	ptr->wakeFlag = false;
	ptr->status = kKernelMode;
	ptr->file = 0;
	return ptr;
}

/*_________________________________________________________________________*/

void DisposeApplContext (TApplContextPtr context)
{
	if (context) {
		FlushCommandFifo(context);
		kfree (context);
	}
}

/*_________________________________________________________________________*/
/* close all remaining applications associated with a file descriptor */

void CloseAll (TMSGlobalPtr gm, struct file* f)
{
	TClientsPtr g = Clients(gm);
	int i;
	
	for (i = 0; i < MaxAppls; i++){
		TApplPtr appl = g->appls[i];
		if (appl) {
			LinuxContextPtr c = (LinuxContextPtr)appl->context;
			if (c && (c->file == f)) MSClose(i,gm);
		}
	}

}

/*_________________________________________________________________________*/

void SetUserMode (short refnum, TClientsPtr g,struct file* f)
{
	if (CheckRefNum(g, refnum)) {
		TApplPtr appl = g->appls[refnum];
		if (appl) {
			LinuxContextPtr c = (LinuxContextPtr)appl->context;
			if (c) {
				c->status = kUserMode;
				c->file = f;
			}
		}
	}
}

/*_________________________________________________________________________*/

static void wakeUp (TApplContextPtr context)
{
	LinuxContextPtr linuxContext = (LinuxContextPtr)context;
	
	if (!linuxContext->wakeFlag){
		wake_up_interruptible(GetCommandQueue(context));
		linuxContext->wakeFlag = true;	
	}
}

/*_________________________________________________________________________*/

void DriverWakeUp (TApplPtr appl)
{
	LinuxContextPtr context = (LinuxContextPtr)appl->context;
	
	if (context->status) {
		if (Wakeup(appl)) Wakeup(appl)(appl->refNum);
	}
	else {
	}
}

/*_________________________________________________________________________*/

void DriverSleep  (TApplPtr appl)
{
	LinuxContextPtr context = (LinuxContextPtr)appl->context;
	
	if (context->status) {
		if (Sleep(appl)) Sleep(appl)(appl->refNum);
	}
	else {
	}
}

/*_________________________________________________________________________*/

void CallApplAlarm (TApplContextPtr c, ApplAlarmPtr alarm, short refNum, long alarmCode)
{
	MidiEvPtr ev;
	LinuxContextPtr context = (LinuxContextPtr)c;
	
	if (context->status) {
		alarm (refNum, alarmCode);
	}else {
		/* put an applAlarm event in the application command fifo  */	
		ev = MSNewEv(typeApplAlarm,FreeList(Memory(gMem)));  // A REVOIR

		if (ev) {
		  	MSSetField(ev,0,alarmCode);
		  	fifoput(GetCommand(context), (cell*)ev);
			wakeUp(context);
		}
	}
}

/*_________________________________________________________________________*/

void CallRcvAlarm (TApplContextPtr c, RcvAlarmPtr alarm, short refNum)
{
	MidiEvPtr ev;
	LinuxContextPtr context = (LinuxContextPtr)c;
	
	if (context->status) {
		alarm (refNum);	
	}
	else {
		/* put a rcvAlarm event in the application command fifo  */
		ev = MSNewEv(typeRcvAlarm,FreeList(Memory(gMem)));  // A REVOIR

		if (ev) {
			fifoput(GetCommand(context), (cell*)ev);
			wakeUp(context);
		}
	}
}

/*_________________________________________________________________________*/

void CallTaskCode  (TApplContextPtr c, MidiEvPtr e)
{
	LinuxContextPtr context = (LinuxContextPtr)c;
	
	if (e) {
		if (context->status) {
			TTaskExtPtr task = (TTaskExtPtr)LinkST(e);
			(*task->fun)(Date(e), RefNum(e), task->arg1, task->arg2, task->arg3);
		}
		else {
			fifoput(GetCommand(context), (cell*)e);
			wakeUp(context);
		}
	}
}

/*_________________________________________________________________________*/
/* not implemented */

void CallDTaskCode  (TApplContextPtr c, MidiEvPtr e)
{
	LinuxContextPtr context = (LinuxContextPtr)c;
	if (context->status && e) {
		TTaskExtPtr task = (TTaskExtPtr)LinkST(e);
		(*task->fun)(Date(e), RefNum(e), task->arg1, task->arg2, task->arg3);
	}

}

/*_________________________________________________________________________*/
/* synchronization specific part                                           */
/*_________________________________________________________________________*/

Boolean MSCompareAndSwap (void **adr, void *compareTo, void *swapWith) 
{
	return false;
}

/*__________________________________________________________________________*/
void msOpenMutex  (unsigned int mutex) {}
void msCloseMutex  (unsigned int mutex) {}

/*_________________________________________________________________________*/
/* initialization part, including time task                                */
/*_________________________________________________________________________*/

static void InitMachine (TLinuxPtr machine)
{
	#ifndef LINUX_24
	machine->timerTask.next = NULL;			/* Next item in list - queue_task will do this for us */
	#endif
	machine->timerTask.sync = 0;    		/* A flag meaning we haven't been inserted into a task queue yet */
	machine->timerTask.routine = TimerTask;   	/* The function to run */
	machine->timerTask.data = gMem;        		/* the arg parameter to the interrupt routine : the MidiShare global data */
	INIT_WAIT_QUEUE(machine->stopQueue);
	machine->phase = 0;
	machine->status = true;
}

/*__________________________________________________________________________*/


/* 	The timer resolution is defined by constant HZ. In order
	to call ClockHandler() 1000 times per second, we have to 
	call ClockHandler() 1000/HZ times per TimerTask. We have also
	to take into account that HZ may not be a divider of 1000 or
	can even be greater than 1000 hz.	
*/	
static void TimerTask(void * arg)
{
	TMSGlobalPtr g  = (TMSGlobalPtr) arg;
	TLinuxPtr machine = (TLinuxPtr) g->local;
	
	if (machine->status == 0) {
		wake_up_interruptible(&machine->stopQueue);   
  	} else {
		long i = machine->phase;
		while (i<1000) { ClockHandler(g); i+=HZ; }
		machine->phase = i-1000;
		queue_task(&machine->timerTask, &tq_timer);  
  	}
}

/*__________________________________________________________________________*/

void SpecialWakeUp (TMSGlobalPtr g)
{
	/* access the global structure */
	g->local = &gMachine;  
	if (g->local) InitMachine (g->local);
}

/*__________________________________________________________________________*/

void SpecialSleep  (TMSGlobalPtr g){}

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
	machine->status = false;
	interruptible_sleep_on(&machine->stopQueue);
}

/*__________________________________________________________________________*/

MidiEvPtr MSAvailCommand (short refNum, TClientsPtr g)
{
	if( CheckRefNum( g, refNum)) {
		TApplPtr appl = g->appls[refNum]; 
		LinuxContextPtr context = (LinuxContextPtr)appl->context;
		
		if (fifosize(&context->commands) == 0){
			context->wakeFlag = false;
			interruptible_sleep_on(&context->commandsQueue);
		} 
		return (MidiEvPtr)fifoavail(&context->commands);
	}
	return 0; 
}

/*__________________________________________________________________________*/

MidiEvPtr MSGetCommand (short refNum, TClientsPtr g)
{
	if( CheckRefNum( g, refNum)) {
		TApplPtr appl = g->appls[refNum]; 
		LinuxContextPtr context = (LinuxContextPtr)appl->context;
		
		if (fifosize(&context->commands) == 0){
			context->wakeFlag = false;
			interruptible_sleep_on(&context->commandsQueue);
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

/*__________________________________________________________________________*/

unsigned int MSPoll(short refNum, TClientsPtr g, struct file * f, poll_table * wait)
{
	unsigned int mask = 0;
	
	if( CheckRefNum( g, refNum)) {
		TApplPtr appl = g->appls[refNum]; 
		LinuxContextPtr context = (LinuxContextPtr)appl->context;
		poll_wait(f, &context->commandsQueue, wait);
		mask |= POLLIN | POLLRDNORM;  
	}
	
	return mask; 
}

/*__________________________________________________________________________*/

Boolean ForgetTaskSync (MidiEvPtr * taskPtr, MidiEvPtr content)
{
	if (*taskPtr == content) {
      		EvType(content) = typeDead;
    		*taskPtr = 0;
    		return true;
	}
	return false;
}

/*_________________________________________________________________________*/
/* memory allocation implementation                                        */
/*_________________________________________________________________________*/

FarPtr(void) AllocateMemory (MemoryType type, unsigned long size)
{ 
	return (void*)kmalloc(size,GFP_KERNEL);
}

/*_________________________________________________________________________*/

void DisposeMemory  (FarPtr(void) memPtr)
{
	if (memPtr) kfree(memPtr);
}



