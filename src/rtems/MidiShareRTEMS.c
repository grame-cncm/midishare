/*******************************************************************************
 * C H A M E L E O N    S. D. K.                                               *
 *******************************************************************************
 *  $Archive:: /Chameleon.sdk/system/midishare/rtems/MidiShareRTEMS.c          $
 *     $Date: 2005/12/08 13:39:26 $
 * $Revision: 1.1.2.1 $
 *-----------------------------------------------------------------------------*
 * This file is part of the Chameleon Software Development Kit                 *
 *                                                                             *
 * Copyright (C) 2001 soundart                                                 *
 * www.soundart-hot.com                                                        *
 * codemaster@soundart-hot.com                                                 *
 ******************************************************************************/

/*

  This library is free software; you can redistribute it and modify it under 
  the terms of the GNU Library General Public License as published by the 
  Free Software Foundation version 2 of the License, or any later version.

  This library is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public 
  License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#include <bsp.h>
#include <stdlib.h>
#include "MidiShareKernel.h"
#include "msClockDriver.h"
#include "msMidiDriver.h"

static rtems_id _sem_id = 0;

void *AllocateMemory(MemoryType type, DWORD size)
{
	return malloc((rtems_unsigned32) size);
}

void DisposeMemory(void *memPtr)
{
	free(memPtr);
}

/*******************************************************************************
 * Internal functions implementation
 ******************************************************************************/

BOOL CompareAndSwap(register void **adr, register void *compareTo, register void *swapWith) 
{
	BOOL retcode = TRUE;
	ISR_Level level;

	_ISR_Disable(level);

	if (*adr == compareTo)
		*adr = swapWith;
	else 
		retcode = FALSE;

	_ISR_Enable(level);

	return retcode;
}

BOOL ForgetTaskSync(register MidiEvPtr *taskPtr, register MidiEvPtr content)
{
	BOOL retcode = FALSE;
	ISR_Level level;

	_ISR_Disable(level);

	if (*taskPtr == content) 
	{
		EvType(content) = typeDead;
		*taskPtr = 0;
    	retcode = TRUE;
	}
	_ISR_Enable(level);

	return retcode;
}

TApplContextPtr CreateApplContext()
{
	return NULL;
}

void DisposeApplContext(TApplContextPtr context)
{
}

void CallApplAlarm(TApplContextPtr context, ApplAlarmPtr alarm, short refNum, long alarmCode)
{
    (*alarm)(refNum, alarmCode);     /* real alarm call */
}

void CallRcvAlarm(TApplContextPtr context, RcvAlarmPtr alarm, short refNum)
{
    (*alarm)(refNum);                /* real alarm call */
}

void CallTaskCode(TApplContextPtr context, MidiEvPtr e)
{
	TTaskExtPtr task = (TTaskExtPtr) LinkST(e);	/* event extension */

    (*task->fun)(Date(e), RefNum(e), task->arg1, task->arg2, task->arg3);
}

void CallDTaskCode(TApplContextPtr context, MidiEvPtr e)
{
	TTaskExtPtr task = (TTaskExtPtr) LinkST(e);	/* event extension */

    (*task->fun)(Date(e), RefNum(e), task->arg1, task->arg2, task->arg3);
}

void SpecialWakeUp(TMSGlobalPtr g)
{
	msOpenMidiDriver();
}

void SpecialSleep(TMSGlobalPtr g)
{
	msCloseMidiDriver();
}

void OpenTimeInterrupts(TMSGlobalPtr g)
{
	msOpenClockDriver();
}

void CloseTimeInterrupts(TMSGlobalPtr g)
{
	msCloseClockDriver();
}

void DriverWakeUp(TApplPtr appl)
{
	if (Wakeup(appl)) 
    	Wakeup(appl)(appl->refNum);
}

void DriverSleep(TApplPtr appl)
{
	if (Sleep(appl)) 
 		Sleep(appl)(appl->refNum);
}

short MidiOpen(MidiName name)
{
	short retcode;

	if (rtems_semaphore_obtain(_sem_id, RTEMS_WAIT, RTEMS_NO_TIMEOUT) != RTEMS_SUCCESSFUL)
		return MIDIerrSpace;

	retcode = MSOpen(name, gMemPtr);

	if (rtems_semaphore_release(_sem_id) != RTEMS_SUCCESSFUL)
		return MIDIerrSpace;

	return retcode;
}

void MidiClose(short refNum)
{
	if (rtems_semaphore_obtain(_sem_id, RTEMS_WAIT, RTEMS_NO_TIMEOUT) != RTEMS_SUCCESSFUL)
		return;

	MSClose(refNum, gMemPtr);

	if (rtems_semaphore_release(_sem_id) != RTEMS_SUCCESSFUL)
		return;
}

rtems_status_code MidiShareInitialize(void)
{
	rtems_status_code status;

	status = rtems_semaphore_create(
		rtems_build_name('M', 'I', 'D', 'I'),
		1, 
		RTEMS_BINARY_SEMAPHORE | RTEMS_INHERIT_PRIORITY | RTEMS_PRIORITY,
		RTEMS_NO_PRIORITY,
		&_sem_id
	);
	if (status != RTEMS_SUCCESSFUL)
		return status;

	MSSpecialInit(40000, gMemPtr);

	return RTEMS_SUCCESSFUL;
}
