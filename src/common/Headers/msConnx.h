/*
  MidiShare Project
  Copyright (C) Grame 1999

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

  modifications history:
   [08-09-99] DF - new memory management

*/

#ifndef __msConnections__
#define __msConnections__

#include "msTypes.h"
#include "msAppls.h"

void RemAllDstCon (TApplPtr appl, lifo* freelist);
void RemAllSrcCon (TApplPtr appl, lifo* freelist);

MSFunctionType(void)    MSConnect     (short src, short dest, Boolean state, TClientsPtr g);
MSFunctionType(Boolean) MSIsConnected (short src, short dest, TClientsPtr g);

#endif
