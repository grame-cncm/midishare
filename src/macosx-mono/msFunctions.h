
/*

  Copyright © Grame, Sony CSL-Paris 2001

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

/*--------------------------- Global MidiShare environment --------------------*/
  short MIDISHAREAPI MidiGetVersion(void);

  short MIDISHAREAPI MidiCountAppls(void);
  short MIDISHAREAPI MidiGetIndAppl(short index);
  short MIDISHAREAPI MidiGetNamedAppl(MidiName name);

/*----------------------------- SMPTE synchronization -------------------------*/
  void  MIDISHAREAPI MidiGetSyncInfo(SyncInfoPtr p);
  void  MIDISHAREAPI MidiSetSyncMode(unsigned short mode);
  long  MIDISHAREAPI MidiGetExtTime(void);
  long  MIDISHAREAPI MidiInt2ExtTime(long xl);
  long  MIDISHAREAPI MidiExt2IntTime(long xl);
  void  MIDISHAREAPI MidiTime2Smpte(long time, short format, SmpteLocPtr loc);
  long  MIDISHAREAPI MidiSmpte2Time(SmpteLocPtr loc);

/*----------------------------- Open / close application ----------------------*/
  short MIDISHAREAPI MidiOpen(MidiName applName);
  void  MIDISHAREAPI MidiClose(short refNum);

/*--------------------------- Application configuration -----------------------*/
  MidiName      MIDISHAREAPI MidiGetName(short refNum);
  void          MIDISHAREAPI MidiSetName(short refNum, MidiName applName);
  void*         MIDISHAREAPI MidiGetInfo(short refNum);
  void          MIDISHAREAPI MidiSetInfo(short refNum,void* InfoZone);
  RcvAlarmPtr   MIDISHAREAPI MidiGetRcvAlarm(short refNum);
  void          MIDISHAREAPI MidiSetRcvAlarm(short refNum, RcvAlarmPtr alarm);
  ApplAlarmPtr  MIDISHAREAPI MidiGetApplAlarm(short refNum);
  void          MIDISHAREAPI MidiSetApplAlarm(short refNum, ApplAlarmPtr alarm);

/*----------------------------- Filters configuration -------------------------*/
  MidiFilterPtr MIDISHAREAPI MidiGetFilter(short refNum);
  void          MIDISHAREAPI MidiSetFilter(short refNum, MidiFilterPtr f);
 
  MidiFilterPtr MIDISHAREAPI MidiNewFilter (void);
  void          MIDISHAREAPI MidiFreeFilter (MidiFilterPtr);
  void          MIDISHAREAPI MidiAcceptPort(MidiFilterPtr f, short port, Boolean);
  void          MIDISHAREAPI MidiAcceptChan(MidiFilterPtr f, short chan, Boolean);
  void          MIDISHAREAPI MidiAcceptType(MidiFilterPtr f, short type, Boolean);
  Boolean       MIDISHAREAPI MidiIsAcceptedPort(MidiFilterPtr f, short port);
  Boolean       MIDISHAREAPI MidiIsAcceptedChan(MidiFilterPtr f, short chan);
  Boolean       MIDISHAREAPI MidiIsAcceptedType(MidiFilterPtr f, short type);

/*------------------------- Inter-Application Connections ---------------------*/
  void    MIDISHAREAPI MidiConnect(short src, short dest , Boolean state);
  Boolean MIDISHAREAPI MidiIsConnected(short src, short dest);

/*-------------------------------- Serial ports -------------------------------*/
  Boolean MIDISHAREAPI MidiGetPortState(short port);
  void    MIDISHAREAPI MidiSetPortState(short port, Boolean state);

/*-------------------------- Events and memory managing -----------------------*/
  unsigned long MIDISHAREAPI MidiFreeSpace(void);
  unsigned long MIDISHAREAPI MidiTotalSpace(void);
  unsigned long MIDISHAREAPI MidiGrowSpace(long n);

  MidiEvPtr     MIDISHAREAPI MidiNewCell(void);
  void          MIDISHAREAPI MidiFreeCell(MidiEvPtr e);

  MidiEvPtr     MIDISHAREAPI MidiNewEv(short typeNum);
  MidiEvPtr     MIDISHAREAPI MidiCopyEv(MidiEvPtr e);
  void          MIDISHAREAPI MidiFreeEv(MidiEvPtr e);

  void          MIDISHAREAPI MidiSetField(MidiEvPtr e, long f, long v);
  long          MIDISHAREAPI MidiGetField(MidiEvPtr e, long f);
  void          MIDISHAREAPI MidiAddField(MidiEvPtr e, long v);
  long          MIDISHAREAPI MidiCountFields(MidiEvPtr e);

  MidiEvPtr     MIDISHAREAPI MidiGetLink        (MidiEvPtr e);
  void          MIDISHAREAPI MidiSetLink        (MidiEvPtr e, MidiEvPtr next);

  long          MIDISHAREAPI MidiGetDate        (MidiEvPtr e);
  void          MIDISHAREAPI MidiSetDate        (MidiEvPtr e, long);
  short         MIDISHAREAPI MidiGetRefNum      (MidiEvPtr e);
  void          MIDISHAREAPI MidiSetRefNum      (MidiEvPtr e, short);
  short         MIDISHAREAPI MidiGetType        (MidiEvPtr e);
  void          MIDISHAREAPI MidiSetType        (MidiEvPtr e, short);
  short         MIDISHAREAPI MidiGetChan        (MidiEvPtr e);
  void          MIDISHAREAPI MidiSetChan        (MidiEvPtr e, short);
  short         MIDISHAREAPI MidiGetPort        (MidiEvPtr e);
  void          MIDISHAREAPI MidiSetPort        (MidiEvPtr e, short);

  long          MIDISHAREAPI MidiGetData0       (MidiEvPtr e);
  long          MIDISHAREAPI MidiGetData1       (MidiEvPtr e);
  long          MIDISHAREAPI MidiGetData2       (MidiEvPtr e);
  long          MIDISHAREAPI MidiGetData3       (MidiEvPtr e);
  void          MIDISHAREAPI MidiSetData0       (MidiEvPtr e, long);
  void          MIDISHAREAPI MidiSetData1       (MidiEvPtr e, long);
  void          MIDISHAREAPI MidiSetData2       (MidiEvPtr e, long);
  void          MIDISHAREAPI MidiSetData3       (MidiEvPtr e, long);


/*------------------------------- Sequence managing ---------------------------*/
  MidiSeqPtr    MIDISHAREAPI MidiNewSeq(void);
  void          MIDISHAREAPI MidiAddSeq(MidiSeqPtr s, MidiEvPtr e);
  void          MIDISHAREAPI MidiFreeSeq(MidiSeqPtr s);
  void          MIDISHAREAPI MidiClearSeq(MidiSeqPtr s);
  void          MIDISHAREAPI MidiApplySeq(MidiSeqPtr s, ApplyProcPtr proc);

/*------------------------------------- Time ----------------------------------*/
  unsigned long MIDISHAREAPI MidiGetTime(void);

/*------------------------------------ Sending --------------------------------*/
  void MIDISHAREAPI MidiSendIm(short refNum, MidiEvPtr e);
  void MIDISHAREAPI MidiSend(short refNum, MidiEvPtr e);
  void MIDISHAREAPI MidiSendAt(short refNum, MidiEvPtr e, unsigned long d);

/*------------------------------------ Receving -------------------------------*/
  unsigned long   MIDISHAREAPI MidiCountEvs(short refNum);
  MidiEvPtr       MIDISHAREAPI MidiGetEv(short refNum);
  MidiEvPtr       MIDISHAREAPI MidiAvailEv(short refNum);
  void            MIDISHAREAPI MidiFlushEvs(short refNum);

/*----------------------------------- Mail boxes ------------------------------*/
  void* MIDISHAREAPI MidiReadSync( void ** adrMem);
   void* MIDISHAREAPI MidiWriteSync(void ** adrMem, void* val);

/*---------------------------------- Task Managing ----------------------------*/
  void       MIDISHAREAPI MidiCall (TaskPtr routine, unsigned long date, short refNum, void* a1, void* a2, void* a3);
  MidiEvPtr  MIDISHAREAPI MidiTask(TaskPtr routine, unsigned long date, short refNum, void* a1, void* a2, void* a3);
  MidiEvPtr  MIDISHAREAPI MidiDTask(TaskPtr routine, unsigned long date, short refNum, void* a1, void* a2, void* a3);
  void       MIDISHAREAPI MidiForgetTask(MidiEvPtr *e);
  long       MIDISHAREAPI MidiCountDTasks(short refNum);
  void       MIDISHAREAPI MidiFlushDTasks(short refNum);
  void       MIDISHAREAPI MidiExec1DTask(short refNum);

/*----------------------------------- MidiShare -------------------------------*/
  Boolean       MIDISHAREAPI MidiShare(void);

#ifdef __cplusplus
}
#endif

#endif

