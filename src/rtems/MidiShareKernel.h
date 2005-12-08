/*******************************************************************************
 * C H A M E L E O N    S. D. K.                                               *
 *******************************************************************************
 *  $Archive:: /Chameleon.sdk/system/midishare/rtems/MidiShareKernel.h         $
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

#ifndef __MidiShareKernel_H__
#define	__MidiShareKernel_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _MSC_VER
# define inline __inline
#else
# define inline __inline__
#endif


#include "msDefs.h"
#include "msAlarms.h"
#include "msAppFun.h"
#include "msAppls.h"
#include "msConnx.h"
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


extern TMSGlobalPtr gMemPtr;

/*----------------------------------- MidiShare -------------------------------*/

static inline
BOOL	MidiShare		(void)
		{ return TRUE; }
 
/*--------------------------- Global MidiShare environment --------------------*/

static inline
short 	MidiGetVersion	(void) 			
		{ return MSGetVersion (gMemPtr); }
static inline
short 	MidiCountAppls	(void) 			
		{ return MSCountAppls (Clients(gMemPtr)); }
static inline
short 	MidiGetIndAppl 	(short index) 	
		{ return MSGetIndAppl (index, Clients(gMemPtr)); }
static inline
short 	MidiGetNamedAppl(MidiName name)	
		{ return MSGetNamedAppl (name, Clients(gMemPtr)); }
 
/*----------------------------- SMPTE synchronization -------------------------*/

static inline
void 	MidiGetSyncInfo	(SyncInfoPtr p) 
		{ MSGetSyncInfo (p, gMemPtr); }
static inline
void 	MidiSetSyncMode	(WORD mode) 
		{ MSSetSyncMode (mode, gMemPtr); }
static inline
long 	MidiGetExtTime	(void)      	
		{ return MSGetExtTime (gMemPtr); }
static inline
long 	MidiInt2ExtTime	(long time) 	
		{ return MSInt2ExtTime (time, gMemPtr); }
static inline
long 	MidiExt2IntTime	(long time) 	
		{ return MSExt2IntTime (time, gMemPtr); }
static inline
void 	MidiTime2Smpte	(long time, short format, SmpteLocPtr loc) 
		{ MSTime2Smpte (time, format, loc, gMemPtr); }
static inline
long 	MidiSmpte2Time	(SmpteLocPtr loc) 
		{ return MSSmpte2Time (loc, gMemPtr); }

/*----------------------------- Open / close application ----------------------*/

short	MidiOpen (MidiName name);
void 	MidiClose (short refNum);

/*--------------------------- Application configuration -----------------------*/

static inline
MidiName MidiGetName 		(short refNum) 
		{ return MSGetName (refNum, Clients(gMemPtr)); }
static inline
void 	MidiSetName 		(short refNum, MidiName applName) 
		{ MSSetName (refNum, applName, Clients(gMemPtr)); }
static inline
void* 	MidiGetInfo 		(short refNum) 
		{ return MSGetInfo (refNum, Clients(gMemPtr)); }
static inline
void 	MidiSetInfo 		(short refNum, void* InfoZone) 
		{ MSSetInfo (refNum, InfoZone, Clients(gMemPtr)); }
static inline
MidiFilterPtr MidiGetFilter (short refNum) 
		{ return MSGetFilter (refNum, Clients(gMemPtr)); }
static inline
void 	MidiSetFilter 		(short refNum, MidiFilterPtr f) 
		{ MSSetFilter (refNum, f, Clients(gMemPtr)); }
static inline
RcvAlarmPtr MidiGetRcvAlarm (short refNum) 
		{ return MSGetRcvAlarm (refNum, Clients(gMemPtr)); }
static inline
void 	MidiSetRcvAlarm		(short refNum, RcvAlarmPtr alarm) 
		{ MSSetRcvAlarm (refNum, alarm, Clients(gMemPtr)); }
static inline
ApplAlarmPtr MidiGetApplAlarm (short refNum) 
		{ return MSGetApplAlarm (refNum, Clients(gMemPtr)); }
static inline
void 	MidiSetApplAlarm	(short refNum, ApplAlarmPtr alarm) 
		{ MSSetApplAlarm (refNum, alarm, Clients(gMemPtr)); }

/*------------------------------- Drivers management --------------------------*/

static inline
short 	MidiRegisterDriver 	(TDriverInfos * infos, TDriverOperation *op)
		{ return MSRegisterDriver (infos, op, gMemPtr); }
static inline
void 	MidiUnregisterDriver 	(short refnum)
		{ MSUnregisterDriver (refnum, gMemPtr); }
static inline
short	MidiCountDrivers 	()
		{ return MSCountDrivers (Clients(gMemPtr)); }
static inline
short	MidiGetIndDriver 	(short index)
		{ return MSGetIndDriver (index, Clients(gMemPtr)); }
static inline
BOOL	MidiGetDriverInfos 	(short refnum, TDriverInfos * infos)
		{ return MSGetDriverInfos (refnum, infos, Clients(gMemPtr)); }

/*-------------------------------- Slots management ---------------------------*/

static inline
SlotRefNum	MidiAddSlot 		(short refnum, MidiName name, SlotDirection direction)
			{ return MSAddSlot (refnum, name, direction, Clients(gMemPtr)); }
static inline
SlotRefNum	MidiGetIndSlot		(short refnum, short index)
			{ return MSGetIndSlot (refnum, index, Clients(gMemPtr)); }
static inline
void		MidiRemoveSlot 		(SlotRefNum slot)
			{ MSRemoveSlot (slot, Clients(gMemPtr)); }
static inline
void		MidiSetSlotName 	(SlotRefNum slot, MidiName name)
			{ MSSetSlotName (slot, name, Clients(gMemPtr)); }
static inline
BOOL		MidiGetSlotInfos 	(SlotRefNum slot, TSlotInfos * infos)
			{ return MSGetSlotInfos (slot, infos, Clients(gMemPtr)); }
static inline
void		MidiConnectSlot		(short port, SlotRefNum slot, BOOL state)
			{ MSConnectSlot (port, slot, state, Clients(gMemPtr)); }
static inline
BOOL		MidiIsSlotConnected (short port, SlotRefNum slot)
			{ return MSIsSlotConnected (port, slot, Clients(gMemPtr)); }

/*-------------------------------- Serial ports -------------------------------*/

static inline
BOOL 	MidiGetPortState 	(short port)
			{ return MSGetPortState(port); }
static inline
void 		MidiSetPortState(short port, BOOL state)
			{ MSSetPortState(port, state); }

/*------------------------- Inter-Application Connections ---------------------*/

static inline
void 		MidiConnect (short src, short dest , BOOL state) 
			{ MSConnect (src, dest , state, Clients(gMemPtr)); }
static inline
BOOL 	MidiIsConnected	(short src, short dest) 
			{ return MSIsConnected (src, dest, Clients(gMemPtr)); }

/*-------------------------- Events and memory managing -----------------------*/

static inline
DWORD 		MidiFreeSpace	(void)   
			{ return MSFreeSpace (Memory(gMemPtr)); }
static inline
DWORD 	  	MidiTotalSpace 	(void)   
			{ return MSTotalSpace (Memory(gMemPtr)); }
static inline
DWORD 	  	MidiGrowSpace 	(DWORD n) 
			{ return MSGrowSpace (n, Memory(gMemPtr)); }
static inline
MidiEvPtr 	MidiNewCell 	(void)     
			{ return MSNewCell (FreeList(Memory(gMemPtr))); }
static inline
void 	  	MidiFreeCell 	(MidiEvPtr e) 
			{ MSFreeCell (e, FreeList(Memory(gMemPtr))); }
static inline
MidiEvPtr 	MidiNewEv 	(short typeNum) 
			{ return MSNewEv (typeNum, FreeList(Memory(gMemPtr))); }
static inline
MidiEvPtr 	MidiCopyEv 	(MidiEvPtr e) 
			{ return MSCopyEv (e, FreeList(Memory(gMemPtr))); }
static inline
void 	  	MidiFreeEv 	(MidiEvPtr e) 
			{ MSFreeEv (e, FreeList(Memory(gMemPtr))); }
static inline
void 		MidiSetField 	(MidiEvPtr e, long f, long v) 
			{ MSSetField (e, f, v); }
static inline
long		MidiGetField 	(MidiEvPtr e, long f) 
			{ return MSGetField (e, f); }
static inline
long 		MidiCountFields (MidiEvPtr e)         
			{ return MSCountFields (e); }
static inline
void 		MidiAddField 	(MidiEvPtr e, long v) 
			{ MSAddField (e, v, FreeList(Memory(gMemPtr))); }

/*------------------------------- Sequence managing ---------------------------*/

static inline
MidiSeqPtr 	MidiNewSeq	(void) 
			{ return MSNewSeq (FreeList(Memory(gMemPtr))); }
static inline
void 		MidiAddSeq 	(MidiSeqPtr s, MidiEvPtr e) 
			{ MSAddSeq (s, e); }
static inline
void 		MidiFreeSeq 	(MidiSeqPtr s) 
			{ MSFreeSeq (s, FreeList(Memory(gMemPtr))); }
static inline
void 		MidiClearSeq 	(MidiSeqPtr s) 
			{ MSClearSeq (s, FreeList(Memory(gMemPtr))); }
static inline
void 		MidiApplySeq 	(MidiSeqPtr s, ApplyProcPtr proc) 
			{ MSApplySeq (s, proc); }

/*------------------------------------- Time ----------------------------------*/

static inline
long 		MidiGetTime	(void) 
			{ return MSGetTime (gMemPtr); }

/*------------------------------------ Sending --------------------------------*/

static inline
void 		MidiSendIm 	(short refNum, MidiEvPtr e) 
			{ MSSendIm (refNum, e, SorterList(gMemPtr), CurrTime(gMemPtr)); }
static inline
void 		MidiSend 	(short refNum, MidiEvPtr e) 
			{ MSSend (refNum, e, SorterList(gMemPtr)); }
static inline
void 		MidiSendAt 	(short refNum, MidiEvPtr e, long d) 
			{ MSSendAt (refNum, e, d, SorterList(gMemPtr)); }

/*------------------------------------ Receving -------------------------------*/

static inline
long 		MidiCountEvs 	(short refNum) 
			{ return MSCountEvs (refNum, Clients(gMemPtr)); }
static inline
MidiEvPtr 	MidiGetEv 	(short refNum) 
			{ return MSGetEv (refNum, Clients(gMemPtr)); }
static inline
MidiEvPtr 	MidiAvailEv 	(short refNum) 
			{ return MSAvailEv (refNum, Clients(gMemPtr)); }
static inline
void 		MidiFlushEvs 	(short refNum) 
			{ MSFlushEvs (refNum, Clients(gMemPtr)); }

/*----------------------------------- Mail boxes ------------------------------*/

static inline
void* 		MidiReadSync 	(void** adrMem) 
			{ return MSReadSync (adrMem); }
static inline
void* 		MidiWriteSync 	(void** adrMem, void* val) 
			{ return MSWriteSync (adrMem, val); }

/*---------------------------------- Task Managing ----------------------------*/

static inline
void		MidiCall(TaskPtr routine, long date, short refNum, long a1,long a2,long a3) 
			{ MSCall (routine,date,refNum,a1,a2,a3, FreeList(Memory(gMemPtr)), SorterList(gMemPtr)); }
static inline
MidiEvPtr	MidiTask (TaskPtr task,long date,short refNum,long a1,long a2,long a3) 
			{ return MSTask (task, date, refNum, a1,a2,a3, FreeList(Memory(gMemPtr)), SorterList(gMemPtr)); }
static inline
MidiEvPtr	MidiDTask(TaskPtr task,long date,short refNum,long a1,long a2,long a3) 
			{ return MSDTask (task, date, refNum, a1,a2,a3, FreeList(Memory(gMemPtr)),	SorterList(gMemPtr)); }
static inline
void		MidiForgetTask	(MidiEvPtr *e) 
			{ MSForgetTask (e); }
static inline
long		MidiCountDTasks (short refnum) 
			{ return MSCountDTasks (refnum, Clients(gMemPtr)); }
static inline
void		MidiFlushDTasks (short refnum) 
			{ MSFlushDTasks (refnum, Clients(gMemPtr)); }
static inline
void		MidiExec1DTask 	(short refnum) 
			{ MSExec1DTask (refnum, Clients(gMemPtr)); }

/*---------------------------------- Filter Managing ----------------------------*/

static inline
MidiFilterPtr MidiNewFilter  (void) 
			{ return MSNewFilter (); }
static inline
void		MidiFreeFilter (MidiFilterPtr filter) 
			{ MSFreeFilter (filter) ; }

static inline
void		MidiAcceptPort(MidiFilterPtr f, short port, BOOL state) 
			{ MSAcceptPort (f, port, state); }
static inline
void		MidiAcceptChan(MidiFilterPtr f, short chan, BOOL state) 
			{ MSAcceptChan (f, chan, state); }
static inline
void		MidiAcceptType(MidiFilterPtr f, short type, BOOL state) 
			{ MSAcceptType (f, type, state); }

static inline
BOOL		MidiIsAcceptedPort(MidiFilterPtr f, short port) 
			{ return MSIsAcceptedPort (f, port); }
static inline
BOOL		MidiIsAcceptedChan(MidiFilterPtr f, short chan) 
			{ return MSIsAcceptedChan (f, chan); }
static inline
BOOL		MidiIsAcceptedType(MidiFilterPtr f, short type) 
			{ return MSIsAcceptedType (f, type); }


#ifdef __cplusplus
}
#endif

#endif
