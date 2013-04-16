/*
  MidiShare Project
  Copyright (C) Grame 1999

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

  modifications history:
   [06-09-99] DF reducing dependencies by replacing msExtern.h by mem.h

*/

#include "msFilter.h"
#include "msMem.h"


#define AllocateNewFilter(size)		(MidiFilterPtr)AllocateMemory(kernelSharedMemory, size)
#define FreeFilter(filter)			DisposeMemory(filter)


MSFunctionType(MidiFilterPtr) MSNewFilter (void) {
	return AllocateNewFilter (sizeof(TFilter));
}

MSFunctionType(void) MSFreeFilter (MidiFilterPtr filter) {
	FreeFilter (filter);
}

MSFunctionType(void) MSAcceptPort(MidiFilterPtr f, short port, Boolean state) {
	if (state) AcceptBit(f->port, port);
	else RejectBit (f->port, port);
}

MSFunctionType(void) MSAcceptChan(MidiFilterPtr f, short chan, Boolean state) {	
	if (state) AcceptBit(f->channel, chan);
	else RejectBit (f->channel, chan);
}

MSFunctionType(void) MSAcceptType(MidiFilterPtr f, short type, Boolean state) {	
	if (state) AcceptBit(f->evType, type);
	else RejectBit (f->evType, type);
}

MSFunctionType(Boolean) MSIsAcceptedPort(MidiFilterPtr f, short port) {
	return (Boolean)IsAcceptedBit (f->port, port);
}

MSFunctionType(Boolean) MSIsAcceptedChan(MidiFilterPtr f, short chan) {	
	return (Boolean)IsAcceptedBit (f->channel, chan);
}

MSFunctionType(Boolean) MSIsAcceptedType(MidiFilterPtr f, short type) {	
	return (Boolean)IsAcceptedBit (f->evType, type);
}

