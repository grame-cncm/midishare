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
   [08-09-99] DF - removing the sorter dependencies

*/

#include "msXmtRcv.h"
#include "msEvents.h"
#include "msExtern.h"

/*__________________________________________________________________________*/
#ifdef MSKernel
MSFunctionType(void) MSSend (short refNum, MidiEvPtr e, TMSGlobalPtr g)
{
	if( e) {
		RefNum(e)= (uchar)refNum;
		fifoput (SorterList(g), (cell*)e);
	}
}

#else

MSFunctionType(void) MSSend (short refNum, MidiEvPtr e, TMSGlobalPtr g)
{
	if( e) {
		RefNum(e)= (uchar)refNum;
		SendToServer (e, g);
	}
}

MSFunctionType(MidiEvPtr) MSSendSync (short refNum, MidiEvPtr e, TMSGlobalPtr g)
{
	if( e) {
		RefNum(e)= (uchar)refNum;
		return SendToServerSync (e, g);
	}
	return 0;
}

#endif

/*===========================================================================
  MidiShare external functions implementation
=========================================================================== */
#ifdef MSKernel
#	define GetAppl(g, ref)	g->clients.appls[ref]
#else
#	define GetAppl(g, ref)	g->appls[ref]
#endif

MSFunctionType(void) MSSendIm (short refNum, MidiEvPtr e, TMSGlobalPtr g)
{
	if( e) {
		Date(e)   = g->pub->time;
		MSSend (refNum, e, g);
	}
}

/*__________________________________________________________________________*/
MSFunctionType(void) MSSendAt (short refNum, MidiEvPtr e, unsigned long d, 
								TMSGlobalPtr g)
{
	if( e) {
		Date(e)= d;
		MSSend (refNum, e, g);
	}
}

/*__________________________________________________________________________*/
MSFunctionType(unsigned long) MSCountEvs (short refNum, TMSGlobalPtr g)
{
	if (CheckGlobRefNum( g, refNum)) {
		TApplPtr appl = GetAppl(g,refNum); 
		return fifosize (&appl->rcv);
	}
	return 0;
}

/*__________________________________________________________________________*/
MSFunctionType(MidiEvPtr) MSGetEv (short refNum, TMSGlobalPtr g)
{
	if (CheckGlobRefNum( g, refNum)) {
		TApplPtr appl = GetAppl(g,refNum); 
		return (MidiEvPtr)fifoget (&appl->rcv);
	}
	return 0; 
}

/*__________________________________________________________________________*/
MSFunctionType(MidiEvPtr) MSAvailEv (short refNum, TMSGlobalPtr g)
{
	if (CheckGlobRefNum( g, refNum)) {
		TApplPtr appl = GetAppl(g,refNum); 
		return (MidiEvPtr)fifoavail (&appl->rcv);
	}
	return 0;
}

/*__________________________________________________________________________*/
MSFunctionType(void) MSFlushEvs (short refNum, TMSGlobalPtr g)
{
	TApplPtr appl;
	MidiEvPtr ev, next;

	if (CheckGlobRefNum( g, refNum)) {
		appl = GetAppl(g,refNum); 
		ev = (MidiEvPtr)fifoclear (&appl->rcv);
		while( ev) {
			next= Link(ev);
			MSFreeEv (ev, &g->memory.freeList);
			ev= next;
		}
	}
}
