/*

  Copyright © Grame 1999-2002

  This library is free software; you can redistribute it and modify it under 
  the terms of the GNU Library General Public License as published by the 
  Free Software Foundation version 2 of the License, or any later version.

  This library is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public License 
  for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

  Grame Research Laboratory, 9, rue du Garet 69001 Lyon - France
  grame@grame.fr

*/

#include "msFields.h"


MSFunctionType(MidiEvPtr)  MSGetLink (MidiEvPtr e)                  { return Link(e); }
MSFunctionType(void)       MSSetLink (MidiEvPtr e, MidiEvPtr next)  { Link(e) = next; }

MSFunctionType(unsigned long) MSGetDate   (MidiEvPtr e)                { return Date(e); }
MSFunctionType(void)  MSSetDate   (MidiEvPtr e, unsigned long date)    { Date(e) = date; }
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
