/*
  Copyright � Grame 2003

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
  
*/

#include <stdio.h>

#include "msTasks.h"
#include "msFunctions.h"

#include "msRTClientProc.h"
#include "msLibContext.h"

/*____________________________________________________________________________*/
static void RcvAlarmLoop( TMSGlobalPtr g)
{
	TApplPtr FAR *applPtr, appl;
	RcvAlarmPtr alarm;

	*NextActiveAppl(g) = 0;              /* close the active applications table */
	applPtr= ActiveAppl(g);              /* starts at the table beginning       */
	while( *applPtr) {
		appl= *applPtr++;
		appl->rcvFlag= (uchar)kNoRcvFlag;	/* unmark the application    */
		alarm= appl->rcvAlarm;              /* get the application alarm */
		if( alarm)                          /* when the alarm exists     */
			alarm(appl->refNum);
	}
	ActiveAppl(g)[0] = 0;
}

/*____________________________________________________________________________*/
static inline void RawAccept( TMSGlobalPtr g, TApplPtr appl, fifo * q, MidiEvPtr ev)
{
	fifoput (q, (cell *)ev);
	if( !++appl->rcvFlag) *NextActiveAppl(g)++ = appl;
}

/*____________________________________________________________________________*/
static int RTSendFlush (msLibContextPtr c)
{
	Ev2StreamPtr stream = &c->RT.stream;
    long n; short len;
	fifo * f = &c->RTSnd;
	MidiEvPtr e = (MidiEvPtr)fifoget(f);

	msStreamStart (stream, c->RT.wbuff, kWriteBuffSize);
	while (e) {
		if (!msStreamPutEvent (stream, e)) {
			do {
				len = msStreamSize(stream);
				n = CCRTWrite (c->cchan, c->RT.wbuff, len);
				if (n != len) goto failed;
			} while (!msStreamContEvent (stream));
		}
		MidiFreeEv (e);
		e = (MidiEvPtr)fifoget(f);
	}
	len = msStreamSize(stream);
	n = CCRTWrite (c->cchan, c->RT.wbuff, len);
    return (n == len);

failed:
perror ("RTSendFlush: CCRTWrite failed");
	MidiFreeEv (e);
    return false;
}

/*____________________________________________________________________________*/
Boolean RTSend (MidiEvPtr e, TMSGlobalPtr g)
{
	msLibContextPtr c = (msLibContextPtr)g->context;
	if (e) {
		fifoput (&c->RTSnd, (cell *)e);
	}
	return true;
}

//int gRTClientProcFlag = 0;
//MidiEvPtr gProcessCallEv = 0;
//MidiEvPtr gProcessCallEv2 = 0;
//TaskPtr gProcessFun = 0;
//long gProcessFun2 = 0;
//int gPCCount=0;
/*____________________________________________________________________________*/
static void ProcessCall (TMSGlobalPtr g, MidiEvPtr e)
{
//	gPCCount++;
	msLibContextPtr c = (msLibContextPtr)g->context;
//gProcessCallEv = e;
	TTaskExtPtr task = (TTaskExtPtr)LinkST(e);      /* event extension */
	c->send = RTSend;
//gRTClientProcFlag = 3123;
//gProcessFun = task->fun;
//gProcessFun2 = LinkST((MidiEvPtr)(task->fun))->val[0];
//gProcessCallEv2 = e;
	task->fun (Date(e), RefNum(e), task->arg1, task->arg2, task->arg3);
//	gProcessFun (Date(gProcessCallEv), RefNum(gProcessCallEv), task->arg1, task->arg2, task->arg3);
//gRTClientProcFlag = 3124;
	c->send = StdSend;
}

/*____________________________________________________________________________*/
static void DispatchEvent (TMSGlobalPtr g, MidiEvPtr e)
{
	short type= EvType(e);
	short refNum= RefNum(e) & 0x7F;     /* unflag the refnum number     */
	TApplPtr appl; MidiSTPtr ext; MidiEvPtr process;
	
	if (type >= typeMidiOpen)			/* event is ignored  */
		goto reject;
	if (!CheckGlobRefNum(g, refNum))  	/* check if refnum is valid     */
		goto reject;

//        gRTClientProcFlag = 311;
	appl = g->appls[refNum];
	switch (type) {
		case typeProcess:
//        gRTClientProcFlag = 312;
            ext = LinkST(e);
            process = (MidiEvPtr)ext->val[0];
           if (EvType(process) == typeProcess)
                ProcessCall (g, process);
//			MidiFreeEv (e);
//			MidiFreeEv (process);
			break;
		case typeDProcess:
//        gRTClientProcFlag = 313;
            ext = LinkST(e);
            process = (MidiEvPtr)ext->val[0];
            if (EvType(process) == typeDProcess)
                fifoput (&appl->dTasks, (cell *)process);
			else MidiFreeEv (process);
			MidiFreeEv (e);
			break;
		case typeApplAlarm:
//        gRTClientProcFlag = 314;
			if (appl->applAlarm) {
				appl->applAlarm (appl->refNum, e->info.longField);
				MidiFreeEv (e);
			}
			else RawAccept(g, appl, &appl->rcv, e);
			break;
		default:
//        gRTClientProcFlag = 315;
			RawAccept(g, appl, &appl->rcv, e);
	}
//        gRTClientProcFlag = 316;
	return;

reject:
//        gRTClientProcFlag = 310;
	MidiFreeEv (e);					/* free the event  					*/
}

/*____________________________________________________________________________*/
ThreadProc(RTClientProc, arg)
{
	TMSGlobalPtr g = (TMSGlobalPtr)arg;
	msLibContextPtr c = (msLibContextPtr)g->context;
	msStreamBuffer * parse = &c->RT.parse;
    MidiEvPtr e; long n; int ret;

	while (true) {
		n = CCRTRead (c->cchan, c->RT.rbuff, kReadBuffSize);
        if (n <= 0) break; /* corrupted communication channel: exit the RT thread */

		NextActiveAppl(g) = ActiveAppl(g);
		e = msStreamStartBuffer (parse, n, &ret);
		while (e) {
//gRTClientProcFlag = 31;
			DispatchEvent (g, e);
//gRTClientProcFlag = 32;
			e = msStreamGetEvent (parse, &ret);
		}
		RcvAlarmLoop (g);
		if (!RTSendFlush (c))
			perror ("RTClientProc: RTSendFlush error");
	}
//	fprintf (stderr, "RTClientProc: CCRTRead error %ld\n", n);
	return 0;
}
