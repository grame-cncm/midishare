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
   [08-09-99] DF - adaptation to the new memory management
   [13-09-99] DF - removing direct fifo access in ClockHandler
   [07-08-02] DF - Accept redesigned due to server memory changes

*/

#include "msEvents.h"
#include "msKernel.h"
#include "msSorter.h"
#include "msExtern.h"


/*===========================================================================
  Internal functions prototypes
  =========================================================================== */
static void Accept			( TMSGlobalPtr g, TApplPtr appl, MidiEvPtr ev);
static void DriverAccept    ( TMSGlobalPtr g, TApplPtr appl, MidiEvPtr ev);
static void Propose		    ( TMSGlobalPtr g, TApplPtr appl, MidiEvPtr ev);
static void DispatchEvents	( TMSGlobalPtr g, MidiEvPtr ev);
static void RcvAlarmLoop	( TMSGlobalPtr g);
static void ConnectionsLoop ( TMSGlobalPtr g, TApplPtr appl, MidiEvPtr ev);
static void SlotConnectionsLoop (TMSGlobalPtr g, TApplPtr appl, MidiEvPtr ev);
static short IsAcceptedEvent (MidiFilterPtr filter, MidiEvPtr ev);

/*===========================================================================
  External functions implementation
  ===========================================================================*/ 
void ClockHandler (TMSGlobalPtr g)
{
	 THorlogePtr h; TDatedEvPtr e;
	 MidiEvPtr ready;

	 h= &g->clock;
	 if( ++h->reenterCount) {
		 return;
	 }

	 do {
		 CurrTime(g)++;
		 e = (TDatedEvPtr)fifoclear (SorterList(g));
		 ready = (MidiEvPtr)SorterClock(Sorter(g), e);
		 if( ready) {
		 	NextActiveAppl(g) = ActiveAppl(g);
		 	DispatchEvents(g, ready);
		 	RcvAlarmLoop(g);
		 }

	} while( h->reenterCount--);
}


/*===========================================================================
  Internal functions implementation
  ===========================================================================*/ 
static inline void RawAccept( TMSGlobalPtr g, TApplPtr appl, fifo * q, MidiEvPtr ev)
{
	fifoput (q, (cell *)ev);
	if( !++appl->rcvFlag) *NextActiveAppl(g)++ = appl;
}

static void Accept( TMSGlobalPtr g, TApplPtr appl, MidiEvPtr ev)
{
	switch (folder(appl)) {
		case kClientFolder:
			/* 
			   event is sent to MidiShare application. It goes through the 
			   implicit connection between the MidiShare application and the
			   MidiShare driver.
			*/
			if (pub(appl,refNum) == MidiShareRef) {
				ConnectionsLoop (g, Clients(g)->appls[MidiShareDriverRef], ev);
				MSFreeEv (ev, FreeList(Memory(g)));
			}
			else 			/* event destination is another application */
				RawAccept (g, appl, &appl->rcv, ev);
			break;

		case kDriverFolder:
			/* 
			   event is sent to the MidiShare driver. It goes through the 
			   implicit connection between the MidiShare driver and the
			   MidiShare application for each slot connection. 
			*/
			if (pub(appl,refNum) == MidiShareDriverRef) {
				SlotConnectionsLoop (g, Clients(g)->appls[MidiShareDriverRef], ev);
			}
			else 			/* event destination is another driver */
				DriverAccept (g, appl, ev);
			MSFreeEv (ev, FreeList(Memory(g)));
			break;
	}
}

/*__________________________________________________________________________________*/
static short IsAcceptedEvent (MidiFilterPtr filter, MidiEvPtr ev) 
{
	Byte type  = EvType(ev);
	Byte canal = (Byte)(Canal(ev) & 0x0f);
	return !filter
			|| ((IsAcceptedBit( filter->port, Port(ev)) 
				&& IsAcceptedBit( filter->evType, type))
				&& ((type > typePitchWheel) || IsAcceptedBit( filter->channel, canal)));
}

/*__________________________________________________________________________________*/
static void Propose( TMSGlobalPtr g, TApplPtr appl, MidiEvPtr ev) 
{
	if (IsAcceptedEvent(appl->filter, ev)) {
		MidiEvPtr copy= MSCopyEv( ev, FreeList(Memory(g)));
		if( copy)
			Accept( g, appl, copy);
	}
}

/*__________________________________________________________________________________*/
static void DriverAccept( TMSGlobalPtr g, TApplPtr appl, MidiEvPtr ev) 
{
	/* 
	   the DriverAccept function operates similarly to Accept but looks for
	   the destination slot. If the tharget slot has a receive alarm, the event
	   is pushed in the slot fifo, otherwise  the event port field is replaced
	   by the slot reference number and it's pushed in the driver fifo. The
	   latter ensures notably the compatibility with the v1.86 behavior.
	*/
	if (IsAcceptedEvent(appl->filter, ev)) {
		TDriverPtr drv = Driver(appl);
		SlotInfosPtr slot = drv->slots;
		Byte port = Port(ev);
		while (slot) {
			if ((pub(slot,direction) & MidiOutputSlot) && IsAcceptedBit(pub(slot,cnx), port)) { 
				MidiEvPtr copy= MSCopyEv( ev, FreeList(Memory(g)));
				if (copy) {
					if (slot->rcvAlarm) {
						RawAccept( g, appl, &slot->rcv, copy);
					}
					else {
						Port(copy) = (Byte)pub(slot, slotRefNum);
						RawAccept( g, appl, &appl->rcv, copy);
					}
				}
			}
			slot = slot->next;
		}
	}
}

/*__________________________________________________________________________________*/
static void ProcessCall( TMSGlobalPtr g, TApplPtr appl, MidiEvPtr ev)
{
	if (appl->netFlag) {
		fifoput (&appl->rcv, (cell *)ev);
		if( !++appl->rcvFlag) *NextActiveAppl(g)++ = appl;
	}
	else
		CallTaskCode (appl->context, ev);
}

/*__________________________________________________________________________________*/
static inline void AcceptTask (TApplPtr appl, MidiEvPtr ev)
{
	if (appl->netFlag)
		fifoput (&appl->rcv, (cell *)ev);
	else
		fifoput (&appl->dTasks, (cell *)ev);
}

/*__________________________________________________________________________________*/
/*	Connections loop								*/
/*__________________________________________________________________________________*/
static void SlotConnectionsLoop (TMSGlobalPtr g, TApplPtr appl, MidiEvPtr ev)
{
	short slotRef = Port(ev); int i;
	SlotInfosPublicPtr slot = &pub(Clients(g), slots)[slotRef];

	if (!(slot->direction & MidiInputSlot)) return;
	for (i=0; i<(MaxPorts/8); i++) {
		if (slot->cnx[i]) {
			int j;
			for (j=0; j<8; j++) {
				if (IsAcceptedBit(&slot->cnx[i], j)) {
					Port(ev) = (Byte)((i<<3) + j);
					ConnectionsLoop (g, Clients(g)->appls[MidiShareRef], ev);
				}
			}
		}
	}
}

static void ConnectionsLoop (TMSGlobalPtr g, TApplPtr appl, MidiEvPtr ev)
{
	TConnectionsPtr cnx; int i;
	cnx= &pub(appl, cnx);		        /* get the destination connections list */
	
	for (i=0; i<MaxAppls; i++) {
		if (IsAcceptedBit(cnx->dst, i)) {
			TApplPtr dst = Clients(g)->appls[i];
			Propose(g, dst, ev);	    /* propose to each connected appl */
		}
	}
}

/*__________________________________________________________________________________*/
/*	DispatchEvents								*/
/*__________________________________________________________________________________*/
static void DispatchEvents (TMSGlobalPtr g, MidiEvPtr ev)
{
	MSMemoryPtr mem = Memory(g);
	MidiEvPtr next;
	short refNum; Byte type;

	do {
		type= EvType(ev);
		next= Link(ev);
		if( type >= typeReserved) {			/* typeReserved : event is ignored  */
			MSFreeEv( ev, FreeList(mem));	/* free the event  					*/
			ev= next;						/* next event						*/
			continue;						/* and loop							*/
		}
		
		refNum= RefNum(ev) & 0x7F;              /* unflag the refnum number     */
		if( CheckRefNum(Clients(g), refNum)) {  /* check if refnum is valid     */
			TApplPtr appl = Appls(g)[refNum];   /* get the corresponding client */

			if( (type >= typeQuarterFrame) || (type < typePrivate)) {
				if( RefNum(ev) & 0x80)	{	        /* refnum is marked			*/
					RefNum(ev) = (Byte)refNum;
					Accept( g, appl, ev);		    /* event is for appl itself */
				}
				else {					        	/* standard event received  */
					ConnectionsLoop (g, appl, ev);
					MSFreeEv( ev, FreeList(mem));   /* and free the event */
				}
			}
			else if (type == typeProcess) {		/* event is a realtime task		*/
				ProcessCall( g, appl, ev);		    /* execute the task				*/
//#ifndef __linux__
//				MSFreeEv( ev, FreeList(mem));
//#endif
			}
			else if (type == typeDProcess) {    /* typeDProcess : defered task	*/
				AcceptTask(appl, ev);    		/* store in the appl dtasks fifo*/
			}
			else if( type >= typePrivate) {          /* it's a private event     */
				RawAccept( g, appl, &appl->rcv, ev); /* event is for appl itself */
			}
			else MSFreeEv( ev, FreeList(mem));
		}
		else MSFreeEv( ev, FreeList(mem));  /* free the event       */
		ev= next;                 /* go to the next event */
	} while( ev);                 /* and loop             */
}

/*__________________________________________________________________________________*/
/*	- RcvAlarmLoop - go thru the applications list and when necessary               */
/*	call the rcvAlarm                                                               */
/*__________________________________________________________________________________*/
static void RcvAlarmLoop( TMSGlobalPtr g)
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
			CallRcvAlarm (appl->context, alarm, pub(appl, refNum));
		}
		else if (appl->netFlag) {	/* remote applications can't set an alarm in kernel space */
			CallNetSend (g, appl);
		}
	}
}
