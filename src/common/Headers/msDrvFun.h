/*

  Copyright © Grame 2000

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

*/


#ifndef __msDriverFun__
#define __msDriverFun__

#include "msDefs.h"
#include "msKernel.h"
#include "msTypes.h"

/*_______________________________________________________________*/
/* drivers register & unregister */
MSFunctionType(short) 	MSRegisterDriver   (TDriverInfos * infos, TDriverOperation *op, TMSGlobalPtr g);
MSFunctionType(void) 	MSUnregisterDriver (short refnum, TMSGlobalPtr g);

/*_______________________________________________________________*/
/* getting information on drivers */
MSFunctionType(short)	MSCountDrivers	(TClientsPtr g);
MSFunctionType(short)	MSGetIndDriver	(short index, TClientsPtr g);
MSFunctionType(Boolean)	MSGetDriverInfos (short refnum, TDriverInfos * infos, TClientsPtr g);

/*_______________________________________________________________*/
/* slots management */
MSFunctionType(SlotRefNum) MSAddSlot 	  (short refnum, SlotName name, SlotDirection direction, TClientsPtr g);
MSFunctionType(SlotRefNum) MSGetIndSlot	  (short refnum, short index, TClientsPtr g);
MSFunctionType(void) 	MSSetSlotName 	  (SlotRefNum slot, SlotName name, TClientsPtr g);
MSFunctionType(void)    MSRemoveSlot 	  (SlotRefNum slot, TClientsPtr g);
MSFunctionType(Boolean)	MSGetSlotInfos 	  (SlotRefNum slot, TSlotInfos * infos, TClientsPtr g);
MSFunctionType(void)    MSConnectSlot	  (short port, SlotRefNum slot, Boolean state, TClientsPtr g);
MSFunctionType(Boolean)	MSIsSlotConnected (short port, SlotRefNum slot, TClientsPtr g);

/*_______________________________________________________________*/
/* internal functions */
void OpenDrivers	(TMSGlobalPtr g);
void CloseDrivers	(TMSGlobalPtr g);

#endif
