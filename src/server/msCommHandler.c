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
#include "EventToStream.h"
#include "StreamToEvent.h"

#include "msApplContext.h"
#include "msCommHandler.h"
#include "msLog.h"
#include "msServerInit.h"
#include "msSharedMem.h"
#include "msThreads.h"
#include "osglue.h"

#include "msRTListenProc.h"

#define kReadBuffSize	2048
#define kWriteBuffSize	2048

typedef struct CommChans * CommChansPtr;
typedef struct CommChans{
    CommChansPtr 	next;
    CommunicationChan comm;
    msThreadPtr 	thread;
    msThreadPtr 	rtthread;
    msStreamBuffer 	parse;
    Ev2StreamRec	stream;
    char 			rbuff[kReadBuffSize];
    char 			wbuff[kWriteBuffSize];
}CommChans;

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
static char * Event2Text (MidiEvPtr e, char *buff, short len)
{
	char * ptr = buff;
    long i, n = MidiCountFields (e);
    len -= 1;
    n = (n > len) ? len : n;
    for (i=0; i<n; i++) {
        *ptr++ = (char)MidiGetField (e, i);
    }
    *ptr = 0;
	return buff;
}

/*____________________________________________________________________________*/
static void SetFilter (short ref, MidiEvPtr e)
{
    ShMemID id; SharedMemHandler memh; void *ptr;
#ifdef WIN32
    char fid[keyMaxSize+1];
    Event2Text (e, fid, keyMaxSize+1);
    id = *fid ? fid : 0;
#else
    id = e->info.longField;
#endif
    if (id) {
        memh = msSharedMemOpen (id, &ptr);
        if (memh) {
            MidiSetFilter (ref, (MidiFilterPtr)ptr);
        }
        else LogWriteErr ("SetFilter: msSharedMemOpen failed");
    }
    else  MidiSetFilter (ref, 0);
}

/*____________________________________________________________________________*/
static MidiEvPtr NetMidiOpen (MidiEvPtr e, CommunicationChan cc)
{
    char name[256]; short ref=0; msApplContextPtr context=0;
	
	MidiEvPtr reply = MidiNewEv (typeMidiOpenRes);
	if (!reply) { 
		LogWrite ("NetMidiOpen: MidiShare memory allocation failed");
		goto err;
	}
	context = NewApplContext();
	if (!context) { 
		LogWrite ("NetMidiOpen: memory allocation failed\n");
		goto err;
	}
	Event2Text (e, name, 256);
	ref = MidiOpen (name);
	RefNum(reply) = (unsigned char)ref;
	if (ref > 0) {
		TApplPtr appl = GetAppl(ref);
		if (!appl) {
			LogWrite ("NetMidiOpen: unconsistent application state");
			goto err;
		}
		appl->netFlag = 1;	/* this flag indicates that this is a remote application */
							/* set to 0 by default, the application state is non-consistent */
							/* between the MidiOpen call and this step. It's however considered */
							/* that we can support this unconsistency as it only affects the way */
							/* tasks and alarms are handled by the kernel and none of them */
							/* can be scheduled before the call returned */
		context->chan = cc;
		context->filterh = 0;
		CCInc (cc);
		appl->context = context;
	}
	return reply;

err:
	if (context) FreeApplContext(context);
	if (reply) MidiFreeEv(reply);
	if (ref > 0) MidiClose (ref);
	return 0;
}

/*____________________________________________________________________________*/
static MidiEvPtr NetMidiClose (MidiEvPtr e, CommunicationChan cc)
{
	MidiEvPtr reply = 0;
	short ref = RefNum(e);
	msApplContextPtr context = ApplContext(ref);

	if (context && (context->chan == cc)) {
		if (context->filterh) msSharedMemClose(context->filterh);
		MidiClose (ref);
		FreeApplContext(context);
		CCDec (cc);
	}
	reply = MidiNewEv (typeMidiCommSync);
	if (!reply) {
		LogWrite ("NetMidiClose: MidiShare memory allocation failed");
	}
	return reply;
}

/*____________________________________________________________________________*/
static MidiEvPtr EventHandlerProc (MidiEvPtr e, CommunicationChan cc)
{
    MidiEvPtr res = 0; char name[256]; 

    if (EvType(e) >= typeReserved)
        goto unexpected;

    switch (EvType(e)) {
        case typeRcvAlarm:
        case typeApplAlarm:
        case typeMidiOpenRes:
            goto unexpected;

        case typeMidiOpen:
			res = NetMidiOpen (e, cc);
			MidiFreeEv(e);
			break;
        case typeMidiClose:
			res = NetMidiClose (e, cc);
			MidiFreeEv(e);
			break;
        case typeMidiConnect:
            MidiConnect (e->info.cnx.src, e->info.cnx.dst, CnxState(e));
			MidiFreeEv(e);
           break;
        case typeMidiSetName:
            MidiSetName (RefNum(e), Event2Text (e, name, 256));
			MidiFreeEv(e);
            break;
        case typeMidiSetInfo:
            MidiSetInfo (RefNum(e), (void *)e->info.longField);
			MidiFreeEv(e);
            break;
        case typeMidiSetFilter:
			SetFilter (RefNum(e), e);
			MidiFreeEv(e);
            break;
        default:
            MidiSend (RefNum(e), e);
    }
    return res;

unexpected:
    LogWrite ("EventHandlerProc: unexpected event type %d", EvType(e));
    return 0;
}

/*____________________________________________________________________________*/
static Boolean SendEvent (MidiEvPtr e, CommChansPtr pl)
{
    Ev2StreamPtr stream = &pl->stream;
    long n; short len;

    msStreamStart (stream);
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

fprintf (stderr, "New CommHandlerProc: pipes pair %lx id = %d\n", (long)pl->comm, (int)CCGetID(pl->comm));
    do {
        long n = CCRead (pl->comm, pl->rbuff, kReadBuffSize);
        if (n > 0) {
            int ret;  MidiEvPtr reply, e;
			e = msStreamStartBuffer (parse, n, &ret);
			if (e) {
				reply = EventHandlerProc(e, pl->comm);
				if (reply && !SendEvent (reply, pl))
					break;
			}
			else if (ret != kStreamNoMoreData) {
				msStreamParseReset (parse); /* only one event expected on the commands channel */
				LogWrite ("CommHandlerProc: msStreamGetEvent read error (%d)", ret);
				break;
			}
        }
        else if (n < 0) {
            LogWriteErr ("CommHandlerProc read error (%ld)", n);
            break;
        }
    } while (CCRefCount(pl->comm));
//	if (CCRefCount(pl->comm))
		LogWrite ("CommHandlerProc exit: CloseCommunicationChannel (refcount %d)", CCRefCount(pl->comm));
    CCClose (pl->comm);
    pl->comm = 0;
    pl->thread = 0;
    return 0;
}

/*____________________________________________________________________________*/
static void CloseOneClientChannel (CommChansPtr pl)
{
    if (pl->comm) CCClose (pl->comm);
    if (pl->thread) msThreadDelete (pl->thread);
    free (pl);
}

/*____________________________________________________________________________*/
cdeclAPI(void) CloseAllClientChannels(void)
{
    CommChansPtr pl = gCCList;
    while (pl) {
        CommChansPtr next = pl->next;
        CloseOneClientChannel (pl);
        pl = next;
    }
    gCCList = 0;
}

/*____________________________________________________________________________*/
void InitCommHandlers ()
{
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
        LogWrite ("NewClientChannel: CommChans memory allocation failed");
        return;
    }
    msStreamParseInit (&cl->parse, gParseMthTable, cl->rbuff, kReadBuffSize);
    msStreamInit (&cl->stream, gStreamMthTable, cl->wbuff, kWriteBuffSize);
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
//	if (!RTCommInit (ServerContext, cc)) goto err;
    gCCList = cl;
	return;

err:
	CCClose (cc);
	free (cl);
}
