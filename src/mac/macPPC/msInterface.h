/*

  Copyright © Grame 2000

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

#ifndef __msInterface__
#define __msInterface__

/*--------------------------- Global MidiShare environment --------------------*/
MSFunctionType(short) 		MidiGetVersion	(void);
MSFunctionType(short) 		MidiCountAppls	(void);
MSFunctionType(short) 		MidiGetIndAppl 	(short index);
MSFunctionType(short) 		MidiGetNamedAppl(MidiName name);
 
/*----------------------------- SMPTE synchronization -------------------------*/
MSFunctionType(void) 		MidiGetSyncInfo	(SyncInfoPtr p);
MSFunctionType(void) 		MidiSetSyncMode	(unsigned short mode);
MSFunctionType(long) 		MidiGetExtTime	(void);
MSFunctionType(long) 		MidiInt2ExtTime	(long);
MSFunctionType(long) 		MidiExt2IntTime	(long);
MSFunctionType(void) 		MidiTime2Smpte	(long time, short format, SmpteLocPtr loc);
MSFunctionType(long) 		MidiSmpte2Time	(SmpteLocPtr loc);

MSFunctionType(unsigned long *)	MidiGetTimeAddr	();

/*----------------------------- Open / close application ----------------------*/
MSFunctionType(short) 		MidiOpen 		(MidiName applName);
MSFunctionType(void) 		MidiClose 		(short refNum);

/*--------------------------- Application configuration -----------------------*/
MSFunctionType(MidiName) 	MidiGetName 	(short refNum);
MSFunctionType(void) 		MidiSetName 	(short refNum, MidiName applName);
MSFunctionType(void *) 		MidiGetInfo 	(short refNum);	
MSFunctionType(void) 		MidiSetInfo 	(short refNum, void* InfoZone);
MSFunctionType(MidiFilterPtr) MidiGetFilter (short refNum);
MSFunctionType(void) 		MidiSetFilter 	(short refNum, MidiFilterPtr f);
MSFunctionType(RcvAlarmPtr) MidiGetRcvAlarm (short refNum);		
MSFunctionType(void) 		MidiSetRcvAlarm (short refNum, RcvAlarmPtr alarm);	
MSFunctionType(ApplAlarmPtr) MidiGetApplAlarm (short refNum);		
MSFunctionType(void) 		MidiSetApplAlarm (short refNum, ApplAlarmPtr alarm);

/*------------------------------- Drivers management --------------------------*/
MSFunctionType(short) 	MidiRegisterDriver   (TDriverInfos * infos, TDriverOperation *op);
MSFunctionType(void) 	MidiUnregisterDriver (short refnum);
MSFunctionType(short)	MidiCountDrivers	();
MSFunctionType(short)	MidiGetIndDriver	(short index);
MSFunctionType(Boolean)	MidiGetDriverInfos 	(short refnum, TDriverInfos * infos);

/*-------------------------------- Slots management ---------------------------*/
MSFunctionType(SlotRefNum)	MidiAddSlot 		(short refnum);
MSFunctionType(SlotRefNum)	MidiGetIndSlot		(short refnum, short index);
MSFunctionType(void)		MidiRemoveSlot 		(SlotRefNum slot);
MSFunctionType(Boolean)		MidiGetSlotInfos 	(SlotRefNum slot, TSlotInfos * infos);
MSFunctionType(void)		MidiConnectSlot		(short port, SlotRefNum slot, Boolean state);
MSFunctionType(Boolean)		MidiIsSlotConnected	(short port, SlotRefNum slot);

/*------------------------- Inter-Application Connections ---------------------*/
MSFunctionType(void) 	MidiConnect 	(short src, short dest , Boolean state);
MSFunctionType(Boolean) MidiIsConnected (short src, short dest);

/*-------------------------- Events and memory managing -----------------------*/
MSFunctionType(unsigned long) 	MidiFreeSpace	(void);
MSFunctionType(unsigned long)	MidiDesiredSpace ();
MSFunctionType(unsigned long) 	MidiTotalSpace (void);

MSFunctionType(MidiEvPtr) MidiNewCell 	(void);
MSFunctionType(void) 	  MidiFreeCell 	(MidiEvPtr e);
MSFunctionType(unsigned long) MidiGrowSpace (long n);

MSFunctionType(MidiEvPtr) MidiNewEv 	(short typeNum);
MSFunctionType(MidiEvPtr) MidiCopyEv 	(MidiEvPtr e);
MSFunctionType(void) 	  MidiFreeEv 	(MidiEvPtr e);

MSFunctionType(void) 	MidiSetField 	(MidiEvPtr e, long f, long v);
MSFunctionType(long)	MidiGetField 	(MidiEvPtr e, long f);
MSFunctionType(void) 	MidiAddField 	(MidiEvPtr e, long v);
MSFunctionType(long) 	MidiCountFields (MidiEvPtr e);

/*------------------------------- Sequence managing ---------------------------*/
MSFunctionType(MidiSeqPtr) 	MidiNewSeq		(void);
MSFunctionType(void) 		MidiAddSeq 		(MidiSeqPtr s, MidiEvPtr e);
MSFunctionType(void) 		MidiFreeSeq 	(MidiSeqPtr s);
MSFunctionType(void) 		MidiClearSeq 	(MidiSeqPtr s);
MSFunctionType(void) 		MidiApplySeq 	(MidiSeqPtr s, ApplyProcPtr proc);

/*------------------------------------- Time ----------------------------------*/
MSFunctionType(long) 	MidiGetTime		(void);

/*------------------------------------ Sending --------------------------------*/
MSFunctionType(void) 	MidiSendIm 		(short refNum, MidiEvPtr e);
MSFunctionType(void) 	MidiSend 		(short refNum, MidiEvPtr e);
MSFunctionType(void) 	MidiSendAt 		(short refNum, MidiEvPtr e, long d);

/*------------------------------------ Receving -------------------------------*/
MSFunctionType(unsigned long) MidiCountEvs 	(short refNum);
MSFunctionType(MidiEvPtr) 	MidiGetEv 		(short refNum);
MSFunctionType(MidiEvPtr) 	MidiAvailEv 	(short refNum);
MSFunctionType(void) 		MidiFlushEvs 	(short refNum);

/*----------------------------------- Mail boxes ------------------------------*/
MSFunctionType(void *) 	MidiReadSync 	(void* adrMem);
MSFunctionType(void *) 	MidiWriteSync 	(void* adrMem, void* val);

/*---------------------------------- Task Managing ----------------------------*/
MSFunctionType(void)		MidiCall 	(TaskPtr routine, long date, short refNum, long a1,long a2,long a3);
MSFunctionType(MidiEvPtr)	MidiTask 	(TaskPtr routine, long date, short refNum, long a1,long a2,long a3);
MSFunctionType(MidiEvPtr)	MidiDTask 	(TaskPtr routine, long date, short refNum, long a1,long a2,long a3);
MSFunctionType(void)		MidiForgetTask (MidiEvPtr *e);
MSFunctionType(long)		MidiCountDTasks (short refnum);
MSFunctionType(void)		MidiFlushDTasks (short refnum);
MSFunctionType(void)		MidiExec1DTask 	(short refnum);

/*----------------------------------- MidiShare -------------------------------*/
MSFunctionType(Boolean) 	MidiShare();

#endif
