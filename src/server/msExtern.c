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

//#include "dlfcn.h"
//#include "msPrefs.h"
//#include "portaudio.h"


MutexResCode msOpenMutex  (MutexRef ref) { return kSuccess; }
MutexResCode msCloseMutex (MutexRef ref) { return kSuccess; }
/*------------------------------------------------------------------------------*/
Boolean MSCompareAndSwap (FarPtr(void) *adr, FarPtr(void) compareTo, FarPtr(void) swapWith)
{
        *adr = swapWith;
        return true;
}

/*------------------------------------------------------------------------------*/
/*                      Drivers loading                     					*/
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/*                      initializations : wakeup & sleep                        */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
void SpecialWakeUp (TMSGlobalPtr g) 
{
}


/*------------------------------------------------------------------------------*/
void SpecialSleep  (TMSGlobalPtr g)
{
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
	WakeupPtr wakeup = Wakeup(appl);
	if (wakeup) {
		wakeup (pub(appl, refNum));
	}
}

/*_________________________________________________________________________*/
void DriverSleep (TApplPtr appl)
{
	SleepPtr sleep = Sleep(appl);
	if (sleep) {
	     sleep (pub(appl, refNum));
	}
}

/*__________________________________________________________________________*/
/*      Interrupt handlers                                                                                                              */
/*__________________________________________________________________________*/


/*_________________________________________________________________________*/
void OpenTimeInterrupts(TMSGlobalPtr g)
{
}

/*_________________________________________________________________________*/
void CloseTimeInterrupts(TMSGlobalPtr g)
{
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

