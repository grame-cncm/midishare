/*
  MidiShare Project
  Copyright (C) Grame 2000

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

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
MSFunctionType(SlotRefNum) MSAddSlot 	  (short refnum, const char* name, SlotDirection direction, TClientsPtr g);
MSFunctionType(SlotRefNum) MSGetIndSlot	  (short refnum, short index, TClientsPtr g);
MSFunctionType(void) 	MSSetSlotName 	  (SlotRefNum slot, const char* name, TClientsPtr g);
MSFunctionType(void)    MSRemoveSlot 	  (SlotRefNum slot, TClientsPtr g);
MSFunctionType(Boolean)	MSGetSlotInfos 	  (SlotRefNum slot, TSlotInfos * infos, TClientsPtr g);
MSFunctionType(void)    MSConnectSlot	  (short port, SlotRefNum slot, Boolean state, TClientsPtr g);
MSFunctionType(Boolean)	MSIsSlotConnected (short port, SlotRefNum slot, TClientsPtr g);

/*_______________________________________________________________*/
/* internal functions */
void OpenDrivers	(TMSGlobalPtr g);
void CloseDrivers	(TMSGlobalPtr g);

#endif
