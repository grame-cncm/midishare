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
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>

#include "msTasks.h"
#include "msFunctions.h"
#include "msExtern.h"

#include "msRTListenProc.h"
#include "msServerContext.h"
#include "msApplContext.h"
#include "msLog.h"

//*____________________________________________________________________________*/
void CallNetSend  (TMSGlobalPtr g, TApplPtr appl)
{
	msServerContextPtr c = (msServerContextPtr)g->context;
	msApplContextPtr  ac = (msApplContextPtr)appl->context;
	Ev2StreamPtr stream = &c->RT.stream;
    long n; short len;

//printf ("CallNetSend start\n");
	fifo * f = &appl->rcv;
	MidiEvPtr e = (MidiEvPtr)fifoget(f);

	msStreamStart (stream);
	while (e) {
		if (!msStreamPutEvent (stream, e)) {
			do {
				len = msStreamSize(stream);
				n = CCRTWrite (ac->chan, c->RT.buff, len);
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
	n = CCRTWrite (ac->chan, c->RT.buff, len);
    if (n != len) goto failed;
//printf ("CallNetSend exit\n");
	return;

failed:
    /* in case of failure, the client should be rejected */
	/* and the client application closed */
	LogWriteErr ("CCRTWrite failed for client %s (%d)\n", pub(appl, name), pub(appl, refNum));
	if (ac->filterh) msSharedMemClose(ac->filterh);
	MidiClose (pub(appl, refNum));
	CCDec (ac->chan);
	FreeApplContext(ac);
}

/*____________________________________________________________________________*/
static int GetListeners (TApplPtr * appls, fd_set * read, fd_set * except)
{
	int i, n = 0, h; TApplPtr appl;

	FD_ZERO(read);
	FD_ZERO(except);
	for (i=0; i < MaxAppls; i++) {
		appl = appls[i];
		if (appl && appl->netFlag) {
			msApplContextPtr ac = (msApplContextPtr)appl->context;
			h = CCRTReadHandler(ac->chan);
			FD_SET(h, read);
			FD_SET(h, except);
			if (h > n) n = h;
		}
	}
	return n;
}

/*____________________________________________________________________________*/
static void EventHandlerProc (MidiEvPtr e)
{
    if (EvType(e) < typeMidiOpen)
		MidiSend (RefNum(e), e);
    else MidiFreeEv (e);
}

/*____________________________________________________________________________*/
static void ReadClients (msServerContextPtr c, TApplPtr * appls, int n, fd_set * read, fd_set * except)
{
	int i; TApplPtr appl; msStreamBufferPtr parse = &c->RT.parse;

	for (i=0; (i < MaxAppls) && n; i++) {
		appl = appls[i];
		if (appl && appl->netFlag) {
			msApplContextPtr ac = (msApplContextPtr)appl->context;
			if (FD_ISSET(CCRTReadHandler(ac->chan), read)) {
				if (CCRTRead (ac->chan, c->RT.buff, kCommBuffSize) > 0) {
					do {
						int ret;
						MidiEvPtr e = msStreamGetEvent (parse, &ret);
						if (e) {
							EventHandlerProc(e);
							if (!msStreamGetSize(parse)) break;
						}
						else if (ret != kStreamNoMoreData) {
							LogWrite ("msStreamGetEvent read error (%d)", ret);
							break;
						}
					} while (true);
					msStreamParseReset (parse);
				}
				n--;
			}
		}
	}
}

/*____________________________________________________________________________*/
ThreadProc(RTListenProc, arg)
{
	TMSGlobalPtr g = (TMSGlobalPtr)arg;
	msServerContextPtr c = (msServerContextPtr)g->context;
	TApplPtr  * appls = g->clients.appls;

	fd_set read, except; int ret, n; struct timeval t = { 0, 1000 };

printf ("RTListenProc is starting g=%lx\n", (long)g);
	while (true) {
		n = GetListeners(appls, &read, &except);
		if (!n) {
			usleep (1000);
			continue;
		}
		ret = select (n+1, &read, 0, &except, &t);
		if (ret > 0) {
			ReadClients (c, appls, ret, &read, &except);
		}
		else if (ret < 0) LogWriteErr("RTListenProc select error");
	}
printf ("RTListenProc stopped\n");
	return 0;
}
