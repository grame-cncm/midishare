/*******************************************************************************
 * C H A M E L E O N    S. D. K.                                               *
 *******************************************************************************
 *  $Archive:: /Chameleon.sdk/system/midishare/common/Drivers/msDriver.c       $
 *     $Date: 2005/12/08 13:38:28 $
 * $Revision: 1.6.6.1 $
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


#include "msAlarms.h"
#include "msApplTools.h"
#include "msDrvFun.h"
#include "msDriver.h"
#include "msExtern.h"
#include "msMem.h"
#include "msTypes.h"

#define NewMap(size)	(PortMapPtr)AllocateMemory(kStdMemory, size)
#define NewSlotInfo()	(SInfosPtr)AllocateMemory(kStdMemory, sizeof(SInfos))
#define FreeMap(map)		DisposeMemory(map)
#define FreeSlotInfo(info)	DisposeMemory(info)

#define CheckDriverRefNum( g, r)    (CheckRefNum(g, r) && (g->appls[r]->folder==kDriverFolder))
#define CheckSlotRef(drv, ref)		((ref>=0) && (ref<MaxSlots) && drv->map[ref])

/*===========================================================================
  Internal functions prototypes
  =========================================================================== */
static void clearSlot2PortMap (char * map);
static void clearPort2SlotMap (char * map);
static void closeDriver (short ref, TDriverPtr drv, TMSGlobalPtr g);

/*===========================================================================
  External MidiShare functions implementation
  =========================================================================== */		
BOOL MSGetPortState(short port)
{
	return GetPortState(port);
}

void MSSetPortState(short port, BOOL state)
{
	SetPortState(port, state);
}

/*------------------------------------------------------------------------------*/
short MSRegisterDriver(TDriverInfos * infos, TDriverOperation *op, TMSGlobalPtr g)
{
	TClientsPtr clients = Clients(g);
	short ref = MIDIerrSpace;
	if (CheckClientsCount(clients)) {
		TApplPtr appl = NewAppl(sizeof(TAppl) + sizeof(TDriver));
		if (appl) {
			for (ref = MidiShareDriverRef-1; clients->appls[ref]; ref--)
				;
			makeDriver (clients, appl, ref, infos, op);
			clients->nbDrivers++;
			if (clients->nbAppls) {
				DriverWakeUp (appl);
				CallAlarm (ref, MIDIOpenDriver, clients);
			}
		}
	}
	return ref;
}

/*____________________________________________________________________________*/
void MSUnregisterDriver(short ref, TMSGlobalPtr g)
{
	TDriverPtr drv; TAppl saved;
	TClientsPtr clients = Clients(g);	
	if (!CheckDriverRefNum(clients, ref) || (ref == MidiShareDriverRef))
		return;

	saved = *clients->appls[ref];
	drv = Driver(clients->appls[ref]);
	closeDriver (ref, drv, g);
	clients->nbDrivers--;
	if (Clients(g)->nbAppls) {			
		DriverSleep (&saved);		
		CallAlarm (ref, MIDICloseDriver, clients);
	}
}

/*____________________________________________________________________________*/
short MSCountDrivers(TClientsPtr g)
{
	return g->nbDrivers;
}

/*____________________________________________________________________________*/
BOOL MSGetDriverInfos (short ref, TDriverInfos * infos, TClientsPtr g)
{
	TApplPtr appl;
	if (!CheckDriverRefNum(g, ref))
		return FALSE;

	appl = g->appls[ref];
	setName (infos->name, appl->name);
	infos->version = appl->driver->version;
	infos->slots   = appl->driver->slotsCount;
	infos->reserved[0] = infos->reserved[1] = 0;
	return TRUE;
}

/*____________________________________________________________________________*/
short MSGetIndDriver(short index, TClientsPtr g)
{
	short ref = MaxAppls;
	
	if (index>0 && index<= g->nbDrivers) {
		TApplPtr appl;
		do { 
			ref--;
			appl = g->appls[ref];
			if (appl && (appl->folder == kDriverFolder)) index--;
		} while (index && ref);
		return ref;
	}
	return MIDIerrIndex;
}

/*____________________________________________________________________________*/
SlotRefNum MSAddSlot(short drvRef, SlotName name, SlotDirection direction, TClientsPtr g)
{
	TDriverPtr drv; SlotRefNum slot;
	
	slot.drvRef = drvRef;
	if (!CheckDriverRefNum(g, drvRef)) {
		slot.slotRef = MIDIerrRefNum;
		return slot;
	}
	
	drv = Driver(g->appls[drvRef]);
	*(long *)(&slot) = MIDIerrSpace;
	if (drv->slotsCount < MaxSlots) {
		short ref; PortMapPtr map; SInfosPtr iPtr;
		map = NewMap (sizeof(PortMap));
		iPtr= NewSlotInfo();
		if (map && iPtr) {
			for (ref = 0; drv->map[ref]; ref++)
				;
			clearSlot2PortMap (map);
			drv->map[ref] = map;
			setName (iPtr->name, name);
			iPtr->direction = direction;
			drv->slotInfos[ref] = iPtr;
			drv->slotsCount++;
			slot.drvRef = drvRef;
			slot.slotRef = ref;
			if (g->nbAppls)
				CallAlarm (drvRef, MIDIAddSlot, g);
		}
		else slot.slotRef = MIDIerrSpace;
	}
	return slot;	
}

/*____________________________________________________________________________*/
void MSSetSlotName(SlotRefNum slot, SlotName name, TClientsPtr g)
{
	TDriverPtr drv; SInfosPtr iPtr;
	if (!CheckDriverRefNum(g, slot.drvRef))
		return;
	
	drv = Driver(g->appls[slot.drvRef]);
	iPtr = drv->slotInfos[slot.slotRef];
	if (!iPtr) return;
	setName (iPtr->name, name);
	if (g->nbAppls)
		CallAlarm (slot.drvRef, MIDIChgSlotName, g);
}	

/*____________________________________________________________________________*/
void MSRemoveSlot(SlotRefNum slot, TClientsPtr g)
{
	TDriverPtr drv; short i, slotRef;
	if (!CheckDriverRefNum(g, slot.drvRef))
		return;

	drv = Driver(g->appls[slot.drvRef]);
	if (CheckSlotRef(drv, slot.slotRef)) {
		slotRef = slot.slotRef;
		if (drv->map[slotRef])
			FreeMap(drv->map[slotRef]);
		if (drv->slotInfos[slotRef])
			FreeSlotInfo(drv->slotInfos[slotRef]);
		drv->map[slotRef] = 0;
		drv->slotInfos[slotRef] = 0;
		for (i=0; i<MaxPorts; i++) {
			RejectBit(drv->port[i], slotRef);
		}
		drv->slotsCount--;
		if (g->nbAppls)
			CallAlarm (slot.drvRef, MIDIRemoveSlot, g);
	}
}

/*____________________________________________________________________________*/
BOOL MSGetSlotInfos(SlotRefNum slot, TSlotInfos * infos, TClientsPtr g)
{
	TDriverPtr drv;
	if (!CheckDriverRefNum(g, slot.drvRef))
		return FALSE;

	drv = Driver(g->appls[slot.drvRef]);
	if (CheckSlotRef(drv, slot.slotRef)) {
		short i; PortMapPtr map = drv->map[slot.slotRef];
		SInfosPtr inf = drv->slotInfos[slot.slotRef];
		for (i=0; i<PortMapSize; i++)
			infos->cnx[i] = map[i];
		infos->reserved[0] = 0;
		infos->reserved[1] = 0;
		setName (infos->name, inf->name);
		infos->direction = inf->direction;
		return TRUE;
	}
	return FALSE;
}

/*____________________________________________________________________________*/
SlotRefNum MSGetIndSlot(short refnum, short index, TClientsPtr g)
{
	TDriverPtr drv; SlotRefNum slot;
	short n = -1;
	slot.drvRef = refnum;
	if (!CheckDriverRefNum(g, refnum)) {
		slot.slotRef = MIDIerrRefNum;
		return slot;
	}

	drv = Driver(g->appls[refnum]);
	if (index>0 && index<= drv->slotsCount) {
		do { 
			n++;
			if (drv->map[n]) index--;
		} while (index);
		slot.drvRef = refnum;
		slot.slotRef = n;
	}
	else *(long *)(&slot) = MIDIerrIndex;
	return slot;
}

/*____________________________________________________________________________*/
void MSConnectSlot(short port, SlotRefNum slot, BOOL state, TClientsPtr g)
{
	TDriverPtr drv;
	short slotRef; BOOL change = FALSE;
	slotRef = slot.slotRef;
	if (!CheckDriverRefNum(g, slot.drvRef))
		return;

	drv = Driver(g->appls[slot.drvRef]);
	if (CheckSlotRef(drv, slotRef)) {
		if (state) {
			if (!IsAcceptedBit(drv->port[port], slotRef)) {
				AcceptBit(drv->port[port], slotRef);
				AcceptBit(drv->map[slotRef], port);
				change = TRUE;
			}
		} else if (IsAcceptedBit(drv->port[port], slotRef)){
			RejectBit(drv->port[port], slotRef);
			RejectBit(drv->map[slotRef], port);
			change = TRUE;
		}
		if (change && g->nbAppls)
			CallAlarm (slot.drvRef, MIDIChgSlotConnect, g);
	}
}

/*____________________________________________________________________________*/
BOOL MSIsSlotConnected(short port, SlotRefNum slot, TClientsPtr g)
{
	TDriverPtr drv;
	if (!CheckDriverRefNum(g, slot.drvRef))
		return FALSE;

	drv = Driver(g->appls[slot.drvRef]);
	return CheckSlotRef(drv, slot.slotRef) ?
		   IsAcceptedBit (drv->port[port], slot.slotRef) && 
		   IsAcceptedBit (drv->map[slot.slotRef], port)
		   : FALSE;
}

/*===========================================================================
  External functions
  =========================================================================== */
void OpenDrivers(TMSGlobalPtr g) 
{
	TClientsPtr clients = Clients(g);
	short ref, n = MSCountDrivers (clients) - 1;
	for (ref = MidiShareDriverRef-1; n && ref; ref--) {
		TApplPtr appl = clients->appls[ref];
		if (appl && (appl->folder == kDriverFolder)) {
			DriverWakeUp(appl);
			n--;
		}
	}
}

/*____________________________________________________________________________*/
void CloseDrivers(TMSGlobalPtr g)
{
	TClientsPtr clients = Clients(g);
	short ref, n = MSCountDrivers (clients) - 1;
	for (ref = MidiShareDriverRef-1; n && ref; ref--) {
		TApplPtr appl = clients->appls[ref];
		if (appl && (appl->folder == kDriverFolder)) {
			DriverSleep (appl);
			appl->srcList = appl->dstList = 0;
			n--;
		}
	}
}

/*____________________________________________________________________________*/
void makeDriver(TClientsPtr g, TApplPtr appl, short ref,
				TDriverInfos * infos, TDriverOperation *op)
{
	short i; TDriverPtr drv;

	makeClient (g, appl, ref, infos->name, kDriverFolder);
	Driver(appl) = drv = (TDriverPtr)(appl + 1);
	if (op) drv->op = *op;
	else {
		drv->op.wakeup = 0;
		drv->op.sleep = 0;
		drv->op.reserved[0] = drv->op.reserved[1] = drv->op.reserved[2] = 0;
	}
	drv->version = infos->version;
	drv->slotsCount = 0;
	for (i=0; i<MaxSlots; i++) {
		drv->slotInfos[i] = 0;
		drv->map[i] = 0;
	}
	clearPort2SlotMap (&drv->port[0][0]);
}

/*===========================================================================
  Internal functions
  =========================================================================== */
static void clearSlot2PortMap(char * map)
{
	short i;
	for (i=0; i<PortMapSize; i++) {
		*map++ = 0;
	}
}

static void clearPort2SlotMap(char * map)
{
	short i, j;
	for (i=0; i<MaxPorts; i++) {
		for (j=0; j< SlotMapSize; j++)
			*map++ = 0;
	}
}

/*____________________________________________________________________________*/
static void closeDriver(short ref, TDriverPtr drv, TMSGlobalPtr g)
{
	short i;
	for (i = 0; i < MaxSlots; i++) {
		if (drv->slotInfos[i])
			FreeSlotInfo(drv->slotInfos[i]);
		if (drv->map[i])
			FreeMap(drv->map[i]);
	}
	closeClient (ref, g);
}
