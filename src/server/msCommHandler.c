/*
  Copyright © Grame 2002

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
#include <stdlib.h>

#include "msFunctions.h"
#include "msKernel.h"

#include "msApplContext.h"
#include "msCommHandler.h"
#include "msEventsHandler.h"
#include "msLog.h"
#include "msServerInit.h"
#include "msThreads.h"
#include "osglue.h"

#include "msRTListenProc.h"

CommChansPtr gCCList = 0;
msStreamParseMethodTbl	gParseMthTable;
msStreamMthTbl 			gStreamMthTable;

/*____________________________________________________________________________*/
static void CCClose (CommunicationChan cc)
{
	RTCommPtr rt = CCGetInfos (cc);
	CloseCommunicationChannel (cc);
	RTCommStop (rt);
}

/*____________________________________________________________________________*/
static Boolean SendEvent (MidiEvPtr e, CommChansPtr pl)
{
    Ev2StreamPtr stream = &pl->stream;
    long n; short len;

    msStreamStart (stream, pl->wbuff, kWriteBuffSize);
    if (!msStreamPutEvent (stream, e)) {
        do {
            len = msStreamSize(stream);
            n = CCWrite (pl->comm, pl->wbuff, len);
            if (n != len) goto failed;
        } while (!msStreamContEvent (stream));
    }
    else {
        len = msStreamSize(stream);
        n = CCWrite (pl->comm, pl->wbuff, len);
        if (n != len) goto failed;
    }
    return true;
    
failed:
	LogWriteErr ("CCWrite failed (%ld)", n);
    return false;
}

/*____________________________________________________________________________*/
static ThreadProc(CommHandlerProc, p)
{
	CommChansPtr pl = (CommChansPtr)p;
	msStreamBufferPtr parse = &pl->parse;
    msServerContextPtr c = ServerContext;

//fprintf (stderr, "New CommHandlerProc: pipes pair %lx id = %d\n", (long)pl->comm, (int)CCGetID(pl->comm));
    do {
        long n = CCRead (pl->comm, pl->rbuff, kReadBuffSize);
        if (n > 0) {
            int ret;  MidiEvPtr reply, e;
			e = msStreamStartBuffer (parse, n, &ret);
			if (e) {
//				reply = DTEventHandlerProc(e, pl->comm);
				reply = c->netDTEvHandlerTbl[EvType(e)](e, pl->comm);
				if (reply && !SendEvent (reply, pl))
					break;
			}
			else if (ret != kStreamNoMoreData) {
				msStreamParseReset (parse); /* only one event expected on the commands channel */
				LogWrite ("CommHandlerProc: msStreamGetEvent read error (%d)", ret);
				break;
			}
        }
//        else if (n < 0) {
        else {
            LogWriteErr ("CommHandlerProc CCRead error (%ld)", n);
            break;
        }
    } while (CCRefCount(pl->comm));
	if (CCRefCount(pl->comm)) {		
        LogWrite ("CommHandlerProc exit: CloseCommunicationChannel (refcount %d)", CCRefCount(pl->comm));
    }
    pl->thread = 0;
    RemoveCommChanRsrc (pl, CCRefCount(pl->comm));
    return 0;
}

/*____________________________________________________________________________*/
static int DetachCommChan (CommChansPtr pl)
{
    CommChansPtr list = gCCList;
    if (list == pl) {
        gCCList = pl->next;
        return 1;
    }
    while (list) {
        if (list->next == pl) {
            list->next = pl->next;
            return 1;
        }
        list = list->next;
    }
    return 0;
}

/*____________________________________________________________________________*/
cdeclAPI(void) CloseAllClientChannels(void)
{
    CommChansPtr pl = gCCList;
    while (pl) {
        CommChansPtr next = pl->next;
        if (pl->comm) CCClose (pl->comm);
        pl->comm = 0;
        if (pl->thread) msThreadDelete (pl->thread);
        pl->thread = 0;
        free (pl);
        pl = next;
    }
    gCCList = 0;
}

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
				LogWrite ("Client application \"%s\" (%d) dropped at time %ld", 
                    pub(appl, name), pub(appl, refNum), MidiGetTime());
				if (ac->filterh) msSharedMemClose(ac->filterh);
				MidiClose (pub(appl, refNum));
				FreeApplContext(ac);
			}
		}
	}
}

/*____________________________________________________________________________*/
void RemoveCommChanRsrc (CommChansPtr ccp, int dropclients)
{
    RTCommPtr rt;
	msServerContext * sc = ServerContext;
	
//	msMutexLock (sc->OCMutex);
    if (!DetachCommChan (ccp))
        LogWrite ("RemoveCommChanRsrc: DetachCommChan failed");
//    if (ccp->thread) msThreadDelete (ccp->thread);
//    if (ccp->rtthread) msThreadDelete (ccp->rtthread);
    ccp->thread = ccp->rtthread = 0;
	rt = CCGetInfos (ccp->comm);
    CCSetInfos (ccp->comm, 0);
    if (rt) DisposeMemory (rt);
    if (dropclients) DropClients (ccp->comm);
    if (ccp->comm) CloseCommunicationChannel (ccp->comm);
    ccp->comm = 0;
    DisposeMemory (ccp);
//	msMutexUnlock (sc->OCMutex);
}

/*____________________________________________________________________________*/
CommChansPtr GetCommChanRsrc (CommunicationChan cc)
{
    CommChansPtr pl = gCCList;
    while (pl) {
        if (pl->comm == cc) return pl;
        pl = pl->next;
    }
    return 0;
}

/*____________________________________________________________________________*/
void InitCommHandlers ()
{
	msServerContextPtr c = ServerContext;

    InitNetEventsHandler (c->netDTEvHandlerTbl, false);
    InitNetEventsHandler (c->netRTEvHandlerTbl, true);
    msStreamParseInitMthTbl (gParseMthTable);
    msStreamInitMthTbl (gStreamMthTable);
    gCCList = 0;
    atexit (CloseAllClientChannels);
}

/*____________________________________________________________________________*/
void NewClientChannel (CommunicationChan cc)
{
    msThreadPtr thread; CommChansPtr cl;
    
    cl = (CommChansPtr)malloc (sizeof(CommChans));
    if (!cl) {
        CCClose (cc);
        LogWrite ("NewClientChannel: memory allocation failed");
        return;
    }
    msStreamParseInit (&cl->parse, gParseMthTable, cl->rbuff, kReadBuffSize);
    msStreamInit (&cl->stream, gStreamMthTable);
    cl->next = gCCList;
    cl->comm = cc;
    /* init RT communication before creating the main appl thread */
	if (!RTCommInit (ServerContext, cc)) goto err;
    thread = msThreadCreate (CommHandlerProc, cl, kServerHighPriority);
	if (!thread) {
        LogWrite ("NewClientChannel: msThreadCreate failed");
        goto err;
    }
    cl->thread = thread;
    gCCList = cl;
	return;

err:
	CCClose (cc);
	free (cl);
}
