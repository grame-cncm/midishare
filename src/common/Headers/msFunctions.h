
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


#ifndef __msFunctions__
#define __msFunctions__

#include "msDefs.h"

/*******************************************************************************
*                                                               ENTRY POINTS
*******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

#ifdef MSKernel
  void           MidiShareSpecialInit (unsigned long defaultSpace, void *publicMemory);
#endif

/*--------------------------- Global MidiShare environment --------------------*/
  short          MidiGetVersion   (void);

  short          MidiCountAppls   (void);
  short          MidiGetIndAppl   (short index);
  short          MidiGetNamedAppl (MidiName name);

/*----------------------------- SMPTE synchronization -------------------------*/
  void           MidiGetSyncInfo (SyncInfoPtr p);
  void           MidiSetSyncMode (unsigned short mode);
  long           MidiGetExtTime  (void);
  long           MidiInt2ExtTime (long xl);
  long           MidiExt2IntTime (long xl);
  void           MidiTime2Smpte  (long time, short format, SmpteLocPtr loc);
  long           MidiSmpte2Time  (SmpteLocPtr loc);

/*----------------------------- Open / close application ----------------------*/
  short          MidiOpen  (MidiName applName);
  void           MidiClose (short refNum);

/*--------------------------- Application configuration -----------------------*/
  MidiName       MidiGetName(short refNum);
  void           MidiSetName(short refNum, MidiName applName);
  void*          MidiGetInfo(short refNum);
  void           MidiSetInfo(short refNum,void* InfoZone);
  RcvAlarmPtr    MidiGetRcvAlarm(short refNum);
  void           MidiSetRcvAlarm(short refNum, RcvAlarmPtr alarm);
  ApplAlarmPtr   MidiGetApplAlarm(short refNum);
  void           MidiSetApplAlarm(short refNum, ApplAlarmPtr alarm);

/*----------------------------- Filters configuration -------------------------*/
  MidiFilterPtr  MidiGetFilter(short refNum);
  void           MidiSetFilter(short refNum, MidiFilterPtr f);
 
  MidiFilterPtr  MidiNewFilter (void);
  void           MidiFreeFilter (MidiFilterPtr);
  void           MidiAcceptPort(MidiFilterPtr f, short port, Boolean);
  void           MidiAcceptChan(MidiFilterPtr f, short chan, Boolean);
  void           MidiAcceptType(MidiFilterPtr f, short type, Boolean);
  Boolean        MidiIsAcceptedPort(MidiFilterPtr f, short port);
  Boolean        MidiIsAcceptedChan(MidiFilterPtr f, short chan);
  Boolean        MidiIsAcceptedType(MidiFilterPtr f, short type);

/*------------------------- Inter-Application Connections ---------------------*/
  void           MidiConnect(short src, short dest , Boolean state);
  Boolean        MidiIsConnected(short src, short dest);

/*-------------------------- Events and memory managing -----------------------*/
  unsigned long  MidiFreeSpace(void);
  unsigned long  MidiDesiredSpace(void);
  unsigned long  MidiTotalSpace(void);
  unsigned long  MidiGrowSpace(long n);

  MidiEvPtr      MidiNewCell(void);
  void           MidiFreeCell(MidiEvPtr e);

  MidiEvPtr      MidiNewEv(short typeNum);
  MidiEvPtr      MidiCopyEv(MidiEvPtr e);
  void           MidiFreeEv(MidiEvPtr e);

  void           MidiSetField(MidiEvPtr e, long f, long v);
  long           MidiGetField(MidiEvPtr e, long f);
  void           MidiAddField(MidiEvPtr e, long v);
  long           MidiCountFields(MidiEvPtr e);

  MidiEvPtr      MidiGetLink        (MidiEvPtr e);
  void           MidiSetLink        (MidiEvPtr e, MidiEvPtr next);

  long           MidiGetDate        (MidiEvPtr e);
  void           MidiSetDate        (MidiEvPtr e, long);
  short          MidiGetRefNum      (MidiEvPtr e);
  void           MidiSetRefNum      (MidiEvPtr e, short);
  short          MidiGetType        (MidiEvPtr e);
  void           MidiSetType        (MidiEvPtr e, short);
  short          MidiGetChan        (MidiEvPtr e);
  void           MidiSetChan        (MidiEvPtr e, short);
  short          MidiGetPort        (MidiEvPtr e);
  void           MidiSetPort        (MidiEvPtr e, short);

  long           MidiGetData0       (MidiEvPtr e);
  long           MidiGetData1       (MidiEvPtr e);
  long           MidiGetData2       (MidiEvPtr e);
  long           MidiGetData3       (MidiEvPtr e);
  void           MidiSetData0       (MidiEvPtr e, long);
  void           MidiSetData1       (MidiEvPtr e, long);
  void           MidiSetData2       (MidiEvPtr e, long);
  void           MidiSetData3       (MidiEvPtr e, long);


/*------------------------------- Sequence managing ---------------------------*/
  MidiSeqPtr     MidiNewSeq   (void);
  void           MidiAddSeq   (MidiSeqPtr s, MidiEvPtr e);
  void           MidiFreeSeq  (MidiSeqPtr s);
  void           MidiClearSeq (MidiSeqPtr s);
  void           MidiApplySeq (MidiSeqPtr s, ApplyProcPtr proc);

  MidiEvPtr      MidiGetFirstEv (MidiSeqPtr s);
  MidiEvPtr      MidiGetLastEv  (MidiSeqPtr s);
  void           MidiSetFirstEv (MidiSeqPtr s, MidiEvPtr e);
  void           MidiSetLastEv  (MidiSeqPtr s, MidiEvPtr e);

/*------------------------------------- Time ----------------------------------*/
  unsigned long  MidiGetTime (void);

/*------------------------------------ Sending --------------------------------*/
  void           MidiSendIm (short refNum, MidiEvPtr e);
  void           MidiSend   (short refNum, MidiEvPtr e);
  void           MidiSendAt (short refNum, MidiEvPtr e, unsigned long d);

/*------------------------------------ Receving -------------------------------*/
  unsigned long  MidiCountEvs (short refNum);
  MidiEvPtr      MidiGetEv    (short refNum);
  MidiEvPtr      MidiAvailEv  (short refNum);
  void           MidiFlushEvs (short refNum);

/*------------------------------- Drivers management --------------------------*/
#ifdef MSKernel
  short          MidiRegisterDriver   (TDriverInfos * infos, TDriverOperation *op);
  void           MidiUnregisterDriver (short refnum);
#endif
  short          MidiCountDrivers     (void);
  short          MidiGetIndDriver     (short index);
  Boolean        MidiGetDriverInfos   (short refnum, TDriverInfos * infos);

/*-------------------------------- Slots management ---------------------------*/
#ifdef MSKernel
  SlotRefNum     MidiAddSlot     (short refnum, MidiName name, SlotDirection direction);
  void           MidiSetSlotName (SlotRefNum slot, MidiName name);
  void           MidiRemoveSlot  (SlotRefNum slot);
#endif
  SlotRefNum     MidiGetIndSlot  (short refnum, short index);
  Boolean        MidiGetSlotInfos    (SlotRefNum slot, TSlotInfos * infos);
  void           MidiConnectSlot     (short port, SlotRefNum slot, Boolean state);
  Boolean        MidiIsSlotConnected (short port, SlotRefNum slot);

/*----------------------------------- Mail boxes ------------------------------*/
  void*          MidiReadSync  (void ** adrMem);
  void*          MidiWriteSync (void ** adrMem, void* val);

/*---------------------------------- Task Managing ----------------------------*/
  void           MidiCall (TaskPtr routine, unsigned long date, short refNum, long a1,long a2,long a3);
  MidiEvPtr      MidiTask (TaskPtr routine, unsigned long date, short refNum, long a1,long a2,long a3);
  MidiEvPtr      MidiDTask(TaskPtr routine, unsigned long date, short refNum, long a1,long a2,long a3);
  void           MidiForgetTask(MidiEvPtr FAR *e);
  long           MidiCountDTasks(short refNum);
  void           MidiFlushDTasks(short refNum);
  void           MidiExec1DTask(short refNum);

/*----------------------------------- MidiShare -------------------------------*/
  Boolean        MidiShare(void);

#ifdef __cplusplus
}
#endif

#endif

