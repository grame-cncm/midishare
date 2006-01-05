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

*/

#ifndef __msInit__
#define __msInit__

#include "msKernel.h"

/* MidiShare functions interface */
MSFunctionType(short)  MSGetVersion  (TMSGlobalPtr g);

/* initialization function */
MSFunctionType(void)   MSSpecialInit ( ulong defaultSpace, TMSGlobalPtr g);

/* global errors */
MSFunctionType(long)   MSGetError ( TMSGlobalPtr g);

void MidiShareWakeup (TMSGlobalPtr g);
void MidiShareSleep  (TMSGlobalPtr g);

#endif
