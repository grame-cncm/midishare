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

#include <Errors.h>
#include <Gestalt.h>
#include <MacMemory.h>
#include <OSUtils.h>
#include <Script.h>
#include <Timer.h>

#include "DriverLoader.h" 
#include "msDriver.h" 
#include "msExtern.h"
//#include "msLoader.h" 
#include "msSync.h" 

/*_________________________________________________________________________*/
/* mac ppc desc structure                                                  */
/*_________________________________________________________________________*/
typedef struct TMachine {
    Boolean    powerManager;       /* Power Manager present       */
    Boolean    virtualMemory;      /* virtual memory is active    */
    SysEnvRec  env;
    TMTask     tm;                 /* Time Manager task record    */
} TMac, * TMacPtr;

typedef struct {
	long contextReg;
} MacPPCContext, * MacPPCContextPtr;

#define GetContext(c)	(((MacPPCContextPtr)c)->contextReg)
#define TimeTask(g)		(((TMacPtr)g->local)->tm)
#define Drivers(g)		(((TMacPtr)g->local)->drivers)


/*_________________________________________________________________________*/
/* application specific part: context, alarm, tasks                        */
/*_________________________________________________________________________*/
static asm long GetContextRegister ()
{
	mr 	r3, r2
}

/*_________________________________________________________________________*/
static asm long SetContextRegister (register long reg)
{
	mr	r3, r2
	mr	r2, reg
}

/*_________________________________________________________________________*/
TApplContextPtr CreateApplContext ()
{
	MacPPCContextPtr ptr = (MacPPCContextPtr)NewPtr (sizeof(MacPPCContext));
	if (ptr) ptr->contextReg = GetContextRegister();
	return ptr;
}

/*_________________________________________________________________________*/
void DisposeApplContext (TApplContextPtr context)
{
	if (context) DisposePtr ((Ptr)context);
}

/*_________________________________________________________________________*/
void DriverWakeUp (TApplPtr appl)
{
	TApplContextPtr context = appl->context;
	if (Wakeup(appl)) {
		WakeupPtr wakeup = Wakeup(appl);
		long oldContext = SetContextRegister (GetContext(context));
    	wakeup (appl->refNum);
		SetContextRegister (oldContext);
	}
}

/*_________________________________________________________________________*/
void DriverSleep (TApplPtr appl)
{
	TApplContextPtr context = appl->context;
	if (Sleep(appl)) {
		SleepPtr sleep = Sleep(appl);
		long oldContext = SetContextRegister (GetContext(context));
 		sleep (appl->refNum);
		SetContextRegister (oldContext);
	}
}

/*_________________________________________________________________________*/
Boolean DriverSlotInfos (TApplPtr appl, SlotRefNum slot, TSlotInfos * infos)
{
	Boolean ret = false;
	TApplContextPtr context = appl->context;
	if (SlotInfo(appl)) {
		SlotInfoPtr slotInf = SlotInfo(appl);
		long oldContext = SetContextRegister (GetContext(context));
   		ret = slotInf (slot, infos);
		SetContextRegister (oldContext);
	}
	return ret;
}

/*_________________________________________________________________________*/
void CallApplAlarm (TApplContextPtr context, ApplAlarmPtr alarm, short refNum, long alarmCode)
{
	long oldContext = SetContextRegister (GetContext(context));
    (*alarm) (refNum, alarmCode);     /* real alarm call */
	SetContextRegister (oldContext);
}

/*_________________________________________________________________________*/
void CallRcvAlarm (TApplContextPtr context, RcvAlarmPtr alarm, short refNum)
{
	long oldContext = SetContextRegister (GetContext(context));
    (*alarm) (refNum);                /* real alarm call */
	SetContextRegister (oldContext);
}

/*_________________________________________________________________________*/
void CallTaskCode  (TApplContextPtr context, MidiEvPtr e)
{
	TTaskExtPtr task = (TTaskExtPtr)LinkST(e);	/* event extension */
	long oldContext = SetContextRegister (GetContext(context));
    (*task->fun)(Date(e), RefNum(e), task->arg1, task->arg2, task->arg3);
	SetContextRegister (oldContext);
}

/*_________________________________________________________________________*/
void CallDTaskCode  (TApplContextPtr context, MidiEvPtr e)
{
	TTaskExtPtr task = (TTaskExtPtr)LinkST(e);	/* event extension */
	long oldContext = SetContextRegister (GetContext(context));
    (*task->fun)(Date(e), RefNum(e), task->arg1, task->arg2, task->arg3);
	SetContextRegister (oldContext);
}


/*_________________________________________________________________________*/
/* synchronization specific part                                           */
/*_________________________________________________________________________*/
Boolean CompareAndSwap (register void **adr, register void *compareTo, register void *swapWith)
{
	register Boolean result;
	register void * tmp;
	asm {
		lwarx	tmp, 0, adr
		cmpw	tmp, compareTo
		bne-	failed
		stwcx.	swapWith, 0, adr
		bne-	failed
		li		result, 1
		b		end
	failed:
		li		result, 0
	end:
	}
	return result;
}


/*__________________________________________________________________________*/
MutexResCode OpenMutex  (MutexRef ref)	{ return kSuccess; }
MutexResCode CloseMutex (MutexRef ref)	{ return kSuccess; }


/*_________________________________________________________________________*/
/* initialization part, including time task                                */
/*_________________________________________________________________________*/
static void InitMachine (TMacPtr mach)
{
	OSErr err; long rep;
	
	err= SysEnvirons( curSysEnvVers, &mach->env);
	if( err== noErr) {
		err= Gestalt( gestaltPowerMgrAttr, &rep);
		if( err== noErr)
			 mach->powerManager= rep & (1 << gestaltPMgrExists);
		else
			 mach->powerManager= false;
		err= Gestalt( gestaltVMAttr, &rep);
		if( err== noErr)
			 mach->virtualMemory= rep & (1 << gestaltVMPresent);
		else
			 mach->virtualMemory= false;
	}
}

/*__________________________________________________________________________*/
static void SaveMemoryPtr (void) 
{
	asm {
		nop
		nop
	}
}

/*__________________________________________________________________________*/
void SaveMemory (TMSGlobalPtr g)
{
	TMSGlobalPtr * gPtr = (TMSGlobalPtr *)SaveMemoryPtr;
	*gPtr = g;
}

/*__________________________________________________________________________*/
static inline TMSGlobalPtr GetMemory()
{
	TMSGlobalPtr * gPtr = (TMSGlobalPtr *)SaveMemoryPtr;
	return *gPtr;
}

/*__________________________________________________________________________*/
static pascal void timeManagerTask(void) 
{
	TMSGlobalPtr g = GetMemory();
	PrimeTime((QElemPtr)&TimeTask(g), 1);
	ClockHandler(g); 
}

/*__________________________________________________________________________*/
void SpecialWakeUp (TMSGlobalPtr g)
{
	g->local = NewPtrSys (sizeof(TMac));
	if (g->local) {
		InitMachine ((TMacPtr)g->local);
		LoadDrivers ();
	}
}

void SpecialSleep  (TMSGlobalPtr g)
{
	if (g->local) {
		DisposePtr ((Ptr)g->local);
	}
	g->local = 0;
}

/*__________________________________________________________________________*/
/* time interrrupt management                                               */
/*__________________________________________________________________________*/
void OpenTimeInterrupts (TMSGlobalPtr g)
{
	TimeTask(g).tmAddr = NewTimerProc(timeManagerTask);
	TimeTask(g).tmWakeUp = 0;
	TimeTask(g).tmReserved = 0;
	InsXTime ((QElemPtr)&TimeTask(g));
	PrimeTime((QElemPtr)&TimeTask(g), -1000);
}

void CloseTimeInterrupts (TMSGlobalPtr g)
{
	QElemPtr task = (QElemPtr)&TimeTask(g);
//	INT_OFF();
	RmvTime(task);
//	INT_ON();
}

