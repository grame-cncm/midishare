/*

  Copyright © Grame 1999

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
  grame@rd.grame.fr

*/

#ifndef __msEvents__
#define __msEvents__

#include "msDefs.h"
#include "msMemory.h"

/* MidiShare functions interface */
MSFunctionType(MidiEvPtr)  MSNewCell     (MSMemoryPtr g);
MSFunctionType(void)       MSFreeCell    (MidiEvPtr e, MSMemoryPtr g);
MSFunctionType(MidiEvPtr)  MSNewEv       (short typeNum, MSMemoryPtr g);
MSFunctionType(void)       MSFreeEv      (MidiEvPtr e, MSMemoryPtr g);
MSFunctionType(MidiEvPtr)  MSCopyEv      (MidiEvPtr e, MSMemoryPtr g);
MSFunctionType(void)       MSSetField    (MidiEvPtr e, unsigned long f, long v, MSMemoryPtr g);
MSFunctionType(long)       MSGetField    (MidiEvPtr e, long f, MSMemoryPtr g);
MSFunctionType(long)       MSCountFields (MidiEvPtr e, MSMemoryPtr g);
MSFunctionType(long)       MSAddField    (MidiEvPtr e, long v, MSMemoryPtr g);

/* private events management functions */
void InitEvents( MSMemoryPtr g);

#endif
