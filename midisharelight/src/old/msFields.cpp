/*

  Copyright (c) Grame 1999, 2011

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
  research@grame.fr

*/

#include "msFields.h"


MidiEvPtr  MSGetLink (MidiEvPtr e)                  { return Link(e); }
void       MSSetLink (MidiEvPtr e, MidiEvPtr next)  { Link(e) = next; }

ulong MSGetDate   (MidiEvPtr e)                { return Date(e); }
void  MSSetDate   (MidiEvPtr e, ulong date)    { Date(e) = date; }
short MSGetRefNum (MidiEvPtr e)                { return RefNum(e); }
void  MSSetRefNum (MidiEvPtr e, short ref)     { RefNum(e) = (Byte)ref; }
short MSGetType   (MidiEvPtr e)                { return EvType(e); }
void  MSSetType   (MidiEvPtr e, short type)    { EvType(e) = (Byte)type; }
short MSGetChan   (MidiEvPtr e)                { return Chan(e); }
void  MSSetChan   (MidiEvPtr e, short chan)    { Chan(e) = (Byte)chan; }
short MSGetPort   (MidiEvPtr e)                { return Port(e); }
void  MSSetPort   (MidiEvPtr e, short port)    { Port(e) = (Byte)port; }

long  MSGetData0 (MidiEvPtr e)            { return Data(e)[0]; }
long  MSGetData1 (MidiEvPtr e)            { return Data(e)[1]; }
long  MSGetData2 (MidiEvPtr e)            { return Data(e)[2]; }
long  MSGetData3 (MidiEvPtr e)            { return Data(e)[3]; }
void  MSSetData0 (MidiEvPtr e, long v)    { Data(e)[0] = (Byte)v; }
void  MSSetData1 (MidiEvPtr e, long v)    { Data(e)[1] = (Byte)v; }
void  MSSetData2 (MidiEvPtr e, long v)    { Data(e)[2] = (Byte)v; }
void  MSSetData3 (MidiEvPtr e, long v)    { Data(e)[3] = (Byte)v; }

MidiEvPtr  MSGetFirstEv (MidiSeqPtr seq)               { return seq->first; }
void       MSSetFirstEv (MidiSeqPtr seq, MidiEvPtr e)  { seq->first = e; }
MidiEvPtr  MSGetLastEv  (MidiSeqPtr seq)               { return seq->last; }
void       MSSetLastEv  (MidiSeqPtr seq, MidiEvPtr e)  { seq->last = e; }
