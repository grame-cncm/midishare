/*

  Copyright  Grame 1999

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

struct file;

int mskGetVersion(unsigned long userptr,struct file *);
int mskCountAppls(unsigned long userptr,struct file *);
int mskGetIndAppl(unsigned long userptr,struct file *);
int mskGetNamedAppl(unsigned long userptr,struct file *);
int mskGetSyncInfo (unsigned long userptr,struct file *);
int mskSetSyncMode (unsigned long userptr,struct file *);
int mskGetExtTime (unsigned long userptr,struct file *);
int mskInt2ExtTime (unsigned long userptr,struct file *);
int mskExt2IntTime (unsigned long userptr,struct file *);
int mskTime2Smpte (unsigned long userptr,struct file *);
int mskSmpte2Time (unsigned long userptr,struct file *);
int mskGetTimeAddr  (unsigned long userptr,struct file *);

int mskOpen(unsigned long userptr,struct file *);
int mskClose(unsigned long userptr,struct file *);

int mskGetName   (unsigned long userptr,struct file *);
int mskSetName  (unsigned long userptr,struct file *);

int mskGetInfo  (unsigned long userptr,struct file *);
int mskSetInfo  (unsigned long userptr,struct file *);

int mskGetFilter  (unsigned long userptr,struct file *);
int mskSetFilter  (unsigned long userptr,struct file *);

int mskSetRcvAlarm   (unsigned long userptr,struct file *);
int mskSetApplAlarm   (unsigned long userptr,struct file *);

int mskConnect   (unsigned long userptr,struct file *);
int mskIsConnected   (unsigned long userptr,struct file *);

int mskGetPortState  (unsigned long userptr,struct file *);
int mskSetPortState (unsigned long userptr,struct file *);

int mskGetTime(unsigned long userptr,struct file *);

int mskSendIm(unsigned long userptr,struct file *);
int mskSend(unsigned long userptr,struct file *);
int mskSendAt(unsigned long userptr,struct file *);

int mskCountEvs(unsigned long userptr,struct file *);
int mskGetEv(unsigned long userptr,struct file *);
int mskAvailEv (unsigned long userptr,struct file *);
int mskFlushEvs(unsigned long userptr,struct file *);

int mskReadSync(unsigned long userptr,struct file *);
int mskWriteSync(unsigned long userptr,struct file *);

int mskCall(unsigned long userptr,struct file *);
int mskTask(unsigned long userptr,struct file *);
int mskDTask(unsigned long userptr,struct file *);
int mskForgetTask (unsigned long userptr,struct file *);
int mskCountDTasks(unsigned long userptr,struct file *);
int mskFlushDTasks(unsigned long userptr,struct file *);

int mskGetDTask(unsigned long userptr,struct file *);
int mskGetCommand(unsigned long userptr,struct file *);

int mskNewFilter (unsigned long userptr,struct file *);
int mskFreeFilter (unsigned long userptr,struct file *);

int mskAcceptChan (unsigned long userptr,struct file *);
int mskAcceptPort (unsigned long userptr,struct file *);
int mskAcceptType (unsigned long userptr,struct file *);

int mskIsAcceptedChan (unsigned long userptr,struct file *);
int mskIsAcceptedPort (unsigned long userptr,struct file *);
int mskIsAcceptedType (unsigned long userptr,struct file *);

int mskQuitAction (unsigned long userptr,struct file *);

/* release 1.80 */
int mskRegisterDriver (unsigned long userptr,struct file *);
int mskUnregisterDriver (unsigned long userptr,struct file *);
int mskCountDrivers (unsigned long userptr,struct file *);
int mskGetIndDriver (unsigned long userptr,struct file *);
int mskGetDriverInfos (unsigned long userptr,struct file *);
int mskAddSlot (unsigned long userptr,struct file *);
int mskGetIndSlot (unsigned long userptr,struct file *);
int mskRemoveSlot  (unsigned long userptr,struct file *);
int mskGetSlotInfos (unsigned long userptr,struct file *);
int mskConnectSlot (unsigned long userptr,struct file *);
int mskIsSlotConnected (unsigned long userptr,struct file *);

int mskGetError(unsigned long userptr,struct file *);

void mskCloseAll (struct file* f);


extern TMSGlobalPtr	gMem;


#endif
