/*
  MidiShare Project
  Copyright (C) Grame 1999

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#ifndef __msTime__
#define __msTime__

#include "msKernel.h"


/*____________________________________________________________________________*/
void InitTime (TMSGlobalPtr g);
void OpenTime (TMSGlobalPtr g);

/*____________________________________________________________________________*/
MSFunctionType(unsigned long) MSGetTime (TMSGlobalPtr g);
MSFunctionType(unsigned long*) MSGetTimeAddr(TMSGlobalPtr g);

MSFunctionType(void) MSTime2Smpte  (long time, short format, SmpteLocPtr loc, TMSGlobalPtr g);
MSFunctionType(long) MSSmpte2Time  (SmpteLocPtr loc, TMSGlobalPtr g);

#endif
