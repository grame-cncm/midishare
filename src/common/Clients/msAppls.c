/*******************************************************************************
 * C H A M E L E O N    S. D. K.                                               *
 *******************************************************************************
 *  $Archive:: /Chameleon.sdk/system/midishare/common/Clients/msAppls.c        $
 *     $Date: 2005/12/08 13:38:28 $
 * $Revision: 1.6.2.1 $
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
   [08-09-99] DF - new fifo management

*/

#include "msAlarms.h"
#include "msAppls.h"
#include "msAppFun.h"
#include "msApplTools.h"
#include "msConnx.h"
#include "msDriver.h"
#include "msExtern.h"
#include "msInit.h"
#include "msXmtRcv.h"
#include "msMem.h"

#define kMidiShareName 	"MidiShare"

#define CheckApplRefNum( g, r) (CheckRefNum(g, r) && (g->appls[ref]->folder==kClientFolder))

/*===========================================================================
  Internal functions prototypes
  =========================================================================== */
static BOOL  equalApplName   (TApplPtr ap, MidiName name);

/*===========================================================================
  External MidiShare functions implementation
  =========================================================================== */		
short MSOpen (MidiName name, TMSGlobalPtr g)
{
	TApplPtr appl, drv;
	TClientsPtr clients = Clients(g);
	short ref = MIDIerrSpace;
	if (clients->nbAppls == 0) {
		drv  = NewAppl (sizeof(TAppl) + sizeof(TDriver));
		if (drv) {
			TDriverInfos infos;
			setName (infos.name, kMidiShareName);
			infos.version = MSGetVersion (g);
		    makeDriver(clients, drv, MidiShareDriverRef, &infos, 0);
			Clients(g)->nbDrivers++;
		}
		MidiShareWakeup(g);
		appl = NewAppl (sizeof(TAppl));
		if (appl) {
		    makeClient(clients, appl, MidiShareRef, kMidiShareName, kClientFolder);
			Clients(g)->nbAppls++;
		}
	}
	if (CheckClientsCount(clients)) {
		appl = NewAppl (sizeof(TAppl));
		if (appl) {
			for (ref = 1; clients->appls[ref]; ref++)
				;
			makeClient(clients, appl, ref, name, kClientFolder);
			Clients(g)->nbAppls++;
			CallAlarm (ref, MIDIOpenAppl, clients);
		}
	}
	return ref;
}

/*____________________________________________________________________________*/
void MSClose (short ref, TMSGlobalPtr g)
{	
	TClientsPtr clients = Clients(g);
	
	if (!CheckApplRefNum(clients, ref) || (ref == MidiShareRef))
		return;
		
	closeClient (ref, g);
	clients->nbAppls--;
	if (clients->nbAppls == 1) {
		FreeAppl(clients->appls[MidiShareRef]);
		clients->appls[MidiShareRef] = 0;
		clients->nbAppls = 0;
		MidiShareSleep(g);
		FreeAppl(clients->appls[MidiShareDriverRef]);
		clients->appls[MidiShareDriverRef] = 0;
		clients->nbDrivers--;
	} else {
		CallAlarm(ref, MIDICloseAppl, clients);
	}
}

/*____________________________________________________________________________*/
short MSCountAppls (TClientsPtr g)
{
	return g->nbAppls;
}

/*____________________________________________________________________________*/
short MSGetIndAppl (short index, TClientsPtr g)
{
	short ref = -1;
	
	if (index>0 && index<= g->nbAppls) {
		TApplPtr appl;
		do { 
			ref++;
			appl = g->appls[ref];
			if (appl && (appl->folder != kDriverFolder)) index--;
		} while (index);
		return ref;
	}
	return MIDIerrIndex;
}

/*____________________________________________________________________________*/
short MSGetNamedAppl (MidiName name, TClientsPtr g)
{
	short ref =0;
	while (ref < MaxAppls && (!g->appls[ref] || !equalApplName(g->appls[ref], name))) {
		ref++;
	}
	return (ref<MaxAppls) ? ref : (short)MIDIerrIndex;
}

/*____________________________________________________________________________*/
MidiName MSGetName(short ref, TClientsPtr g)
{
	if (CheckRefNum(g,ref)) {
		return ref ? g->appls[ref]->name : kMidiShareName;
	} else {
		return 0;
	}
}

/*____________________________________________________________________________*/
void MSSetName(short ref, MidiName name, TClientsPtr g)
{
	if (CheckRefNum(g,ref) && (ref > 0)) {
		setName(g->appls[ref]->name, name);
		CallAlarm (ref, MIDIChgName, g);
	}
}

/*____________________________________________________________________________*/
void *MSGetInfo (short ref, TClientsPtr g)
{
	void *info = 0;
	if (CheckRefNum(g,ref)) {
		info = g->appls[ref]->info;
	}
	return info;
}

/*____________________________________________________________________________*/
void MSSetInfo (short ref, void* info, TClientsPtr g)
{
	if (CheckRefNum(g,ref)) {
		g->appls[ref]->info = info;
	}
}

/*____________________________________________________________________________*/
MidiFilterPtr MSGetFilter(short ref, TClientsPtr g)
{
	return CheckRefNum(g,ref) ? g->appls[ref]->filter : 0;
}

/*____________________________________________________________________________*/
void MSSetFilter(short ref, MidiFilterPtr filter, TClientsPtr g)
{
	if (CheckRefNum(g,ref)) {
		g->appls[ref]->filter = filter;
	}
}
 
/*____________________________________________________________________________*/
RcvAlarmPtr MSGetRcvAlarm(short ref, TClientsPtr g)
{
	return CheckRefNum(g,ref) ? g->appls[ref]->rcvAlarm : 0;
}

/*____________________________________________________________________________*/
void MSSetRcvAlarm(short ref, RcvAlarmPtr alarm, TClientsPtr g)
{
	if (CheckRefNum(g,ref)) {
		g->appls[ref]->rcvAlarm = alarm;
	}
}

/*____________________________________________________________________________*/
ApplAlarmPtr MSGetApplAlarm(short ref, TClientsPtr g)
{
	return CheckRefNum(g,ref) ? g->appls[ref]->applAlarm : 0;
}

/*____________________________________________________________________________*/
void MSSetApplAlarm(short ref, ApplAlarmPtr alarm, TClientsPtr g)
{
	if (CheckRefNum(g,ref)) {
		g->appls[ref]->applAlarm = alarm;
	}
}

/*===========================================================================
  External functions
  =========================================================================== */
void InitAppls (TClientsPtr g, MSMemoryPtr mem)
{
	short i;
	
	g->nbAppls = 0;
	g->nbDrivers = 0;
	g->memory  = mem;
	g->nextActiveAppl = 0;
	for (i = 0; i < MaxAppls; i++) {
		g->appls[i] = 0;
		g->activesAppls[i] = 0;
	}
}

/*____________________________________________________________________________*/
void makeClient (TClientsPtr g, TApplPtr appl, short ref, MidiName name, short folder)
{
	setName(appl->name, name);	
	appl->context = CreateApplContext();
	appl->info = 0;
	appl->folder = (BYTE)folder;
	appl->refNum = (BYTE)ref;
	appl->rcvFlag = (BYTE)kNoRcvFlag;
	appl->rcvAlarm = 0;
	appl->applAlarm = 0;
	appl->srcList = 0;
	appl->dstList = 0;
	appl->filter = 0;
	appl->driver = 0;
	fifoinit (&appl->rcv);
	fifoinit (&appl->dTasks);
	g->appls[ref] = appl;
}

/*____________________________________________________________________________*/
void closeClient (short ref, TMSGlobalPtr g)
{
	TClientsPtr clients = Clients(g);
	TApplPtr appl = clients->appls[ref];

	RemAllSrcCon (appl, FreeList(Memory(g)));
	RemAllDstCon (appl, FreeList(Memory(g)));
	MSFlushEvs (ref, clients);
	MSFlushDTasks (ref, clients);
	DisposeApplContext (appl->context);
	FreeAppl (appl);
	clients->appls[ref] = 0;
}

/*===========================================================================
  Internal functions implementation
  =========================================================================== */
void setName (MidiName dst, MidiName name)
{
	int i;
	for (i=0; *name && (i< MaxApplNameLen-1); i++) {
		*dst++ = *name++;
	}
	*dst = 0;
}

static BOOL equalApplName (TApplPtr ap, MidiName name)
{
	MidiName apname = ap->name;
	while ((*apname != 0) && (*apname == *name) ) { apname++; name++; }
	return (*apname == *name);
}
