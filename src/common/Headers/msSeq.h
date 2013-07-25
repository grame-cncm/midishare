/*
  MidiShare Project
  Copyright (C) Grame 1999

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

  modifications history:
   [08-09-99] DF - adaptation to the new memory management

*/

#ifndef __msSeq__
#define __msSeq__

#ifdef MSLIGHT
#include "MidiShareLight.h"
#define MSFunctionType(type) type
#else
#include "msDefs.h"
#include "msTypes.h"
#endif
#include "lflifo.h"

/* MidiShare functions interface */
MSFunctionType(MidiSeqPtr) MSNewSeq   (lifo* freelist);
MSFunctionType(void)       MSAddSeq   (MidiSeqPtr s, MidiEvPtr e);
MSFunctionType(void)       MSFreeSeq  (MidiSeqPtr s, lifo* freelist);
MSFunctionType(void)       MSClearSeq (MidiSeqPtr s, lifo* freelist);
MSFunctionType(void)       MSApplySeq (MidiSeqPtr s, ApplyProcPtr proc);

#endif
