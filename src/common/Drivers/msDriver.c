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
  grame@rd.grame.fr

*/


#include "msAlarms.h"
#include "msApplTools.h"
#include "msDrvFun.h"
#include "msDriver.h"
#include "msExtern.h"
#include "msMem.h"
#include "msTypes.h"

#include <stdio.h>

#define NewSlot(size)		(SlotInfosPtr)AllocateMemory(kernelSharedMemory, size)
#define FreeSlot(slot)		DisposeMemory(slot)
#define FreeDriver(drv)		DisposeMemory(drv)

#define NewMap(size)	(PortMapPtr)AllocateMemory(kStdMemory, size)
#define NewSlotInfo()	(SInfosPtr)AllocateMemory(kStdMemory, sizeof(SInfos))
#define FreeMap(map)		DisposeMemory(map)
#define FreeSlotInfo(info)	DisposeMemory(info)

#define CheckDriversCount(g)        (nbDrivers(g) < MaxDrivers - 1)
#define CheckDriverRefNum( g, r)    (CheckRefNum(g, r) && (g->appls[r]->folder==kDriverFolder))
#define CheckSlotRef(drv, ref)		((ref>=0) && (ref<MaxSlots) && drv->map[ref])

/*===========================================================================
  Internal functions prototypes
  =========================================================================== */
static void clearSlotCnx (char * cnx);
static SlotInfosPtr findSlot (short ref, SlotInfosPtr slot, SlotInfosPtr * previous);
static short getNextFreePublicDriver (TClientsPublic * cpub);
static short makeSlot (SlotInfosPtr slot, short drvRef, SlotName name, SlotDirection direction, TClientsPtr g);

/*===========================================================================
  External MidiShare functions implementation
  =========================================================================== */		
MSFunctionType(short) MSRegisterDriver (TDriverInfos * infos, TDriverOperation *op, TMSGlobalPtr g)
{
	TClientsPtr clients = Clients(g);
	short ref = MIDIerrSpace;
	if (CheckClientsCount(clients) && CheckDriversCount(clients)) {
		TApplPtr appl  = NewAppl(sizeof(TAppl));
		TDriverPtr drv = NewDriver(sizeof(TDriver));
		if (appl && drv) {
			for (ref = clients->lastDrvRef - 1; clients->appls[ref]; ref--) {
				if (ref == clients->lastDrvRef) { /* should never happend */
					break;
				}
				if (ref == MidiShareRef + 1) ref=MidiShareDriverRef;
			}
			clients->lastDrvRef = ref;
			makeClient (clients, appl, ref, infos->name, kDriverFolder);
			makeDriver (clients, drv, ref, infos, op);
			Driver(appl) = drv;
			if (nbAppls(clients)) {
				DriverWakeUp (appl);
				CallAlarm (ref, MIDIOpenDriver, clients);
			}
			return ref;
		}
		else {
			if (appl) FreeAppl(appl);
			if (drv)  FreeDriver(drv);
		}
	}
	return MIDIerrSpace;
}

/*____________________________________________________________________________*/
MSFunctionType(void) MSUnregisterDriver (short ref, TMSGlobalPtr g)
{
	TDriverPtr drv;
	TClientsPtr clients = Clients(g);	
	if (!CheckDriverRefNum(clients, ref) || (ref == MidiShareDriverRef))
		return;

	if (nbAppls(Clients(g))) {			
		DriverSleep (clients->appls[ref]);		
		CallAlarm (ref, MIDICloseDriver, clients);
	}
	drv = Driver(clients->appls[ref]);
	closeDriver (ref, drv, g);
	closeClient (ref, g);
}

/*____________________________________________________________________________*/
MSFunctionType(short) MSCountDrivers (TClientsPtr g)
{
	return nbDrivers(g);
}

/*____________________________________________________________________________*/
MSFunctionType(Boolean) MSGetDriverInfos (short ref, TDriverInfos * infos, TClientsPtr g)
{
	TApplPtr appl;
	if (!CheckDriverRefNum(g, ref))
		return false;

	appl = g->appls[ref];
	setName (infos->name, pub(appl,name));
	infos->version = pub(Driver(appl),version);
	infos->slots   = pub(Driver(appl),slotsCount);
	infos->reserved[0] = infos->reserved[1] = 0;
	return true;
}

/*____________________________________________________________________________*/
MSFunctionType(short) MSGetIndDriver (short index, TClientsPtr g)
{
	short ref = MaxAppls;
	
	if (index>0 && index<= nbDrivers(g)) {
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
MSFunctionType(SlotRefNum) MSAddSlot (short drvRef, SlotName name, SlotDirection direction, TClientsPtr g)
{
	TDriverPtr drv; SlotRefNum slot;
	
	slot.drvRef = drvRef;
	if (!CheckDriverRefNum(g, drvRef)) {
		slot.slotRef = MIDIerrRefNum;
		return slot;
	}
	
	drv = Driver(g->appls[drvRef]);
	*(long *)(&slot) = MIDIerrSpace;
	if (g->nbSlots < MaxSlots) {
		SlotInfosPtr sptr = NewSlot(sizeof(SlotInfos));
		if (sptr) {
			g->nbSlots++;			
			slot.drvRef = drvRef;
			slot.slotRef = makeSlot (sptr, drvRef, name, direction, g);
			pub(drv, slotsCount)++;

			/* link the new slot to the driver slots list */
			sptr->next = drv->slots;
			drv->slots = sptr;

			/* and if necessary, triggers an application alarm */
			if (nbAppls(g))
				CallAlarm (drvRef, MIDIAddSlot, g);
		}
	}
	return slot;	
}

/*____________________________________________________________________________*/
MSFunctionType(void) MSSetSlotName (SlotRefNum slot, SlotName name, TClientsPtr g)
{
	TDriverPtr drv; SlotInfosPtr sptr;
	if (!CheckDriverRefNum(g, slot.drvRef))
		return;
	
	drv = Driver(g->appls[slot.drvRef]);
	sptr = findSlot(slot.slotRef, drv->slots, 0);
	if (!sptr) return;
	setName (pub(sptr,name), name);
	if (nbAppls(g))
		CallAlarm (slot.drvRef, MIDIChgSlotName, g);
}	

/*____________________________________________________________________________*/
MSFunctionType(void) MSRemoveSlot (SlotRefNum slot, TClientsPtr g)
{
	TDriverPtr drv; SlotInfosPtr sptr, previous;
	if (!CheckDriverRefNum(g, slot.drvRef))
		return;

	drv = Driver(g->appls[slot.drvRef]);
	sptr = findSlot (slot.slotRef, drv->slots, &previous);
	if (sptr) {
		/* first unlink the slot from the driver slots list */
		if (previous)
			previous->next = sptr->next;
		else drv->slots = sptr->next;
		pub(drv, slotsCount)--;
		g->nbSlots--;

		/* clear next public slot information */
		pub(sptr,driverRefNum) = MIDIerrRefNum;
		pub(sptr,slotRefNum) = MIDIerrRefNum;

		/* free then the private slot structure */
		FreeSlot(sptr);
			
		/* and if necessary, triggers an application alarm */
		if (nbAppls(g))
			CallAlarm (slot.drvRef, MIDIRemoveSlot, g);		
	}
}

/*____________________________________________________________________________*/
MSFunctionType(Boolean) MSGetSlotInfos (SlotRefNum slot, TSlotInfos * infos, TClientsPtr g)
{
	TDriverPtr drv; SlotInfosPtr sptr; 
	int i; char * cnxPtr;
	if (!CheckDriverRefNum(g, slot.drvRef))
		return false;

	drv = Driver(g->appls[slot.drvRef]);
	sptr = findSlot(slot.slotRef, drv->slots, 0);
	if (!sptr) return false;

	setName (infos->name, pub(sptr,name));
	infos->direction = (SlotDirection)pub(sptr, direction);
	cnxPtr = pub(sptr, cnx);
	for (i=0; i<MaxPorts/8; i++, cnxPtr++) {
		infos->cnx[i] = *cnxPtr;
	}
	infos->reserved[0] = 0;
	infos->reserved[1] = 0;
	return true;
}

/*____________________________________________________________________________*/
MSFunctionType(SlotRefNum) MSGetIndSlot (short refnum, short index, TClientsPtr g)
{
	TDriverPtr drv; SlotRefNum slot;
	SlotInfosPtr sptr;

	slot.drvRef = refnum;
	if (!CheckDriverRefNum(g, refnum)) {
		slot.slotRef = MIDIerrRefNum;
		return slot;
	}

	drv = Driver(g->appls[refnum]);
	sptr = drv->slots;
	*(long *)(&slot) = MIDIerrIndex;
	if ((index > 0) && (index <= MaxSlots)) {
		while (sptr) {
			if (index == 1) {
				slot.drvRef = refnum;
				slot.slotRef = pub(sptr, slotRefNum);
				break;
			}
			index--;
			sptr = sptr->next;
		}
	}
	return slot;
}

/*____________________________________________________________________________*/
MSFunctionType(void) MSConnectSlot (short port, SlotRefNum slot, Boolean state, TClientsPtr g)
{
	TDriverPtr drv; SlotInfosPtr sptr;
	short slotRef; Boolean change = false;
	slotRef = slot.slotRef;
	if (!CheckDriverRefNum(g, slot.drvRef))
		return;
	if (port >= MaxPorts) return;

	drv = Driver(g->appls[slot.drvRef]);
	sptr = findSlot(slot.slotRef, drv->slots, 0);
	if (sptr) {
		if (state) {
			if (!IsAcceptedBit(pub(sptr,cnx), port)) {
				AcceptBit(pub(sptr,cnx), port);
				change = true;
			}
		} else if (IsAcceptedBit(pub(sptr,cnx), port)){
			RejectBit(pub(sptr,cnx), port);
			change = true;
		}
		if (change && nbAppls(g))
			CallAlarm (slot.drvRef, MIDIChgSlotConnect, g);
	}
}

/*____________________________________________________________________________*/
MSFunctionType(Boolean) MSIsSlotConnected (short port, SlotRefNum slot, TClientsPtr g)
{
	TDriverPtr drv; SlotInfosPtr sptr;
	if (!CheckDriverRefNum(g, slot.drvRef))
		return false;
	if (port >= MaxPorts) 
		return false;

	drv = Driver(g->appls[slot.drvRef]);
	sptr = findSlot(slot.slotRef, drv->slots, 0);
	if (sptr)
		return IsAcceptedBit (pub(sptr,cnx), port);
	return false;
}

/*===========================================================================
  External functions
  =========================================================================== */
void OpenDrivers (TMSGlobalPtr g) 
{
	TClientsPtr clients = Clients(g);
	int ref, n = MSCountDrivers (clients) - 1;
	for (ref = MidiShareDriverRef-1; n && ref; ref--) {
		TApplPtr appl = clients->appls[ref];
		if (appl && (appl->folder == kDriverFolder)) {
			DriverWakeUp(appl);
			n--;
		}
	}
}

/*____________________________________________________________________________*/
void CloseDrivers (TMSGlobalPtr g)
{
	TClientsPtr clients = Clients(g);
	int ref, n = MSCountDrivers (clients) - 1;
	for (ref = MidiShareDriverRef-1; n && ref; ref--) {
		TApplPtr appl = clients->appls[ref];
		if (appl && (appl->folder == kDriverFolder)) {
			DriverSleep (appl);
//			appl->srcList = appl->dstList = 0;
			n--;
		}
	}
}

/*____________________________________________________________________________*/
void makeDriver (TClientsPtr g, TDriverPtr drv, short ref, 
				 TDriverInfos * infos, TDriverOperation *op)
{
	short i; 

	/* set first kernel private information */
	if (op) drv->op = *op;
	else {
		drv->op.wakeup = 0;
		drv->op.sleep = 0;
	}
	drv->op.reserved[0] = drv->op.reserved[1] = drv->op.reserved[2] = 0;
	drv->slots = 0;                           /* no slots declared */

	/* set next driver public information */
	i = getNextFreePublicDriver (g->pub);     /* get a free index in the public section */
	if (i < MaxDrivers) {                     /* it should always be */
		nbDrivers(g)++;
		drv->pub = &pub(g, drivers[i]);       /* makes the driver pointing to this public area */
		pub(drv, refNum) = ref;
		pub(drv, version) = infos->version;
		pub(drv, slotsCount) = 0;
		pub(g->appls[ref], drvidx) = i;       /* stores the index of the public driver infos */
	}
}

/*____________________________________________________________________________*/
void closeDriver (short ref, TDriverPtr drv, TMSGlobalPtr g)
{
	SlotInfosPtr slot = drv->slots;
	
	/* clear first driver public information */
	nbDrivers(Clients(g))--;
	pub(drv, refNum) = MIDIerrRefNum;
	pub(drv, version) = 0;
	pub(drv, slotsCount) = 0;

	/* release now the allocated slots */
	while (slot) {
		SlotInfosPtr next = slot->next;
		Clients(g)->nbSlots--;

		/* clear the public slot information */
		pub(slot,driverRefNum) = MIDIerrRefNum;
		pub(slot,slotRefNum) = MIDIerrRefNum;

		/* free then the private slot structure */
		FreeSlot(slot);
		slot = next;
	}
	drv->slots = 0;
	FreeDriver(drv);
}

/*===========================================================================
  Internal functions
  =========================================================================== */
static short getNextFreePublicDriver (TClientsPublic * cpub)
{
	short i;
	for (i=0; i<MaxDrivers; i++) {
		if (cpub->drivers[i].refNum == MIDIerrRefNum)
			break;
	}
	return i;
}

static void clearSlotCnx (char * cnx)
{
	short i;
	for (i=0; i<MaxPorts/8; i++) {
		*cnx++ = 0;
	}
}

/*____________________________________________________________________________*/
static SlotInfosPtr findSlot (short ref, SlotInfosPtr slot, SlotInfosPtr * previous)
{
	SlotInfosPtr prev = 0;
	while (slot) {
		if (pub(slot, slotRefNum) == ref) {
			if (previous) *previous = prev;
			return slot;
		}
		prev = slot;
		slot = slot->next;
	}
	return 0;
}

/*____________________________________________________________________________*/
static short makeSlot (SlotInfosPtr slot, short drvRef, SlotName name, 
					  SlotDirection direction, TClientsPtr g)
{
		short ref;

		/* set first private slot information */
		slot->rcvAlarm = 0;
		fifoinit (&slot->rcv);
		slot->unused = 0;
	
		/* look for a free reference number */
		for (ref = g->lastSlot + 1; pub(g,slots[ref].slotRefNum) != MIDIerrRefNum; ref++) {
			if (ref == g->lastSlot) {  /* should never happend */
				break;
			}
			if (ref == MaxSlots - 1) ref=-1;
		}

		pub(g,slots[ref].slotRefNum) = ref;
		g->lastSlot = ref;
		slot->pub= &pub(g,slots[ref]);
		
		/* set next public slot information */
		pub(slot,driverRefNum) = drvRef;
		pub(slot,slotRefNum) = ref;
		setName (pub(slot,name), name);
		clearSlotCnx (pub(slot, cnx));
		pub(slot,direction) = (char)direction;
		pub(slot,infos) = 0;
		return ref;
}
