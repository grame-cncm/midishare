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
   [08-09-99] DF - new memory management

*/

#include "msAlarms.h"
#include "msConnx.h"
#include "lflifo.h"

/*===========================================================================
  private prototypes
=========================================================================== */
static void Rem1DstCon  (TApplPtr appl, TConnectionPtr cnx);
static void Rem1SrcCon  (TApplPtr appl, TConnectionPtr cnx);
static void ClearConnection (TApplPtr appSrc, TApplPtr appDest, TClientsPtr g);
static void SetConnection   (TApplPtr appSrc, TApplPtr appDest, TClientsPtr g);
static TConnectionPtr FindConnection (TApplPtr appSrc, TApplPtr appDest);


/*===========================================================================
  MidiShare external functions implementation
=========================================================================== */
MSFunctionType(void) MSConnect (short src, short dest, Boolean state, TClientsPtr g)
{
	TApplPtr appSrc, appDest;

	if (CheckRefNum(g,src) && CheckRefNum(g,dest)) {
		appSrc  = g->appls[src]; 
		appDest = g->appls[dest];
		if (appSrc->folder == appDest->folder) {
			if( state) SetConnection (appSrc, appDest, g);
			else ClearConnection (appSrc, appDest, g);
		}
	}
}

MSFunctionType(Boolean) MSIsConnected (short src, short dest, TClientsPtr g)
{
	TApplPtr appSrc, appDest;

	if( CheckRefNum(g,src) && CheckRefNum(g,dest) ) {
		appSrc  = g->appls[src]; 
		appDest = g->appls[dest];
		return (FindConnection (appSrc,appDest) != 0);
	}
	return false;
}

/*__________________________________________________________________________*/
/* 	- RemAllDstCon : remove all the application output connections          */
/*__________________________________________________________________________*/
void RemAllDstCon (TApplPtr appl, lifo* freelist)
{
	TConnectionPtr cnx, next;
	cnx= appl->dstList;						/* application connections list  */
	while( cnx) {
		next= cnx->nextDst;                 /* get the next connection       */
		Rem1SrcCon (cnx->itsDst, cnx);      /* remove the current connection */
		lfpush (freelist, (cell *)cnx);     /* free the connection           */
		cnx= next;							/* current = next connection     */
	}
	appl->dstList= 0;                       /*  connection list is now empty */
}
	
/*__________________________________________________________________________*/
/* 	- RemAllSrcCon : remove all the application input connections           */
/*__________________________________________________________________________*/
void RemAllSrcCon (TApplPtr appl, lifo* freelist)
{
	TConnectionPtr cnx, next;

	cnx= appl->srcList;						/* application connections list  */
	while( cnx) {
		next= cnx->nextSrc;                 /* get the next connection       */
		Rem1DstCon(cnx->itsSrc, cnx);       /* remove the current connection */
		lfpush (freelist, (cell *)cnx);     /* free the connection           */
		cnx= next;							/* current = next connection     */
	}
	appl->srcList= 0;                       /*  connection list is now empty */
}


/*===========================================================================
  internal functions
=========================================================================== */

/*__________________________________________________________________________*/
/* 	- SetConnection : connect 2 applications                                */
/*__________________________________________________________________________*/
static void SetConnection (TApplPtr appSrc, TApplPtr appDest, TClientsPtr g)
{
	if( !FindConnection (appSrc, appDest)) {
		TConnectionPtr cnx= (TConnectionPtr)lfpop(FreeList(g->memory));
		if( cnx) {
			cnx->itsSrc= appSrc;
			cnx->itsDst= appDest;
			cnx->nextSrc= appDest->srcList;
			appDest->srcList= cnx;
			cnx->nextDst= appSrc->dstList;
			appSrc->dstList= cnx;
			CallAlarm (appSrc->refNum, MIDIChgConnect, g);
		}
	}
}

/*__________________________________________________________________________*/
/* 	- ClearConnection : disconnect 2 applications                           */
/*__________________________________________________________________________*/
static void ClearConnection (TApplPtr appSrc, TApplPtr appDest, TClientsPtr g)
{
	TConnectionPtr cnx = FindConnection( appSrc, appDest);
	if( cnx) {
		Rem1SrcCon (appDest, cnx);
		Rem1DstCon (appSrc, cnx);
		lfpush (FreeList(g->memory), (cell*)cnx);
		CallAlarm (appSrc->refNum, MIDIChgConnect, g);
	}
}
	
/*__________________________________________________________________________*/
/* 	- Rem1DstCon: remove 1 connection from the application destination list */
/*__________________________________________________________________________*/
static void Rem1DstCon (TApplPtr appl, TConnectionPtr cnx)
{
	TConnectionPtr c = appl->dstList;					
	if (c == cnx)						
		appl->dstList= c->nextDst;		
	else {
		while (c->nextDst != cnx) {      /* assume that cnx is in the list */
			c = c->nextDst;
		}
		c->nextDst= cnx->nextDst;
	}
}
	
/*__________________________________________________________________________*/
/* 	- Rem1SrcCon : remove 1 connection from the application source list     */
/*__________________________________________________________________________*/
static void Rem1SrcCon (TApplPtr appl, TConnectionPtr cnx)
{
	TConnectionPtr c = appl->srcList;					
	if (c == cnx) {						
		appl->srcList= c->nextSrc;		
	} else {
		while (c->nextSrc != cnx) {      /* assume that cnx is in the list */
			c = c->nextSrc;
		}
		c->nextSrc= cnx->nextSrc;
	}
}

/*__________________________________________________________________________*/
/* 	- FindConnection : returns a pointer to a connection when it exists     */
/*__________________________________________________________________________*/
static TConnectionPtr FindConnection (TApplPtr appSrc, TApplPtr appDest) 
{
	TConnectionPtr cnx = appSrc->dstList;
	while( cnx) {
		if( cnx->itsDst == appDest) return cnx;
		cnx= cnx->nextDst;
	}
	return 0;
}
