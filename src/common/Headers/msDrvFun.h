/*******************************************************************************
 * C H A M E L E O N    S. D. K.                                               *
 *******************************************************************************
 *  $Archive:: /Chameleon.sdk/SYSTEM/midishare/common/Headers/msDrvFun.h       $
 *     $Date: 2005/12/08 13:38:28 $
 * $Revision: 1.2.6.1 $
 *-----------------------------------------------------------------------------*
 * This file is part of the Chameleon Software Development Kit                 *
 *                                                                             *
 * Copyright (C) 2001 soundart                                                 *
 * www.soundart-hot.com                                                        *
 * codemaster@soundart-hot.com                                                 *
 ******************************************************************************/

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
  research@grame.fr

*/


#ifndef __msDriverFun__
#define __msDriverFun__

#include "msDefs.h"
#include "msKernel.h"
#include "msTypes.h"

/*_______________________________________________________________*/
/* port state */
BOOL 	MSGetPortState(short port);
void 		MSSetPortState(short port, BOOL state);

/*_______________________________________________________________*/
/* drivers register & unregister */
short 	MSRegisterDriver   (TDriverInfos * infos, TDriverOperation *op, TMSGlobalPtr g);
void 	MSUnregisterDriver (short refnum, TMSGlobalPtr g);

/*_______________________________________________________________*/
/* getting information on drivers */
short	MSCountDrivers	(TClientsPtr g);
short	MSGetIndDriver	(short index, TClientsPtr g);
BOOL	MSGetDriverInfos (short refnum, TDriverInfos * infos, TClientsPtr g);

/*_______________________________________________________________*/
/* slots management */
SlotRefNum	MSAddSlot 		  (short refnum, SlotName name, SlotDirection direction, TClientsPtr g);
SlotRefNum	MSGetIndSlot	  (short refnum, short index, TClientsPtr g);
void 		MSSetSlotName	  (SlotRefNum slot, SlotName name, TClientsPtr g);
void		MSRemoveSlot 	  (SlotRefNum slot, TClientsPtr g);
BOOL		MSGetSlotInfos 	  (SlotRefNum slot, TSlotInfos * infos, TClientsPtr g);
void		MSConnectSlot	  (short port, SlotRefNum slot, BOOL state, TClientsPtr g);
BOOL		MSIsSlotConnected (short port, SlotRefNum slot, TClientsPtr g);

/*_______________________________________________________________*/
/* internal functions */
void OpenDrivers	(TMSGlobalPtr g);
void CloseDrivers	(TMSGlobalPtr g);

#endif
