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

#include "msAlarms.h"
#include "msAppls.h"
#include "msAppFun.h"
#include "msConnx.h"
#include "msEvents.h"
#include "msExtern.h"
#include "msInit.h"
#include "msSync.h"
#include "msTasks.h"
#include "msXmtRcv.h"

#ifdef PascalNames
# define kMidiShareName 	"\pMidiShare"
#else
# define kMidiShareName 	"MidiShare"
#endif

#define NewAppl(size)		(TApplPtr)AllocateMemory(kernelSharedMemory, size)
#define FreeAppl(appl)		DisposeMemory(appl)

/*===========================================================================
  Internal functions prototypes
  =========================================================================== */
static void     setApplName     (TApplPtr ap, MidiName name);
static Boolean  equalApplName   (TApplPtr ap, MidiName name);
static void     makeAppl	    (TClientsPtr g, TApplPtr ap, short ref, MidiName n);
static void     initFifo        (TFifoPtr fifo);

/*===========================================================================
  External MidiShare functions implementation
  =========================================================================== */		
MSFunctionType(short) MSOpen (MidiName name, TMSGlobalPtr g)
{
	TApplPtr appl;
	TClientsPtr clients = Clients(g);
	short ref = MIDIerrSpace;
	if (clients->nbAppls == 0) {
		MidiShareWakeup(g);
		appl = NewAppl (sizeof(TAppl));
		if (appl) {
		    makeAppl(clients, appl, 0, kMidiShareName);
		}
	}
	if (clients->nbAppls < MaxAppls) {
		appl = NewAppl (sizeof(TAppl));
		if (appl) {
			for (ref = 1; clients->appls[ref]; ref++)
				;
			makeAppl(clients, appl, ref, name);
			CallAlarm (ref, MIDIOpenAppl, clients);
		}
	}
	return ref;
}

/*____________________________________________________________________________*/
MSFunctionType(void) MSClose (short ref, TMSGlobalPtr g)
{
	TClientsPtr clients = Clients(g);
	TApplPtr appl; Boolean cnxChange;
	
	if (CheckRefNum(clients, ref)) {
		appl = clients->appls[ref];
		
		cnxChange = (appl->srcList != 0) || (appl->dstList != 0);
		RemAllSrcCon (appl, Memory(g));
		RemAllDstCon (appl, Memory(g));
		MSFlushEvs (ref, clients);
		MSFlushDTasks (ref, clients);
		FreeAppl (appl);
		clients->appls[ref] = 0;
		clients->nbAppls--;
		DisposeApplContext (appl->context);
		if (clients->nbAppls == 1) {
			FreeAppl(clients->appls[0]);
			clients->appls[0] = 0;
			clients->nbAppls = 0;
			MidiShareSleep(g);
		} else {
			CallAlarm(ref, MIDICloseAppl, clients);
			if (cnxChange) {
				CallAlarm(ref, MIDIChgConnect, clients);
			}
		}
	} 
}

/*____________________________________________________________________________*/
MSFunctionType(short) MSCountAppls (TClientsPtr g)
{
	return g->nbAppls;
}

/*____________________________________________________________________________*/
MSFunctionType(short) MSGetIndAppl (short index, TClientsPtr g)
{
	short ref = -1;
	
	if (index>0 && index<= g->nbAppls) {
		do { 
			ref++;
			if (g->appls[ref]) index--;
		} while (index);
		return ref;
	}
	return MIDIerrIndex;
}

/*____________________________________________________________________________*/
MSFunctionType(short) MSGetNamedAppl (MidiName name, TClientsPtr g)
{
	short ref =0;
	while (ref < MaxAppls && (!g->appls[ref] || !equalApplName(g->appls[ref], name))) {
		ref++;
	}
	return (ref<MaxAppls) ? ref : (short)MIDIerrIndex;
}

/*____________________________________________________________________________*/
MSFunctionType(MidiName) MSGetName(short ref, TClientsPtr g)
{
	if (CheckRefNum(g,ref)) {
		return ref ? g->appls[ref]->name : kMidiShareName;
	} else {
		return 0;
	}
}

/*____________________________________________________________________________*/
MSFunctionType(void) MSSetName(short ref, MidiName name, TClientsPtr g)
{
	if (CheckRefNum(g,ref) && (ref > 0)) {
		setApplName(g->appls[ref], name);
		CallAlarm (ref, MIDIChgName, g);
	}
}

/*____________________________________________________________________________*/
MSFunctionType(FarPtr(void)) MSGetInfo (short ref, TClientsPtr g)
{
	FarPtr(void) info = 0;
	if (CheckRefNum(g,ref)) {
		info = g->appls[ref]->info;
	}
	return info;
}

/*____________________________________________________________________________*/
MSFunctionType(void) MSSetInfo (short ref, FarPtr(void) info, TClientsPtr g)
{
	if (CheckRefNum(g,ref)) {
		g->appls[ref]->info = info;
	}
}

/*____________________________________________________________________________*/
MSFunctionType(MidiFilterPtr) MSGetFilter(short ref, TClientsPtr g)
{
	return CheckRefNum(g,ref) ? g->appls[ref]->filter : 0;
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
  External initialization functions
  =========================================================================== */
void InitAppls (TClientsPtr g, MSMemoryPtr mem)
{
	short i;
	
	g->nbAppls = 0;
	g->memory  = mem;
	g->nextActiveAppl = 0;
	for (i = 0; i < MaxAppls; i++) {
		g->appls[i] = 0;
		g->activesAppls[i] = 0;
	}
}

/*===========================================================================
  Internal functions implementation
  =========================================================================== */
static void setApplName (TApplPtr ap, MidiName name)
{
	MidiName dst= ap->name;
#ifdef PascalNames
	int i, count = *name + 1;
	for (i=0; (i<count) && (i<MaxApplNameLen); i++) {
		*dst++ = *name++;
	}
#else
	for (int i=0; *name && (i<MaxApplNameLen); i++) {
		*dst++ = *name++;
	}
	*dst[MaxApplNameLen-1] = 0;
#endif
}

static Boolean equalApplName (TApplPtr ap, MidiName name)
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

static void initFifo (TFifoPtr fifo)
{
	fifo->count = 0;
	fifo->head = 0;
 	fifo->tail = (MidiEvPtr)&fifo->head;
}

static void makeAppl(TClientsPtr g, TApplPtr appl, short ref, MidiName n)
{
	setApplName(appl, n);	
	appl->context = CreateApplContext();
	appl->info = 0;
	appl->refNum = (uchar)ref;
	appl->rcvFlag = (uchar)kNoRcvFlag;
	appl->rcvAlarm = 0;
	appl->applAlarm = 0;
	appl->srcList = 0;
	appl->dstList = 0;
	appl->filter = 0;
	initFifo (&appl->rcv);
	initFifo (&appl->dTasks);
	g->appls[ref] = appl;
	g->nbAppls++;
}
