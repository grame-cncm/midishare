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

*/

#define __Macintosh__

#include "msTypes.h"

#include "msKernel.h"
#include "msDispatch.h"
#include "msAlarms.h"
#include "msAppFun.h"
#include "msConnx.h"
#include "msEvents.h"
#include "msExtern.h"
#include "msFields.h"
#include "msFilter.h"
#include "msInit.h"
#include "msKernel.h"
#include "msSeq.h"
#include "msSync.h"
#include "msTasks.h"
#include "msTime.h"
#include "msXmtRcv.h"

#include "oldMSSupport.h"

//_________________________________________________________
// DECLARATION DES VARIABLES GLOBALES
TMSGlobal   	gMSMem = {0};
TMSGlobalPtr	gMem = &gMSMem;


/*--------------------------- Global MidiShare environment --------------------*/
MSFunctionType(void) MidiShareSpecialInit(unsigned long defaultSpace) {
  MSSpecialInit (defaultSpace, gMem);
}

MSFunctionType(short) MGetIndAppl(short index, TMSGlobalPtr g) {
  return MSGetIndAppl (index, Clients(g));
}

MSFunctionType(short) MGetNamedAppl(MidiName name, TMSGlobalPtr g) {
  return MSGetNamedAppl (name, Clients(g));
}


/*--------------------------- Application configuration -----------------------*/

MSFunctionType(short) MCountAppls (TMSGlobalPtr g) {
  return MSCountAppls (Clients(g));
}

MSFunctionType(MidiName) MGetName (short refNum, TMSGlobalPtr g) {
  return MSGetName (refNum, Clients(g));
}

MSFunctionType(void) MSetName (short refNum, MidiName applName, TMSGlobalPtr g) {
  MSSetName (refNum,applName, Clients(g));
}

MSFunctionType(FarPtr(void)) MGetInfo (short refNum, TMSGlobalPtr g) {
  return MSGetInfo (refNum, Clients(g));
}

MSFunctionType(void) MSetInfo (short refNum, FarPtr(void) InfoZone, TMSGlobalPtr g) {
  MSSetInfo (refNum,InfoZone, Clients(g));
}

MSFunctionType(MidiFilterPtr) MGetFilter (short refNum, TMSGlobalPtr g) {
  return MSGetFilter (refNum, Clients(g));
}

MSFunctionType(void) MSetFilter (short refNum,MidiFilterPtr filter, TMSGlobalPtr g) {
  MSSetFilter (refNum,filter, Clients(g));
}

MSFunctionType(RcvAlarmPtr) MGetRcvAlarm (short refNum, TMSGlobalPtr g) {
  return MSGetRcvAlarm (refNum, Clients(g));
}

MSFunctionType(void) MSetRcvAlarm (short refNum, RcvAlarmPtr alarm, TMSGlobalPtr g) {
  MSSetRcvAlarm (refNum,alarm, Clients(g));
}

MSFunctionType(ApplAlarmPtr) MGetApplAlarm (short refNum, TMSGlobalPtr g) {
  return MSGetApplAlarm (refNum, Clients(g));
}

MSFunctionType(void) MSetApplAlarm (short refNum, ApplAlarmPtr alarm, TMSGlobalPtr g) {
  MSSetApplAlarm (refNum,alarm, Clients(g));
}


/*------------------------- Inter-Application Connections ---------------------*/

MSFunctionType(void) MConnect(short src, short dest , Boolean state, TMSGlobalPtr g) {
  MSConnect (src,dest,state, Clients(g));
}

MSFunctionType(Boolean) MIsConnected(short src, short dest, TMSGlobalPtr g) {
  return MSIsConnected (src,dest, Clients(g));
}


/*-------------------------- Events and memory managing -----------------------*/
MSFunctionType(unsigned long) MFreeSpace(TMSGlobalPtr g) {
  return MSFreeSpace (Memory(g));
}

MSFunctionType(unsigned long) MTotalSpace(TMSGlobalPtr g) {
  return MSTotalSpace (Memory(g));
}

MSFunctionType(unsigned long) MDesiredSpace(TMSGlobalPtr g) {
  return MSDesiredSpace (Memory(g));
}

MSFunctionType(MidiEvPtr) MNewCell(TMSGlobalPtr g) {
  return MSNewCell (Memory(g));
}

MSFunctionType(void) MFreeCell(MidiEvPtr e, TMSGlobalPtr g) {
  MSFreeCell (e, Memory(g));
}

MSFunctionType(unsigned long) MGrowSpace(long n, TMSGlobalPtr g) {
  return MSGrowSpace (n, Memory(g));
}

MSFunctionType(MidiEvPtr) MNewEv(short typeNum, TMSGlobalPtr g) {
  return MSNewEv (typeNum, Memory(g));
}

MSFunctionType(MidiEvPtr) MCopyEv(MidiEvPtr e, TMSGlobalPtr g) {
  return MSCopyEv (e, Memory(g));
}

MSFunctionType(void) MFreeEv(MidiEvPtr e, TMSGlobalPtr g) {
  MSFreeEv (e, Memory(g));
}

MSFunctionType(void) MSetField(MidiEvPtr e, long f, long v, TMSGlobalPtr g) {
  MSSetField (e,f,v, Memory(g));
}

MSFunctionType(long) MGetField(MidiEvPtr e, long f, TMSGlobalPtr g) {
  return MSGetField (e,f, Memory(g));
}

MSFunctionType(void) MAddField(MidiEvPtr e, long v, TMSGlobalPtr g) {
  MSAddField (e,v, Memory(g));
}

MSFunctionType(long) MCountFields(MidiEvPtr e, TMSGlobalPtr g) {
  return MSCountFields (e, Memory(g));
}

/*------------------------- compatibility support ----------------------------*/
#ifdef __Macintosh__
MSFunctionType(void) OldMSSetField (MidiEvPtr e, short f, long v, TMSGlobalPtr g) {
  MSSetField (e, (long)f, v, Memory(g));
}

MSFunctionType(long) OldMSGetField (MidiEvPtr e, short f, TMSGlobalPtr g) {
  return MSGetField (e, (long)f, Memory(g));
}

MSFunctionType(short) OldMSCountFields( MidiEvPtr e, TMSGlobalPtr g) {
  return (short)MSCountFields (e, Memory(g));
}

MSFunctionType(Boolean) MSGetPortState(short port, TMSGlobalPtr g) {
  return false;
}

MSFunctionType(void) MSSetPortState(short port, Boolean state, TMSGlobalPtr g) {
}


#endif

/*------------------------------- Sequence managing ---------------------------*/
MSFunctionType(MidiSeqPtr) MNewSeq(TMSGlobalPtr g) {
  return MSNewSeq (Memory(g));
}

MSFunctionType(void) MAddSeq(MidiSeqPtr s, MidiEvPtr e, TMSGlobalPtr g) {
  MSAddSeq (s, e);
}

MSFunctionType(void) MFreeSeq(MidiSeqPtr s, TMSGlobalPtr g) {
  MSFreeSeq (s, Memory(g));
}

MSFunctionType(void) MClearSeq(MidiSeqPtr s, TMSGlobalPtr g) {
  MSClearSeq (s, Memory(g));
}

MSFunctionType(void) MApplySeq(MidiSeqPtr s, ApplyProcPtr proc, TMSGlobalPtr g) {
  MSApplySeq (s, proc, Memory(g));
}


/*------------------------------------ Sending --------------------------------*/
MSFunctionType(void) MSendIm(short refNum, MidiEvPtr e, TMSGlobalPtr g) {
  MSSendIm (refNum,e, Sorter(g));
}

MSFunctionType(void) MSend(short refNum, MidiEvPtr e, TMSGlobalPtr g) {
  MSSend (refNum,e, Sorter(g));
}

MSFunctionType(void) MSendAt(short refNum, MidiEvPtr e, unsigned long d, TMSGlobalPtr g) {
  MSSendAt (refNum,e,d, Sorter(g));
}


/*----------------------------------- Receiving -------------------------------*/
MSFunctionType(unsigned long) MCountEvs(short refNum, TMSGlobalPtr g) {
  return MSCountEvs (refNum, Clients(g));
}

MSFunctionType(MidiEvPtr) MGetEv(short refNum, TMSGlobalPtr g) {
  return MSGetEv (refNum, Clients(g));
}

MSFunctionType(MidiEvPtr) MAvailEv(short refNum, TMSGlobalPtr g) {
  return MSAvailEv (refNum, Clients(g));
}

MSFunctionType(void) MFlushEvs(short refNum, TMSGlobalPtr g) {
  MSFlushEvs (refNum, Clients(g));
}


/*---------------------------------- Task Managing ----------------------------*/

MSFunctionType(void) MCall(TaskPtr routine, unsigned long date, short refNum, long a1,long a2,long a3, TMSGlobalPtr g) {
  MSCall (routine,date,refNum,a1,a2,a3, Memory(g), Sorter(g));
}

MSFunctionType(MidiEvPtr) MTask(TaskPtr routine, unsigned long date, short refNum, long a1,long a2,long a3, TMSGlobalPtr g) {
  return MSTask (routine,date,refNum,a1,a2,a3, Memory(g), Sorter(g));
}

MSFunctionType(MidiEvPtr) MDTask(TaskPtr routine, unsigned long date, short refNum, long a1,long a2,long a3, TMSGlobalPtr g) {
  return MSDTask (routine,date,refNum,a1,a2,a3, Memory(g), Sorter(g));
}

MSFunctionType(void) MForgetTask(MidiEvPtr FAR *e, TMSGlobalPtr g) {
  MSForgetTask (e);
}

MSFunctionType(long) MCountDTasks(short refNum, TMSGlobalPtr g) {
  return MSCountDTasks (refNum, Clients(g));
}

MSFunctionType(void) MFlushDTasks(short refNum, TMSGlobalPtr g) {
  MSFlushDTasks (refNum, Clients(g));
}

MSFunctionType(void) MExec1DTask(short refNum, TMSGlobalPtr g) {
  MSExec1DTask (refNum, Clients(g), CurrTime(g) );
} 
