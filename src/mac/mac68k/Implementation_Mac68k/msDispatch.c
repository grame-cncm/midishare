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

#include "msDispatch.h"
#include "msAppFun.h"
#include "msInit.h"
#include "msMail.h"
#include "msSmpte.h"
#include "msTime.h"

#include "oldMSSupport.h"

static pascal void doNothing( TMSGlobalPtr unused1);

void * _MSFunctionsTbl[];
void msEntryPoint ();
void MSFunctionsTbl ();

asm void msEntryPoint ()
{
	memPtr:	dc.l	0

	start:
		add.l	d0,d0
		add.l	d0,d0
		move.l	(sp),-(sp)
		move.l	memPtr,4(sp)
		lea		MSFunctionsTbl,a0
		move.l	(a0,d0.l), a0
		jmp		(a0)
	end:
}

asm void MSFunctionsTbl() {
		dc.l MSGetVersion, 	MCountAppls, 	MGetIndAppl, 	MGetNamedAppl		/* 0x00 */
		dc.l MSOpen,		MSClose,		MGetName,		MSetName			/* 0x04 */
		dc.l MGetInfo,		MSetInfo,		MGetFilter,		MSetFilter			/* 0x08 */
		dc.l MGetRcvAlarm,	MSetRcvAlarm,	MGetApplAlarm,	MSetApplAlarm		/* 0x0c */
		dc.l MConnect,		MIsConnected,	MSGetPortState,	MSSetPortState		/* 0x10 */
		dc.l MFreeSpace,	MNewEv,			MCopyEv,		MFreeEv				/* 0x14 */
		dc.l OldMSSetField,	OldMSGetField,	MAddField,		OldMSCountFields	/* 0x18 */
		dc.l doNothing,		MNewSeq,		MAddSeq,		MFreeSeq			/* 0x1c */
		dc.l MClearSeq,		MApplySeq,		MSGetTime,		MSendIm				/* 0x20 */
		dc.l MSend,			MSendAt,		MCountEvs,		MGetEv				/* 0x24 */
		dc.l MAvailEv,		MFlushEvs,		MSReadSync,		MSWriteSync			/* 0x28 */
		dc.l MCall,			MTask,			MDTask,			MForgetTask			/* 0x2c */
		dc.l MCountDTasks,	MFlushDTasks,	MExec1DTask,	MNewCell			/* 0x30 */
		dc.l MFreeCell,		MTotalSpace,	MGrowSpace,		MSSpecialInit		/* 0x34 */
		dc.l MSGetSyncInfo,	MSSetSyncMode,	MSetField,		MGetField			/* 0x38 */
		dc.l MCountFields,	MSGetExtTime,	MSInt2ExtTime,	MSExt2IntTime		/* 0x38 */
		dc.l MSTime2Smpte,	MSSmpte2Time,	MSGetTimeAddr,	MDesiredSpace		/* 0x3c */
}

static pascal void doNothing (TMSGlobalPtr unused1) {}
