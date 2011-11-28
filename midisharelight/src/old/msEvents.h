/*

  Copyright � Grame 1999

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
  
  modifications history:
   [08-09-99] DF - using lifo for memory management

*/

#ifndef __msEvents__
#define __msEvents__

#include "MidiShareLight.h"
#include "lflifo.h"

#define MSNewCell(fl)		(MidiEvPtr)lfpop(fl)
#define MSFreeCell(e, fl)	lfpush (fl, (lifocell*)(e))

/* MidiShare functions interface */
MidiEvPtr  MSNewEv       (short typeNum, lifo* freelist);
void       MSFreeEv      (MidiEvPtr e, lifo* freelist);
MidiEvPtr  MSCopyEv      (MidiEvPtr e, lifo* freelist);
void       MSSetField    (MidiEvPtr e, unsigned long f, long v);
long       MSGetField    (MidiEvPtr e, long f);
long       MSCountFields (MidiEvPtr e);
void       MSAddField    (MidiEvPtr e, long v, lifo* freelist);
MidiEvPtr  MSNewCellFunction (lifo* freelist);
void       MSFreeCellFunction (MidiEvPtr e, lifo* freelist);

/* initialization function */
void InitEvents (void);

#endif
