/*

  Copyright © Grame 1999-2002

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

  modifications history:
   [08-09-99] DF - new fifo management
   [06-08-02] DF - userland server implementation

*/

#include "msAlarms.h"
#include "msAppls.h"
#include "msAppFun.h"
#include "msApplTools.h"
#include "msConnx.h"
#include "msDriver.h"
#include "msDrvFun.h"
#include "msInit.h"
#include "msMem.h"
#include "msXmtRcv.h"

#include "msExtern.h"

#ifdef PascalNames
# define kMidiShareName 	"\pMidiShare"
#else
# define kMidiShareName 	"MidiShare"
#endif

#define CheckApplRefNum( g, r) (CheckRefNum(g, r) && (folder(g->appls[ref])==kClientFolder))
#ifdef MSKernel
#	define UnusedAppl(g, ref)	(g->appls[ref] == 0)
#else
#	define UnusedAppl(g, ref)	(g->appls[ref].refNum == MIDIerrRefNum)
#endif

/*===========================================================================
  Internal functions prototypes
  =========================================================================== */
static Boolean  equalApplName   (TApplPtr ap, MidiName name);

/*===========================================================================
  External MidiShare functions implementation
  =========================================================================== */		

/*____________________________________________________________________________*/
MSFunctionType(short) MSCountAppls (TClientsPtr g)
{
	return nbAppls(g);
}

/*____________________________________________________________________________*/
MSFunctionType(short) MSGetIndAppl (short index, TClientsPtr g)
{
	short ref = -1;
	
	if (index>0 && index<= nbAppls(g)) {
		TApplPtr appl;
		do { 
			ref++;
			appl = GetApplPtr(g, ref);
			if (appl && (folder(appl) != kDriverFolder)) index--;
		} while (index);
		return ref;
	}
	return MIDIerrIndex;
}

/*____________________________________________________________________________*/
MSFunctionType(short) MSGetNamedAppl (MidiName name, TClientsPtr g)
{
	short ref =0;
	while (ref < MaxAppls && (UnusedAppl(g,ref) || !equalApplName(GetApplPtr(g, ref), name))) {
		ref++;
	}
	return (ref<MaxAppls) ? ref : (short)MIDIerrIndex;
}

/*____________________________________________________________________________*/
MSFunctionType(MidiName) MSGetName(short ref, TClientsPtr g)
{
	if (CheckRefNum(g,ref)) {
		TApplPtr appl = GetApplPtr(g, ref);
		return ref ? pub(appl, name) : kMidiShareName;
	} else {
		return 0;
	}
}

/*____________________________________________________________________________*/
MSFunctionType(FarPtr(void)) MSGetInfo (short ref, TClientsPtr g)
{
	FarPtr(void) info = 0;
	if (CheckRefNum(g,ref)) {
		TApplPtr appl = GetApplPtr(g, ref);
		info = pub(appl, info);
	}
	return info;
}

/*____________________________________________________________________________*/
MSFunctionType(void) MSSetInfo (short ref, FarPtr(void) info, TClientsPtr g)
{
	if (CheckRefNum(g,ref)) {
		TApplPtr appl = GetApplPtr(g, ref);
		pub(appl, info) = info;
	}
}

/*____________________________________________________________________________*/
MSFunctionType(MidiFilterPtr) MSGetFilter(short ref, TClientsPtr g)
{
	if (CheckRefNum(g,ref)) {
		TApplPtr appl = GetApplPtr(g, ref);
		return pub(appl, filter);
	}
	return 0;
}

#ifdef MSKernel
/*===========================================================================
  External MidiShare server functions implementation
  =========================================================================== */		
MSFunctionType(short) MSOpen (MidiName name, TMSGlobalPtr g)
{
	TApplPtr appl; TDriverPtr drv;
	TClientsPtr clients = Clients(g);
	short ref = MIDIerrSpace;
	if (nbAppls(clients) == 0) {
		clients->lastRef = 0;
		clients->lastDrvRef = MidiShareDriverRef;
		clients->lastSlot = -1;
		appl = NewAppl (sizeof(TAppl));
		drv  = NewDriver (sizeof(TDriver));
		if (appl && drv) {
			TDriverInfos infos;
			setName (infos.name, kMidiShareName);
			infos.version = MSGetVersion (g);
		    makeClient(clients, appl, MidiShareDriverRef, kMidiShareName, kDriverFolder);
		    makeDriver(clients, drv, MidiShareDriverRef, &infos, 0);
		    Driver(appl) = drv;
		}
		MidiShareWakeup(g);
		appl = NewAppl (sizeof(TAppl));
		if (appl) {
		    makeClient(clients, appl, MidiShareRef, kMidiShareName, kClientFolder);
			nbAppls(clients)++;
		}
		else return MIDIerrSpace;
	}
	if (CheckClientsCount(clients)) {
		appl = NewAppl (sizeof(TAppl));
		if (appl) {
			for (ref = clients->lastRef+1; clients->appls[ref]; ref++) {			
				if (ref == clients->lastRef) break;  /* should never happend */
				if (ref == MidiShareDriverRef-1) ref=MidiShareRef;
			}
			clients->lastRef = ref;
			makeClient(clients, appl, ref, name, kClientFolder);
			nbAppls(clients)++;
			CallAlarm (ref, MIDIOpenAppl, clients);
		}
	}
	return ref;
}

/*____________________________________________________________________________*/
MSFunctionType(void) MSClose (short ref, TMSGlobalPtr g)
{	
	TClientsPtr clients = Clients(g);
	
	if (!CheckApplRefNum(clients, ref) || (ref == MidiShareRef))
		return;
		
	closeClient (ref, g);
	nbAppls(clients)--;
	if (nbAppls(clients) == 1) {
		closeClient (MidiShareRef, g);
		nbAppls(clients) = 0;
		MidiShareSleep(g);
		closeDriver (MidiShareDriverRef, Driver(clients->appls[MidiShareDriverRef]), g);
		closeClient (MidiShareDriverRef, g);
	} else {
		CallAlarm(ref, MIDICloseAppl, clients);
	}
}

/*____________________________________________________________________________*/
MSFunctionType(void) MSSetName(short ref, MidiName name, TClientsPtr g)
{
	if (CheckRefNum(g,ref) && (ref > 0)) {
		setName(pub(g->appls[ref], name), name);
		CallAlarm (ref, MIDIChgName, g);
	}
}

/*____________________________________________________________________________*/
MSFunctionType(void) MSSetFilter(short ref, MidiFilterPtr filter, TClientsPtr g)
{
	if (CheckRefNum(g,ref)) {
		g->appls[ref]->filter = filter;
	}
}
 
/*____________________________________________________________________________*/
MSFunctionType(RcvAlarmPtr) MSGetRcvAlarm(short ref, TClientsPtr g)
{
	return CheckRefNum(g,ref) ? g->appls[ref]->rcvAlarm : 0;
}

/*____________________________________________________________________________*/
MSFunctionType(void) MSSetRcvAlarm(short ref, RcvAlarmPtr alarm, TClientsPtr g)
{
	if (CheckRefNum(g,ref)) {
		g->appls[ref]->rcvAlarm = alarm;
	}
}

/*____________________________________________________________________________*/
MSFunctionType(ApplAlarmPtr) MSGetApplAlarm(short ref, TClientsPtr g)
{
	return CheckRefNum(g,ref) ? g->appls[ref]->applAlarm : 0;
}

/*____________________________________________________________________________*/
MSFunctionType(void) MSSetApplAlarm(short ref, ApplAlarmPtr alarm, TClientsPtr g)
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
	
	nbAppls(g) = 0;
	nbDrivers(g) = 0;
	g->memory = mem;
	g->nextActiveAppl = 0;
	for (i = 0; i < MaxAppls; i++) {
		g->appls[i] = 0;
		g->activesAppls[i] = 0;
	}
}

/*____________________________________________________________________________*/
void makeClient (TClientsPtr g, TApplPtr appl, short ref, MidiName name, short folder)
{
	/* set first kernel private information */
	folder(appl) = (uchar)folder;
	appl->rcvFlag = (uchar)kNoRcvFlag;
	fifoinit (&appl->rcv);
	fifoinit (&appl->dTasks);
	appl->filter = 0;
	appl->rcvAlarm = 0;
	appl->applAlarm = 0;
	appl->driver = 0;
	appl->context = CreateApplContext();

	/* set next public information */
	appl->pub = &g->pub->appls[ref];
	setName(pub(appl,name), name);	
	pub(appl, info) = 0;
	pub(appl, refNum) = (uchar)ref;
	pub(appl, drvidx) = MIDIerrRefNum;
	pub(appl, filter) = 0;
	RemAllDstCon (appl);

	/* and finaly stores the kernel application handler */
	g->appls[ref] = appl;
}

/*____________________________________________________________________________*/
void closeClient (short ref, TMSGlobalPtr g)
{
	TClientsPtr clients = Clients(g);
	TApplPtr appl = clients->appls[ref];

	/* clear first application public information */
	pub(appl, info) = 0;
	pub(appl, refNum) = MIDIerrRefNum;
	pub(appl, drvidx) = MIDIerrRefNum;
	pub(appl, filter) = 0;
	RemAllDstCon (appl);
	
	/* clear next kernel private information */
	MSFlushEvs (ref, clients);
	MSFlushDTasks (ref, clients);
	DisposeApplContext (appl->context);
	clients->appls[ref] = 0;
	FreeAppl (appl);
}

/*===========================================================================
  Internal functions implementation
  =========================================================================== */
void setName (MidiName dst, MidiName name)
{
#ifdef PascalNames
	int i, count = *name + 1;
	for (i=0; (i<count) && (i<MaxApplNameLen); i++) {
		*dst++ = *name++;
	}
#else
	int i;
	for (i=0; *name && (i< MaxApplNameLen-1); i++) {
		*dst++ = *name++;
	}
	*dst = 0;
#endif
}

#endif /* MSKernel */

static Boolean equalApplName (TApplPtr ap, MidiName name)
{
	MidiName apname = pub(ap, name);
#ifdef PascalNames
	int count = *apname + 1;
	while (count--) {
		if (*apname++ != *name++) return false;
	}
	return true;
#else
	while ((*apname != 0) && (*apname == *name) ) { apname++; name++; }
	return (*apname == *name);
#endif
}
