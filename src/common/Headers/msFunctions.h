
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


#ifdef WIN32
# ifdef MSExport
#  define MSAPI(type)	_declspec (dllexport) type
# else
#  define MSAPI(type)	_declspec (dllimport) type
# endif
#endif

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
  MSAPI(short)          MidiGetVersion   (void);

  MSAPI(short)          MidiCountAppls   (void);
  MSAPI(short)          MidiGetIndAppl   (short index);
  MSAPI(short)          MidiGetNamedAppl (MidiName name);

/*----------------------------- SMPTE synchronization -------------------------*/
  MSAPI(void)           MidiGetSyncInfo (SyncInfoPtr p);
  MSAPI(void)           MidiSetSyncMode (unsigned short mode);
  MSAPI(long)           MidiGetExtTime  (void);
  MSAPI(long)           MidiInt2ExtTime (long xl);
  MSAPI(long)           MidiExt2IntTime (long xl);
  MSAPI(void)           MidiTime2Smpte  (long time, short format, SmpteLocPtr loc);
  MSAPI(long)           MidiSmpte2Time  (SmpteLocPtr loc);

/*----------------------------- Open / close application ----------------------*/
  MSAPI(short)          MidiOpen  (MidiName applName);
  MSAPI(void)           MidiClose (short refNum);

/*--------------------------- Application configuration -----------------------*/
  MSAPI(MidiName)       MidiGetName(short refNum);
  MSAPI(void)           MidiSetName(short refNum, MidiName applName);
  MSAPI(void*)          MidiGetInfo(short refNum);
  MSAPI(void)           MidiSetInfo(short refNum,void* InfoZone);
  MSAPI(RcvAlarmPtr)    MidiGetRcvAlarm(short refNum);
  MSAPI(void)           MidiSetRcvAlarm(short refNum, RcvAlarmPtr alarm);
  MSAPI(ApplAlarmPtr)   MidiGetApplAlarm(short refNum);
  MSAPI(void)           MidiSetApplAlarm(short refNum, ApplAlarmPtr alarm);

/*----------------------------- Filters configuration -------------------------*/
  MSAPI(MidiFilterPtr)  MidiGetFilter(short refNum);
  MSAPI(void)           MidiSetFilter(short refNum, MidiFilterPtr f);
 
  MSAPI(MidiFilterPtr)  MidiNewFilter (void);
  MSAPI(void)           MidiFreeFilter (MidiFilterPtr);
  MSAPI(void)           MidiAcceptPort(MidiFilterPtr f, short port, Boolean);
  MSAPI(void)           MidiAcceptChan(MidiFilterPtr f, short chan, Boolean);
  MSAPI(void)           MidiAcceptType(MidiFilterPtr f, short type, Boolean);
  MSAPI(Boolean)        MidiIsAcceptedPort(MidiFilterPtr f, short port);
  MSAPI(Boolean)        MidiIsAcceptedChan(MidiFilterPtr f, short chan);
  MSAPI(Boolean)        MidiIsAcceptedType(MidiFilterPtr f, short type);

/*------------------------- Inter-Application Connections ---------------------*/
  MSAPI(void)           MidiConnect(short src, short dest , Boolean state);
  MSAPI(Boolean)        MidiIsConnected(short src, short dest);

/*-------------------------- Events and memory managing -----------------------*/
  MSAPI(unsigned long)  MidiFreeSpace(void);
  MSAPI(unsigned long)  MidiDesiredSpace(void);
  MSAPI(unsigned long)  MidiTotalSpace(void);
  MSAPI(unsigned long)  MidiGrowSpace(long n);

  MSAPI(MidiEvPtr)      MidiNewCell(void);
  MSAPI(void)           MidiFreeCell(MidiEvPtr e);

  MSAPI(MidiEvPtr)      MidiNewEv(short typeNum);
  MSAPI(MidiEvPtr)      MidiCopyEv(MidiEvPtr e);
  MSAPI(void)           MidiFreeEv(MidiEvPtr e);

  MSAPI(void)           MidiSetField(MidiEvPtr e, long f, long v);
  MSAPI(long)           MidiGetField(MidiEvPtr e, long f);
  MSAPI(void)           MidiAddField(MidiEvPtr e, long v);
  MSAPI(long)           MidiCountFields(MidiEvPtr e);

  MSAPI(MidiEvPtr)      MidiGetLink        (MidiEvPtr e);
  MSAPI(void)           MidiSetLink        (MidiEvPtr e, MidiEvPtr next);

  MSAPI(long)           MidiGetDate        (MidiEvPtr e);
  MSAPI(void)           MidiSetDate        (MidiEvPtr e, long);
  MSAPI(short)          MidiGetRefNum      (MidiEvPtr e);
  MSAPI(void)           MidiSetRefNum      (MidiEvPtr e, short);
  MSAPI(short)          MidiGetType        (MidiEvPtr e);
  MSAPI(void)           MidiSetType        (MidiEvPtr e, short);
  MSAPI(short)          MidiGetChan        (MidiEvPtr e);
  MSAPI(void)           MidiSetChan        (MidiEvPtr e, short);
  MSAPI(short)          MidiGetPort        (MidiEvPtr e);
  MSAPI(void)           MidiSetPort        (MidiEvPtr e, short);

  MSAPI(long)           MidiGetData0       (MidiEvPtr e);
  MSAPI(long)           MidiGetData1       (MidiEvPtr e);
  MSAPI(long)           MidiGetData2       (MidiEvPtr e);
  MSAPI(long)           MidiGetData3       (MidiEvPtr e);
  MSAPI(void)           MidiSetData0       (MidiEvPtr e, long);
  MSAPI(void)           MidiSetData1       (MidiEvPtr e, long);
  MSAPI(void)           MidiSetData2       (MidiEvPtr e, long);
  MSAPI(void)           MidiSetData3       (MidiEvPtr e, long);


/*------------------------------- Sequence managing ---------------------------*/
  MSAPI(MidiSeqPtr)     MidiNewSeq   (void);
  MSAPI(void)           MidiAddSeq   (MidiSeqPtr s, MidiEvPtr e);
  MSAPI(void)           MidiFreeSeq  (MidiSeqPtr s);
  MSAPI(void)           MidiClearSeq (MidiSeqPtr s);
  MSAPI(void)           MidiApplySeq (MidiSeqPtr s, ApplyProcPtr proc);

  MSAPI(MidiEvPtr)      MidiGetFirstEv (MidiSeqPtr s);
  MSAPI(MidiEvPtr)      MidiGetLastEv  (MidiSeqPtr s);
  MSAPI(void)           MidiSetFirstEv (MidiSeqPtr s, MidiEvPtr e);
  MSAPI(void)           MidiSetLastEv  (MidiSeqPtr s, MidiEvPtr e);

/*------------------------------------- Time ----------------------------------*/
  MSAPI(unsigned long)  MidiGetTime (void);

/*------------------------------------ Sending --------------------------------*/
  MSAPI(void)           MidiSendIm (short refNum, MidiEvPtr e);
  MSAPI(void)           MidiSend   (short refNum, MidiEvPtr e);
  MSAPI(void)           MidiSendAt (short refNum, MidiEvPtr e, unsigned long d);

/*------------------------------------ Receving -------------------------------*/
  MSAPI(unsigned long)  MidiCountEvs (short refNum);
  MSAPI(MidiEvPtr)      MidiGetEv    (short refNum);
  MSAPI(MidiEvPtr)      MidiAvailEv  (short refNum);
  MSAPI(void)           MidiFlushEvs (short refNum);

/*------------------------------- Drivers management --------------------------*/
#ifdef MSKernel
  short          MidiRegisterDriver   (TDriverInfos * infos, TDriverOperation *op);
  void           MidiUnregisterDriver (short refnum);
#endif
  MSAPI(short)          MidiCountDrivers     (void);
  MSAPI(short)          MidiGetIndDriver     (short index);
  MSAPI(Boolean)        MidiGetDriverInfos   (short refnum, TDriverInfos * infos);

/*-------------------------------- Slots management ---------------------------*/
#ifdef MSKernel
  SlotRefNum     MidiAddSlot     (short refnum, MidiName name, SlotDirection direction);
  void           MidiSetSlotName (SlotRefNum slot, MidiName name);
  void           MidiRemoveSlot  (SlotRefNum slot);
#endif
  MSAPI(SlotRefNum)     MidiGetIndSlot  (short refnum, short index);
  MSAPI(Boolean)        MidiGetSlotInfos    (SlotRefNum slot, TSlotInfos * infos);
  MSAPI(void)           MidiConnectSlot     (short port, SlotRefNum slot, Boolean state);
  MSAPI(Boolean)        MidiIsSlotConnected (short port, SlotRefNum slot);

/*----------------------------------- Mail boxes ------------------------------*/
  MSAPI(void*)          MidiReadSync  (void ** adrMem);
  MSAPI(void*)          MidiWriteSync (void ** adrMem, void* val);

/*---------------------------------- Task Managing ----------------------------*/
  MSAPI(void)           MidiCall (TaskPtr routine, unsigned long date, short refNum, long a1,long a2,long a3);
  MSAPI(MidiEvPtr)      MidiTask (TaskPtr routine, unsigned long date, short refNum, long a1,long a2,long a3);
  MSAPI(MidiEvPtr)      MidiDTask(TaskPtr routine, unsigned long date, short refNum, long a1,long a2,long a3);
  MSAPI(void)           MidiForgetTask(MidiEvPtr FAR *e);
  MSAPI(long)           MidiCountDTasks(short refNum);
  MSAPI(void)           MidiFlushDTasks(short refNum);
  MSAPI(void)           MidiExec1DTask(short refNum);

/*----------------------------------- MidiShare -------------------------------*/
  MSAPI(Boolean)        MidiShare(void);

#ifdef __cplusplus
}
#endif

#endif

