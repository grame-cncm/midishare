/*

  Copyright © Grame 1999-2002

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

  modifications history:
   [08-09-99] DF - adaptation to the new memory management
                   includes now ForgetTaskSync
   [13-09-99] DF - removing direct fifo access
   [16-01-01] SL - ForgetTaskSync moved to platform dependent code

*/

#include "msTasks.h"
#include "msEvents.h"
#include "msExtern.h"
#include "msSync.h"
#include "msXmtRcv.h"

#ifdef MSKernel
#	define GetAppl(g, ref)	g->clients.appls[ref]
#else
#	define GetAppl(g, ref)	g->appls[ref]
#endif

/*===========================================================================
  External MidiShare functions implementation
  =========================================================================== */
MSFunctionType(void) MSCall (TaskPtr task, unsigned long date, short r, 
                             long a1,long a2,long a3, TMSGlobalPtr g)
{
	MSTask (task, date, r, a1, a2, a3, g);
}

/*__________________________________________________________________________________*/
#ifndef MSKernel
static MidiEvPtr SendNetTask (MidiEvPtr task, short r, TMSGlobalPtr g)
{
	MidiEvPtr ev = MSNewEv (typeProcess, &g->memory.freeList);
	if( ev) {
		TTaskExtPtr ext = (TTaskExtPtr)LinkST(ev);
		ext->fun  = (TaskPtr)task;
		RefNum(ev) = RefNum(task) = (uchar)r;
		Date(ev) = Date(task);
		if (SendToServer (ev, g))
			return task;
		MSFreeEv (ev,  &g->memory.freeList);
	}
	MSFreeEv (task,  &g->memory.freeList);
	return 0;
}
#endif

/*__________________________________________________________________________________*/
MSFunctionType(MidiEvPtr) MSTask (TaskPtr task, unsigned long date, short r, 
                             long a1,long a2,long a3, TMSGlobalPtr g)
{
	MidiEvPtr ev = MSNewEv (typeProcess, &g->memory.freeList);
	if( ev) {
		TTaskExtPtr ext = (TTaskExtPtr)LinkST(ev);
		ext->fun  = task;
		ext->arg1 = a1;
		ext->arg2 = a2;
		ext->arg3 = a3;
		Date(ev)  = date;
#ifdef MSKernel
        MSSend (r, ev, g);
#else
		ev = SendNetTask(ev, r, g);
#endif
	}
	return ev;
}

/*__________________________________________________________________________________*/
MSFunctionType(MidiEvPtr) MSDTask (TaskPtr task, unsigned long date, short r, 
                             long a1,long a2,long a3, TMSGlobalPtr g)
{
	MidiEvPtr ev = MSNewEv (typeDProcess, &g->memory.freeList);
	if( ev) {
		TTaskExtPtr ext = (TTaskExtPtr)LinkST(ev);
		ext->fun  = task;
		ext->arg1 = a1;
		ext->arg2 = a2;
		ext->arg3 = a3;
		Date(ev)  = date;
#ifdef MSKernel
        MSSend (r, ev, g);
#else
		ev = SendNetTask(ev, r, g);
#endif
	}
	return ev;
}

/*__________________________________________________________________________________*/
MSFunctionType(void) MSForgetTask (MidiEvPtr *e)
{
	MidiEvPtr ev = e ? *e : 0;
	if (ev && ((EvType(ev) == typeProcess) || (EvType(ev) == typeDProcess))) {
      	ForgetTaskSync (e, ev);
	}
}

/*__________________________________________________________________________________*/
MSFunctionType(unsigned long) MSCountDTasks (short refnum, TMSGlobalPtr g)
{
	return CheckGlobRefNum(g, refnum) ? fifosize(&GetAppl(g,refnum)->dTasks) : 0;
}

/*__________________________________________________________________________________*/
MSFunctionType(void) MSFlushDTasks (short refnum, TMSGlobalPtr g)
{
	MidiEvPtr ev, next;
	if (CheckGlobRefNum(g, refnum)) {
		ev = (MidiEvPtr)fifoclear (&GetAppl(g,refnum)->dTasks);
		while (ev) {
			next= Link(ev);
			MSFreeEv (ev, &g->memory.freeList);
			ev = next;
		}
	}
}

/*__________________________________________________________________________________*/
MSFunctionType(void) MSExec1DTask (short refnum, TMSGlobalPtr g)
{
	if( CheckGlobRefNum( g, refnum)) {
		TApplPtr appl = GetAppl(g,refnum);
		if (fifosize (&appl->dTasks)) {
			MidiEvPtr ev = (MidiEvPtr)fifoget (&appl->dTasks);
			if (ev) {
                /* you can assume that the calling process owns the deferred task ie : 
                   the task is located in its address space */
#ifdef MSKernel
                CallDTaskCode (appl->context, ev);
#else
                CallDTaskCode (g->context, ev);
#endif
				MSFreeEv (ev, &g->memory.freeList);		
			}
		}
	}	
}
