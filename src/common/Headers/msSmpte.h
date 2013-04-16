/*
  MidiShare Project
  Copyright (C) Grame 1999

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#ifndef __msSmpte__
#define __msSmpte__

#include "msTypes.h"
#include "msKernel.h"

MSFunctionType(void) MSGetSyncInfo (SyncInfoPtr p, TMSGlobalPtr g);
MSFunctionType(void) MSSetSyncMode (unsigned short mode, TMSGlobalPtr g);
MSFunctionType(long) MSGetExtTime (TMSGlobalPtr g);
MSFunctionType(long) MSInt2ExtTime (long time, TMSGlobalPtr g);
MSFunctionType(long) MSExt2IntTime (long time, TMSGlobalPtr g);

#endif
