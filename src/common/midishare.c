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

short  MidiGetVersion(void) {
  	return MSGetVersion(gMem);
}
short  MidiCountAppls(void) {
 	return MSCountAppls(ClientsPub(gMem));
}
short  MidiGetIndAppl(short index) {
  	return MSGetIndAppl( index, ClientsPub(gMem));
}
short  MidiGetNamedAppl(MidiName name) {
 	return MSGetNamedAppl( name, ClientsPub(gMem));
}
 
/*----------------------------- SMPTE synchronization -------------------------*/
void  MidiGetSyncInfo(SyncInfoPtr p) {
 	 MSGetSyncInfo( p, gMem);
}
void  MidiSetSyncMode(unsigned short mode) {
  	MSSetSyncMode( mode, gMem);
}
long  MidiGetExtTime(void) {
  	return MSGetExtTime(gMem);
}
long  MidiInt2ExtTime(long  xl) {
  	return MSInt2ExtTime( xl, gMem);
}
long  MidiExt2IntTime(long xl) {
  	return MSExt2IntTime( xl, gMem);
}
void  MidiTime2Smpte(long time, short format, SmpteLocPtr loc) {
  	MSTime2Smpte( time,format,loc, gMem);
}
long  MidiSmpte2Time(SmpteLocPtr loc) {
  	return MSSmpte2Time( loc, gMem);
}

/*----------------------------- Open / close application ----------------------*/
short  MidiOpen(MidiName applName) {
 	 return MSOpen( applName, gMem);
}
void  MidiClose(short refNum) {
  	MSClose( refNum, gMem);
}

/*--------------------------- Application configuration -----------------------*/
MidiName  MidiGetName(short refNum) {
  	return MSGetName( refNum, ClientsPub(gMem));
}
void  MidiSetName(short refNum, MidiName applName) {
  	MSSetName( refNum,applName, CSArg(gMem));
}
void*  MidiGetInfo(short refNum) {
 	return MSGetInfo( refNum, ClientsPub(gMem));
}
void  MidiSetInfo(short refNum, void* InfoZone) {
  	MSSetInfo( refNum,InfoZone, CSArg(gMem));
}
MidiFilterPtr  MidiGetFilter(short refNum) {
  	return MSGetFilter( refNum, CSArg(gMem));
}
void  MidiSetFilter(short refNum, MidiFilterPtr f) {
  	MSSetFilter( refNum,f, CSArg(gMem));
}
RcvAlarmPtr   MidiGetRcvAlarm(short refNum) {
  	return MSGetRcvAlarm( refNum, CSArg(gMem));
}
void  MidiSetRcvAlarm(short refNum, RcvAlarmPtr alarm) {
  	MSSetRcvAlarm( refNum,alarm, CSArg(gMem));
}
ApplAlarmPtr  MidiGetApplAlarm(short refNum) {
 	return MSGetApplAlarm( refNum, CSArg(gMem));
}
void  MidiSetApplAlarm(short refNum, ApplAlarmPtr alarm) {
  	MSSetApplAlarm( refNum,alarm, CSArg(gMem));
}
MidiFilterPtr  MidiNewFilter(void) {
  	return MSNewFilter();
}
void  MidiFreeFilter(MidiFilterPtr filter) {
  	MSFreeFilter( filter);
}

/*------------------------- Inter-Application Connections ---------------------*/
void  MidiConnect(short src, short dest , Boolean state) {
  	MSConnect( src,dest,state, gMem);
}
Boolean  MidiIsConnected(short src, short dest) {
  	return MSIsConnected( src,dest, ClientsPub(gMem));
}

/*-------------------------- Events and memory managing -----------------------*/
unsigned long  MidiFreeSpace(void) {
  	return MSFreeSpace(Memory(gMem));
}
unsigned long  MidiDesiredSpace(void) {
  	return MSDesiredSpace(Memory(gMem));
}
unsigned long  MidiTotalSpace(void) {
  	return MSTotalSpace(Memory(gMem));
}
unsigned long  MidiGrowSpace(long n) {
  	return MSGrowSpace( n, Memory(gMem));
}

MidiEvPtr  MidiNewCell(void) {
  	return MSNewCell(FreeList(Memory(gMem)));
}
void  MidiFreeCell(MidiEvPtr e) { 
  	if (e) MSFreeCell( e, FreeList(Memory(gMem)));
}
MidiEvPtr  MidiNewEv(short typeNum) {
  	return MSNewEv( typeNum, FreeList(Memory(gMem)));
}
MidiEvPtr  MidiCopyEv(MidiEvPtr e) {
  	return MSCopyEv( e, FreeList(Memory(gMem)));
}
void  MidiFreeEv(MidiEvPtr e) {
  	MSFreeEv( e, FreeList(Memory(gMem)));
}
void  MidiSetField(MidiEvPtr e, long f, long v) {
  	MSSetField( e,f,v);
}
long  MidiGetField(MidiEvPtr e, long f) {
  	return MSGetField( e,f);
}
void  MidiAddField(MidiEvPtr e, long v) {
  	MSAddField( e,v, FreeList(Memory(gMem)));
}
long  MidiCountFields(MidiEvPtr e) {
 	 return MSCountFields( e);
}

MidiEvPtr MidiGetLink   (MidiEvPtr e)          { return Link(e); }
void      MidiSetLink   (MidiEvPtr e, MidiEvPtr next) { Link(e) = next; }
long      MidiGetDate   (MidiEvPtr e)          { return Date(e); }
void      MidiSetDate   (MidiEvPtr e, long d)  { Date(e) = d; }
short     MidiGetRefNum (MidiEvPtr e)          { return RefNum(e); }
void      MidiSetRefNum (MidiEvPtr e, short r) { RefNum(e) = (Byte)r; }
short     MidiGetType   (MidiEvPtr e)          { return EvType(e); }
void      MidiSetType   (MidiEvPtr e, short t) { EvType(e) = (Byte)t; }
short     MidiGetChan   (MidiEvPtr e)          { return Chan(e); }
void      MidiSetChan   (MidiEvPtr e, short c) { Chan(e) = (Byte)c; }
short     MidiGetPort   (MidiEvPtr e)          { return Port(e); }
void      MidiSetPort   (MidiEvPtr e, short p) { Port(e) = (Byte)p; }

long      MidiGetData0  (MidiEvPtr e) { return Data(e)[0]; }
long      MidiGetData1  (MidiEvPtr e) { return Data(e)[1]; }
long      MidiGetData2  (MidiEvPtr e) { return Data(e)[2]; }
long      MidiGetData3  (MidiEvPtr e) { return Data(e)[3]; }
void      MidiSetData0  (MidiEvPtr e, long v) { Data(e)[0]=(Byte)v; }
void      MidiSetData1  (MidiEvPtr e, long v) { Data(e)[1]=(Byte)v; }
void      MidiSetData2  (MidiEvPtr e, long v) { Data(e)[2]=(Byte)v; }
void      MidiSetData3  (MidiEvPtr e, long v) { Data(e)[3]=(Byte)v; }

/*------------------------------- Sequence managing ---------------------------*/
MidiSeqPtr  MidiNewSeq(void) {
  	return MSNewSeq(FreeList(Memory(gMem)));
}
void  MidiAddSeq(MidiSeqPtr s, MidiEvPtr e) {
  	MSAddSeq( s,e);
}
void  MidiFreeSeq(MidiSeqPtr s) {
  	MSFreeSeq( s, FreeList(Memory(gMem)));
}
void  MidiClearSeq(MidiSeqPtr s) {
  	MSClearSeq( s, FreeList(Memory(gMem)));
}
void  MidiApplySeq(MidiSeqPtr s, ApplyProcPtr proc) {
  	MSApplySeq( s,proc);
}

MidiEvPtr  MidiGetFirstEv(MidiSeqPtr s) { return s->first; }
MidiEvPtr  MidiGetLastEv (MidiSeqPtr s)  { return s->last; }
void       MidiSetFirstEv(MidiSeqPtr s, MidiEvPtr e) { s->first = e; }
void       MidiSetLastEv (MidiSeqPtr s, MidiEvPtr e) { s->last = e; }

/*------------------------------------- Time ----------------------------------*/
unsigned long  MidiGetTime() {
  	return gMem->pub->time;
}

/*------------------------------------ Sending --------------------------------*/
void  MidiSendIm(short refNum, MidiEvPtr e) {
  	MSSendIm (refNum, e, gMem);
}
void  MidiSend(short refNum, MidiEvPtr e) {
  	MSSend (refNum, e, gMem);
}
void  MidiSendAt(short refNum, MidiEvPtr e, unsigned long d) {
  	MSSendAt (refNum, e, d, gMem);
}

/*------------------------------------ Receving -------------------------------*/
unsigned long  MidiCountEvs(short refNum) {
  	return MSCountEvs( refNum, gMem);
}
MidiEvPtr  MidiGetEv(short refNum) {
  	return MSGetEv( refNum, gMem);
}
MidiEvPtr  MidiAvailEv(short refNum) {
  	return MSAvailEv( refNum, gMem);
}
void  MidiFlushEvs(short refNum) {
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

short  MidiCountDrivers () {
  	return MSCountDrivers (ClientsPub(gMem));
}
short  MidiGetIndDriver (short index) {
  	return MSGetIndDriver (index, ClientsPub(gMem));
}
Boolean  MidiGetDriverInfos (short refnum, TDriverInfos * infos) {
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

SlotRefNum  MidiGetIndSlot (short refnum, short index) {
  	return MSGetIndSlot (refnum, index, ClientsPub(gMem));
}
Boolean  MidiGetSlotInfos (SlotRefNum slot, TSlotInfos * infos) {
  	return MSGetSlotInfos (slot, infos, ClientsPub(gMem));
}	
void  MidiConnectSlot (short port, SlotRefNum slot, Boolean state) {
  	MSConnectSlot (port, slot, state, gMem);
}
Boolean  MidiIsSlotConnected (short port, SlotRefNum slot) {
  	return MSIsSlotConnected (port, slot, ClientsPub(gMem));
}

/*----------------------------------- Mail boxes ------------------------------*/
void*  MidiReadSync(void** adrMem) {
  	return MSReadSync (adrMem);
}
void*  MidiWriteSync(void** adrMem,void* val) {
  	return MSWriteSync (adrMem,val);
}

/*---------------------------------- Task Managing ----------------------------*/
void  MidiCall(TaskPtr routine, unsigned long date, short refNum, long a1,long a2,long a3) {
  	MSCall (routine, date, refNum, a1, a2, a3, gMem);
}
MidiEvPtr  MidiTask(TaskPtr routine, unsigned long date, short refNum, long a1,long a2,long a3) {
  	return MSTask (routine, date, refNum, a1, a2, a3, gMem);
}
MidiEvPtr  MidiDTask(TaskPtr routine, unsigned long date, short refNum, long a1,long a2,long a3) {
  	return MSDTask (routine, date, refNum, a1, a2, a3, gMem);
}
void  MidiForgetTask(MidiEvPtr FAR *e) {
  	MSForgetTask (e);
}
long  MidiCountDTasks(short refNum) {
  	return MSCountDTasks (refNum, gMem);
}
void  MidiFlushDTasks(short refNum) {
  	MSFlushDTasks (refNum, gMem);
}
void  MidiExec1DTask(short refNum) {
  	MSExec1DTask (refNum, gMem);
} 

/*---------------------------------- Filters --------------------------------*/

void  MidiAcceptPort(MidiFilterPtr f, short port, Boolean state) {
        MSAcceptPort (f, port, state);
}
void  MidiAcceptChan(MidiFilterPtr f, short chan, Boolean state) {
        MSAcceptChan (f, chan, state);
}
void  MidiAcceptType(MidiFilterPtr f, short type, Boolean state) {
        MSAcceptType (f, type, state);
}
Boolean  MidiIsAcceptedPort(MidiFilterPtr f, short port) {
        return MSIsAcceptedPort (f, port);
}
Boolean  MidiIsAcceptedChan(MidiFilterPtr f, short chan) {
        return MSIsAcceptedChan (f, chan);
}
Boolean  MidiIsAcceptedType(MidiFilterPtr f, short type) {
        return MSIsAcceptedType (f, type);
}

#ifdef MSKernel
	Boolean  MidiShare(void) { return true; }
#else
	Boolean  MidiShare(void) { return CheckMidiShare(gMem); }
#endif

