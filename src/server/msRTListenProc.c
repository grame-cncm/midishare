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

#include <mach/mach_time.h>
//*____________________________________________________________________________*/
static void DropClients  (CommunicationChan cc)
{
	TApplPtr * appls = Appls(gMem);
	int i;
	
	for (i=0; i<MaxAppls; i++) {
		TApplPtr appl = appls[i];
		if (appl && appl->netFlag) {
			msApplContextPtr  ac = (msApplContextPtr)appl->context;
			if (cc == ac->chan) {
				LogWrite ("Client application \"%s\" (%d) dropped at time %lld", pub(appl, name), pub(appl, refNum), mach_absolute_time());
				if (ac->filterh) msSharedMemClose(ac->filterh);
				MidiClose (pub(appl, refNum));
				CCDec (cc);
				FreeApplContext(ac);
			}
		}
	}
}

//*____________________________________________________________________________*/
void CallNetSendAlarm  (TApplPtr appl, MidiEvPtr alarm)
{
	msApplContextPtr  ac = (msApplContextPtr)appl->context;
	CommunicationChan cc = ac->chan;
	RTCommPtr rt = (RTCommPtr)CCGetInfos (cc);
	Ev2StreamPtr stream = &rt->stream;
    long n; short len; void *buff;

	buff=rt->wbuff[rt->index++];
	if (rt->index >= kMaxWBuffers) rt->index = 0;
	msStreamStart (stream, buff, kRTWriteBuffSize);
	RefNum(alarm) = (uchar)pub(appl, refNum);
    if (!msStreamPutEvent (stream, alarm)) {
        LogWriteErr ("CallNetSendAlarm failed for client %s (%d)", pub(appl, name), pub(appl, refNum));
    }
    else {
        len = msStreamSize(stream);
        n = CCRTWrite (cc, buff, len);
        if (n != len) goto failed;
    }
    MidiFreeEv (alarm);
	return;

failed:
    MidiFreeEv (alarm);
}

//*____________________________________________________________________________*/
void CallNetSend  (TMSGlobalPtr g, TApplPtr appl)
{
	msApplContextPtr  ac = (msApplContextPtr)appl->context;
	CommunicationChan cc = ac->chan;
	RTCommPtr rt = (RTCommPtr)CCGetInfos (cc);
	Ev2StreamPtr stream = &rt->stream;
    long n; short len; void *buff;

	buff=rt->wbuff[rt->index++];
	if (rt->index >= kMaxWBuffers) rt->index = 0;

	fifo * f = &appl->rcv;
	MidiEvPtr e = (MidiEvPtr)fifoget(f);

	msStreamStart (stream, buff, kRTWriteBuffSize);
	while (e) {
		RefNum(e) = (uchar)pub(appl, refNum);
		if (!msStreamPutEvent (stream, e)) {
			do {
				len = msStreamSize(stream);
				n = CCRTWrite (cc, buff, len);
				if (n != len) {
					MidiFreeEv (e);
					return;
				}
			} while (!msStreamContEvent (stream));
		}
		MidiFreeEv (e);
		e = (MidiEvPtr)fifoget(f);
	}
	len = msStreamSize(stream);
	n = CCRTWrite (cc, buff, len);
//    if (n != len) goto failed;
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
		else {
			LogWriteErr ("RTListenProc: CCRTRead read error (%ld)", n);
			break;
		}
	} while (CCRefCount(cc));
	DropClients (cc);
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
		msStreamInit 	  (&rt->stream, c->streamMthTable);
		rt->index = 0;
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
