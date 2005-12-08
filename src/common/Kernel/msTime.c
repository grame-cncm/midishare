/*******************************************************************************
 * C H A M E L E O N    S. D. K.                                               *
 *******************************************************************************
 *  $Archive:: /Chameleon.sdk/SYSTEM/midishare/common/Kernel/msTime.c          $
 *     $Date: 2005/12/08 13:38:30 $
 * $Revision: 1.4.6.1 $
 *-----------------------------------------------------------------------------*
 * This file is part of the Chameleon Software Development Kit                 *
 *                                                                             *
 * Copyright (C) 2001 soundart                                                 *
 * www.soundart-hot.com                                                        *
 * codemaster@soundart-hot.com                                                 *
 ******************************************************************************/

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
DWORD MSGetTime (TMSGlobalPtr g)
{
	return CurrTime(g);
}

/*__________________________________________________________________________________*/
DWORD* MSGetTimeAddr(TMSGlobalPtr g)
{
	return &CurrTime(g);
}

/*__________________________________________________________________________________*/
void MSTime2Smpte(long time, short format, SmpteLocPtr loc, TMSGlobalPtr g)
{
	/* not yet implemented */
}

/*__________________________________________________________________________________*/
long MSSmpte2Time(SmpteLocPtr loc, TMSGlobalPtr g)
{
	/* not yet implemented */
	return 0;
}


/*===========================================================================
  External initialization functions
  =========================================================================== */
void InitTime(TMSGlobalPtr g)
{
}
	
/*__________________________________________________________________________________*/
void OpenTime(TMSGlobalPtr g)
{
	InitHorloge(g);
	SorterInit(Sorter(g), 10);
}
