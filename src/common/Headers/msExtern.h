/*
  MidiShare Project
  Copyright (C) Grame 1999

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
  
  modifications history:
   [08-09-99] removing synchronization and memory allocation functions

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
/*                                synchronization                               */
/*------------------------------------------------------------------------------*/
/* mutex definitions */
enum { kOpenCloseMutex = 0, kMutexCount };

void msOpenMutex  (unsigned int mutex);
void msCloseMutex (unsigned int mutex);


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
TApplContextPtr CreateApplContext (void);
void			DisposeApplContext (TApplContextPtr context);
void CallApplAlarm (TApplContextPtr context, ApplAlarmPtr alarm, short refNum, long alarmCode);
void CallRcvAlarm  (TApplContextPtr context, RcvAlarmPtr alarm, short refNum);
void CallTaskCode  (TApplContextPtr context, MidiEvPtr ev);
void CallDTaskCode (TApplContextPtr context, MidiEvPtr ev);

void DriverWakeUp (TApplPtr appl);
void DriverSleep  (TApplPtr appl);

long RealTimeOffset (THorlogePtr h);
void AdjustTimer    (THost context, long offset, long period);

#endif
