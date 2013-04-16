/*
  MidiShare Project
  Copyright (C) Grame 1999

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

  modifications history:
   [08-09-99] DF - mail box functions finalization

*/

#ifndef __msMail__
#define __msMail__

#include "msTypes.h"

/* MidiShare functions interface */
MSFunctionType(void*) MSReadSync	(void** adrMem);
MSFunctionType(void*) MSWriteSync	(void** adrMem, void* val);

#endif
