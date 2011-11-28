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

#ifndef __msFields__
#define __msFields__

#include "MidiShareLight.h"

MidiEvPtr MSGetLink (MidiEvPtr e);
void      MSSetLink (MidiEvPtr e, MidiEvPtr next);

ulong MSGetDate   (MidiEvPtr e);
void  MSSetDate   (MidiEvPtr e, ulong date);
short MSGetRefNum (MidiEvPtr e);
void  MSSetRefNum (MidiEvPtr e, short ref);
short MSGetType   (MidiEvPtr e);
void  MSSetType   (MidiEvPtr e, short type);
short MSGetChan   (MidiEvPtr e);
void  MSSetChan   (MidiEvPtr e, short chan);
short MSGetPort   (MidiEvPtr e);
void  MSSetPort   (MidiEvPtr e, short port);

long  MSGetData0 (MidiEvPtr e);
long  MSGetData1 (MidiEvPtr e);
long  MSGetData2 (MidiEvPtr e);
long  MSGetData3 (MidiEvPtr e);
void  MSSetData0 (MidiEvPtr e, long v);
void  MSSetData1 (MidiEvPtr e, long v);
void  MSSetData2 (MidiEvPtr e, long v);
void  MSSetData3 (MidiEvPtr e, long v);

MidiEvPtr MSGetFirstEv (MidiSeqPtr seq);
void      MSSetFirstEv (MidiSeqPtr seq, MidiEvPtr e);
MidiEvPtr MSGetLastEv  (MidiSeqPtr seq);
void      MSSetLastEv  (MidiSeqPtr seq, MidiEvPtr e);

#endif
