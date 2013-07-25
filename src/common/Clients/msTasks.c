/*
  MidiShare Project
  Copyright (C) Grame 1999

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

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


/*===========================================================================
  External MidiShare functions implementation
  =========================================================================== */
MSFunctionType(void) MSCall (TaskPtr task, unsigned long date, short r, 
                             void* a1, void* a2, void* a3, 
                             lifo* freelist, fifo* schedlist)
{
	MidiEvPtr ev;
	MidiSTPtr ext;
	
	ev= MSNewEv (typeProcess, freelist);
	if( ev) {
		Date(ev)= date;
		RefNum(ev)= (uchar)r;
		ext= LinkST(ev);
		ext->val[0]= task;
		ext->val[1]= a1;
		ext->val[2]= a2;
		ext->val[3]= a3;
		fifoput (schedlist, (fifocell*)ev);
	}
}

/*__________________________________________________________________________________*/
MSFunctionType(MidiEvPtr) MSTask (TaskPtr task, unsigned long date, short r, 
                             void* a1, void* a2, void* a3, 
                             lifo* freelist, fifo* schedlist)
{
	MidiEvPtr ev;
	MidiSTPtr ext;
	
	ev= MSNewEv( typeProcess, freelist);
	if( ev) {
		Date(ev)= date;
		RefNum(ev)= (uchar)r;
		ext= LinkST(ev);
		ext->val[0]= task;
		ext->val[1]= a1;
		ext->val[2]= a2;
		ext->val[3]= a3;
		fifoput (schedlist, (fifocell*)ev);
	}
	return ev;
}

/*__________________________________________________________________________________*/
MSFunctionType(MidiEvPtr) MSDTask (TaskPtr task, unsigned long date, short r, 
                             void* a1, void* a2, void* a3, 
                             lifo* freelist, fifo* schedlist)
{
	MidiEvPtr ev;
	MidiSTPtr ext;
	
	ev= MSNewEv( typeDProcess, freelist);
	if( ev) {
		Date(ev)= date;
		RefNum(ev)= (uchar)r;
		ext= LinkST(ev);
		ext->val[0]= task;
		ext->val[1]= a1;
		ext->val[2]= a2;
		ext->val[3]= a3;
		fifoput (schedlist, (fifocell*)ev);
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
MSFunctionType(atomic_long) MSCountDTasks (short refnum, TClientsPtr g)
{
	return CheckRefNum( g, refnum) ? fifosize (&g->appls[refnum]->dTasks) : 0;
}

/*__________________________________________________________________________________*/
MSFunctionType(void) MSFlushDTasks (short refnum, TClientsPtr g)
{
	MidiEvPtr ev, next;
	if (CheckRefNum(g, refnum)) {
		ev = (MidiEvPtr)fifoflush (&g->appls[refnum]->dTasks);
		while (ev) {
			next= Link(ev);
			MSFreeEv (ev, FreeList(g->memory));
			ev = next;
		}
	}
}

/*__________________________________________________________________________________*/
MSFunctionType(void) MSExec1DTask (short refnum, TClientsPtr g)
{
	TApplPtr appl;
	MidiEvPtr ev;
	if( CheckRefNum( g, refnum)) {
		appl = g->appls[refnum];
		if (fifosize (&g->appls[refnum]->dTasks)) {
			ev = (MidiEvPtr)fifoget (&appl->dTasks);
			if (ev) {
                /* you can assume that the calling process owns the deferred task ie : 
                   the task is located in its address space */
                CallDTaskCode (appl->context, ev);
				MSFreeEv (ev, FreeList(g->memory));		
			}
		}
	}	
}

