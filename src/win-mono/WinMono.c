/*

  Copyright © Grame, Sony CSL-Paris 2001

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
  grame@grame.fr
  
  modifications history:

*/

#include "msExtern.h"
#include "windows.h"
#include "msMem.h"
#include "msPrefs.h"

/*------------------------------------------------------------------------------*/
/* windows specific resources		*/
typedef struct {
	 UINT 	wTimerID;
	 UINT 	wTimerRes;
} WinTimer, * WinTimerPtr;

typedef struct WinDriver WinDriver, * WinDriverPtr;
struct WinDriver {
	WinDriverPtr	next;
	HINSTANCE		libRef;
};

typedef struct {
	WinTimer	timer;
	WinDriverPtr drivers;
} WinRsrc, * WinRsrcPtr;

WinRsrc gWinRsrc = { 0 };

MutexResCode msOpenMutex  (MutexRef ref) { return kSuccess; }
MutexResCode msCloseMutex (MutexRef ref) { return kSuccess; }
/*------------------------------------------------------------------------------*/
Boolean CompareAndSwap (FarPtr(void) *adr, FarPtr(void) compareTo, FarPtr(void) swapWith)
{
	*adr = swapWith;
	return true;
}

/*------------------------------------------------------------------------------*/
/*                      initializations : wakeup & sleep                        */
/*------------------------------------------------------------------------------*/
static Boolean LoadDriver (char *drvName) 
{
	WinDriverPtr mem = (WinDriverPtr)AllocateMemory (kStdMemory, sizeof(WinDriver));
	if (!mem) return false;
	mem->next = gWinRsrc.drivers;
	mem->libRef = LoadLibrary (drvName);
	if (mem->libRef) {
 		gWinRsrc.drivers = mem;
	}
	else {
		DisposeMemory (mem);
		return false;
	}
	return true;
}

void SpecialWakeUp (TMSGlobalPtr g) 
{
	unsigned short i, n = CountDrivers ();
	char str[256];
	for (i=0; i<n; i++) {
		if (GetDriver (i, str, 256)) {
			LoadDriver (str);
		}
	}
}

void SpecialSleep  (TMSGlobalPtr g)
{
	WinDriverPtr next, drv = gWinRsrc.drivers;
	gWinRsrc.drivers = 0;
	while (drv) {
		next = drv->next;
		FreeLibrary (drv->libRef);
		DisposeMemory (drv);
		drv = next;
	}
}

/*------------------------------------------------------------------------------*/
/*                   client applications context and tasks                      */
/*------------------------------------------------------------------------------*/
TApplContextPtr CreateApplContext ()							{ return 0; }
void			DisposeApplContext (TApplContextPtr context)	{ }

/*_________________________________________________________________________*/
void CallApplAlarm (TApplContextPtr context, ApplAlarmPtr alarm, short refNum, long alarmCode)
{
     (*alarm) (refNum, alarmCode);     /* real alarm call */
}

/*_________________________________________________________________________*/
void CallRcvAlarm (TApplContextPtr context, RcvAlarmPtr alarm, short refNum)
{
    (*alarm) (refNum);                /* real alarm call */
 }

/*_________________________________________________________________________*/
void CallTaskCode  (TApplContextPtr context, MidiEvPtr e)
{
	TTaskExtPtr task = (TTaskExtPtr)LinkST(e);      /* event extension */
    (*task->fun)(Date(e), RefNum(e), task->arg1, task->arg2, task->arg3);
}

/*_________________________________________________________________________*/
void CallDTaskCode  (TApplContextPtr context, MidiEvPtr e)
{
	TTaskExtPtr task = (TTaskExtPtr)LinkST(e);      /* event extension */
	(*task->fun)(Date(e), RefNum(e), task->arg1, task->arg2, task->arg3);
}

/*_________________________________________________________________________*/
void DriverWakeUp (TApplPtr appl) 
{
	WakeupPtr wakeup = Wakeup(appl);
	if (wakeup) {
        wakeup (appl->refNum);
	}
}

/*_________________________________________________________________________*/
void DriverSleep (TApplPtr appl)
{
	SleepPtr sleep = Sleep(appl);
	if (sleep) {
		sleep (appl->refNum);
	}
}

/*__________________________________________________________________________*/
/*	Interrupt handlers														*/
/*__________________________________________________________________________*/
static void CALLBACK TimerProc(UINT wID,UINT wMsg,DWORD dwUser,DWORD dw1,DWORD dw2)
{
	ClockHandler((TMSGlobalPtr)dwUser);
}

void OpenTimeInterrupts(TMSGlobalPtr g)
{
	TIMECAPS tc;
	MMRESULT res;
	WinTimerPtr t = &gWinRsrc.timer;

	if ( timeGetDevCaps(&tc,sizeof(TIMECAPS)) == TIMERR_NOERROR ) {
		t->wTimerRes = 1;
		res = timeBeginPeriod (t->wTimerRes);	

		if (res != TIMERR_NOERROR) return;
		t->wTimerID = timeSetEvent(1,t->wTimerRes,TimerProc,(DWORD)g,TIME_PERIODIC);
	}
}

/*_________________________________________________________________________*/
void CloseTimeInterrupts(TMSGlobalPtr g)
{
	MMRESULT res;
	WinTimerPtr t = &gWinRsrc.timer;

	if (t->wTimerID ) {
		res = timeKillEvent(t->wTimerID);
		t->wTimerID = 0;
	}
	timeEndPeriod (t->wTimerRes);
}
