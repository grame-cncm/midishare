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

#include "msXmtRcv.h"
#include "msEvents.h"
#include "msSync.h"


/*===========================================================================
  MidiShare external functions implementation
=========================================================================== */
MSFunctionType(void) MSSendIm (short refNum, MidiEvPtr e, TsorterBlockPtr sb)
{
	if( e) {
		RefNum(e) = (uchar)refNum;
		Date(e)   = SorterDate(sb);
		SorterPut (sb, (TDatedEvPtr)e);
	}
}

/*__________________________________________________________________________*/
MSFunctionType(void) MSSend (short refNum, MidiEvPtr e, TsorterBlockPtr sb)
{
	if( e) {
		RefNum(e)= (uchar)refNum;
		SorterPut (sb, (TDatedEvPtr)e);
	}
}

/*__________________________________________________________________________*/
MSFunctionType(void) MSSendAt (short refNum, MidiEvPtr e, unsigned long d, TsorterBlockPtr sb)
{
	if( e) {
		RefNum(e)= (uchar)refNum;
		Date(e)= d;
		SorterPut (sb, (TDatedEvPtr)e);
	}
}

/*__________________________________________________________________________*/
MSFunctionType(ulong) MSCountEvs (short refNum, TClientsPtr g)
{
	TApplPtr appl;
	if( CheckRefNum( g, refNum)) {
		appl = g->appls[refNum]; 
		return RcvFifoCount(appl);
	}
	return 0;
}

/*__________________________________________________________________________*/
MSFunctionType(MidiEvPtr) MSGetEv (short refNum, TClientsPtr g)
{
	TApplPtr appl; ulong n;
	MidiEvPtr ev= 0;

	if( CheckRefNum( g, refNum)) {
		appl = g->appls[refNum]; 
		n = RcvFifoCount(appl);
		if( n ) {
			ev = PopFifoEv (&appl->rcv);
		}
	}
	return ev; 
}

/*__________________________________________________________________________*/
MSFunctionType(MidiEvPtr) MSAvailEv (short refNum, TClientsPtr g)
{
	TApplPtr appl;

	if( CheckRefNum( g, refNum)) {
		appl = g->appls[refNum]; 
		return RcvFifoHead(appl);
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
		if (RcvFifoCount(appl)) {
			ev = ClearFifo (&appl->rcv);
			while( ev) {
				next= Link(ev);
				MSFreeEv(ev, g->memory);
				ev= next;
			}
		}
	}
}
