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

#define NewMap(size)		(PortMapPtr)AllocateMemory(kStdMemory, size)
#define FreeMap(map)		DisposeMemory(map)

#define CheckDriverRefNum( g, r)    (CheckRefNum(g, r) && (g->appls[r]->folder==kDriverFolder))
#define CheckSlotRef(drv, ref)		((ref>=0) && (ref<MaxSlots) && drv->map[ref])

/*===========================================================================
  Internal functions prototypes
  =========================================================================== */
static void clearSlot2PortMap (char * map);
static void clearPort2SlotMap (char * map);
static short CountSlots (TClientsPtr g);


/*===========================================================================
  External MidiShare functions implementation
  =========================================================================== */		
MSFunctionType(short) MSRegisterDriver (TDriverInfos * infos, TDriverOperation *op, TMSGlobalPtr g)
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
MSFunctionType(void) MSUnregisterDriver (short ref, TMSGlobalPtr g)
{
	TDriverPtr drv; short i;
	TClientsPtr clients = Clients(g);	
	if (!CheckDriverRefNum(clients, ref) || (ref == MidiShareDriverRef))
		return;

	if (Clients(g)->nbAppls)
		DriverSleep (clients->appls[ref]);		
	drv = Driver(clients->appls[ref]);
	for (i = 0; i < MaxSlots; i++)
		if (drv->map[i]) FreeMap(drv->map[i]);
	closeClient (ref, g);
	clients->nbDrivers--;
	if (Clients(g)->nbAppls)			
		CallAlarm (ref, MIDICloseDriver, clients);
}

/*____________________________________________________________________________*/
MSFunctionType(short) MSCountDrivers (TClientsPtr g)
{
	return g->nbDrivers;
}

/*____________________________________________________________________________*/
MSFunctionType(Boolean) MSGetDriverInfos (short ref, TDriverInfos * infos, TClientsPtr g)
{
	TApplPtr appl;
	if (!CheckDriverRefNum(g, ref))
		return false;

	appl = g->appls[ref];
	setApplName (infos->name, appl->name);
	infos->version = appl->driver->version;
	infos->slots   = appl->driver->slotsCount;
	return true;
}

/*____________________________________________________________________________*/
MSFunctionType(short) MSGetIndDriver (short index, TClientsPtr g)
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
MSFunctionType(SlotRefNum) MSAddSlot (short drvRef, TClientsPtr g)
{
	TDriverPtr drv; IntSlotRefNum slotref;
	if (!CheckDriverRefNum(g, drvRef))
		return MIDIerrRefNum;
	
	drv = Driver(g->appls[drvRef]);
	slotref.u.lvalue = MIDIerrSpace;
	if (drv->slotsCount < MaxSlots) {
		short ref; PortMapPtr map;
		map = NewMap (sizeof(PortMap));
		if (map) {
			for (ref = 0; drv->map[ref]; ref++)
				;
			clearSlot2PortMap (map);
			drv->map[ref] = map;
			drv->slotsCount++;
			slotref.u.s.drvRef = drvRef;
			slotref.u.s.slotRef = ref;
			if (g->nbAppls)
				CallAlarm (drvRef, MIDIAddSlot, g);
		}
	}
	return slotref.u.lvalue;	
}

/*____________________________________________________________________________*/
MSFunctionType(void) MSRemoveSlot (SlotRefNum slot, TClientsPtr g)
{
	TDriverPtr drv; IntSlotRefNum ref; short i, slotRef;
	ref.u.lvalue = slot;
	if (!CheckDriverRefNum(g, ref.u.s.drvRef))
		return;

	drv = Driver(g->appls[ref.u.s.drvRef]);
	if (CheckSlotRef(drv, ref.u.s.slotRef)) {
		slotRef = ref.u.s.slotRef;
		FreeMap(drv->map[slotRef]);
		drv->map[slotRef] = 0;
		for (i=0; i<MaxPorts; i++) {
			RejectBit(drv->port[i], slotRef);
		}
		drv->slotsCount--;
		if (g->nbAppls)
			CallAlarm (ref.u.s.drvRef, MIDIRemoveSlot, g);
	}
}

/*____________________________________________________________________________*/
MSFunctionType(Boolean) MSGetSlotInfos (SlotRefNum slot, TSlotInfos * infos, TClientsPtr g)
{
	TDriverPtr drv; IntSlotRefNum ref;
	ref.u.lvalue = slot;
	if (!CheckDriverRefNum(g, ref.u.s.drvRef))
		return false;

	drv = Driver(g->appls[ref.u.s.drvRef]);
	if (CheckSlotRef(drv, ref.u.s.slotRef) && drv->op.slotInfo) {
		short i; PortMapPtr map = drv->map[ref.u.s.slotRef];
		for (i=0; i<PortMapSize; i++)
			infos->cnx[i] = map[i];
		return DriverSlotInfos (g->appls[ref.u.s.drvRef], slot, infos);
	}
	return false;
}

/*____________________________________________________________________________*/
MSFunctionType(SlotRefNum) MSGetIndSlot (short refnum, short index, TClientsPtr g)
{
	TDriverPtr drv; IntSlotRefNum ref;
	short n = -1;
	if (!CheckDriverRefNum(g, refnum))
		return MIDIerrRefNum;

	drv = Driver(g->appls[refnum]);
	if (index>0 && index<= drv->slotsCount) {
		do { 
			n++;
			if (drv->map[n]) index--;
		} while (index);
		ref.u.s.drvRef = refnum;
		ref.u.s.slotRef = n;
		return ref.u.lvalue;
	}
	return MIDIerrIndex;
}

/*____________________________________________________________________________*/
MSFunctionType(void) MSConnectSlot (short port, SlotRefNum slot, Boolean state, TClientsPtr g)
{
	TDriverPtr drv; IntSlotRefNum ref;
	short slotRef; Boolean change = false;
	ref.u.lvalue = slot;
	slotRef = ref.u.s.slotRef;
	if (!CheckDriverRefNum(g, ref.u.s.drvRef))
		return;

	drv = Driver(g->appls[ref.u.s.drvRef]);
	if (CheckSlotRef(drv, slotRef)) {
		if (state) {
			if (!IsAcceptedBit(drv->port[port], slotRef)) {
				AcceptBit(drv->port[port], slotRef);
				AcceptBit(drv->map[slotRef], port);
				change = true;
			}
		} else if (IsAcceptedBit(drv->port[port], slotRef)){
			RejectBit(drv->port[port], slotRef);
			RejectBit(drv->map[slotRef], port);
			change = true;
		}
		if (change && g->nbAppls)
			CallAlarm (ref.u.s.drvRef, MIDIChgSlotConnect, g);
	}
}

/*____________________________________________________________________________*/
MSFunctionType(Boolean) MSIsSlotConnected (short port, SlotRefNum slot, TClientsPtr g)
{
	TDriverPtr drv; IntSlotRefNum ref;
	ref.u.lvalue = slot;
	if (!CheckDriverRefNum(g, ref.u.s.drvRef))
		return false;

	drv = Driver(g->appls[ref.u.s.drvRef]);
	return CheckSlotRef(drv, ref.u.s.slotRef) ?
		   IsAcceptedBit (drv->port[port], ref.u.s.slotRef) && 
		   IsAcceptedBit (drv->map[ref.u.s.slotRef], port)
		   : false;
}

/*===========================================================================
  External functions
  =========================================================================== */
void OpenDrivers (TMSGlobalPtr g) 
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
void CloseDrivers (TMSGlobalPtr g)
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
void makeDriver (TClientsPtr g, TApplPtr appl, short ref,
				TDriverInfos * infos, TDriverOperation *op)
{
	short i; TDriverPtr drv;

	makeClient (g, appl, ref, infos->name, kDriverFolder);
	Driver(appl) = drv = (TDriverPtr)(appl + 1);
	if (op) drv->op = *op;
	else {
		drv->op.wakeup = 0;
		drv->op.sleep = 0;
		drv->op.slotInfo = 0;
	}
	drv->version = infos->version;
	drv->slotsCount = 0;
	for (i=0; i<MaxSlots; i++)
		drv->map[i] = 0;
	clearPort2SlotMap (&drv->port[0][0]);
}

/*===========================================================================
  Internal functions
  =========================================================================== */
static void clearSlot2PortMap (char * map)
{
	short i;
	for (i=0; i<PortMapSize; i++) {
		*map++ = 0;
	}
}

static void clearPort2SlotMap (char * map)
{
	short i, j;
	for (i=0; i<MaxPorts; i++) {
		for (j=0; j< SlotMapSize; j++)
			*map++ = 0;
	}
}
