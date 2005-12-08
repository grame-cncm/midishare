/*******************************************************************************
 * C H A M E L E O N    S. D. K.                                               *
 *******************************************************************************
 *  $Archive:: /Chameleon.sdk/system/midishare/common/Kernel/msInit.c          $
 *     $Date: 2005/12/08 13:38:30 $
 * $Revision: 1.11.2.1 $
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
   [08-09-99] DF - adaptation to the new memory management
                   version number increased

*/

#include "msAppFun.h"
#include "msInit.h"
#include "msDrvFun.h"
#include "msEvents.h"
#include "msExtern.h"
#include "msTime.h"


/*===========================================================================
  External MidiShare functions implementation
  =========================================================================== */		

void MSSpecialInit( DWORD defaultSpace, TMSGlobalPtr g)
{
	InitEvents ();
	InitMemory(Memory(g), defaultSpace);
	InitAppls (Clients(g), Memory(g));
	InitTime( g);
}

short MSGetVersion (TMSGlobalPtr g)
{
	return 186;
}


/*===========================================================================
  External initialization functions
  =========================================================================== */
void MidiShareWakeup(TMSGlobalPtr g) 
{
    fifoinit (SorterList(g));
    OpenMemory (Memory(g));
    SpecialWakeUp (g);
    OpenTime (g);
    OpenTimeInterrupts (g);
	OpenDrivers(g);
}

void MidiShareSleep(TMSGlobalPtr g) 
{
    CloseTimeInterrupts (g);
	CloseDrivers(g);
    SpecialSleep (g);
    CloseMemory (Memory(g));
}
