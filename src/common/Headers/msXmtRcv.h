/*
  MidiShare Project
  Copyright (C) Grame 1999

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

  modifications history:
   [08-09-99] DF - removing the sorter dependencies

*/

#ifndef __msXmtRcv__
#define __msXmtRcv__

#include "msTypes.h"
#include "msAppls.h"
#include "lffifo.h"

MSFunctionType(void)        MSSendIm   (short refNum, MidiEvPtr e, fifo* schedlist, unsigned long curdate);
MSFunctionType(void)        MSSend     (short refNum, MidiEvPtr e, fifo* schedlist);
MSFunctionType(void)        MSSendAt   (short refNum, MidiEvPtr e, unsigned long d, fifo* schedlist);
MSFunctionType(atomic_long) MSCountEvs (short refNum, TClientsPtr g);
MSFunctionType(MidiEvPtr)   MSGetEv    (short refNum, TClientsPtr g);
MSFunctionType(MidiEvPtr)   MSAvailEv  (short refNum, TClientsPtr g);
MSFunctionType(void)        MSFlushEvs (short refNum, TClientsPtr g);

#endif
