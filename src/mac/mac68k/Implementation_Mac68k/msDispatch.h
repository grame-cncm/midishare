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

#ifndef __msDispatch__
#define __msDispatch__

#include "msKernel.h"

MSFunctionType(void) MidiShareSpecialInit(unsigned long defaultSpace);

MSFunctionType(short) MGetIndAppl(short index, TMSGlobalPtr g);
MSFunctionType(short) MGetNamedAppl(MidiName name, TMSGlobalPtr g);

/*--------------------------- Application configuration -----------------------*/
MSFunctionType(short) 			MCountAppls (TMSGlobalPtr g);
MSFunctionType(MidiName) 		MGetName (short refNum, TMSGlobalPtr g);
MSFunctionType(void) 			MSetName (short refNum, MidiName applName, TMSGlobalPtr g);
MSFunctionType(FarPtr(void)) 	MGetInfo (short refNum, TMSGlobalPtr g);
MSFunctionType(void) 			MSetInfo (short refNum, FarPtr(void) InfoZone, TMSGlobalPtr g);
MSFunctionType(MidiFilterPtr) 	MGetFilter (short refNum, TMSGlobalPtr g);
MSFunctionType(void) 			MSetFilter (short refNum,MidiFilterPtr filter, TMSGlobalPtr g);
MSFunctionType(RcvAlarmPtr) 	MGetRcvAlarm (short refNum, TMSGlobalPtr g);
MSFunctionType(void) 			MSetRcvAlarm (short refNum, RcvAlarmPtr alarm, TMSGlobalPtr g);
MSFunctionType(ApplAlarmPtr) 	MGetApplAlarm (short refNum, TMSGlobalPtr g);
MSFunctionType(void) 			MSetApplAlarm (short refNum, ApplAlarmPtr alarm, TMSGlobalPtr g);

/*------------------------- Inter-Application Connections ---------------------*/
MSFunctionType(void) 		MConnect(short src, short dest , Boolean state, TMSGlobalPtr g);
MSFunctionType(Boolean) 	MIsConnected(short src, short dest, TMSGlobalPtr g);

/*-------------------------- Events and memory managing -----------------------*/
MSFunctionType(unsigned long) 	MFreeSpace(TMSGlobalPtr g);
MSFunctionType(MidiEvPtr) 		MNewCell(TMSGlobalPtr g);
MSFunctionType(void) 			MFreeCell(MidiEvPtr e, TMSGlobalPtr g);
MSFunctionType(unsigned long) 	MTotalSpace(TMSGlobalPtr g);
MSFunctionType(unsigned long) 	MDesiredSpace(TMSGlobalPtr g);
MSFunctionType(unsigned long) 	MGrowSpace(long n, TMSGlobalPtr g);
MSFunctionType(MidiEvPtr) 		MNewEv(short typeNum, TMSGlobalPtr g);
MSFunctionType(MidiEvPtr) 		MCopyEv(MidiEvPtr e, TMSGlobalPtr g);
MSFunctionType(void) 	MFreeEv(MidiEvPtr e, TMSGlobalPtr g);
MSFunctionType(void) 	MSetField(MidiEvPtr e, long f, long v, TMSGlobalPtr g);
MSFunctionType(long) 	MGetField(MidiEvPtr e, long f, TMSGlobalPtr g);
MSFunctionType(void) 	MAddField(MidiEvPtr e, long v, TMSGlobalPtr g);
MSFunctionType(long) 	MCountFields(MidiEvPtr e, TMSGlobalPtr g);

/*------------------------------- Sequence managing ---------------------------*/
MSFunctionType(MidiSeqPtr) 	MNewSeq(TMSGlobalPtr g);
MSFunctionType(void) 		MAddSeq(MidiSeqPtr s, MidiEvPtr e, TMSGlobalPtr g);
MSFunctionType(void) 		MFreeSeq(MidiSeqPtr s, TMSGlobalPtr g);
MSFunctionType(void) 		MClearSeq(MidiSeqPtr s, TMSGlobalPtr g);
MSFunctionType(void) 		MApplySeq(MidiSeqPtr s, ApplyProcPtr proc, TMSGlobalPtr g);

/*------------------------------------ Sending --------------------------------*/
MSFunctionType(void) 	MSendIm(short refNum, MidiEvPtr e, TMSGlobalPtr g);
MSFunctionType(void) 	MSend(short refNum, MidiEvPtr e, TMSGlobalPtr g);
MSFunctionType(void) 	MSendAt(short refNum, MidiEvPtr e, unsigned long d, TMSGlobalPtr g);

/*----------------------------------- Receiving -------------------------------*/
MSFunctionType(unsigned long) 	MCountEvs(short refNum, TMSGlobalPtr g);
MSFunctionType(MidiEvPtr) 		MGetEv(short refNum, TMSGlobalPtr g);
MSFunctionType(MidiEvPtr) 		MAvailEv(short refNum, TMSGlobalPtr g);
MSFunctionType(void)			MFlushEvs(short refNum, TMSGlobalPtr g);

/*---------------------------------- Task Managing ----------------------------*/
MSFunctionType(void)      	MCall(TaskPtr routine, unsigned long date, short refNum, long a1,long a2,long a3, TMSGlobalPtr g);
MSFunctionType(MidiEvPtr) 	MTask(TaskPtr routine, unsigned long date, short refNum, long a1,long a2,long a3, TMSGlobalPtr g);
MSFunctionType(MidiEvPtr) 	MDTask(TaskPtr routine, unsigned long date, short refNum, long a1,long a2,long a3, TMSGlobalPtr g);
MSFunctionType(void) 		MForgetTask(MidiEvPtr FAR *e, TMSGlobalPtr g);
MSFunctionType(long) 		MCountDTasks(short refNum, TMSGlobalPtr g);
MSFunctionType(void) 		MFlushDTasks(short refNum, TMSGlobalPtr g);
MSFunctionType(void) 		MExec1DTask(short refNum, TMSGlobalPtr g);

#endif