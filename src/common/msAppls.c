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

#include <stdio.h>

#include "msAlarms.h"
#include "msAppls.h"
#include "msAppFun.h"
#include "msApplTools.h"
#include "msConnx.h"
#include "msDriver.h"
#include "msDrvFun.h"
#include "msEvents.h"
#include "msFilter.h"
#include "msInit.h"
#include "msMem.h"
#include "msMemory.h"
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
static Boolean  equalApplName   (TApplPublicPtr ap, MidiName name);

/*===========================================================================
  External MidiShare functions implementation
  =========================================================================== */		
/*                        functions based on public memory                    */

/*____________________________________________________________________________*/
MSFunctionType(short) MSCountAppls (TClientsPublicPtr g)
{
	return g->nbAppls;
}

/*____________________________________________________________________________*/
MSFunctionType(short) MSGetIndAppl (short index, TClientsPublicPtr g)
{
	short ref = -1;
	
	if (index>0 && index<= g->nbAppls) {
		TApplPublicPtr appl;
		do { 
			ref++;
			appl = &g->appls[ref];
			if ((appl->refNum!=MIDIerrRefNum) && (appl->folder != kDriverFolder)) index--;
		} while (index);
		return ref;
	}
	return MIDIerrIndex;
}

/*____________________________________________________________________________*/
MSFunctionType(short) MSGetNamedAppl (MidiName name, TClientsPublicPtr g)
{
	short ref =0;
	while (ref < MaxAppls && 
		((g->appls[ref].refNum == MIDIerrRefNum) || !equalApplName(&g->appls[ref], name))) {
		ref++;
	}
	return (ref<MaxAppls) ? ref : (short)MIDIerrIndex;
}

/*____________________________________________________________________________*/
MSFunctionType(MidiName) MSGetName(short ref, TClientsPublicPtr g)
{
	if (CheckPublicRefNum(g,ref)) {
		TApplPublicPtr appl = &g->appls[ref];
		return ref ? appl->name : kMidiShareName;
	} else {
		return 0;
	}
}

/*____________________________________________________________________________*/
MSFunctionType(FarPtr(void)) MSGetInfo (short ref, TClientsPublicPtr g)
{
	FarPtr(void) info = 0;
	if (CheckPublicRefNum(g,ref)) {
		TApplPublicPtr appl = &g->appls[ref];
		info = appl->info;
	}
	return info;
}

#ifndef MSKernel
#ifdef PascalNames
	static void     pstring2ev      (MidiEvPtr e, unsigned char * str, lifo * freelist);
#	define str2ev	pstring2ev
#else
	static void     string2ev       (MidiEvPtr e, char * str, lifo * freelist);
#	define str2ev	string2ev
#endif

/*____________________________________________________________________________*/
/*                    functions handled in client  context                    */
/*____________________________________________________________________________*/
MSFunctionType(short) MSOpen (MidiName name, TMSGlobalPtr g)
{
	MidiEvPtr e; char ref; TApplPtr appl;

	if (!InitComm(g)) return MIDIerrComm;

	e = MSNewEv (typeMidiOpen, &g->memory.freeList);
	appl = NewAppl (sizeof(TAppl));
	if (!e || !appl) { ref = MIDIerrSpace; goto err; }

	str2ev (e, name, &g->memory.freeList);
	Date(e) = g->pub->time;
	e = MSSendSync (0, e, g);
	if (!e)  { ref = MIDIerrComm; goto err; }

	if (EvType(e) == typeMidiOpenRes) {
		ref = RefNum(e);
		fifoinit (&appl->rcv);
		fifoinit (&appl->dTasks);
		appl->refNum = ref;
		appl->filter = 0;
		appl->rcvAlarm = 0;
		appl->applAlarm = 0;
		appl->rcvFlag = (uchar)kNoRcvFlag;
		g->appls[appl->refNum] = appl;
		g->nbAppls++;
	}
	else { 
		fprintf(stderr, "MSOpen: unexpected result received %d\n", EvType(e));
		ref = MIDIerrComm; 
		goto err; 
	}
	MSFreeEv (e, &g->memory.freeList);
	return ref;

err:
	if (e) 	MSFreeEv (e, &g->memory.freeList);
	if (appl) FreeAppl (appl);
	CloseComm(g);
	return ref;
}

/*____________________________________________________________________________*/
MSFunctionType(void) MSClose (short ref, TMSGlobalPtr g)
{
    if (CheckGlobRefNum(g,ref)) {
        MidiEvPtr e = MSNewEv (typeMidiClose, &g->memory.freeList);
        if (e) {
            if (MSGetFilter(ref, g)) MSSetFilter (ref, 0, g);
            Date(e) = g->pub->time;
            MSSendSync (ref, e, g);
            FreeAppl (g->appls[ref]);
            g->appls[ref] = 0;
			g->nbAppls--;
        }
        CloseComm(g);
    }
}

/*____________________________________________________________________________*/
/*                         commands handled by the server                     */
/*____________________________________________________________________________*/
MSFunctionType(void) MSSetName(short ref, MidiName name, TMSGlobalPtr g)
{
	MidiEvPtr e = MSNewEv (typeMidiSetName, &g->memory.freeList);
	if (e) {
		str2ev (e, name, &g->memory.freeList);
		Date(e) = g->pub->time;
		MSSend (ref, e, g);
	}
}

/*____________________________________________________________________________*/
MSFunctionType(void) MSSetInfo (short ref, FarPtr(void) info, TMSGlobalPtr g)
{
	MidiEvPtr e = MSNewEv (typeMidiSetInfo, &g->memory.freeList);
	if (e) {
		Date(e) = g->pub->time;
        e->info.longField = (long)info;
		MSSend (ref, e, g);
	}
}

/*____________________________________________________________________________*/
MSFunctionType(void) MSSetFilter(short ref, MidiFilterPtr filter, TMSGlobalPtr g)
{
    if (CheckGlobRefNum(g,ref)) {
        MidiEvPtr e;
        
        if (!g->appls[ref]->filter && !filter) return;
        
        e = MSNewEv (typeMidiSetFilter, &g->memory.freeList);
		if (e) {
#ifdef WIN32
            char * fid =  filter ? GetShMemID(filter) : "";
			string2ev (e, (char *)fid, &g->memory.freeList);
#else
            long * fid =  filter ? GetShMemID(filter) : 0;
            e->info.longField = fid ? *fid : 0;
#endif
			Date(e) = g->pub->time;
			MSSend (ref, e, g);
			g->appls[ref]->filter = filter;
		}
	}
}
 
/*____________________________________________________________________________*/
/*                            functions locally handled                       */
/*____________________________________________________________________________*/

/*____________________________________________________________________________*/
MSFunctionType(MidiFilterPtr) MSGetFilter(short ref, TMSGlobalPtr g)
{
    if (CheckGlobRefNum(g,ref)) {
        return g->appls[ref]->filter;
    }
    return 0;
}

/*____________________________________________________________________________*/
MSFunctionType(RcvAlarmPtr) MSGetRcvAlarm(short ref, TMSGlobalPtr g)
{
	return CheckGlobRefNum(g,ref) ? g->appls[ref]->rcvAlarm : 0;
}

/*____________________________________________________________________________*/
MSFunctionType(void) MSSetRcvAlarm(short ref, RcvAlarmPtr alarm, TMSGlobalPtr g)
{
	if (CheckGlobRefNum(g,ref)) {
		g->appls[ref]->rcvAlarm = alarm;
	}
}

/*____________________________________________________________________________*/
MSFunctionType(ApplAlarmPtr) MSGetApplAlarm(short ref, TMSGlobalPtr g)
{
	return CheckGlobRefNum(g,ref) ? g->appls[ref]->applAlarm : 0;
}

/*____________________________________________________________________________*/
MSFunctionType(void) MSSetApplAlarm(short ref, ApplAlarmPtr alarm, TMSGlobalPtr g)
{
	if (CheckGlobRefNum(g,ref)) {
		g->appls[ref]->applAlarm = alarm;
	}
}

#ifdef PascalNames
static void pstring2ev (MidiEvPtr e, unsigned char * str, lifo * freelist)
{
	unsigned char n = *str++;
	while (n--) MSAddField (e, *str++, freelist);
}
#else
static void string2ev (MidiEvPtr e, char * str, lifo * freelist)
{
	while (*str) MSAddField (e, *str++, freelist);
}
#endif

#else
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
#ifdef __mono__
		MidiShareWakeup(g); Wakeup now done at channel initialization
#else
        OpenDrivers(g);
#endif
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
MSFunctionType(void) MSSetInfo (short ref, FarPtr(void) info, TClientsPtr g)
{
	if (CheckRefNum(g,ref)) {
		pub(g->appls[ref], info) = info;
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

/*____________________________________________________________________________*/
MSFunctionType(MidiFilterPtr) MSGetFilter(short ref, TClientsPtr g)
{
	if (CheckRefNum(g,ref)) {
		return g->appls[ref]->filter;
	}
	return 0;
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
	appl->rcvFlag = (uchar)kNoRcvFlag;
	fifoinit (&appl->rcv);
	fifoinit (&appl->dTasks);
	appl->filter = 0;
	appl->rcvAlarm = 0;
	appl->applAlarm = 0;
	appl->driver = 0;
	appl->netFlag = 0;
	appl->context = CreateApplContext();

	/* set next public information */
	appl->pub = &g->pub->appls[ref];
	appl->pub->folder = (uchar)folder;
	setName(pub(appl,name), name);	
	pub(appl, info) = 0;
	pub(appl, refNum) = (uchar)ref;
	pub(appl, drvidx) = MIDIerrRefNum;
	RemAllDstCon (appl->pub);

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
	RemAllDstCon (appl->pub);
	
	/* clear next kernel private information */
	MSFlushEvs (ref, g);
	MSFlushDTasks (ref, g);
	DisposeApplContext (appl->context);
	clients->appls[ref] = 0;
	FreeAppl (appl);
}

#endif /* MSKernel */

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

static Boolean equalApplName (TApplPublicPtr ap, MidiName name)
{
	MidiName apname = ap->name;
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
