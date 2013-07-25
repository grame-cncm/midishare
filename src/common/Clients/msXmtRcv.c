/*
  MidiShare Project
  Copyright (C) Grame 1999

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

  modifications history:
   [08-09-99] DF - removing the sorter dependencies

*/

#include "msXmtRcv.h"
#include "msEvents.h"
#include "lffifo.h"


/*===========================================================================
  MidiShare external functions implementation
=========================================================================== */
MSFunctionType(void) MSSendIm (short refNum, MidiEvPtr e, fifo* schedlist, 
								unsigned long curdate)
{
	if( e) {
		RefNum(e) = (uchar)refNum;
		Date(e)   = curdate;
		fifoput (schedlist, (fifocell*)e);
	}
}

/*__________________________________________________________________________*/
MSFunctionType(void) MSSend (short refNum, MidiEvPtr e, fifo* schedlist)
{
	if( e) {
		RefNum(e)= (uchar)refNum;
		fifoput (schedlist, (fifocell*)e);
	}
}

/*__________________________________________________________________________*/
MSFunctionType(void) MSSendAt (short refNum, MidiEvPtr e, unsigned long d, 
								fifo* schedlist)
{
	if( e) {
		RefNum(e)= (uchar)refNum;
		Date(e)= d;
		fifoput (schedlist, (fifocell*)e);
	}
}

/*__________________________________________________________________________*/
MSFunctionType(atomic_long) MSCountEvs (short refNum, TClientsPtr g)
{
	if( CheckRefNum( g, refNum)) {
		TApplPtr appl = g->appls[refNum]; 
		return fifosize (&appl->rcv);
	}
	return 0;
}

/*__________________________________________________________________________*/
MSFunctionType(MidiEvPtr) MSGetEv (short refNum, TClientsPtr g)
{
	if( CheckRefNum( g, refNum)) {
		TApplPtr appl = g->appls[refNum]; 
		return (MidiEvPtr)fifoget (&appl->rcv);
	}
	return 0; 
}

/*__________________________________________________________________________*/
MSFunctionType(MidiEvPtr) MSAvailEv (short refNum, TClientsPtr g)
{
	if( CheckRefNum( g, refNum)) {
		TApplPtr appl = g->appls[refNum]; 
		return (MidiEvPtr)fifoavail (&appl->rcv);
	}
	return 0;
}

/*__________________________________________________________________________*/
MSFunctionType(void) MSFlushEvs (short refNum, TClientsPtr g)
{
	TApplPtr appl;
	MidiEvPtr ev, next;

	if( CheckRefNum( g, refNum)) {
		appl = g->appls[refNum]; 
		ev = (MidiEvPtr)fifoflush (&appl->rcv);
		while( ev) {
			next= Link(ev);
			MSFreeEv (ev, FreeList(g->memory));
			ev= next;
		}
	}
}
