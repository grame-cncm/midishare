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
  grame@grame.fr
  
  modifications history:
   [08-09-99] DF - using lifo for memory management

*/

#ifndef __msEvents__
#define __msEvents__

#include "msDefs.h"
#include "lflifo.h"
#include "msTypes.h"

#define MSNewCell(fl)		(MidiEvPtr)lfpop(fl)
#define MSFreeCell(e, fl)	lfpush (fl, (cell*)(e))

/* MidiShare functions interface */
MSFunctionType(MidiEvPtr)  MSNewEv       (short typeNum, lifo* freelist);
MSFunctionType(void)       MSFreeEv      (MidiEvPtr e, lifo* freelist);
MSFunctionType(MidiEvPtr)  MSCopyEv      (MidiEvPtr e, lifo* freelist);
MSFunctionType(void)       MSSetField    (MidiEvPtr e, unsigned long f, long v);
MSFunctionType(long)       MSGetField    (MidiEvPtr e, unsigned long f);
MSFunctionType(long)       MSCountFields (MidiEvPtr e);
MSFunctionType(void)       MSAddField    (MidiEvPtr e, long v, lifo* freelist);
MSFunctionType(MidiEvPtr)  MSNewCellFunction (lifo* freelist);
MSFunctionType(void)       MSFreeCellFunction (MidiEvPtr e, lifo* freelist);

/* initialization function */
void InitEvents ();

#endif
