/*
  MidiShare Project
  Copyright (C) Grame 1999

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

  modifications history:
   [08-09-99] DF - Sorter initialization change

*/

#include "msTime.h"

/*===========================================================================
  Internal functions implementation
  =========================================================================== */
static void InitHorloge( TMSGlobalPtr g)
{
	THorlogePtr h = &g->currTime;
    h->time        = 0; 
    h->reenterCount = -1;
    h->offset = 0;
    h->adjustCount = kClockAdjustCount;
}

/*===========================================================================
  External MidiShare functions implementation
  =========================================================================== */		
MSFunctionType(unsigned long) MSGetTime (TMSGlobalPtr g)
{
	return CurrTime(g);
}

/*__________________________________________________________________________________*/
MSFunctionType(unsigned long*) MSGetTimeAddr(TMSGlobalPtr g)
{
	return &CurrTime(g);
}

/*__________________________________________________________________________________*/
MSFunctionType(void) MSTime2Smpte (long time, short format, SmpteLocPtr loc, TMSGlobalPtr g)
{
	/* not yet implemented */
}

/*__________________________________________________________________________________*/
MSFunctionType(long) MSSmpte2Time (SmpteLocPtr loc, TMSGlobalPtr g)
{
	/* not yet implemented */
	return 0;
}


/*===========================================================================
  External initialization functions
  =========================================================================== */
void InitTime( TMSGlobalPtr g)
{
}
	
/*__________________________________________________________________________________*/
void OpenTime( TMSGlobalPtr g)
{
	InitHorloge( g);
	SorterInit( Sorter(g), 10);
}
