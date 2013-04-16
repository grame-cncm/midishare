/*
  MidiShare Project
  Copyright (C) Grame 1999

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#ifndef __msAppFun__
#define __msAppFun__

#include "msKernel.h"

/*------------------------------------------------------------------------*/
/* functions declarations                                                 */
void     InitAppls (TClientsPtr g, MSMemoryPtr mem);

MSFunctionType(short)     MSOpen         (MidiName name, TMSGlobalPtr g);
MSFunctionType(void)      MSClose        (short ref, TMSGlobalPtr g);

MSFunctionType(short)     MSCountAppls   (TClientsPtr g);
MSFunctionType(short)     MSGetIndAppl   (short index, TClientsPtr g);
MSFunctionType(short)     MSGetNamedAppl (MidiName name, TClientsPtr g);

MSFunctionType(MidiName)     MSGetName  (short ref, TClientsPtr g);
MSFunctionType(void)         MSSetName  (short ref, MidiName name, TClientsPtr g);
MSFunctionType(void*)		 MSGetInfo  (short ref, TClientsPtr g);
MSFunctionType(void)         MSSetInfo  (short ref, void* info, TClientsPtr g);

MSFunctionType(MidiFilterPtr) MSGetFilter  (short ref, TClientsPtr g);
MSFunctionType(void)          MSSetFilter  (short ref, MidiFilterPtr filter, TClientsPtr g);

MSFunctionType(RcvAlarmPtr)  MSGetRcvAlarm  (short ref, TClientsPtr g);
MSFunctionType(void)         MSSetRcvAlarm  (short ref, RcvAlarmPtr alarm, TClientsPtr g);
MSFunctionType(ApplAlarmPtr) MSGetApplAlarm (short ref, TClientsPtr g);
MSFunctionType(void)         MSSetApplAlarm (short ref, ApplAlarmPtr alarm, TClientsPtr g);

#endif
