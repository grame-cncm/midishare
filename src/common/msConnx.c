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
   [08-09-99] DF - new memory management
   [06-08-02] DF - cnx management revised: now based on bit fields instead of
                   dynamic allocation of midishare cells

*/

#include "msAlarms.h"
#include "msConnx.h"
#include "msDefs.h"

/*===========================================================================
  MidiShare external functions implementation
=========================================================================== */
MSFunctionType(Boolean) MSIsConnected (short src, short dest, TClientsPtr g)
{
	if( CheckRefNum(g,src) && CheckRefNum(g,dest) ) {
		TApplPtr appl = GetApplPtr(g, src);
		TConnectionsPtr srcCnx = &pub(appl, cnx);		
		return IsAcceptedBit(srcCnx->dst, dest);
	}
	return false;
}

#ifdef MSKernel

#define byteNum(ref)   (ref/(MaxAppls/8))

/*===========================================================================
  private prototypes
=========================================================================== */
static void ClearConnection (TApplPtr appSrc, TApplPtr appDest, TClientsPtr g);
static void SetConnection   (TApplPtr appSrc, TApplPtr appDest, TClientsPtr g);


/*===========================================================================
  MidiShare kernel functions implementation
=========================================================================== */
MSFunctionType(void) MSConnect (short src, short dest, Boolean state, TClientsPtr g)
{
	TApplPtr appSrc, appDest;

	if (CheckRefNum(g,src) && CheckRefNum(g,dest)) {
		appSrc  = g->appls[src]; 
		appDest = g->appls[dest];
		if (folder(appSrc) == folder(appDest)) {
			if( state) SetConnection (appSrc, appDest, g);
			else ClearConnection (appSrc, appDest, g);
		}
	}
}

/*__________________________________________________________________________*/
/* 	- RemAllDstCon : remove all the application output connections          */
/*__________________________________________________________________________*/
void RemAllDstCon (TApplPtr appl)
{
	TConnectionsPtr cnx = &pub(appl, cnx);
	int i;
	   
	for (i=0; i<MaxAppls/8; i++)
		cnx->dst[i] = 0;
}

/*===========================================================================
  internal functions
=========================================================================== */

/*__________________________________________________________________________*/
/* 	- SetConnection : connect 2 applications                                */
/*__________________________________________________________________________*/
static void SetConnection (TApplPtr appSrc, TApplPtr appDst, TClientsPtr g)
{
	TConnectionsPtr srcCnx = &pub(appSrc, cnx);
	short srcRef = pub(appSrc, refNum);
	short dstRef = pub(appDst, refNum);
	
	if (!IsAcceptedBit(srcCnx->dst, dstRef)) {
		AcceptBit(srcCnx->dst, dstRef);
		CallAlarm (srcRef, MIDIChgConnect, g);
	}
}

/*__________________________________________________________________________*/
/* 	- ClearConnection : disconnect 2 applications                           */
/*__________________________________________________________________________*/
static void ClearConnection (TApplPtr appSrc, TApplPtr appDst, TClientsPtr g)
{
	TConnectionsPtr srcCnx = &pub(appSrc, cnx);
	short srcRef = pub(appSrc, refNum);
	short dstRef = pub(appDst, refNum);
	
	if (IsAcceptedBit(srcCnx->dst, dstRef)) {
		RejectBit(srcCnx->dst, dstRef);
		CallAlarm (srcRef, MIDIChgConnect, g);
	}
}
#endif
