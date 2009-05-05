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

#include "msPrefs.h"

#include <stdio.h>

TMSGlobal       GlobalMem = { { 0 } };
TMSGlobalPtr    gMem = &GlobalMem;

/* main entry point : called at library load time */
void __MSInitialize(void) __attribute__ ((constructor));
void __MSInitialize(void)
{
	MSSpecialInit (LoadSpace(), gMem); 
}


/*--------------------------- Global MidiShare environment --------------------*/
void  MIDISHAREAPI MidiShareSpecialInit(unsigned long defaultSpace) {
  	MSSpecialInit( defaultSpace, gMem);
}
long  MIDISHAREAPI MidiGetError(void) {
  	return MSGetError(gMem);
}
short MIDISHAREAPI MidiGetVersion(void) {
  	return MSGetVersion(gMem);
}
short MIDISHAREAPI MidiCountAppls(void) {
 	return MSCountAppls(Clients(gMem));
}
short MIDISHAREAPI MidiGetIndAppl(short index) {
  	return MSGetIndAppl( index, Clients(gMem));
}
short MIDISHAREAPI MidiGetNamedAppl(MidiName name) {
 	return MSGetNamedAppl( name, Clients(gMem));
}
 
/*----------------------------- SMPTE synchronization -------------------------*/
void MIDISHAREAPI MidiGetSyncInfo(SyncInfoPtr p) {
 	 MSGetSyncInfo( p, gMem);
}
void MIDISHAREAPI MidiSetSyncMode(unsigned short mode) {
  	MSSetSyncMode( mode, gMem);
}
long MIDISHAREAPI MidiGetExtTime(void) {
  	return MSGetExtTime(gMem);
}
long MIDISHAREAPI MidiInt2ExtTime(long  xl) {
  	return MSInt2ExtTime( xl, gMem);
}
long MIDISHAREAPI MidiExt2IntTime(long xl) {
  	return MSExt2IntTime( xl, gMem);
}
void MIDISHAREAPI MidiTime2Smpte(long time, short format, SmpteLocPtr loc) {
  	MSTime2Smpte( time,format,loc, gMem);
}
long MIDISHAREAPI MidiSmpte2Time(SmpteLocPtr loc) {
  	return MSSmpte2Time( loc, gMem);
}

/*----------------------------- Open / close application ----------------------*/
short MIDISHAREAPI MidiOpen(MidiName applName) {
 	 return MSOpen( applName, gMem);
}
void MIDISHAREAPI MidiClose(short refNum) {
  	MSClose( refNum, gMem);
}

/*--------------------------- Application configuration -----------------------*/
MidiName MIDISHAREAPI MidiGetName(short refNum) {
  	return MSGetName( refNum, Clients(gMem));
}
void MIDISHAREAPI MidiSetName(short refNum, MidiName applName) {
  	MSSetName( refNum,applName, Clients(gMem));
}
void* MIDISHAREAPI MidiGetInfo(short refNum) {
 	return MSGetInfo( refNum, Clients(gMem));
}
void MIDISHAREAPI MidiSetInfo(short refNum, void* InfoZone) {
  	MSSetInfo( refNum,InfoZone, Clients(gMem));
}
MidiFilterPtr MIDISHAREAPI MidiGetFilter(short refNum) {
  	return MSGetFilter( refNum, Clients(gMem));
}
void MIDISHAREAPI MidiSetFilter(short refNum, MidiFilterPtr f) {
  	MSSetFilter( refNum,f, Clients(gMem));
}
RcvAlarmPtr  MIDISHAREAPI MidiGetRcvAlarm(short refNum) {
  	return MSGetRcvAlarm( refNum, Clients(gMem));
}
void MIDISHAREAPI MidiSetRcvAlarm(short refNum, RcvAlarmPtr alarm) {
  	MSSetRcvAlarm( refNum,alarm, Clients(gMem));
}
ApplAlarmPtr MIDISHAREAPI MidiGetApplAlarm(short refNum) {
 	return MSGetApplAlarm( refNum, Clients(gMem));
}
void MIDISHAREAPI MidiSetApplAlarm(short refNum, ApplAlarmPtr alarm) {
  	MSSetApplAlarm( refNum,alarm, Clients(gMem));
}
MidiFilterPtr MIDISHAREAPI MidiNewFilter(void) {
  	return MSNewFilter();
}
void MIDISHAREAPI MidiFreeFilter(MidiFilterPtr filter) {
  	MSFreeFilter( filter);
}

/*------------------------- Inter-Application Connections ---------------------*/
void MIDISHAREAPI MidiConnect(short src, short dest , Boolean state) {
  	MSConnect( src,dest,state, Clients(gMem));
}
Boolean MIDISHAREAPI MidiIsConnected(short src, short dest) {
  	return MSIsConnected( src,dest, Clients(gMem));
}

/*-------------------------------- Serial ports -------------------------------*/
Boolean MIDISHAREAPI MidiGetPortState(short port) {
  	return true;
}
void MIDISHAREAPI MidiSetPortState(short port, Boolean state) {
}

/*-------------------------- Events and memory managing -----------------------*/
unsigned long MIDISHAREAPI MidiFreeSpace(void) {
  	return MSFreeSpace(Memory(gMem));
}
unsigned long MIDISHAREAPI MidiDesiredSpace(void) {
  	return MSDesiredSpace(Memory(gMem));
}
MidiEvPtr MIDISHAREAPI MidiNewCell(void) {
  	return MSNewCell(FreeList(Memory(gMem)));
}
void MIDISHAREAPI MidiFreeCell(MidiEvPtr e) { 
  	if (e) MSFreeCell( e, FreeList(Memory(gMem)));
}
unsigned long MIDISHAREAPI MidiTotalSpace(void) {
  	return MSTotalSpace(Memory(gMem));
}
unsigned long MIDISHAREAPI MidiGrowSpace(long n) {
  	return MSGrowSpace( n, Memory(gMem));
}
MidiEvPtr MIDISHAREAPI MidiNewEv(short typeNum) {
  	return MSNewEv( typeNum, FreeList(Memory(gMem)));
}
MidiEvPtr MIDISHAREAPI MidiCopyEv(MidiEvPtr e) {
  	return MSCopyEv( e, FreeList(Memory(gMem)));
}
void MIDISHAREAPI MidiFreeEv(MidiEvPtr e) {
  	MSFreeEv( e, FreeList(Memory(gMem)));
}
void MIDISHAREAPI MidiSetField(MidiEvPtr e, long f, long v) {
  	MSSetField( e,f,v);
}
long MIDISHAREAPI MidiGetField(MidiEvPtr e, long f) {
  	return MSGetField( e,f);
}
void MIDISHAREAPI MidiAddField(MidiEvPtr e, long v) {
  	MSAddField( e,v, FreeList(Memory(gMem)));
}
long MIDISHAREAPI MidiCountFields(MidiEvPtr e) {
 	 return MSCountFields( e);
}

MidiEvPtr MIDISHAREAPI MidiGetLink (MidiEvPtr e) { return Link(e); }
void    MIDISHAREAPI MidiSetLink        (MidiEvPtr e, MidiEvPtr next) { Link(e) = next; }
long    MIDISHAREAPI MidiGetDate        (MidiEvPtr e)   { return Date(e); }
void    MIDISHAREAPI MidiSetDate        (MidiEvPtr e, long d) { Date(e) = d; }
short   MIDISHAREAPI MidiGetRefNum      (MidiEvPtr e)   { return RefNum(e); }
void    MIDISHAREAPI MidiSetRefNum      (MidiEvPtr e, short r) { RefNum(e) = (Byte)r; }
short   MIDISHAREAPI MidiGetType        (MidiEvPtr e)   { return EvType(e); }
void    MIDISHAREAPI MidiSetType        (MidiEvPtr e, short t) { EvType(e) = (Byte)t; }
short   MIDISHAREAPI MidiGetChan        (MidiEvPtr e)   { return Chan(e); }
void    MIDISHAREAPI MidiSetChan        (MidiEvPtr e, short c) { Chan(e) = (Byte)c; }
short   MIDISHAREAPI MidiGetPort        (MidiEvPtr e)   { return Port(e); }
void    MIDISHAREAPI MidiSetPort        (MidiEvPtr e, short p) { Port(e) = (Byte)p; }

long            MIDISHAREAPI MidiGetData0       (MidiEvPtr e) { return Data(e)[0]; }
long            MIDISHAREAPI MidiGetData1       (MidiEvPtr e) { return Data(e)[1]; }
long            MIDISHAREAPI MidiGetData2       (MidiEvPtr e) { return Data(e)[2]; }
long            MIDISHAREAPI MidiGetData3       (MidiEvPtr e) { return Data(e)[3]; }
void            MIDISHAREAPI MidiSetData0       (MidiEvPtr e, long v) { Data(e)[0]=(Byte)v; }
void            MIDISHAREAPI MidiSetData1       (MidiEvPtr e, long v) { Data(e)[1]=(Byte)v; }
void            MIDISHAREAPI MidiSetData2       (MidiEvPtr e, long v) { Data(e)[2]=(Byte)v; }
void            MIDISHAREAPI MidiSetData3       (MidiEvPtr e, long v) { Data(e)[3]=(Byte)v; }

/*------------------------------- Sequence managing ---------------------------*/
MidiSeqPtr MIDISHAREAPI MidiNewSeq(void) {
  	return MSNewSeq(FreeList(Memory(gMem)));
}
void MIDISHAREAPI MidiAddSeq(MidiSeqPtr s, MidiEvPtr e) {
  	MSAddSeq( s,e);
}
void MIDISHAREAPI MidiFreeSeq(MidiSeqPtr s) {
  	MSFreeSeq( s, FreeList(Memory(gMem)));
}
void MIDISHAREAPI MidiClearSeq(MidiSeqPtr s) {
  	MSClearSeq( s, FreeList(Memory(gMem)));
}
void MIDISHAREAPI MidiApplySeq(MidiSeqPtr s, ApplyProcPtr proc) {
  	MSApplySeq( s,proc);
}

MidiEvPtr MIDISHAREAPI MidiGetFirstEv(MidiSeqPtr s) { return s->first; }
MidiEvPtr MIDISHAREAPI MidiGetLastEv (MidiSeqPtr s)  { return s->last; }
void      MIDISHAREAPI MidiSetFirstEv(MidiSeqPtr s, MidiEvPtr e) { s->first = e; }
void      MIDISHAREAPI MidiSetLastEv (MidiSeqPtr s, MidiEvPtr e) { s->last = e; }

/*------------------------------------- Time ----------------------------------*/
unsigned long MIDISHAREAPI MidiGetTime() {
  	return CurrTime(gMem);
}

/*------------------------------------ Sending --------------------------------*/
void MIDISHAREAPI MidiSendIm(short refNum, MidiEvPtr e) {
  	MSSendIm( refNum,e, SorterList(gMem), CurrTime(gMem));
}
void MIDISHAREAPI MidiSend(short refNum, MidiEvPtr e) {
  	MSSend( refNum,e, SorterList(gMem));
}
void MIDISHAREAPI MidiSendAt(short refNum, MidiEvPtr e, unsigned long d) {
  	MSSendAt( refNum,e,d, SorterList(gMem));
}

/*------------------------------------ Receving -------------------------------*/
unsigned long MIDISHAREAPI MidiCountEvs(short refNum) {
  	return MSCountEvs( refNum, Clients(gMem));
}
MidiEvPtr MIDISHAREAPI MidiGetEv(short refNum) {
  	return MSGetEv( refNum, Clients(gMem));
}
MidiEvPtr MIDISHAREAPI MidiAvailEv(short refNum) {
  	return MSAvailEv( refNum, Clients(gMem));
}
void MIDISHAREAPI MidiFlushEvs(short refNum) {
  	MSFlushEvs( refNum, Clients(gMem));
}

/*------------------------------- Drivers management --------------------------*/
short MIDISHAREAPI MidiRegisterDriver (TDriverInfos * infos, TDriverOperation *op) {
  	return MSRegisterDriver (infos, op, gMem);
}
void MIDISHAREAPI MidiUnregisterDriver (short refnum) {
  	MSUnregisterDriver (refnum, gMem);
}
short MIDISHAREAPI MidiCountDrivers () {
  	return MSCountDrivers (Clients(gMem));
}
short MIDISHAREAPI MidiGetIndDriver (short index) {
  	return MSGetIndDriver (index, Clients(gMem));
}
Boolean MIDISHAREAPI MidiGetDriverInfos (short refnum, TDriverInfos * infos) {
  	return MSGetDriverInfos (refnum, infos, Clients(gMem));
}

/*-------------------------------- Slots management ---------------------------*/
SlotRefNum MIDISHAREAPI MidiAddSlot (short refnum, MidiName name, SlotDirection direction) {
  	return MSAddSlot (refnum, name, direction, Clients(gMem));
}
void MIDISHAREAPI MidiSetSlotName (SlotRefNum slot, MidiName name) {
  	MSSetSlotName (slot, name, Clients(gMem));
}
SlotRefNum MIDISHAREAPI MidiGetIndSlot (short refnum, short index) {
  	return MSGetIndSlot (refnum, index, Clients(gMem));
}
void MIDISHAREAPI MidiRemoveSlot (SlotRefNum slot) {
  	MSRemoveSlot (slot, Clients(gMem));
}
Boolean MIDISHAREAPI MidiGetSlotInfos (SlotRefNum slot, TSlotInfos * infos) {
  	return MSGetSlotInfos (slot, infos, Clients(gMem));
}	
void MIDISHAREAPI MidiConnectSlot (short port, SlotRefNum slot, Boolean state) {
  	MSConnectSlot (port, slot, state, Clients(gMem));
}
Boolean MIDISHAREAPI MidiIsSlotConnected (short port, SlotRefNum slot) {
  	return MSIsSlotConnected (port, slot, Clients(gMem));
}

/*----------------------------------- Mail boxes ------------------------------*/
void* MIDISHAREAPI MidiReadSync(void** adrMem) {
  	return MSReadSync( adrMem);
}
void* MIDISHAREAPI MidiWriteSync(void** adrMem,void* val) {
  	return MSWriteSync( adrMem,val);
}

/*---------------------------------- Task Managing ----------------------------*/
void MIDISHAREAPI MidiCall(TaskPtr routine, unsigned long date, short refNum, long a1,long a2,long a3) {
  	MSCall( routine,date,refNum,a1,a2,a3, FreeList(Memory(gMem)), SorterList(gMem));
}
MidiEvPtr MIDISHAREAPI MidiTask(TaskPtr routine, unsigned long date, short refNum, long a1,long a2,long a3) {
  	return MSTask( routine,date,refNum,a1,a2,a3, FreeList(Memory(gMem)), SorterList(gMem));
}
MidiEvPtr MIDISHAREAPI MidiDTask(TaskPtr routine, unsigned long date, short refNum, long a1,long a2,long a3) {
  	return MSDTask( routine,date,refNum,a1,a2,a3, FreeList(Memory(gMem)), SorterList(gMem));
}
void MIDISHAREAPI MidiForgetTask(MidiEvPtr FAR *e) {
  	MSForgetTask( e);
}
long MIDISHAREAPI MidiCountDTasks(short refNum) {
  	return MSCountDTasks( refNum, Clients(gMem));
}
void MIDISHAREAPI MidiFlushDTasks(short refNum) {
  	MSFlushDTasks( refNum, Clients(gMem));
}
void MIDISHAREAPI MidiExec1DTask(short refNum) {
  	MSExec1DTask( refNum, Clients(gMem));
} 

/*---------------------------------- Filters --------------------------------*/

void MIDISHAREAPI MidiAcceptPort(MidiFilterPtr f, short port, Boolean state) {
        MSAcceptPort (f, port, state);
}
void MIDISHAREAPI MidiAcceptChan(MidiFilterPtr f, short chan, Boolean state) {
        MSAcceptChan (f, chan, state);
}
void MIDISHAREAPI MidiAcceptType(MidiFilterPtr f, short type, Boolean state) {
        MSAcceptType (f, type, state);
}
Boolean MIDISHAREAPI MidiIsAcceptedPort(MidiFilterPtr f, short port) {
        return MSIsAcceptedPort (f, port);
}
Boolean MIDISHAREAPI MidiIsAcceptedChan(MidiFilterPtr f, short chan) {
        return MSIsAcceptedChan (f, chan);
}
Boolean MIDISHAREAPI MidiIsAcceptedType(MidiFilterPtr f, short type) {
        return MSIsAcceptedType (f, type);
}

Boolean MIDISHAREAPI MidiShare(void) {
  		return true;
}

