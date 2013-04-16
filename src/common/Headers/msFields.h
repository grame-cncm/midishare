/*
  MidiShare Project
  Copyright (C) Grame 1999-2011

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#ifndef __msFields__
#define __msFields__

#ifdef MSLIGHT
# include "MidiShareLight.h"
# define MSFunctionType(type) type
#else
# include "msDefs.h"
# include "msTypes.h"
#endif

MSFunctionType(MidiEvPtr) MSGetLink (MidiEvPtr e);
MSFunctionType(void)      MSSetLink (MidiEvPtr e, MidiEvPtr next);

MSFunctionType(ulong) MSGetDate   (MidiEvPtr e);
MSFunctionType(void)  MSSetDate   (MidiEvPtr e, ulong date);
MSFunctionType(short) MSGetRefNum (MidiEvPtr e);
MSFunctionType(void)  MSSetRefNum (MidiEvPtr e, short ref);
MSFunctionType(short) MSGetType   (MidiEvPtr e);
MSFunctionType(void)  MSSetType   (MidiEvPtr e, short type);
MSFunctionType(short) MSGetChan   (MidiEvPtr e);
MSFunctionType(void)  MSSetChan   (MidiEvPtr e, short chan);
MSFunctionType(short) MSGetPort   (MidiEvPtr e);
MSFunctionType(void)  MSSetPort   (MidiEvPtr e, short port);

MSFunctionType(long)  MSGetData0 (MidiEvPtr e);
MSFunctionType(long)  MSGetData1 (MidiEvPtr e);
MSFunctionType(long)  MSGetData2 (MidiEvPtr e);
MSFunctionType(long)  MSGetData3 (MidiEvPtr e);
MSFunctionType(void)  MSSetData0 (MidiEvPtr e, long v);
MSFunctionType(void)  MSSetData1 (MidiEvPtr e, long v);
MSFunctionType(void)  MSSetData2 (MidiEvPtr e, long v);
MSFunctionType(void)  MSSetData3 (MidiEvPtr e, long v);

MSFunctionType(MidiEvPtr) MSGetFirstEv (MidiSeqPtr seq);
MSFunctionType(void)      MSSetFirstEv (MidiSeqPtr seq, MidiEvPtr e);
MSFunctionType(MidiEvPtr) MSGetLastEv  (MidiSeqPtr seq);
MSFunctionType(void)      MSSetLastEv  (MidiSeqPtr seq, MidiEvPtr e);

#endif
