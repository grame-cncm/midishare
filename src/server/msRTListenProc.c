/*
  Copyright © Grame 2003

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
#include <string.h>
#include "msTasks.h"
#include "msFunctions.h"
#include "msExtern.h"

#include "msRTListenProc.h"
#include "msServerContext.h"
#include "msApplContext.h"
#include "msLog.h"

//*____________________________________________________________________________*/
static void RejectClient  (TApplPtr appl)
{
	msApplContextPtr  ac = (msApplContextPtr)appl->context;
	CommunicationChan cc = ac->chan;

	if (ac->filterh) msSharedMemClose(ac->filterh);
	MidiClose (pub(appl, refNum));
	CCDec (cc);
	FreeApplContext(ac);
}

//*____________________________________________________________________________*/
void CallNetSendAlarm  (TApplPtr appl, MidiEvPtr alarm)
{
	msApplContextPtr  ac = (msApplContextPtr)appl->context;
	CommunicationChan cc = ac->chan;
	RTCommPtr rt = (RTCommPtr)CCGetInfos (cc);
	Ev2StreamPtr stream = &rt->stream;
    long n; short len;

	msStreamStart (stream);
	RefNum(alarm) = (uchar)pub(appl, refNum);
    if (!msStreamPutEvent (stream, alarm)) {
        LogWriteErr ("CallNetSendAlarm failed for client %s (%d)\n", pub(appl, name), pub(appl, refNum));
    }
    else {
        len = msStreamSize(stream);
        n = CCRTWrite (cc, rt->wbuff, len);
        if (n != len) goto failed;
    }
    MidiFreeEv (alarm);
	return;

failed:
    /* in case of failure, the client should be rejected */
	/* and the client application closed */
    MidiFreeEv (alarm);
	LogWriteErr ("CCRTWrite failed for client %s (%d)\n", pub(appl, name), pub(appl, refNum));
	RejectClient (appl);
}

//*____________________________________________________________________________*/
void CallNetSend  (TMSGlobalPtr g, TApplPtr appl)
{
	msApplContextPtr  ac = (msApplContextPtr)appl->context;
	CommunicationChan cc = ac->chan;
	RTCommPtr rt = (RTCommPtr)CCGetInfos (cc);
	Ev2StreamPtr stream = &rt->stream;
    long n; short len;

	fifo * f = &appl->rcv;
	MidiEvPtr e = (MidiEvPtr)fifoget(f);

//printf ("CallNetSend msStreamStart %lx %lx %lx\n", cc, rt, stream);
	msStreamStart (stream);
	while (e) {
		RefNum(e) = (uchar)pub(appl, refNum);
		if (!msStreamPutEvent (stream, e)) {
			do {
				len = msStreamSize(stream);
				n = CCRTWrite (cc, rt->wbuff, len);
				if (n != len) {
					MidiFreeEv (e);
					goto failed;
				}
			} while (!msStreamContEvent (stream));
		}
		MidiFreeEv (e);
		e = (MidiEvPtr)fifoget(f);
	}
	len = msStreamSize(stream);
	n = CCRTWrite (cc, rt->wbuff, len);
    if (n != len) goto failed;
//printf ("CallNetSend end\n");
	return;

failed:
    /* in case of failure, the client should be rejected */
	/* and the client application closed */
	LogWriteErr ("CCRTWrite failed for client %s (%d)\n", pub(appl, name), pub(appl, refNum));
	RejectClient (appl);
}

/*____________________________________________________________________________*/
static void EventHandlerProc (MidiEvPtr e)
{
    if (EvType(e) < typeMidiOpen)
		MidiSend (RefNum(e), e);
    else MidiFreeEv (e);
}

/*____________________________________________________________________________*/
static ThreadProc(RTListenProc, arg)
{
	CommunicationChan cc = (CommunicationChan)arg;
	RTCommPtr rt = (RTCommPtr)CCGetInfos (cc);
	msStreamBufferPtr parse = &rt->parse;

	do {
        long n = CCRTRead (cc, rt->rbuff, kRTReadBuffSize) ;
        if (n > 0) {
			int ret;
			MidiEvPtr e = msStreamStartBuffer (parse, n, &ret);
			while (e) {
				EventHandlerProc(e);
				e = msStreamGetEvent (parse, &ret);
			}
			if (ret != kStreamNoMoreData)
				LogWrite ("RTListenProc: msStreamGetEvent read error (%d)", ret);
		}
		else break;
	} while (true);
/*
	while (CCRTRead (cc, rt->rbuff, kRTReadBuffSize) > 0) {
		do {
			int ret;
			MidiEvPtr e = msStreamGetEvent (parse, &ret);
			if (e) {
				EventHandlerProc(e);
				if (!msStreamGetSize(parse)) break;
			}
			else {
				if (ret != kStreamNoMoreData)
					LogWrite ("RTListenProc: msStreamGetEvent read error (%d)", ret);
				break;
			}
		} while (true);
		msStreamParseReset (parse);
	}
*/
	return 0;
}

/*____________________________________________________________________________*/
void RTCommStop (RTCommPtr rt)
{
	if (rt) {
		msThreadDelete (rt->RTThread);
		DisposeMemory (rt);		
	}
}

/*____________________________________________________________________________*/
int RTCommInit (msServerContextPtr c, CommunicationChan cc)
{
	RTCommPtr rt = (RTCommPtr)AllocateMemory (sizeof(RTComm));
	if (rt) {
		msStreamParseInit (&rt->parse, c->parseMthTable, rt->rbuff, kRTReadBuffSize);
		msStreamInit 	  (&rt->stream, c->streamMthTable, rt->wbuff, kRTWriteBuffSize);
		CCSetInfos (cc, rt);
		rt->RTThread = msThreadCreate (RTListenProc, cc, kServerLRTPriority);
		if (rt->RTThread) return true;

		CCSetInfos (cc, 0);
		DisposeMemory (rt);
		rt = 0;
		LogWrite ("RTCommInit: cannot create RTListenProc");
	}
	else LogWrite ("RTCommInit: memory allocation failed");
	return false;
}
