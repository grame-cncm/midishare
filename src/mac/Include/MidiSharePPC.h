/*

  Copyright © Grame 1999-2000

  This program is free software; you can redistribute it and modify it under 
  the terms of the GNU General Public License as published by the 
  Free Software Foundation version 2 of the License.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for 
  more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

  Grame Research Laboratory, 9, rue du Garet 69001 Lyon - France
  grame@grame.fr

*/

#ifndef __MidiSharePPC__
#define __MidiSharePPC__

#include "MidiShareDefs.h"


/*******************************************************************************
* 								ENTRY POINTS									
*******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*--------------------------- Global MidiShare environment --------------------*/
short 		MidiGetVersion	(void);
short 		MidiCountAppls	(void);
short 		MidiGetIndAppl 	(short index);
short 		MidiGetNamedAppl(MidiName name);
 
/*----------------------------- SMPTE synchronization -------------------------*/
void 		MidiGetSyncInfo	(SyncInfoPtr p);
void 		MidiSetSyncMode	(unsigned short mode);
long 		MidiGetExtTime	(void);
long 		MidiInt2ExtTime	(long);
long 		MidiExt2IntTime	(long);
void 		MidiTime2Smpte	(long time, short format, SmpteLocPtr loc);
long 		MidiSmpte2Time	(SmpteLocPtr loc);

long*		MidiGetTimeAddr	();

/*----------------------------- Open / close application ----------------------*/
short 		MidiOpen 		(MidiName applName);
void 		MidiClose 		(short refNum);

/*--------------------------- Application configuration -----------------------*/
MidiName 	MidiGetName 	(short refNum);
void 		MidiSetName 	(short refNum, MidiName applName);
void* 		MidiGetInfo 	(short refNum);	
void 		MidiSetInfo 	(short refNum, void* InfoZone);
MidiFilterPtr MidiGetFilter (short refNum);
void 		MidiSetFilter 	(short refNum, MidiFilterPtr f);
RcvAlarmPtr MidiGetRcvAlarm (short refNum);		
void 		MidiSetRcvAlarm (short refNum, RcvAlarmPtr alarm);	
ApplAlarmPtr MidiGetApplAlarm (short refNum);		
void 		MidiSetApplAlarm (short refNum, ApplAlarmPtr alarm);

/*------------------------------- Drivers management --------------------------*/
short 	MidiRegisterDriver   (TDriverInfos * infos, TDriverOperation *op);
void 	MidiUnregisterDriver (short refnum);
short	MidiCountDrivers	();
short	MidiGetIndDriver	(short index);
Boolean	MidiGetDriverInfos 	(short refnum, TDriverInfos * infos);

/*-------------------------------- Slots management ---------------------------*/
SlotRefNum	MidiAddSlot 		(short refnum, MidiName name, SlotDirection direction);
SlotRefNum	MidiGetIndSlot		(short refnum, short index);
void		MidiRemoveSlot 		(SlotRefNum slot);
void		MidiSetSlotName		(SlotRefNum slot, SlotName name);
Boolean		MidiGetSlotInfos 	(SlotRefNum slot, TSlotInfos * infos);
void		MidiConnectSlot		(short port, SlotRefNum slot, Boolean state);
Boolean		MidiIsSlotConnected	(short port, SlotRefNum slot);

/*------------------------- Inter-Application Connections ---------------------*/
void 	MidiConnect 	(short src, short dest , Boolean state);
Boolean MidiIsConnected (short src, short dest);

/*-------------------------- Events and memory managing -----------------------*/
long 	MidiFreeSpace	(void);
long	MidiDesiredSpace ();

MidiEvPtr MidiNewCell 	(void);
void 	  MidiFreeCell 	(MidiEvPtr e);
long 	  MidiTotalSpace (void);
long 	  MidiGrowSpace (long n);

MidiEvPtr MidiNewEv 	(short typeNum);
MidiEvPtr MidiCopyEv 	(MidiEvPtr e);
void 	  MidiFreeEv 	(MidiEvPtr e);

void 	MidiSetField 	(MidiEvPtr e, long f, long v);
long	MidiGetField 	(MidiEvPtr e, long f);
void 	MidiAddField 	(MidiEvPtr e, long v);
long 	MidiCountFields (MidiEvPtr e);

/*------------------------------- Sequence managing ---------------------------*/
MidiSeqPtr 	MidiNewSeq		(void);
void 		MidiAddSeq 		(MidiSeqPtr s, MidiEvPtr e);
void 		MidiFreeSeq 	(MidiSeqPtr s);
void 		MidiClearSeq 	(MidiSeqPtr s);
void 		MidiApplySeq 	(MidiSeqPtr s, ApplyProcPtr proc);

/*------------------------------------- Time ----------------------------------*/
long 	MidiGetTime		(void);

/*------------------------------------ Sending --------------------------------*/
void 	MidiSendIm 		(short refNum, MidiEvPtr e);
void 	MidiSend 		(short refNum, MidiEvPtr e);
void 	MidiSendAt 		(short refNum, MidiEvPtr e, long d);

/*------------------------------------ Receving -------------------------------*/
long 		MidiCountEvs 	(short refNum);
MidiEvPtr 	MidiGetEv 		(short refNum);
MidiEvPtr 	MidiAvailEv 	(short refNum);
void 		MidiFlushEvs 	(short refNum);

/*----------------------------------- Mail boxes ------------------------------*/
void* 	MidiReadSync 	(void* adrMem);
void* 	MidiWriteSync 	(void* adrMem, void* val);

/*---------------------------------- Task Managing ----------------------------*/
void		MidiCall 	(TaskPtr routine, long date, short refNum, long a1,long a2,long a3);
MidiEvPtr	MidiTask 	(TaskPtr routine, long date, short refNum, long a1,long a2,long a3);
MidiEvPtr	MidiDTask 	(TaskPtr routine, long date, short refNum, long a1,long a2,long a3);
void		MidiForgetTask (MidiEvPtr *e);
long		MidiCountDTasks (short refnum);
void		MidiFlushDTasks (short refnum);
void		MidiExec1DTask 	(short refnum);

/*----------------------------------- MidiShare -------------------------------*/
Boolean 	MidiShare();


#ifdef __cplusplus
}
#endif

#endif
