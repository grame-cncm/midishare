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

#ifndef __MSLoader__
#define __MSLoader__


/* #define kMidiShareName "MidiShareDev"  defini dans msAppls.h */

#include "msKernel.h"
#include "msIOCtl.h"

/* functions prototypes */

int mskGetVersion(unsigned long userptr);
int mskCountAppls(unsigned long userptr);
int mskGetIndAppl(unsigned long userptr);
int mskGetNamedAppl(unsigned long userptr);
int mskGetSyncInfo (unsigned long userptr);
int mskSetSyncMode (unsigned long userptr);
int mskGetExtTime (unsigned long userptr);
int mskInt2ExtTime (unsigned long userptr);
int mskExt2IntTime (unsigned long userptr);
int mskTime2Smpte (unsigned long userptr);
int mskSmpte2Time (unsigned long userptr);
int mskGetTimeAddr  (unsigned long userptr);

int mskOpen(unsigned long userptr);
int mskClose(unsigned long userptr);

int mskGetName   (unsigned long userptr);
int mskSetName  (unsigned long userptr);

int mskGetInfo  (unsigned long userptr);
int mskSetInfo  (unsigned long userptr);

int mskGetFilter  (unsigned long userptr);
int mskSetFilter  (unsigned long userptr);

int mskSetRcvAlarm   (unsigned long userptr);
int mskSetApplAlarm   (unsigned long userptr);

int mskConnect   (unsigned long userptr);
int mskIsConnected   (unsigned long userptr);

int mskGetPortState  (unsigned long userptr);
int mskSetPortState (unsigned long userptr);

int mskGetTime(unsigned long userptr);


int mskSendIm(unsigned long userptr);
int mskSend(unsigned long userptr);
int mskSendAt(unsigned long userptr);

int mskCountEvs(unsigned long userptr);
int mskGetEv(unsigned long userptr);
int mskAvailEv (unsigned long userptr);
int mskFlushEvs(unsigned long userptr);

int mskReadSync(unsigned long userptr);
int mskWriteSync(unsigned long userptr);

int mskCall(unsigned long userptr);
int mskTask(unsigned long userptr);
int mskDTask(unsigned long userptr);
int mskForgetTask (unsigned long userptr);
int mskCountDTasks(unsigned long userptr);
int mskFlushDTasks(unsigned long userptr);

int mskGetDTask(unsigned long userptr);
int mskGetCommand(unsigned long userptr);

int mskNewFilter (unsigned long userptr);
int mskFreeFilter (unsigned long userptr);

int mskAcceptChan (unsigned long userptr);
int mskAcceptPort (unsigned long userptr);
int mskAcceptType (unsigned long userptr);

int mskIsAcceptedChan (unsigned long userptr);
int mskIsAcceptedPort (unsigned long userptr);
int mskIsAcceptedType (unsigned long userptr);

int mskQuitAction (unsigned long userptr);


extern TMSGlobalPtr	gMem;


#endif
