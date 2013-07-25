/*
  MidiShare Project
  Copyright (C) Grame 1999

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#ifndef __msFilter__
#define __msFilter__

#include "msDefs.h"
#include "msTypes.h"

MSFunctionType(MidiFilterPtr) MSNewFilter  (void);
MSFunctionType(void)          MSFreeFilter (MidiFilterPtr);

MSFunctionType(void) MSAcceptPort(MidiFilterPtr f, short port, Boolean state);
MSFunctionType(void) MSAcceptChan(MidiFilterPtr f, short chan, Boolean state);
MSFunctionType(void) MSAcceptType(MidiFilterPtr f, short type, Boolean state);

MSFunctionType(Boolean) MSIsAcceptedPort(MidiFilterPtr f, short port);
MSFunctionType(Boolean) MSIsAcceptedChan(MidiFilterPtr f, short chan);
MSFunctionType(Boolean) MSIsAcceptedType(MidiFilterPtr f, short type);

#endif
