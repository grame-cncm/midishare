/*******************************************************************************
 * C H A M E L E O N    S. D. K.                                               *
 *******************************************************************************
 *  $Archive:: /Chameleon.sdk/system/midishare/common/Clients/msXmtRcv.c       $
 *     $Date: 2005/12/08 13:38:28 $
 * $Revision: 1.4.6.1 $
 *-----------------------------------------------------------------------------*
 * This file is part of the Chameleon Software Development Kit                 *
 *                                                                             *
 * Copyright (C) 2001 soundart                                                 *
 * www.soundart-hot.com                                                        *
 * codemaster@soundart-hot.com                                                 *
 ******************************************************************************/

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
void MSSendIm(short refNum, MidiEvPtr e, fifo* schedlist, 
								DWORD curdate)
{
	if( e) {
		RefNum(e) = (BYTE)refNum;
		Date(e)   = curdate;
		fifoput (schedlist, (cell*)e);
	}
}

/*__________________________________________________________________________*/
void MSSend(short refNum, MidiEvPtr e, fifo* schedlist)
{
	if( e) {
		RefNum(e)= (BYTE)refNum;
		fifoput (schedlist, (cell*)e);
	}
}

/*__________________________________________________________________________*/
void MSSendAt(short refNum, MidiEvPtr e, DWORD d, 
								fifo* schedlist)
{
	if( e) {
		RefNum(e)= (BYTE)refNum;
		Date(e)= d;
		fifoput (schedlist, (cell*)e);
	}
}

/*__________________________________________________________________________*/
DWORD MSCountEvs(short refNum, TClientsPtr g)
{
	if( CheckRefNum( g, refNum)) {
		TApplPtr appl = g->appls[refNum]; 
		return fifosize (&appl->rcv);
	}
	return 0;
}

/*__________________________________________________________________________*/
MidiEvPtr MSGetEv(short refNum, TClientsPtr g)
{
	if( CheckRefNum( g, refNum)) {
		TApplPtr appl = g->appls[refNum]; 
		return (MidiEvPtr)fifoget (&appl->rcv);
	}
	return NULL; 
}

/*__________________________________________________________________________*/
MidiEvPtr MSAvailEv(short refNum, TClientsPtr g)
{
	if( CheckRefNum( g, refNum)) {
		TApplPtr appl = g->appls[refNum]; 
		return (MidiEvPtr)fifoavail (&appl->rcv);
	}
	return NULL;
}

/*__________________________________________________________________________*/
void MSFlushEvs(short refNum, TClientsPtr g)
{
	TApplPtr appl;
	MidiEvPtr ev, next;

	if( CheckRefNum( g, refNum)) {
		appl = g->appls[refNum]; 
		ev = (MidiEvPtr)fifoclear (&appl->rcv);
		while( ev) {
			next= Link(ev);
			MSFreeEv (ev, FreeList(g->memory));
			ev= next;
		}
	}
}
