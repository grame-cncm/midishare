/*

  Copyright © Grame 2000

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

#ifndef __exports__
#define __exports__

#include "msAppFun.h"
#include "msConnx.h"
#include "msDrvFun.h"
#include "msEvents.h"
#include "msFields.h"
#include "msFilter.h"
#include "msInit.h"
#include "msKernel.h"
#include "msMail.h"
#include "msMemory.h"
#include "msSeq.h"
#include "msSmpte.h"
#include "msTasks.h"
#include "msTime.h"
#include "msXmtRcv.h"

EXPORT_SYMBOL(gMem);

EXPORT_SYMBOL(MSGetVersion);
EXPORT_SYMBOL(MSCountAppls);
EXPORT_SYMBOL(MSGetIndAppl);
EXPORT_SYMBOL(MSGetNamedAppl);
EXPORT_SYMBOL(MSGetSyncInfo);
EXPORT_SYMBOL(MSSetSyncMode);
EXPORT_SYMBOL(MSGetExtTime);
EXPORT_SYMBOL(MSInt2ExtTime);
EXPORT_SYMBOL(MSExt2IntTime);
EXPORT_SYMBOL(MSTime2Smpte);
EXPORT_SYMBOL(MSSmpte2Time);
EXPORT_SYMBOL(MSOpen);
EXPORT_SYMBOL(MSClose);
EXPORT_SYMBOL(MSGetName);
EXPORT_SYMBOL(MSSetName);
EXPORT_SYMBOL(MSGetInfo);
EXPORT_SYMBOL(MSSetInfo);
EXPORT_SYMBOL(MSGetFilter);
EXPORT_SYMBOL(MSSetFilter);
EXPORT_SYMBOL(MSGetRcvAlarm);
EXPORT_SYMBOL(MSSetRcvAlarm);
EXPORT_SYMBOL(MSGetApplAlarm);
EXPORT_SYMBOL(MSSetApplAlarm);
EXPORT_SYMBOL(MSConnect);
EXPORT_SYMBOL(MSIsConnected);
EXPORT_SYMBOL(MSFreeSpace);
EXPORT_SYMBOL(MSTotalSpace);
EXPORT_SYMBOL(MSGrowSpace);
EXPORT_SYMBOL(MSNewCellFunction);
EXPORT_SYMBOL(MSFreeCellFunction);
EXPORT_SYMBOL(MSNewEv);
EXPORT_SYMBOL(MSCopyEv);
EXPORT_SYMBOL(MSFreeEv);
EXPORT_SYMBOL(MSSetField);
EXPORT_SYMBOL(MSGetField);
EXPORT_SYMBOL(MSCountFields);
EXPORT_SYMBOL(MSAddField);
EXPORT_SYMBOL(MSNewSeq);
EXPORT_SYMBOL(MSAddSeq);
EXPORT_SYMBOL(MSFreeSeq);
EXPORT_SYMBOL(MSClearSeq);
EXPORT_SYMBOL(MSApplySeq);
EXPORT_SYMBOL(MSGetTime);
EXPORT_SYMBOL(MSSendIm);
EXPORT_SYMBOL(MSSend);
EXPORT_SYMBOL(MSSendAt);
EXPORT_SYMBOL(MSCountEvs);
EXPORT_SYMBOL(MSGetEv);
EXPORT_SYMBOL(MSAvailEv);
EXPORT_SYMBOL(MSFlushEvs);
EXPORT_SYMBOL(MSReadSync);
EXPORT_SYMBOL(MSWriteSync);
EXPORT_SYMBOL(MSTask);
EXPORT_SYMBOL(MSDTask);
EXPORT_SYMBOL(MSForgetTask);
EXPORT_SYMBOL(MSCountDTasks);
EXPORT_SYMBOL(MSFlushDTasks);
EXPORT_SYMBOL(MSExec1DTask);
EXPORT_SYMBOL(MSNewFilter);
EXPORT_SYMBOL(MSFreeFilter);
EXPORT_SYMBOL(MSAcceptPort);
EXPORT_SYMBOL(MSAcceptChan);
EXPORT_SYMBOL(MSAcceptType);
EXPORT_SYMBOL(MSIsAcceptedPort);
EXPORT_SYMBOL(MSIsAcceptedChan);
EXPORT_SYMBOL(MSIsAcceptedType);

/* release 1.80 additionnal exports */
EXPORT_SYMBOL(MSRegisterDriver);
EXPORT_SYMBOL(MSUnregisterDriver);
EXPORT_SYMBOL(MSCountDrivers);
EXPORT_SYMBOL(MSGetIndDriver);
EXPORT_SYMBOL(MSGetDriverInfos);
EXPORT_SYMBOL(MSAddSlot);
EXPORT_SYMBOL(MSGetIndSlot);
EXPORT_SYMBOL(MSRemoveSlot);
EXPORT_SYMBOL(MSGetSlotInfos);
EXPORT_SYMBOL(MSConnectSlot);
EXPORT_SYMBOL(MSIsSlotConnected);

/* release 1.82 additionnal exports */
EXPORT_SYMBOL(MSSetSlotName);

#endif

