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
  
  modifications history:
   [08-09-99] DF - Sorter initialization change

*/

#include "msTime.h"

/*===========================================================================
  Internal functions prototypes
  =========================================================================== */
static void InitHorloge (TMSGlobalPtr g);


/*===========================================================================
  External MidiShare functions implementation
  =========================================================================== */		
MSFunctionType(unsigned long) MSGetTime (TMSGlobalPtr g)
{
	return CurrTime(g);
}

/*__________________________________________________________________________________*/
MSFunctionType(FarPtr(unsigned long)) MSGetTimeAddr(TMSGlobalPtr g)
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


/*===========================================================================
  Internal functions implementation
  =========================================================================== */
static void InitHorloge( TMSGlobalPtr g)
{
	THorlogePtr h = &g->currTime;
    h->time        = 0; 
    h->reenterCount = -1; 
}
