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
   [08-09-99] DF - adaptation to the new memory management
                   includes now ForgetTaskSync
   [13-09-99] DF - removing direct fifo access

*/

#include "msTasks.h"
#include "msEvents.h"
#include "msExtern.h"
#include "msSync.h"


static Boolean ForgetTaskSync (MidiEvPtr * taskPtr, MidiEvPtr content);


/*===========================================================================
  External MidiShare functions implementation
  =========================================================================== */
MSFunctionType(void) MSCall (TaskPtr task, unsigned long date, short r, 
                             long a1,long a2,long a3, 
                             lifo* freelist, fifo* schedlist)
{
	MidiEvPtr ev;
	MidiSTPtr ext;
	
	ev= MSNewEv (typeProcess, freelist);
	if( ev) {
		Date(ev)= date;
		RefNum(ev)= (uchar)r;
		ext= LinkST(ev);
		ext->val[0]= (long)task;
		ext->val[1]= a1;
		ext->val[2]= a2;
		ext->val[3]= a3;
		fifoput (schedlist, (cell*)ev);
	}
}

/*__________________________________________________________________________________*/
MSFunctionType(MidiEvPtr) MSTask (TaskPtr task, unsigned long date, short r, 
                             long a1,long a2,long a3, 
                             lifo* freelist, fifo* schedlist)
{
	MidiEvPtr ev;
	MidiSTPtr ext;
	
	ev= MSNewEv( typeProcess, freelist);
	if( ev) {
		Date(ev)= date;
		RefNum(ev)= (uchar)r;
		ext= LinkST(ev);
		ext->val[0]= (long)task;
		ext->val[1]= a1;
		ext->val[2]= a2;
		ext->val[3]= a3;
		fifoput (schedlist, (cell*)ev);
	}
	return ev;
}

/*__________________________________________________________________________________*/
MSFunctionType(MidiEvPtr) MSDTask (TaskPtr task, unsigned long date, short r, 
                             long a1,long a2,long a3, 
                             lifo* freelist, fifo* schedlist)
{
	MidiEvPtr ev;
	MidiSTPtr ext;
	
	ev= MSNewEv( typeDProcess, freelist);
	if( ev) {
		Date(ev)= date;
		RefNum(ev)= (uchar)r;
		ext= LinkST(ev);
		ext->val[0]= (long)task;
		ext->val[1]= a1;
		ext->val[2]= a2;
		ext->val[3]= a3;
		fifoput (schedlist, (cell*)ev);
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
	return CheckRefNum( g, refnum) ? fifosize (&g->appls[refnum]->dTasks) : 0;
}

/*__________________________________________________________________________________*/
MSFunctionType(void) MSFlushDTasks (short refnum, TClientsPtr g)
{
	MidiEvPtr ev, next;
	if (CheckRefNum(g, refnum)) {
		ev = (MidiEvPtr)fifoclear (&g->appls[refnum]->dTasks);
		while (ev) {
			next= Link(ev);
			MSFreeEv (ev, FreeList(g->memory));
			ev = next;
		}
	}
}

/*__________________________________________________________________________________*/
MSFunctionType(void) MSExec1DTask (short refnum, TClientsPtr g, long currtime)
{
#ifndef __Linux__  /* never called under Linux */
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
#endif
}


/*===========================================================================
  Internal functions implementation
  =========================================================================== */

#ifdef __Macintosh__
# ifdef __POWERPC__
# else

static Boolean ForgetTaskSync (register MidiEvPtr * taskPtr, register MidiEvPtr content)
{
	Boolean ret = false;
	INT_OFF();
		if (*taskPtr == content) {
      		EvType(content) = typeDead;
    		*taskPtr = 0;
    		ret = true;
	}
	INT_ON();
	return ret;
}

# endif
#endif /* __Macintosh__ */

#ifdef __Linux__
static Boolean ForgetTaskSync (MidiEvPtr * taskPtr, MidiEvPtr content)
{
	return false;
}
#endif /* __Linux__ */

