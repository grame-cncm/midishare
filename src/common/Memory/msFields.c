/*
  MidiShare Project
  Copyright (C) Grame 1999-2011

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "msFields.h"


MSFunctionType(MidiEvPtr)  MSGetLink (MidiEvPtr e)                  { return Link(e); }
MSFunctionType(void)       MSSetLink (MidiEvPtr e, MidiEvPtr next)  { Link(e) = next; }

MSFunctionType(ulong) MSGetDate   (MidiEvPtr e)                { return Date(e); }
MSFunctionType(void)  MSSetDate   (MidiEvPtr e, ulong date)    { Date(e) = date; }
MSFunctionType(short) MSGetRefNum (MidiEvPtr e)                { return RefNum(e); }
MSFunctionType(void)  MSSetRefNum (MidiEvPtr e, short ref)     { RefNum(e) = (Byte)ref; }
MSFunctionType(short) MSGetType   (MidiEvPtr e)                { return EvType(e); }
MSFunctionType(void)  MSSetType   (MidiEvPtr e, short type)    { EvType(e) = (Byte)type; }
MSFunctionType(short) MSGetChan   (MidiEvPtr e)                { return Chan(e); }
MSFunctionType(void)  MSSetChan   (MidiEvPtr e, short chan)    { Chan(e) = (Byte)chan; }
MSFunctionType(short) MSGetPort   (MidiEvPtr e)                { return Port(e); }
MSFunctionType(void)  MSSetPort   (MidiEvPtr e, short port)    { Port(e) = (Byte)port; }

MSFunctionType(long)  MSGetData0 (MidiEvPtr e)            { return Data(e)[0]; }
MSFunctionType(long)  MSGetData1 (MidiEvPtr e)            { return Data(e)[1]; }
MSFunctionType(long)  MSGetData2 (MidiEvPtr e)            { return Data(e)[2]; }
MSFunctionType(long)  MSGetData3 (MidiEvPtr e)            { return Data(e)[3]; }
MSFunctionType(void)  MSSetData0 (MidiEvPtr e, long v)    { Data(e)[0] = (Byte)v; }
MSFunctionType(void)  MSSetData1 (MidiEvPtr e, long v)    { Data(e)[1] = (Byte)v; }
MSFunctionType(void)  MSSetData2 (MidiEvPtr e, long v)    { Data(e)[2] = (Byte)v; }
MSFunctionType(void)  MSSetData3 (MidiEvPtr e, long v)    { Data(e)[3] = (Byte)v; }

MSFunctionType(MidiEvPtr)  MSGetFirstEv (MidiSeqPtr seq)               { return seq->first; }
MSFunctionType(void)       MSSetFirstEv (MidiSeqPtr seq, MidiEvPtr e)  { seq->first = e; }
MSFunctionType(MidiEvPtr)  MSGetLastEv  (MidiSeqPtr seq)               { return seq->last; }
MSFunctionType(void)       MSSetLastEv  (MidiSeqPtr seq, MidiEvPtr e)  { seq->last = e; }
