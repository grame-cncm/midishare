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

  modifications history:
   [08-09-99] DF - mail boxes functions finalization

*/

#include "msDispatch.h"
#include "msAppFun.h"
#include "msDrvFun.h"
#include "msFilter.h"
#include "msInit.h"
#include "msMail.h"
#include "msSmpte.h"
#include "msTime.h"

#include "oldMSSupport.h"

static pascal void doNothing (TMSGlobalPtr unused1);

void MSFunctionsTbl(); 

#ifdef __Control_Panel__
void __Startup__ ();
asm void __Startup__ ()
#else
void msEntryPoint ();
asm void msEntryPoint ()
#endif
{
	memPtr:	dc.l	0

	start:
		add.l	d0,d0
		add.l	d0,d0
		move.l	(sp),-(sp)
		move.l	memPtr,4(sp)
		lea		MSFunctionsTbl,a0
		jmp		(a0,d0.l)
}

static asm void MSFunctionsTbl() {
		jmp MSGetVersion		// 0x00
		jmp MCountAppls
		jmp MGetIndAppl
		jmp MGetNamedAppl
		jmp MSOpen			// 0x04
		jmp MSClose
		jmp MGetName
		jmp MSetName	
		jmp MGetInfo			// 0x08
		jmp MSetInfo
		jmp MGetFilter
		jmp MSetFilter
		jmp MGetRcvAlarm		// 0x0c
		jmp MSetRcvAlarm
		jmp MGetApplAlarm
		jmp MSetApplAlarm
		jmp MConnect			// 0x10
		jmp MIsConnected
		jmp MSGetPortState
		jmp MSSetPortState
		jmp MFreeSpace		// 0x14
		jmp MNewEv
		jmp MCopyEv
		jmp MFreeEv
		jmp OldMSSetField		// 0x18
		jmp OldMSGetField
		jmp MAddField
		jmp OldMSCountFields
		jmp doNothing			// 0x0c
		jmp MNewSeq
		jmp MAddSeq
		jmp MFreeSeq
		jmp MClearSeq			// 0x20
		jmp MApplySeq
		jmp MSGetTime
		jmp MSendIm
		jmp MSend			// 0x24
		jmp MSendAt
		jmp MCountEvs
		jmp MGetEv
		jmp MAvailEv			// 0x28
		jmp MFlushEvs
		jmp MReadSync
		jmp MWriteSync
		jmp MCall				// 0x2c
		jmp MTask
		jmp MDTask
		jmp MForgetTask
		jmp MCountDTasks		// 0x30
		jmp MFlushDTasks
		jmp MExec1DTask
		jmp MNewCell
		jmp MFreeCell			// 0x34
		jmp MTotalSpace
		jmp MGrowSpace
		jmp MSSpecialInit
		jmp MSGetSyncInfo		// 0x38
		jmp MSSetSyncMode
		jmp MSetField
		jmp MGetField
		jmp MCountFields		// 0x0c
		jmp MSGetExtTime
		jmp MSInt2ExtTime
		jmp MSExt2IntTime
		jmp MSTime2Smpte		// 0x40
		jmp MSSmpte2Time
		jmp MSGetTimeAddr
		jmp MDesiredSpace		// 0x43
		jmp MSRegisterDriver	// 0x44
		jmp MSUnregisterDriver
		jmp MCountDrivers		// 0x46
		jmp MGetIndDriver
		jmp MGetDriverInfos
		jmp OldAddSlot			// 0x49
		jmp MGetIndSlot			// 0x4A
		jmp MRemoveSlot
		jmp MGetSlotInfos
		jmp MConnectSlot		// 0x4D
		jmp MIsSlotConnected
		jmp MSetSlotName		// 0x4F
		jmp MAddSlot			// 0x50

		// filter functions
		jmp MNewFilter			// 0x51
		jmp MFreeFilter
		jmp MAcceptPort
		jmp MAcceptChan
		jmp MAcceptType			// 0x55
		jmp MIsAcceptedPort
		jmp MIsAcceptedChan
		jmp MIsAcceptedType
		
}

static pascal void doNothing (TMSGlobalPtr unused1) {}
