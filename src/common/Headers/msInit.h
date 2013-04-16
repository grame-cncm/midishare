/*
  MidiShare Project
  Copyright (C) Grame 1999

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#ifndef __msInit__
#define __msInit__

#include "msKernel.h"

/* MidiShare functions interface */
MSFunctionType(short)  MSGetVersion  (TMSGlobalPtr g);

/* initialization function */
MSFunctionType(void)   MSSpecialInit ( ulong defaultSpace, TMSGlobalPtr g);

/* global errors */
MSFunctionType(long)   MSGetError ( TMSGlobalPtr g);

void MidiShareWakeup (TMSGlobalPtr g);
void MidiShareSleep  (TMSGlobalPtr g);

#endif
