/*

  Copyright © Grame 2002

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

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "msKernel.h"
#include "msExtern.h"
#include "msMem.h"
#include "msTasks.h"

#include "msServerContext.h"
#include "msRTListenProc.h"
#include "msKernelPrefs.h"

MutexResCode msOpenMutex  (MutexRef ref) { return kSuccess; }
MutexResCode msCloseMutex (MutexRef ref) { return kSuccess; }
/*------------------------------------------------------------------------------*/
Boolean MSCompareAndSwap (FarPtr(void) *adr, FarPtr(void) compareTo, FarPtr(void) swapWith)
{
        *adr = swapWith;
        return true;
}

static void * gDrvRefs[MaxDrivers] = { 0 };
/*------------------------------------------------------------------------------*/
/*                      Drivers loading                     					*/
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/*                      initializations : wakeup & sleep                        */
/*------------------------------------------------------------------------------*/
static void * LoadDriver (char *drvName) 
{
	printf ("load driver %s\n", drvName);
#ifdef WIN32
	return LoadLibrary (drvName);
#endif
}
static void UnloadDriver (void * drvref) 
{
	printf ("unload driver %lx\n", (long)drvref);
#ifdef WIN32
	FreeLibrary ((HMODULE)drvref);
#endif
}

/*------------------------------------------------------------------------------*/
void SpecialWakeUp (TMSGlobalPtr g) 
{
	unsigned short i, n = CountDrivers ();
	for (i=0; i<n; i++) {
		gDrvRefs[i] = LoadDriver (GetDriverName(i));
		if (!gDrvRefs[i]) 
			fprintf (stderr, "LoadLibrary error\n");
	}
}


/*------------------------------------------------------------------------------*/
void SpecialSleep  (TMSGlobalPtr g)
{
	unsigned short i;
	for (i=0; i<MaxDrivers; i++) {
		if (gDrvRefs[i]) UnloadDriver(gDrvRefs[i]);
		gDrvRefs[i] = 0;
	}

/*	WinDriverPtr next, drv = gWinRsrc.drivers;
	gWinRsrc.drivers = 0;
	while (drv) {
		next = drv->next;
		FreeLibrary (drv->libRef);
		DisposeMemory (drv);
		drv = next;
	}
*/
}

/*------------------------------------------------------------------------------*/
/*                   client applications context and tasks                      */
/*------------------------------------------------------------------------------*/
TApplContextPtr CreateApplContext ()     { return 0; }
void  DisposeApplContext (TApplContextPtr context)    { }

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
	WakeupPtr wakeupf = Wakeup(appl);
	if (wakeupf) {
		wakeupf (pub(appl, refNum));
	}
}

/*_________________________________________________________________________*/
void DriverSleep (TApplPtr appl)
{
	SleepPtr sleepf = Sleep(appl);
	if (sleepf) {
	     sleepf (pub(appl, refNum));
	}
}

/*_________________________________________________________________________*/
Boolean ForgetTaskSync (MidiEvPtr * taskPtr, MidiEvPtr content)
{
	if (*taskPtr == content) {
      		EvType(content) = typeDead;
    		*taskPtr = 0;
    		return true;
	}
	return false; 
}

