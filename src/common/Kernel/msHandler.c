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

#include "msEvents.h"
#include "msKernel.h"
#include "msSorter.h"
#include "msExtern.h"


/*===========================================================================
  Internal functions prototypes
  =========================================================================== */
static void NEAR Accept			( TMSGlobalPtr g, TApplPtr appl, MidiEvPtr ev);
static void NEAR Propose		( TMSGlobalPtr g, TApplPtr appl, MidiEvPtr ev);
static void NEAR Process		( MidiEvPtr ev);
static void NEAR DiffProcess	( MidiEvPtr ev);
static void NEAR DispatchEvents	( TMSGlobalPtr g, MidiEvPtr ev);
static void NEAR RcvAlarmLoop	( TMSGlobalPtr g);


/*===========================================================================
  External functions implementation
  ===========================================================================*/ 
void ClockHandler (TMSGlobalPtr g)
{
	 THorlogePtr h;
	 MidiEvPtr ready;

	 h= &g->currTime;
	 if( ++h->reenterCount) {
		 return;
	 }

	 do {
		 h->time++;
		 SorterClock(Sorter(g));

		 NextActiveAppl(g) = ActiveAppl(g);
		 ready= (MidiEvPtr)SorterGet(Sorter(g));

		 if( ready) DispatchEvents(g, ready);
		 RcvAlarmLoop(g);

	}while( h->reenterCount--);
}



/*===========================================================================
  Internal functions implementation
  ===========================================================================*/ 
static void NEAR Accept( TMSGlobalPtr g, TApplPtr appl, MidiEvPtr ev)

{
	if( appl->refNum ) {
		Link(ev)= 0;
		Link(RcvFifoTail(appl)) = ev;
		RcvFifoTail(appl) = ev;
		RcvFifoCount(appl)++;
		if( !++appl->rcvFlag) *NextActiveAppl(g)++ = appl;
	}
	else { 
		/* refnum 0 is MidiShare refnum */
		/* event should be handled by the port manager */
		/* not yet implemented */
		MSFreeEv( ev, Memory(g));
	}
}

/*__________________________________________________________________________________*/
static void NEAR Propose( TMSGlobalPtr g, TApplPtr appl, MidiEvPtr ev) 

{
	MidiEvPtr copy;
	MidiFilterPtr filter;
	Byte type, canal;

	filter = appl->filter;
	type = EvType(ev);
	canal = Canal(ev)&0x0f;
	if (!filter
		|| (IsAcceptedBit( filter->port, Port(ev)) && IsAcceptedBit( filter->evType, type))
			&& ((type > typePitchWheel) || IsAcceptedBit( filter->channel, canal))) {
			copy= MSCopyEv( ev, Memory(g));
			if( copy)
				Accept( g, appl, copy);
	}
}

/*__________________________________________________________________________________*/
static void NEAR ProcessCall( TApplPtr appl, MidiEvPtr ev)
{
	TTaskExtPtr task = (TTaskExtPtr)LinkST(ev);	/* event extension */
	CallTaskCode (appl->context, task, Date(ev), appl->refNum);
}

/*__________________________________________________________________________________*/
static void NEAR AcceptTask(TMSGlobalPtr g, TApplPtr appl, MidiEvPtr ev)

{
	Link(ev) = 0;
	PushFifoEv (&appl->dTasks, ev);
}

/*__________________________________________________________________________________*/
/*	DispatchEvents								*/
/*__________________________________________________________________________________*/
static void NEAR DispatchEvents (TMSGlobalPtr g, MidiEvPtr ev)
{
	MSMemoryPtr mem = Memory(g);
	MidiEvPtr next;
	char refNum; Byte type;

	do {
		type= EvType(ev);
		next= Link(ev);
		if( type >= typeReserved) {			/* typeReserved : event is ignored  */
			MSFreeEv( ev, mem);				/* free the event  					*/
			ev= next;						/* next event						*/
			continue;						/* and loop							*/
		}
		
		refNum= RefNum(ev) & 0x7F;              /* unflag the refnum number     */
		if( CheckRefNum(Clients(g), refNum)) {  /* check if refnum is valid     */
			TApplPtr appl = Appls(g)[refNum];   /* get the corresponding client */

			if( (type >= typeQuarterFrame) || (type < typePrivate)) {
				if( RefNum(ev) & 0x80)	{	        /* refnum is marked			*/
					Accept( g, appl, ev);		    /* event is for appl itself */
				}
				else {					        	/* standard event received  */
					TConnectionPtr cnx;
					cnx= appl->dstList;		        /* get the connections list */
					while( cnx) {
						Propose( g,cnx->itsDst,ev);	/* propose to each connected appl */
						cnx= cnx->nextDst;		    /* loop thru the connection list  */
					}
					MSFreeEv( ev, mem);             /* and free the event */
				}
			}
			else if (type == typeProcess) {		/* event is a realtime task		*/
				ProcessCall( appl, ev);		    /* execute the task				*/
				MSFreeEv( ev, mem);
			}
			else if (type == typeDProcess) {    /* typeDProcess : defered task	*/
				AcceptTask(g, appl, ev);    	/* store in the appl dtasks fifo*/
			}
			else if( type >= typePrivate) {     /* it's a private event 	*/
				Accept( g, appl, ev);           /* event is for appl itself */
			}
			else MSFreeEv( ev, mem);
		}
		else MSFreeEv( ev, mem);  /* free the event       */
		ev= next;                 /* go to the next event */
	} while( ev);                 /* and loop             */
}

/*__________________________________________________________________________________*/
/*	- RcvAlarmLoop - go thru the applications list and when necessary               */
/*	call the rcvAlarm                                                               */
/*__________________________________________________________________________________*/
static void NEAR RcvAlarmLoop( TMSGlobalPtr g)
{
	TApplPtr FAR *applPtr, appl;
	RcvAlarmPtr alarm;

	*NextActiveAppl(g) = 0;               /* close the active applications table */
	applPtr= ActiveAppl(g);              /* starts at the table beginning       */
	while( *applPtr) {
		appl= *applPtr++;
		appl->rcvFlag= (uchar)kNoRcvFlag;	/* unmark the application    */
		alarm= appl->rcvAlarm;              /* get the application alarm */
		if( alarm) {                        /* when the alarm exists     */
			CallRcvAlarm (appl->context, alarm, appl->refNum);
		}
	}

}
