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

*/

#ifndef __msExtern__
#define __msExtern__

#include "msKernel.h"
#include "msTasks.h"

/* 
  external functions declaration :
  these functions are specific to platform dependant implementations 
*/

/*------------------------------------------------------------------------------*/
/*                               memory management                              */
/*------------------------------------------------------------------------------*/
/* defines a memory block maximum size: when a client application requests 
   for a larger block, the allocated memory is splitted in smaller blocks */
#define kMaxEventsPerBlock	4096

typedef enum {	
        kStdMemory,        /* no constraints on the allocated memory  */
                           /* it should only be accessed and owned by the calling process */

        kSharedMemory,     /* the allocated memory should be shared by the kernel and     */
                           /* all the client applications.                                */

        kernelSharedMemory /* the allocated memory should be shared by the kernel and     */
                           /* the calling process                                         */
} MemoryType;

FarPtr(void) AllocateMemory (MemoryType type, long size);
void         DisposeMemory  (FarPtr(void) memPtr);


/*------------------------------------------------------------------------------*/
/*                                synchronization                               */
/*------------------------------------------------------------------------------*/
typedef FarPtr(void) * msListPtr;

/* functions which should provide secure ways to modify linked lists */
/* should be preferably implemented using lock free shared objects   */
void FAR *  PopSync  (msListPtr adr);
void 	    PushSync (msListPtr adr, msListPtr link);
void 	    PushSyncList (msListPtr adr, msListPtr head, msListPtr tail);

Boolean     CompareAndSwap (FarPtr(void) *adr, FarPtr(void) compareTo, FarPtr(void) swapWith);

/* forgetting a realtime task */
Boolean     ForgetTaskSync (MidiEvPtr * taskPtr, MidiEvPtr content);

/* fifos management */
MidiEvPtr ClearFifo  (TFifoPtr fifo);
MidiEvPtr PopFifoEv  (TFifoPtr fifo);
void      PushFifoEv (TFifoPtr fifo, MidiEvPtr ev);

/* mutex definitions */
enum { kMemoryMutex = 1, kWakeUpMutex };
enum { kSuccess, kTimeOut, kUnknownMutex, kFailed };

typedef short 	MutexResCode;
typedef long 	MutexRef;

MutexResCode OpenMutex  (MutexRef ref);
MutexResCode CloseMutex (MutexRef ref);


/*------------------------------------------------------------------------------*/
/*                      initializations : wakeup & sleep                        */
/*------------------------------------------------------------------------------*/
void SpecialWakeUp (TMSGlobalPtr g);
void SpecialSleep  (TMSGlobalPtr g);

void OpenTimeInterrupts (TMSGlobalPtr g);
void CloseTimeInterrupts(TMSGlobalPtr g);


/*------------------------------------------------------------------------------*/
/*                   client applications context and tasks                      */
/*------------------------------------------------------------------------------*/
TApplContextPtr CreateApplContext ();
void			DisposeApplContext (TApplContextPtr context);
void CallApplAlarm (TApplContextPtr context, ApplAlarmPtr alarm, short refNum, long alarmCode);
void CallRcvAlarm  (TApplContextPtr context, RcvAlarmPtr alarm, short refNum);
void CallTaskCode  (TApplContextPtr context, TTaskExtPtr task, long date, short refNum);
void CallDTaskCode (TApplContextPtr context, TTaskExtPtr task, long date, short refNum);

#endif
