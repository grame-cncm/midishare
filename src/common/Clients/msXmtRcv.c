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
   [08-09-99] DF - removing the sorter dependencies

*/

#include "msXmtRcv.h"
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
		fifoput (schedlist, (cell*)e);
	}
}

/*__________________________________________________________________________*/
MSFunctionType(void) MSSend (short refNum, MidiEvPtr e, fifo* schedlist)
{
	if( e) {
		RefNum(e)= (uchar)refNum;
		fifoput (schedlist, (cell*)e);
	}
}

/*__________________________________________________________________________*/
MSFunctionType(void) MSSendAt (short refNum, MidiEvPtr e, unsigned long d, 
								fifo* schedlist)
{
	if( e) {
		RefNum(e)= (uchar)refNum;
		Date(e)= d;
		fifoput (schedlist, (cell*)e);
	}
}

/*__________________________________________________________________________*/
MSFunctionType(ulong) MSCountEvs (short refNum, TClientsPtr g)
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
		ev = (MidiEvPtr)fifoclear (&appl->rcv);
		while( ev) {
			next= Link(ev);
			lfpush (FreeList(g->memory), (cell *)ev);
			ev= next;
		}
	}
}
