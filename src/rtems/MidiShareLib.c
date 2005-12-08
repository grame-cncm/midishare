/*******************************************************************************
 * C H A M E L E O N    S. D. K.                                               *
 *******************************************************************************
 *  $Archive:: /Chameleon.sdk/system/midishare/rtems/MidiShareLib.c            $
 *     $Date: 2005/12/08 13:39:26 $
 * $Revision: 1.1.2.1 $
 *-----------------------------------------------------------------------------*
 * This file is part of the Chameleon Software Development Kit                 *
 *                                                                             *
 * Copyright (C) 2001 soundart                                                 *
 * www.soundart-hot.com                                                        *
 * codemaster@soundart-hot.com                                                 *
 ******************************************************************************/

/*

  This library is free software; you can redistribute it and modify it under 
  the terms of the GNU Library General Public License as published by the 
  Free Software Foundation version 2 of the License, or any later version.

  This library is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public 
  License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#include "msAlarms.h"
#include "msAppFun.h"
#include "msAppls.h"
#include "msConnx.h"
#include "msDefs.h"
#include "msDrvFun.h"
#include "msEvents.h"
#include "msFields.h"
#include "msFilter.h"
#include "msInit.h"
#include "msKernel.h"
#include "msMail.h"
#include "msMem.h"
#include "msMemory.h"
#include "msSeq.h"
#include "msSmpte.h"
#include "msTasks.h"
#include "msTime.h"
#include "msXmtRcv.h"

static TMSGlobal gMem;
TMSGlobalPtr gMemPtr = &gMem;

/*----------------------------------- MidiShare -------------------------------*/

BOOL	MidiShare		(void)
		{ return TRUE; }
 
/*--------------------------- Global MidiShare environment --------------------*/

short 	MidiGetVersion	(void) 			
		{ return MSGetVersion (gMemPtr); }
short 	MidiCountAppls	(void) 			
		{ return MSCountAppls (Clients(gMemPtr)); }
short 	MidiGetIndAppl 	(short index) 	
		{ return MSGetIndAppl (index, Clients(gMemPtr)); }
short 	MidiGetNamedAppl(MidiName name)	
		{ return MSGetNamedAppl (name, Clients(gMemPtr)); }
 
/*----------------------------- SMPTE synchronization -------------------------*/

void 	MidiGetSyncInfo	(SyncInfoPtr p) 
		{ MSGetSyncInfo (p, gMemPtr); }
void 	MidiSetSyncMode	(WORD mode) 
		{ MSSetSyncMode (mode, gMemPtr); }
long 	MidiGetExtTime	(void)      	
		{ return MSGetExtTime (gMemPtr); }
long 	MidiInt2ExtTime	(long time) 	
		{ return MSInt2ExtTime (time, gMemPtr); }
long 	MidiExt2IntTime	(long time) 	
		{ return MSExt2IntTime (time, gMemPtr); }
void 	MidiTime2Smpte	(long time, short format, SmpteLocPtr loc) 
		{ MSTime2Smpte (time, format, loc, gMemPtr); }
long 	MidiSmpte2Time	(SmpteLocPtr loc) 
		{ return MSSmpte2Time (loc, gMemPtr); }

/*--------------------------- Application configuration -----------------------*/

MidiName MidiGetName 		(short refNum) 
		{ return MSGetName (refNum, Clients(gMemPtr)); }
void 	MidiSetName 		(short refNum, MidiName applName) 
		{ MSSetName (refNum, applName, Clients(gMemPtr)); }
void* 	MidiGetInfo 		(short refNum) 
		{ return MSGetInfo (refNum, Clients(gMemPtr)); }
void 	MidiSetInfo 		(short refNum, void* InfoZone) 
		{ MSSetInfo (refNum, InfoZone, Clients(gMemPtr)); }
MidiFilterPtr MidiGetFilter (short refNum) 
		{ return MSGetFilter (refNum, Clients(gMemPtr)); }
void 	MidiSetFilter 		(short refNum, MidiFilterPtr f) 
		{ MSSetFilter (refNum, f, Clients(gMemPtr)); }
RcvAlarmPtr MidiGetRcvAlarm (short refNum) 
		{ return MSGetRcvAlarm (refNum, Clients(gMemPtr)); }
void 	MidiSetRcvAlarm		(short refNum, RcvAlarmPtr alarm) 
		{ MSSetRcvAlarm (refNum, alarm, Clients(gMemPtr)); }
ApplAlarmPtr MidiGetApplAlarm (short refNum) 
		{ return MSGetApplAlarm (refNum, Clients(gMemPtr)); }
void 	MidiSetApplAlarm	(short refNum, ApplAlarmPtr alarm) 
		{ MSSetApplAlarm (refNum, alarm, Clients(gMemPtr)); }

/*------------------------------- Drivers management --------------------------*/

short 	MidiRegisterDriver 	(TDriverInfos * infos, TDriverOperation *op)
		{ return MSRegisterDriver (infos, op, gMemPtr); }
void 	MidiUnregisterDriver 	(short refnum)
		{ MSUnregisterDriver (refnum, gMemPtr); }
short	MidiCountDrivers 	()
		{ return MSCountDrivers (Clients(gMemPtr)); }
short	MidiGetIndDriver 	(short index)
		{ return MSGetIndDriver (index, Clients(gMemPtr)); }
BOOL	MidiGetDriverInfos 	(short refnum, TDriverInfos * infos)
		{ return MSGetDriverInfos (refnum, infos, Clients(gMemPtr)); }

/*-------------------------------- Slots management ---------------------------*/

SlotRefNum	MidiAddSlot 		(short refnum, MidiName name, SlotDirection direction)
			{ return MSAddSlot (refnum, name, direction, Clients(gMemPtr)); }
SlotRefNum	MidiGetIndSlot		(short refnum, short index)
			{ return MSGetIndSlot (refnum, index, Clients(gMemPtr)); }
void		MidiRemoveSlot 		(SlotRefNum slot)
			{ MSRemoveSlot (slot, Clients(gMemPtr)); }
void		MidiSetSlotName 	(SlotRefNum slot, MidiName name)
			{ MSSetSlotName (slot, name, Clients(gMemPtr)); }
BOOL		MidiGetSlotInfos 	(SlotRefNum slot, TSlotInfos * infos)
			{ return MSGetSlotInfos (slot, infos, Clients(gMemPtr)); }
void		MidiConnectSlot		(short port, SlotRefNum slot, BOOL state)
			{ MSConnectSlot (port, slot, state, Clients(gMemPtr)); }
BOOL		MidiIsSlotConnected (short port, SlotRefNum slot)
			{ return MSIsSlotConnected (port, slot, Clients(gMemPtr)); }

/*-------------------------------- Serial ports -------------------------------*/

BOOL 	MidiGetPortState 	(short port)
			{ return MSGetPortState(port); }
void 		MidiSetPortState 	(short port, BOOL state)
			{ MSSetPortState(port, state); }

/*------------------------- Inter-Application Connections ---------------------*/

void 		MidiConnect 		(short src, short dest , BOOL state) 
			{ MSConnect (src, dest , state, Clients(gMemPtr)); }
BOOL 	MidiIsConnected		(short src, short dest) 
			{ return MSIsConnected (src, dest, Clients(gMemPtr)); }

/*-------------------------- Events and memory managing -----------------------*/

long 		MidiFreeSpace	(void)   
			{ return MSFreeSpace (Memory(gMemPtr)); }
long 	  	MidiTotalSpace 	(void)   
			{ return MSTotalSpace (Memory(gMemPtr)); }
long 	  	MidiGrowSpace 	(long n) 
			{ return MSGrowSpace (n, Memory(gMemPtr)); }
MidiEvPtr 	MidiNewCell 	(void)     
			{ return MSNewCell (FreeList(Memory(gMemPtr))); }
void 	  	MidiFreeCell 	(MidiEvPtr e) 
			{ MSFreeCell (e, FreeList(Memory(gMemPtr))); }
MidiEvPtr 	MidiNewEv 	(short typeNum) 
			{ return MSNewEv (typeNum, FreeList(Memory(gMemPtr))); }
MidiEvPtr 	MidiCopyEv 	(MidiEvPtr e) 
			{ return MSCopyEv (e, FreeList(Memory(gMemPtr))); }
void 	  	MidiFreeEv 	(MidiEvPtr e) 
			{ MSFreeEv (e, FreeList(Memory(gMemPtr))); }
void 		MidiSetField 	(MidiEvPtr e, long f, long v) 
			{ MSSetField (e, f, v); }
long		MidiGetField 	(MidiEvPtr e, long f) 
			{ return MSGetField (e, f); }
long 		MidiCountFields (MidiEvPtr e)         
			{ return MSCountFields (e); }
void 		MidiAddField 	(MidiEvPtr e, long v) 
			{ MSAddField (e, v, FreeList(Memory(gMemPtr))); }

/*------------------------------- Sequence managing ---------------------------*/

MidiSeqPtr 	MidiNewSeq	(void) 
			{ return MSNewSeq (FreeList(Memory(gMemPtr))); }
void 		MidiAddSeq 	(MidiSeqPtr s, MidiEvPtr e) 
			{ MSAddSeq (s, e); }
void 		MidiFreeSeq 	(MidiSeqPtr s) 
			{ MSFreeSeq (s, FreeList(Memory(gMemPtr))); }
void 		MidiClearSeq 	(MidiSeqPtr s) 
			{ MSClearSeq (s, FreeList(Memory(gMemPtr))); }
void 		MidiApplySeq 	(MidiSeqPtr s, ApplyProcPtr proc) 
			{ MSApplySeq (s, proc); }

/*------------------------------------- Time ----------------------------------*/

long 		MidiGetTime	(void) 
			{ return MSGetTime (gMemPtr); }

/*------------------------------------ Sending --------------------------------*/

void 		MidiSendIm 	(short refNum, MidiEvPtr e) 
			{ MSSendIm (refNum, e, SorterList(gMemPtr), CurrTime(gMemPtr)); }
void 		MidiSend 	(short refNum, MidiEvPtr e) 
			{ MSSend (refNum, e, SorterList(gMemPtr)); }
void 		MidiSendAt 	(short refNum, MidiEvPtr e, long d) 
			{ MSSendAt (refNum, e, d, SorterList(gMemPtr)); }

/*------------------------------------ Receving -------------------------------*/

long 		MidiCountEvs 	(short refNum) 
			{ return MSCountEvs (refNum, Clients(gMemPtr)); }
MidiEvPtr 	MidiGetEv 	(short refNum) 
			{ return MSGetEv (refNum, Clients(gMemPtr)); }
MidiEvPtr 	MidiAvailEv 	(short refNum) 
			{ return MSAvailEv (refNum, Clients(gMemPtr)); }
void 		MidiFlushEvs 	(short refNum) 
			{ MSFlushEvs (refNum, Clients(gMemPtr)); }

/*----------------------------------- Mail boxes ------------------------------*/

void* 		MidiReadSync 	(void* adrMem) 
			{ return MSReadSync (adrMem); }
void* 		MidiWriteSync 	(void* adrMem, void* val) 
			{ return MSWriteSync (adrMem, val); }

/*---------------------------------- Task Managing ----------------------------*/

void		MidiCall(TaskPtr routine, long date, short refNum, long a1,long a2,long a3) 
			{ MSCall (routine,date,refNum,a1,a2,a3, FreeList(Memory(gMemPtr)), SorterList(gMemPtr)); }
MidiEvPtr	MidiTask (TaskPtr task,long date,short refNum,long a1,long a2,long a3) 
			{ return MSTask (task, date, refNum, a1,a2,a3, FreeList(Memory(gMemPtr)), SorterList(gMemPtr)); }
MidiEvPtr	MidiDTask(TaskPtr task,long date,short refNum,long a1,long a2,long a3) 
			{ return MSDTask (task, date, refNum, a1,a2,a3, FreeList(Memory(gMemPtr)),	SorterList(gMemPtr)); }
void		MidiForgetTask	(MidiEvPtr *e) 
			{ MSForgetTask (e); }
long		MidiCountDTasks (short refnum) 
			{ return MSCountDTasks (refnum, Clients(gMemPtr)); }
void		MidiFlushDTasks (short refnum) 
			{ MSFlushDTasks (refnum, Clients(gMemPtr)); }
void		MidiExec1DTask 	(short refnum) 
			{ MSExec1DTask (refnum, Clients(gMemPtr)); }

/*---------------------------------- Filter Managing ----------------------------*/

MidiFilterPtr MidiNewFilter  (void) 
			{ return MSNewFilter (); }
void		MidiFreeFilter (MidiFilterPtr filter) 
			{ MSFreeFilter (filter) ; }

void		MidiAcceptPort(MidiFilterPtr f, short port, BOOL state) 
			{ MSAcceptPort (f, port, state); }
void		MidiAcceptChan(MidiFilterPtr f, short chan, BOOL state) 
			{ MSAcceptChan (f, chan, state); }
void		MidiAcceptType(MidiFilterPtr f, short type, BOOL state) 
			{ MSAcceptType (f, type, state); }

BOOL		MidiIsAcceptedPort(MidiFilterPtr f, short port) 
			{ return MSIsAcceptedPort (f, port); }
BOOL		MidiIsAcceptedChan(MidiFilterPtr f, short chan) 
			{ return MSIsAcceptedChan (f, chan); }
BOOL		MidiIsAcceptedType(MidiFilterPtr f, short type) 
			{ return MSIsAcceptedType (f, type); }
