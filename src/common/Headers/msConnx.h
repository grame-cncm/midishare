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

#ifndef __msConnections__
#define __msConnections__

#include "msTypes.h"
#include "msAppls.h"
#include "msMemory.h"

void RemAllDstCon (TApplPtr appl, MSMemoryPtr mem);
void RemAllSrcCon (TApplPtr appl, MSMemoryPtr mem);

MSFunctionType(void)    MSConnect     (short src, short dest, Boolean state, TClientsPtr g);
MSFunctionType(Boolean) MSIsConnected (short src, short dest, TClientsPtr g);

#endif
