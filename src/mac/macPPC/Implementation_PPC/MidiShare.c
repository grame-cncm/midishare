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
  
  modifications history:
   [08-09-99] DF - adaptation to new data structures

*/

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

#include "msInterface.h"

TMSGlobal gMem;
static inline TMSGlobalPtr GetData () { return &gMem; }

MSFunctionType(Boolean) MidiShare () {
	return true;
}

/*--------------------------- Global MidiShare environment --------------------*/
MSFunctionType(short) MidiGetVersion (void) {
	return MSGetVersion (GetData ());
}
MSFunctionType(short) MidiCountAppls (void) {
	return MSCountAppls (Clients(GetData()));
}
MSFunctionType(short) MidiGetIndAppl(short index) {
	return MSGetIndAppl (index, Clients(GetData()));
}
MSFunctionType(short) MidiGetNamedAppl(MidiName name) {
	return MSGetNamedAppl (name, Clients(GetData()));
}

/*----------------------------- SMPTE synchronization -------------------------*/
MSFunctionType(void) MidiGetSyncInfo (SyncInfoPtr p) {
	MSGetSyncInfo (p , GetData());
}
MSFunctionType(void) MidiSetSyncMode (unsigned short mode) {
	MSSetSyncMode (mode, GetData());
}
MSFunctionType(long) MidiGetExtTime	(void) {
	return MSGetExtTime (GetData());
}
MSFunctionType(long) MidiInt2ExtTime (long time) {
	return MSInt2ExtTime (time, GetData());
}
MSFunctionType(long) MidiExt2IntTime (long time) {
	return MSExt2IntTime (time, GetData());
}
MSFunctionType(void) MidiTime2Smpte	(long time, short format, SmpteLocPtr loc) {
	MSTime2Smpte (time, format, loc, GetData());
}
MSFunctionType(long) MidiSmpte2Time	(SmpteLocPtr loc) {
	return MSSmpte2Time (loc, GetData());
}
MSFunctionType(unsigned long *) MidiGetTimeAddr () {
	return MSGetTimeAddr (GetData());
}

/*----------------------------- Open / close application ----------------------*/
MSFunctionType(short) MidiOpen (MidiName applName) {
	return MSOpen (applName, GetData());
}
MSFunctionType(void) MidiClose (short refNum) {
	MSClose (refNum, GetData());
}

/*--------------------------- Application configuration -----------------------*/
MSFunctionType(MidiName) MidiGetName (short refNum) {
	return MSGetName (refNum, Clients(GetData()));
}
MSFunctionType(void) MidiSetName (short refNum, MidiName applName) {
	MSSetName (refNum, applName, Clients(GetData()));
}
MSFunctionType(void *) MidiGetInfo (short refNum) {
	return MSGetInfo (refNum, Clients(GetData()));
}
MSFunctionType(void) MidiSetInfo (short refNum, void* InfoZone) {
	MSSetInfo (refNum, InfoZone, Clients(GetData()));
}
MSFunctionType(MidiFilterPtr) MidiGetFilter (short refNum) {
	return MSGetFilter (refNum, Clients(GetData()));
}
MSFunctionType(void) MidiSetFilter (short refNum,MidiFilterPtr filter) {
  MSSetFilter (refNum, filter, Clients(GetData()));
}
MSFunctionType(RcvAlarmPtr) MidiGetRcvAlarm (short refNum) {
	return MSGetRcvAlarm (refNum, Clients(GetData()));
}
MSFunctionType(void) MidiSetRcvAlarm (short refNum, RcvAlarmPtr alarm) {
	MSSetRcvAlarm (refNum, alarm, Clients(GetData()));
}
MSFunctionType(ApplAlarmPtr) MidiGetApplAlarm (short refNum) {
	return MSGetApplAlarm (refNum, Clients(GetData()));
}
MSFunctionType(void) MidiSetApplAlarm (short refNum, ApplAlarmPtr alarm) {
	MSSetApplAlarm (refNum, alarm, Clients(GetData()));
}

/*------------------------------- Drivers management --------------------------*/
MSFunctionType(short) MidiRegisterDriver (TDriverInfos * infos, TDriverOperation *op) {
	return MSRegisterDriver (infos, op, GetData());
}
MSFunctionType(void) MidiUnregisterDriver (short refnum) {
	MSUnregisterDriver (refnum, GetData());
}
MSFunctionType(short) MidiCountDrivers () {
	return MSCountDrivers (Clients(GetData()));
}
MSFunctionType(short) MidiGetIndDriver (short index) {
	return MSGetIndDriver (index, Clients(GetData()));
}
MSFunctionType(Boolean) MidiGetDriverInfos (short refnum, TDriverInfos * infos) {
	return MSGetDriverInfos (refnum, infos, Clients(GetData()));
}

/*-------------------------------- Slots management ---------------------------*/
MSFunctionType(SlotRefNum) MidiAddSlot (short refnum) {
	return MSAddSlot (refnum, Clients(GetData()));
}
MSFunctionType(SlotRefNum) MidiGetIndSlot (short refnum, short index) {
	return MSGetIndSlot (refnum, index, Clients(GetData()));
}
MSFunctionType(void) MidiRemoveSlot (SlotRefNum slot) {
	MSRemoveSlot (slot, Clients(GetData()));
}
MSFunctionType(Boolean) MidiGetSlotInfos (SlotRefNum slot, TSlotInfos * infos) {
	return MSGetSlotInfos (slot, infos, Clients(GetData()));
}
MSFunctionType(void) MidiConnectSlot (short port, SlotRefNum slot, Boolean state) {
	MSConnectSlot (port, slot, state, Clients(GetData()));
}
MSFunctionType(Boolean) MidiIsSlotConnected (short port, SlotRefNum slot) {
	return MSIsSlotConnected (port, slot, Clients(GetData()));
}

/*------------------------- Inter-Application Connections ---------------------*/
MSFunctionType(void) MidiConnect(short src, short dest , Boolean state) {
	MSConnect (src,dest,state, Clients(GetData()));
}
MSFunctionType(Boolean) MidiIsConnected(short src, short dest) {
	return MSIsConnected (src,dest, Clients(GetData()));
}

/*-------------------------- Events and memory managing -----------------------*/
MSFunctionType(unsigned long) MidiFreeSpace() {
	return MSFreeSpace (Memory(GetData()));
}
MSFunctionType(unsigned long) MidiTotalSpace() {
	return MSTotalSpace (Memory(GetData()));
}
MSFunctionType(unsigned long) MidiDesiredSpace() {
	return MSDesiredSpace (Memory(GetData()));
}

MSFunctionType(MidiEvPtr) MidiNewCell() {
	return MSNewCell (FreeList(Memory(GetData())));
}
MSFunctionType(void) MidiFreeCell(MidiEvPtr e) {
  if (e) MSFreeCell (e, FreeList(Memory(GetData())));
}
MSFunctionType(unsigned long) MidiGrowSpace(long n) {
	return MSGrowSpace (n, Memory(GetData()));
}

MSFunctionType(MidiEvPtr) MidiNewEv(short typeNum) {
	return MSNewEv (typeNum, FreeList(Memory(GetData())));
}
MSFunctionType(MidiEvPtr) MidiCopyEv(MidiEvPtr e) {
	return MSCopyEv (e, FreeList(Memory(GetData())));
}
MSFunctionType(void) MidiFreeEv(MidiEvPtr e) {
	MSFreeEv (e, FreeList(Memory(GetData())));
}

MSFunctionType(void) MidiSetField(MidiEvPtr e, long f, long v) {
	MSSetField (e,f,v);
}
MSFunctionType(long) MidiGetField(MidiEvPtr e, long f) {
	return MSGetField (e,f);
}
MSFunctionType(void) MidiAddField(MidiEvPtr e, long v) {
	MSAddField (e,v, FreeList(Memory(GetData())));
}
MSFunctionType(long) MidiCountFields(MidiEvPtr e) {
	return MSCountFields (e);
}

/*------------------------------- Sequence managing ---------------------------*/
MSFunctionType(MidiSeqPtr) MidiNewSeq() {
	return MSNewSeq (FreeList(Memory(GetData())));
}
MSFunctionType(void) MidiAddSeq(MidiSeqPtr s, MidiEvPtr e) {
	MSAddSeq (s, e);
}
MSFunctionType(void) MidiFreeSeq(MidiSeqPtr s) {
	MSFreeSeq (s, FreeList(Memory(GetData())));
}
MSFunctionType(void) MidiClearSeq(MidiSeqPtr s) {
	MSClearSeq (s, FreeList(Memory(GetData())));
}
MSFunctionType(void) MidiApplySeq(MidiSeqPtr s, ApplyProcPtr proc) {
	MSApplySeq (s, proc);
}

/*------------------------------------- Time ----------------------------------*/
MSFunctionType(long) MidiGetTime (void) {
	return MSGetTime (GetData());
}

/*------------------------------------ Sending --------------------------------*/
MSFunctionType(void) MidiSendIm(short refNum, MidiEvPtr e) {
	MSSendAt (refNum,e, CurrTime(GetData()), SorterList(GetData()));
}
MSFunctionType(void) MidiSend(short refNum, MidiEvPtr e) {
	MSSend (refNum,e, SorterList(GetData()));
}
MSFunctionType(void) MidiSendAt(short refNum, MidiEvPtr e, long d) {
	MSSendAt (refNum,e,d, SorterList(GetData()));
}

/*----------------------------------- Receiving -------------------------------*/
MSFunctionType(unsigned long) MidiCountEvs(short refNum) {
	return MSCountEvs (refNum, Clients(GetData()));
}
MSFunctionType(MidiEvPtr) MidiGetEv(short refNum) {
	return MSGetEv (refNum, Clients(GetData()));
}
MSFunctionType(MidiEvPtr) MidiAvailEv(short refNum) {
	return MSAvailEv (refNum, Clients(GetData()));
}
MSFunctionType(void) MidiFlushEvs(short refNum) {
	MSFlushEvs (refNum, Clients(GetData()));
}

/*----------------------------------- Mail boxes ------------------------------*/
MSFunctionType(void *) MidiReadSync(void * adrMem) {
	return MSReadSync ((void **)adrMem);
}
MSFunctionType(void *) MidiWriteSync(void* adrMem, void* val) {
	return MSWriteSync ((void **)adrMem, val);
}

/*---------------------------------- Task Managing ----------------------------*/
MSFunctionType(void) MidiCall(TaskPtr routine, long date, short refNum, long a1,long a2,long a3) {
	TMSGlobalPtr g = GetData();
	MSCall (routine,date,refNum,a1,a2,a3, FreeList(Memory(g)), SorterList(g));
}
MSFunctionType(MidiEvPtr) MidiTask(TaskPtr routine, long date, short refNum, long a1,long a2,long a3) {
	TMSGlobalPtr g = GetData();
	return MSTask (routine,date,refNum,a1,a2,a3, FreeList(Memory(g)), SorterList(g));
}
MSFunctionType(MidiEvPtr) MidiDTask(TaskPtr routine, long date, short refNum, long a1,long a2,long a3) {
	TMSGlobalPtr g = GetData();
	return MSDTask (routine,date,refNum,a1,a2,a3, FreeList(Memory(g)), SorterList(g));
}

MSFunctionType(void) MidiForgetTask(MidiEvPtr *e) {
	MSForgetTask (e);
}
MSFunctionType(long) MidiCountDTasks(short refNum) {
	return MSCountDTasks (refNum, Clients(GetData()));
}
MSFunctionType(void) MidiFlushDTasks(short refNum) {
	MSFlushDTasks (refNum, Clients(GetData()));
}
MSFunctionType(void) MidiExec1DTask(short refNum) {
	MSExec1DTask (refNum, Clients(GetData()));
} 
