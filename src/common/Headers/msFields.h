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

#ifndef __msFields__
#define __msFields__

#include "msDefs.h"
#include "msTypes.h"

MSFunctionType(MidiEvPtr) MSGetLink (MidiEvPtr e);
MSFunctionType(void)      MSSetLink (MidiEvPtr e, MidiEvPtr next);

MSFunctionType(unsigned long) MSGetDate   (MidiEvPtr e);
MSFunctionType(void)  MSSetDate   (MidiEvPtr e, unsigned long date);
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
