/*
  MidiShare Project
  Copyright (C) Grame 2000

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/


#ifndef __msApplTools__
#define __msApplTools__

#include "msAppls.h"
#include "msDefs.h"
#include "msDriver.h"
#include "msKernel.h"

//#define NewAppl(size)		(TApplPtr)AllocateMemory(kernelSharedMemory, size)
TApplPtr NewAppl(int size);

void makeClient (TClientsPtr g, TApplPtr ap, short ref, MidiName n, short folder);
void clearClient (short ref, TMSGlobalPtr g);
void closeClient (short ref, TMSGlobalPtr g);
void makeDriver (TClientsPtr g, TApplPtr appl, short ref, TDriverInfos * infos, TDriverOperation *op);
void setName 	(char* dst, MidiName name);


#endif
