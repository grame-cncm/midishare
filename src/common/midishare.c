/*
  Copyright © Grame 2002

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

#include "msFunctions.h"

#include "msTypes.h"
#include "msAlarms.h"
#include "msAppFun.h"
#include "msConnx.h"
#include "msDrvFun.h"
#include "msEvents.h"
#include "msExtern.h"
#include "msFields.h"
#include "msFilter.h"
#include "msInit.h"
#include "msKernel.h"
#include "msMail.h"
#include "msSeq.h"
#include "msSmpte.h"
#include "msSync.h"
#include "msTasks.h"
#include "msTime.h"
#include "msXmtRcv.h"


TMSGlobal       GlobalMem = { 0 };
TMSGlobalPtr    gMem = &GlobalMem;

#define ClientsPub(g)	&(g)->pub->clients
#ifdef MSKernel
#	define	CSArg(g)	Clients(g)
#else
#	define	CSArg(g)	g
#endif

/*--------------------------- Global MidiShare environment --------------------*/
#ifdef MSKernel
	void   MidiShareSpecialInit(unsigned long defaultSpace, void *publicMemory) {
	  	MSSpecialInit( defaultSpace, gMem, (TMSGlobalPublic *)publicMemory);
	}
#endif

MSAPI(short)  MidiGetVersion(void) {
    return MidiShare() ? MSGetVersion(gMem) : 0;
}
MSAPI(short)  MidiCountAppls(void) {
 	return MSCountAppls(ClientsPub(gMem));
}
MSAPI(short)  MidiGetIndAppl(short index) {
  	return MSGetIndAppl( index, ClientsPub(gMem));
}
MSAPI(short)  MidiGetNamedAppl(MidiName name) {
 	return MSGetNamedAppl( name, ClientsPub(gMem));
}
 
/*----------------------------- SMPTE synchronization -------------------------*/
MSAPI(void)  MidiGetSyncInfo(SyncInfoPtr p) {
 	 MSGetSyncInfo( p, gMem);
}
MSAPI(void)  MidiSetSyncMode(unsigned short mode) {
  	MSSetSyncMode( mode, gMem);
}
MSAPI(long)  MidiGetExtTime(void) {
  	return MSGetExtTime(gMem);
}
MSAPI(long)  MidiInt2ExtTime(long  xl) {
  	return MSInt2ExtTime( xl, gMem);
}
MSAPI(long)  MidiExt2IntTime(long xl) {
  	return MSExt2IntTime( xl, gMem);
}
MSAPI(void)  MidiTime2Smpte(long time, short format, SmpteLocPtr loc) {
  	MSTime2Smpte( time,format,loc, gMem);
}
MSAPI(long)  MidiSmpte2Time(SmpteLocPtr loc) {
  	return MSSmpte2Time( loc, gMem);
}

/*----------------------------- Open / close application ----------------------*/
MSAPI(short)  MidiOpen(MidiName applName) {
 	 return MSOpen( applName, gMem);
}
MSAPI(void)  MidiClose(short refNum) {
  	MSClose( refNum, gMem);
}

/*--------------------------- Application configuration -----------------------*/
MSAPI(MidiName)  MidiGetName(short refNum) {
  	return MSGetName( refNum, ClientsPub(gMem));
}
MSAPI(void)  MidiSetName(short refNum, MidiName applName) {
  	MSSetName( refNum,applName, CSArg(gMem));
}
MSAPI(void*)  MidiGetInfo(short refNum) {
 	return MSGetInfo( refNum, ClientsPub(gMem));
}
MSAPI(void)  MidiSetInfo(short refNum, void* InfoZone) {
  	MSSetInfo( refNum,InfoZone, CSArg(gMem));
}
MSAPI(MidiFilterPtr)  MidiGetFilter(short refNum) {
  	return MSGetFilter( refNum, CSArg(gMem));
}
MSAPI(void)  MidiSetFilter(short refNum, MidiFilterPtr f) {
  	MSSetFilter( refNum,f, CSArg(gMem));
}
MSAPI(RcvAlarmPtr)   MidiGetRcvAlarm(short refNum) {
  	return MSGetRcvAlarm( refNum, CSArg(gMem));
}
MSAPI(void)  MidiSetRcvAlarm(short refNum, RcvAlarmPtr alarm) {
  	MSSetRcvAlarm( refNum,alarm, CSArg(gMem));
}
MSAPI(ApplAlarmPtr)  MidiGetApplAlarm(short refNum) {
 	return MSGetApplAlarm( refNum, CSArg(gMem));
}
MSAPI(void)  MidiSetApplAlarm(short refNum, ApplAlarmPtr alarm) {
  	MSSetApplAlarm( refNum,alarm, CSArg(gMem));
}
MSAPI(MidiFilterPtr)  MidiNewFilter(void) {
  	return MSNewFilter();
}
MSAPI(void)  MidiFreeFilter(MidiFilterPtr filter) {
  	MSFreeFilter( filter);
}

/*------------------------- Inter-Application Connections ---------------------*/
MSAPI(void)  MidiConnect(short src, short dest , Boolean state) {
  	MSConnect( src,dest,state, gMem);
}
MSAPI(Boolean)  MidiIsConnected(short src, short dest) {
  	return MSIsConnected( src,dest, ClientsPub(gMem));
}

/*-------------------------- Events and memory managing -----------------------*/
MSAPI(unsigned long)  MidiFreeSpace(void) {
  	return MSFreeSpace(Memory(gMem));
}
MSAPI(unsigned long)  MidiDesiredSpace(void) {
  	return MSDesiredSpace(Memory(gMem));
}
MSAPI(unsigned long)  MidiTotalSpace(void) {
  	return MSTotalSpace(Memory(gMem));
}
MSAPI(unsigned long)  MidiGrowSpace(long n) {
  	return MSGrowSpace( n, Memory(gMem));
}

MSAPI(MidiEvPtr)  MidiNewCell(void) {
  	return MSNewCell(FreeList(Memory(gMem)));
}
MSAPI(void)  MidiFreeCell(MidiEvPtr e) { 
  	if (e) MSFreeCell( e, FreeList(Memory(gMem)));
}
MSAPI(MidiEvPtr)  MidiNewEv(short typeNum) {
  	return MSNewEv( typeNum, FreeList(Memory(gMem)));
}
MSAPI(MidiEvPtr)  MidiCopyEv(MidiEvPtr e) {
  	return MSCopyEv( e, FreeList(Memory(gMem)));
}
MSAPI(void)  MidiFreeEv(MidiEvPtr e) {
  	MSFreeEv( e, FreeList(Memory(gMem)));
}
MSAPI(void)  MidiSetField(MidiEvPtr e, long f, long v) {
  	MSSetField( e,f,v);
}
MSAPI(long)  MidiGetField(MidiEvPtr e, long f) {
  	return MSGetField( e,f);
}
MSAPI(void)  MidiAddField(MidiEvPtr e, long v) {
  	MSAddField( e,v, FreeList(Memory(gMem)));
}
MSAPI(long)  MidiCountFields(MidiEvPtr e) {
 	 return MSCountFields( e);
}

MSAPI(MidiEvPtr) MidiGetLink   (MidiEvPtr e)          { return Link(e); }
MSAPI(void)      MidiSetLink   (MidiEvPtr e, MidiEvPtr next) { Link(e) = next; }
MSAPI(long)      MidiGetDate   (MidiEvPtr e)          { return Date(e); }
MSAPI(void)      MidiSetDate   (MidiEvPtr e, long d)  { Date(e) = d; }
MSAPI(short)     MidiGetRefNum (MidiEvPtr e)          { return RefNum(e); }
MSAPI(void)      MidiSetRefNum (MidiEvPtr e, short r) { RefNum(e) = (Byte)r; }
MSAPI(short)     MidiGetType   (MidiEvPtr e)          { return EvType(e); }
MSAPI(void)      MidiSetType   (MidiEvPtr e, short t) { EvType(e) = (Byte)t; }
MSAPI(short)     MidiGetChan   (MidiEvPtr e)          { return Chan(e); }
MSAPI(void)      MidiSetChan   (MidiEvPtr e, short c) { Chan(e) = (Byte)c; }
MSAPI(short)     MidiGetPort   (MidiEvPtr e)          { return Port(e); }
MSAPI(void)      MidiSetPort   (MidiEvPtr e, short p) { Port(e) = (Byte)p; }

MSAPI(long)      MidiGetData0  (MidiEvPtr e) { return Data(e)[0]; }
MSAPI(long)      MidiGetData1  (MidiEvPtr e) { return Data(e)[1]; }
MSAPI(long)      MidiGetData2  (MidiEvPtr e) { return Data(e)[2]; }
MSAPI(long)      MidiGetData3  (MidiEvPtr e) { return Data(e)[3]; }
MSAPI(void)      MidiSetData0  (MidiEvPtr e, long v) { Data(e)[0]=(Byte)v; }
MSAPI(void)      MidiSetData1  (MidiEvPtr e, long v) { Data(e)[1]=(Byte)v; }
MSAPI(void)      MidiSetData2  (MidiEvPtr e, long v) { Data(e)[2]=(Byte)v; }
MSAPI(void)      MidiSetData3  (MidiEvPtr e, long v) { Data(e)[3]=(Byte)v; }

/*------------------------------- Sequence managing ---------------------------*/
MSAPI(MidiSeqPtr)  MidiNewSeq(void) {
  	return MSNewSeq(FreeList(Memory(gMem)));
}
MSAPI(void)  MidiAddSeq(MidiSeqPtr s, MidiEvPtr e) {
  	MSAddSeq( s,e);
}
MSAPI(void)  MidiFreeSeq(MidiSeqPtr s) {
  	MSFreeSeq( s, FreeList(Memory(gMem)));
}
MSAPI(void)  MidiClearSeq(MidiSeqPtr s) {
  	MSClearSeq( s, FreeList(Memory(gMem)));
}
MSAPI(void)  MidiApplySeq(MidiSeqPtr s, ApplyProcPtr proc) {
  	MSApplySeq( s,proc);
}

MSAPI(MidiEvPtr)  MidiGetFirstEv(MidiSeqPtr s) { return s->first; }
MSAPI(MidiEvPtr)  MidiGetLastEv (MidiSeqPtr s)  { return s->last; }
MSAPI(void)       MidiSetFirstEv(MidiSeqPtr s, MidiEvPtr e) { s->first = e; }
MSAPI(void)       MidiSetLastEv (MidiSeqPtr s, MidiEvPtr e) { s->last = e; }

/*------------------------------------- Time ----------------------------------*/
MSAPI(unsigned long)  MidiGetTime() {
  	return gMem->pub->time;
}

/*------------------------------------ Sending --------------------------------*/
MSAPI(void)  MidiSendIm(short refNum, MidiEvPtr e) {
  	MSSendIm (refNum, e, gMem);
}
MSAPI(void)  MidiSend(short refNum, MidiEvPtr e) {
  	MSSend (refNum, e, gMem);
}
MSAPI(void)  MidiSendAt(short refNum, MidiEvPtr e, unsigned long d) {
  	MSSendAt (refNum, e, d, gMem);
}

/*------------------------------------ Receving -------------------------------*/
MSAPI(unsigned long)  MidiCountEvs(short refNum) {
  	return MSCountEvs( refNum, gMem);
}
MSAPI(MidiEvPtr)  MidiGetEv(short refNum) {
  	return MSGetEv( refNum, gMem);
}
MSAPI(MidiEvPtr)  MidiAvailEv(short refNum) {
  	return MSAvailEv( refNum, gMem);
}
MSAPI(void)  MidiFlushEvs(short refNum) {
  	MSFlushEvs( refNum, gMem);
}

/*------------------------------- Drivers management --------------------------*/
#ifdef MSKernel
	short  MidiRegisterDriver (TDriverInfos * infos, TDriverOperation *op) {
	  	return MSRegisterDriver (infos, op, gMem);
	}
	void  MidiUnregisterDriver (short refnum) {
	  	MSUnregisterDriver (refnum, gMem);
	}
#endif

MSAPI(short)  MidiCountDrivers () {
  	return MSCountDrivers (ClientsPub(gMem));
}
MSAPI(short)  MidiGetIndDriver (short index) {
  	return MSGetIndDriver (index, ClientsPub(gMem));
}
MSAPI(Boolean)  MidiGetDriverInfos (short refnum, TDriverInfos * infos) {
  	return MSGetDriverInfos (refnum, infos, ClientsPub(gMem));
}

/*-------------------------------- Slots management ---------------------------*/
#ifdef MSKernel
	SlotRefNum  MidiAddSlot (short refnum, MidiName name, SlotDirection direction) {
	  	return MSAddSlot (refnum, name, direction, Clients(gMem));
	}
	void  MidiRemoveSlot (SlotRefNum slot) {
	  	MSRemoveSlot (slot, Clients(gMem));
	}
	void  MidiSetSlotName (SlotRefNum slot, MidiName name) {
	  	MSSetSlotName (slot, name, Clients(gMem));
	}
#endif

MSAPI(SlotRefNum)  MidiGetIndSlot (short refnum, short index) {
  	return MSGetIndSlot (refnum, index, ClientsPub(gMem));
}
MSAPI(Boolean)  MidiGetSlotInfos (SlotRefNum slot, TSlotInfos * infos) {
  	return MSGetSlotInfos (slot, infos, ClientsPub(gMem));
}	
MSAPI(void)  MidiConnectSlot (short port, SlotRefNum slot, Boolean state) {
  	MSConnectSlot (port, slot, state, gMem);
}
MSAPI(Boolean)  MidiIsSlotConnected (short port, SlotRefNum slot) {
  	return MSIsSlotConnected (port, slot, ClientsPub(gMem));
}

/*----------------------------------- Mail boxes ------------------------------*/
MSAPI(void*)  MidiReadSync(void** adrMem) {
  	return MSReadSync (adrMem);
}
MSAPI(void*)  MidiWriteSync(void** adrMem,void* val) {
  	return MSWriteSync (adrMem,val);
}

/*---------------------------------- Task Managing ----------------------------*/
MSAPI(void)  MidiCall(TaskPtr routine, unsigned long date, short refNum, long a1,long a2,long a3) {
  	MSCall (routine, date, refNum, a1, a2, a3, gMem);
}
MSAPI(MidiEvPtr)  MidiTask(TaskPtr routine, unsigned long date, short refNum, long a1,long a2,long a3) {
  	return MSTask (routine, date, refNum, a1, a2, a3, gMem);
}
MSAPI(MidiEvPtr)  MidiDTask(TaskPtr routine, unsigned long date, short refNum, long a1,long a2,long a3) {
  	return MSDTask (routine, date, refNum, a1, a2, a3, gMem);
}
MSAPI(void)  MidiForgetTask(MidiEvPtr FAR *e) {
  	MSForgetTask (e);
}
MSAPI(long)  MidiCountDTasks(short refNum) {
  	return MSCountDTasks (refNum, gMem);
}
MSAPI(void)  MidiFlushDTasks(short refNum) {
  	MSFlushDTasks (refNum, gMem);
}
MSAPI(void)  MidiExec1DTask(short refNum) {
  	MSExec1DTask (refNum, gMem);
} 

/*---------------------------------- Filters --------------------------------*/

MSAPI(void)  MidiAcceptPort(MidiFilterPtr f, short port, Boolean state) {
        MSAcceptPort (f, port, state);
}
MSAPI(void)  MidiAcceptChan(MidiFilterPtr f, short chan, Boolean state) {
        MSAcceptChan (f, chan, state);
}
MSAPI(void)  MidiAcceptType(MidiFilterPtr f, short type, Boolean state) {
        MSAcceptType (f, type, state);
}
MSAPI(Boolean)  MidiIsAcceptedPort(MidiFilterPtr f, short port) {
        return MSIsAcceptedPort (f, port);
}
MSAPI(Boolean)  MidiIsAcceptedChan(MidiFilterPtr f, short chan) {
        return MSIsAcceptedChan (f, chan);
}
MSAPI(Boolean)  MidiIsAcceptedType(MidiFilterPtr f, short type) {
        return MSIsAcceptedType (f, type);
}

#ifdef MSKernel
	Boolean  MidiShare(void) { return true; }
#else
	MSAPI(Boolean)  MidiShare(void) { return CheckMidiShare(gMem); }
#endif

