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

#include "msTasks.h"
#include "msEvents.h"
#include "msSorter.h"
#include "msSync.h"

/*===========================================================================
  External MidiShare functions implementation
  =========================================================================== */
MSFunctionType(void) MSCall (TaskPtr task, unsigned long date, short r, 
                             long a1,long a2,long a3, 
                             MSMemoryPtr g, TsorterBlockPtr sb)
{
	MidiEvPtr ev;
	MidiSTPtr ext;
	
	ev= MSNewEv (typeProcess, g);
	if( ev) {
		Date(ev)= date;
		RefNum(ev)= (uchar)r;
		ext= LinkST(ev);
		ext->ptr1= (Ptr)task;
		ext->ptr2= (Ptr)a1;
		ext->ptr3= (Ptr)a2;
		ext->ptr4= (Ptr)a3;
		SorterPut (sb, (TDatedEvPtr)ev);
	}
}

/*__________________________________________________________________________________*/
MSFunctionType(MidiEvPtr) MSTask (TaskPtr task, unsigned long date, short r, 
                             long a1,long a2,long a3, 
                             MSMemoryPtr g, TsorterBlockPtr sb)
{
	MidiEvPtr ev;
	MidiSTPtr ext;
	
	ev= MSNewEv( typeProcess, g);
	if( ev) {
		Date(ev)= date;
		RefNum(ev)= (uchar)r;
		ext= LinkST(ev);
		ext->ptr1= (Ptr)task;
		ext->ptr2= (Ptr)a1;
		ext->ptr3= (Ptr)a2;
		ext->ptr4= (Ptr)a3;
		SorterPut (sb, (TDatedEvPtr)ev);
	}
	return ev;
}

/*__________________________________________________________________________________*/
MSFunctionType(MidiEvPtr) MSDTask (TaskPtr task, unsigned long date, short r, 
                             long a1,long a2,long a3, 
                             MSMemoryPtr g, TsorterBlockPtr sb)
{
	MidiEvPtr ev;
	MidiSTPtr ext;
	
	ev= MSNewEv( typeDProcess, g);
	if( ev) {
		Date(ev)= date;
		RefNum(ev)= (uchar)r;
		ext= LinkST(ev);
		ext->ptr1= (Ptr)task;
		ext->ptr2= (Ptr)a1;
		ext->ptr3= (Ptr)a2;
		ext->ptr4= (Ptr)a3;
		SorterPut (sb, (TDatedEvPtr)ev);
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
MSFunctionType(long) MSCountDTasks (short refnum, TClientsPtr g)
{
	return CheckRefNum( g, refnum) ? g->appls[refnum]->dTasks.count : 0;
}

/*__________________________________________________________________________________*/
MSFunctionType(void) MSFlushDTasks (short refnum, TClientsPtr g)
{
	MidiEvPtr ev, next;
	if (CheckRefNum(g, refnum)) {
		ev = ClearFifo (&g->appls[refnum]->dTasks);
		while (ev) {
			next= Link(ev);
			MSFreeEv (ev, g->memory);
			ev = next;
		}
	}
}

/*__________________________________________________________________________________*/
MSFunctionType(void) MSExec1DTask (short refnum, TClientsPtr g, long currtime)
{
	TApplPtr appl;
	MidiEvPtr ev;
	if( CheckRefNum( g, refnum)) {
		appl = g->appls[refnum];
		if (appl->dTasks.count) {
			ev = PopFifoEv (&appl->dTasks);
			if (ev) {
				TTaskExtPtr task= (TTaskExtPtr)LinkST(ev);
                /* you can assume that the calling process owns the deferred task ie : 
                   the task is located in its address space */
                CallDTaskCode (appl->context, task, Date(ev), appl->refNum);
				MSFreeEv (ev, g->memory);		
			}
		}
	}	
}
