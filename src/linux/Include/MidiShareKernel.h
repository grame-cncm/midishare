#ifndef __MidiShare__
#define __MidiShare__

/*******************************************************************************
* 						MidiShare Driver API									
*******************************************************************************/

#ifdef __cplusplus
"C" {
#endif

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

extern TMSGlobalPtr gMem;
 
/*--------------------------- Global MidiShare environment --------------------*/

static inline short 	MidiGetVersion	(void) 		{ return MSGetVersion (gMem); }
static inline short 	MidiCountAppls	(void) 		{ return MSCountAppls (Clients(gMem)); }
static inline short 	MidiGetIndAppl 	(short index) 	{ return MSGetIndAppl (index, Clients(gMem)); }
static inline short 	MidiGetNamedAppl(MidiName name)	{ return MSGetNamedAppl (name, Clients(gMem)); }
 
/*----------------------------- SMPTE synchronization -------------------------*/

static inline void 	MidiGetSyncInfo	(SyncInfoPtr p) { MSGetSyncInfo (p, gMem); }
static inline void 	MidiSetSyncMode	(unsigned short mode) { MSSetSyncMode (mode, gMem); }
static inline long 	MidiGetExtTime	(void)      	{ return MSGetExtTime (gMem); }
static inline long 	MidiInt2ExtTime	(long time) 	{ return MSInt2ExtTime (time, gMem); }
static inline long 	MidiExt2IntTime	(long time) 	{ return MSExt2IntTime (time, gMem); }
static inline void 	MidiTime2Smpte	(long time, short format, SmpteLocPtr loc) 
						{ MSTime2Smpte (time, format, loc, gMem); }
static inline long 	MidiSmpte2Time	(SmpteLocPtr loc) { return MSSmpte2Time (loc, gMem); }

/*----------------------------- Open / close application ----------------------*/

static inline short 	        MidiOpen (MidiName name)
						{ return MSOpen (name, gMem); }
static inline void 		MidiClose (short refNum) { MSClose (refNum, gMem); }


/*--------------------------- Application configuration -----------------------*/

static inline MidiName 		MidiGetName 	(short refNum) 
						{ return MSGetName (refNum, Clients(gMem)); }
static inline void 		MidiSetName 	(short refNum, MidiName applName) 
						{ MSSetName (refNum, applName, Clients(gMem)); }
static inline void* 		MidiGetInfo 	(short refNum) 
						{ return MSGetInfo (refNum, Clients(gMem)); }
static inline void 		MidiSetInfo 	(short refNum, void* InfoZone) 
						{ MSSetInfo (refNum, InfoZone, Clients(gMem)); }
static inline MidiFilterPtr	MidiGetFilter 	(short refNum) 
						{ return MSGetFilter (refNum, Clients(gMem)); }
static inline void 		MidiSetFilter 	(short refNum, MidiFilterPtr f) 
						{ MSSetFilter (refNum, f, Clients(gMem)); }
static inline RcvAlarmPtr 	MidiGetRcvAlarm (short refNum) 
						{ return MSGetRcvAlarm (refNum, Clients(gMem)); }
static inline void 		MidiSetRcvAlarm	(short refNum, RcvAlarmPtr alarm) 
						{ MSSetRcvAlarm (refNum, alarm, Clients(gMem)); }
static inline ApplAlarmPtr 	MidiGetApplAlarm (short refNum) 
						{ return MSGetApplAlarm (refNum, Clients(gMem)); }
static inline void 		MidiSetApplAlarm (short refNum, ApplAlarmPtr alarm) 
						{ MSSetApplAlarm (refNum, alarm, Clients(gMem)); }


/*------------------------------- Drivers management --------------------------*/

static inline short 	MidiRegisterDriver 	(TDriverInfos * infos, TDriverOperation *op)
						{ return MSRegisterDriver (infos, op, gMem); }
static inline void 	MidiUnregisterDriver 	(short refnum)
						{ MSUnregisterDriver (refnum, gMem); }
static inline short	MidiCountDrivers 	()
						{ return MSCountDrivers (Clients(gMem)); }
static inline short	MidiGetIndDriver 	(short index)
						{ return MSGetIndDriver (index, Clients(gMem)); }
static inline Boolean	MidiGetDriverInfos 	(short refnum, TDriverInfos * infos)
						{ return MSGetDriverInfos (refnum, infos, Clients(gMem)); }


/*-------------------------------- Slots management ---------------------------*/

static inline SlotRefNum	MidiAddSlot 		(short refnum, MidiName name, SlotDirection direction)
							{ return MSAddSlot (refnum, name, direction, Clients(gMem)); }
static inline SlotRefNum	MidiGetIndSlot		(short refnum, short index)
							{ return MSGetIndSlot (refnum, index, Clients(gMem)); }
static inline void		MidiRemoveSlot 		(SlotRefNum slot)
							{ MSRemoveSlot (slot, Clients(gMem)); }
static inline void		MidiSetSlotName 	(SlotRefNum slot, MidiName name)
							{ MSSetSlotName (slot, name, Clients(gMem)); }
static inline Boolean		MidiGetSlotInfos 	(SlotRefNum slot, TSlotInfos * infos)
							{ return MSGetSlotInfos (slot, infos, Clients(gMem)); }
static inline void		MidiConnectSlot		(short port, SlotRefNum slot, Boolean state)
							{ MSConnectSlot (port, slot, state, Clients(gMem)); }
static inline Boolean		MidiIsSlotConnected 	(short port, SlotRefNum slot)
							{ return MSIsSlotConnected (port, slot, Clients(gMem)); }


/*------------------------- Inter-Application Connections ---------------------*/

static inline void 		MidiConnect 	(short src, short dest , Boolean state) 
						{ MSConnect (src, dest , state, Clients(gMem)); }
static inline Boolean 		MidiIsConnected	(short src, short dest) 
						{ return MSIsConnected (src, dest, Clients(gMem)); }


/*-------------------------- Events and memory managing -----------------------*/

static inline long 		MidiFreeSpace	(void)   { return MSFreeSpace (Memory(gMem)); }
static inline long 	  	MidiTotalSpace 	(void)   { return MSTotalSpace (Memory(gMem)); }
static inline long 	  	MidiGrowSpace 	(long n) { return MSGrowSpace (n, Memory(gMem)); }

static inline MidiEvPtr 	MidiNewCell 	(void)     
						{ return MSNewCellFunction (FreeList(Memory(gMem))); }
static inline void 	  	MidiFreeCell 	(MidiEvPtr e) 
						{ MSFreeCellFunction (e, FreeList(Memory(gMem))); }

static inline MidiEvPtr 	MidiNewEv 	(short typeNum) 
						{ return MSNewEv (typeNum, FreeList(Memory(gMem))); }
static inline MidiEvPtr 	MidiCopyEv 	(MidiEvPtr e) 
						{ return MSCopyEv (e, FreeList(Memory(gMem))); }
static inline void 	  	MidiFreeEv 	(MidiEvPtr e) 
						{ MSFreeEv (e, FreeList(Memory(gMem))); }

static inline void 		MidiSetField 	(MidiEvPtr e, long f, long v) { MSSetField (e, f, v); }
static inline long		MidiGetField 	(MidiEvPtr e, long f) { return MSGetField (e, f); }
static inline long 		MidiCountFields (MidiEvPtr e)         { return MSCountFields (e); }
static inline void 		MidiAddField 	(MidiEvPtr e, long v) 
						{ MSAddField (e, v, FreeList(Memory(gMem))); }


/*------------------------------- Sequence managing ---------------------------*/

static inline MidiSeqPtr 	MidiNewSeq	(void) 
						{ return MSNewSeq (FreeList(Memory(gMem))); }
static inline void 		MidiAddSeq 	(MidiSeqPtr s, MidiEvPtr e) 
						{ MSAddSeq (s, e); }
static inline void 		MidiFreeSeq 	(MidiSeqPtr s) 
						{ MSFreeSeq (s, FreeList(Memory(gMem))); }
static inline void 		MidiClearSeq 	(MidiSeqPtr s) 
						{ MSClearSeq (s, FreeList(Memory(gMem))); }
static inline void 		MidiApplySeq 	(MidiSeqPtr s, ApplyProcPtr proc) 
						{ MSApplySeq (s, proc); }


/*------------------------------------- Time ----------------------------------*/

static inline long 		MidiGetTime	(void) { return MSGetTime (gMem); }


/*------------------------------------ Sending --------------------------------*/

static inline void 		MidiSendIm 	(short refNum, MidiEvPtr e) 
						{ MSSendIm (refNum, e, SorterList(gMem), CurrTime(gMem)); }
static inline void 		MidiSend 	(short refNum, MidiEvPtr e) 
						{ MSSend (refNum, e, SorterList(gMem)); }
static inline void 		MidiSendAt 	(short refNum, MidiEvPtr e, long d) 
						{ MSSendAt (refNum, e, d, SorterList(gMem)); }


/*------------------------------------ Receving -------------------------------*/

static inline long 		MidiCountEvs 	(short refNum) 
						{ return MSCountEvs (refNum, Clients(gMem)); }
static inline MidiEvPtr 	MidiGetEv 	(short refNum) 
						{ return MSGetEv (refNum, Clients(gMem)); }
static inline MidiEvPtr 	MidiAvailEv 	(short refNum) 
						{ return MSAvailEv (refNum, Clients(gMem)); }
static inline void 		MidiFlushEvs 	(short refNum) 
						{ MSFlushEvs (refNum, Clients(gMem)); }


/*----------------------------------- Mail boxes ------------------------------*/

static inline void* 	MidiReadSync 	(void* adrMem) 
						{ return MSReadSync (adrMem); }
static inline void* 	MidiWriteSync 	(void* adrMem, void* val) 
						{ return MSWriteSync (adrMem, val); }


/*---------------------------------- Task Managing ----------------------------*/

static inline MidiEvPtr	MidiTask (TaskPtr task,long date,short refNum,long a1,long a2,long a3) 
						{ return MSTask (task, date, refNum, a1,a2,a3, FreeList(Memory(gMem)),
									   	SorterList(gMem)); }
static inline MidiEvPtr	MidiDTask(TaskPtr task,long date,short refNum,long a1,long a2,long a3) 
						{ return MSDTask (task, date, refNum, a1,a2,a3, FreeList(Memory(gMem)),
									   	SorterList(gMem)); }
static inline void		MidiForgetTask	(MidiEvPtr *e) 
						{ MSForgetTask (e); }
static inline long		MidiCountDTasks (short refnum) 
						{ return MSCountDTasks (refnum, Clients(gMem)); }
static inline void		MidiFlushDTasks (short refnum) 
						{ MSFlushDTasks (refnum, Clients(gMem)); }
static inline void		MidiExec1DTask 	(short refnum) 
						{ MSExec1DTask (refnum, Clients(gMem)); }

/*---------------------------------- Filter Managing ----------------------------*/

static inline MidiFilterPtr MidiNewFilter  (void) 
							{ return MSNewFilter (); }
static inline void          MidiFreeFilter (MidiFilterPtr filter) 
							{ MSFreeFilter (filter) ; }

static inline void MidiAcceptPort(MidiFilterPtr f, short port, Boolean state) 
					{ MSAcceptPort (f, port, state); }
static inline void MidiAcceptChan(MidiFilterPtr f, short chan, Boolean state) 
					{ MSAcceptChan (f, chan, state); }
static inline void MidiAcceptType(MidiFilterPtr f, short type, Boolean state) 
					{ MSAcceptType (f, type, state); }

static inline Boolean MidiIsAcceptedPort(MidiFilterPtr f, short port) 
					{ return MSIsAcceptedPort (f, port); }
static inline Boolean MidiIsAcceptedChan(MidiFilterPtr f, short chan) 
					{ return MSIsAcceptedChan (f, chan); }
static inline Boolean MidiIsAcceptedType(MidiFilterPtr f, short type) 
					{ return MSIsAcceptedType (f, type); }


#ifdef __cplusplus
}
#endif

#endif

