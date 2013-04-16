/*
  MidiShare Project
  Copyright (C) Grame 1999

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
  
  modifications history:
   [08-09-99] DF - using lifo for memory management

*/

#ifndef __msEvents__
#define __msEvents__


#ifdef MSLIGHT
# include "MidiShareLight.h"
# define MSFunctionType(type) type
#else
# include "msDefs.h"
# include "msTypes.h"
#endif
#include "lflifo.h"

#define MSNewCell(fl)		(MidiEvPtr)lfpop(fl)
#define MSFreeCell(e, fl)	lfpush (fl, (lifocell*)(e))

/* MidiShare functions interface */
MSFunctionType(MidiEvPtr)  MSNewEv       (short typeNum, lifo* freelist);
MSFunctionType(void)       MSFreeEv      (MidiEvPtr e, lifo* freelist);
MSFunctionType(MidiEvPtr)  MSCopyEv      (MidiEvPtr e, lifo* freelist);
MSFunctionType(void)       MSSetField    (MidiEvPtr e, unsigned long f, long v);
MSFunctionType(long)       MSGetField    (MidiEvPtr e, long f);
MSFunctionType(long)       MSCountFields (MidiEvPtr e);
MSFunctionType(void)       MSAddField    (MidiEvPtr e, long v, lifo* freelist);
MSFunctionType(MidiEvPtr)  MSNewCellFunction (lifo* freelist);
MSFunctionType(void)       MSFreeCellFunction (MidiEvPtr e, lifo* freelist);

/* initialization function */
void InitEvents (void);

#endif
