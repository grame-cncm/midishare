/*
  Copyright � Grame 2002

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

#define kCommBuffSize	2048
#define kParseBuffSize	2048

typedef struct PipesList * PipesListPtr;
typedef struct PipesList{
    PipesListPtr 	next;
    CommunicationChan comm;
    msThreadPtr 	thread;
    msStreamBuffer 	parse;
    Ev2StreamRec	stream;
    char 			buff[kParseBuffSize];
}PipesList;

PipesListPtr gPList = 0;
msStreamParseMethodTbl 	gParseMthTable;
msStreamMthTbl			gStreamMthTable;

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
    char name[256]; short ref=0;
	
	MidiEvPtr reply = MidiNewEv (typeMidiOpenRes);
	if (!reply) { 
		LogWrite ("NetMidiOpen: MidiShare memory allocation failed");
		goto err;
	}
	msApplContextPtr context = NewApplContext();
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
		FreeApplContext(context);
		MidiClose (ref);
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
			break;
        case typeMidiClose:
			res = NetMidiClose (e, cc);
			break;
        case typeMidiConnect:
            MidiConnect (e->info.cnx.src, e->info.cnx.dst, CnxState(e));
           break;
        case typeMidiSetName:
            MidiSetName (RefNum(e), Event2Text (e, name, 256));
            break;
        case typeMidiSetInfo:
            MidiSetInfo (RefNum(e), (void *)e->info.longField);
            break;
        case typeMidiSetFilter:
            SetFilter (RefNum(e), e);
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
static Boolean SendEvent (MidiEvPtr e, PipesListPtr pl)
{
    Ev2StreamPtr stream = &pl->stream;
    long n; short len;

    msStreamStart (stream);
    if (!msStreamPutEvent (stream, e)) {
        do {
            len = msStreamSize(stream);
            n = CCWrite (pl->comm, pl->buff, len);
            if (n != len) goto failed;
        } while (!msStreamContEvent (stream));
    }
    else {
        len = msStreamSize(stream);
        n = CCWrite (pl->comm, pl->buff, len);
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
	PipesListPtr pl = (PipesListPtr)p;

fprintf (stderr, "New CommHandlerProc: pipes pair %lx id = %d\n", (long)pl->comm, (int)CCGetID(pl->comm));
    do {
        long n = CCRead (pl->comm, pl->buff, kCommBuffSize);
        if (n > 0) {
            int ret;
            MidiEvPtr reply, e = msStreamGetEvent (&pl->parse, &ret);
            if (e) {
				reply = EventHandlerProc(e, pl->comm);
				MidiFreeEv (e);
				if (reply && !SendEvent (reply, pl))
					break;
            }
            else if (ret != kStreamNoMoreData) {
                LogWrite ("msStreamGetEvent read error (%d)", ret);
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
    CloseCommunicationChannel (pl->comm);
    pl->comm = 0;
    pl->thread = 0;
    return 0;
}

/*____________________________________________________________________________*/
static void CloseOneClientChannel (PipesListPtr pl)
{
    if (pl->comm) CloseCommunicationChannel (pl->comm);
    if (pl->thread) msThreadDelete (pl->thread);
    free (pl);
}

/*____________________________________________________________________________*/
cdeclAPI(void) CloseAllClientChannels ()
{
    PipesListPtr pl = gPList;
    while (pl) {
        PipesListPtr next = pl->next;
        CloseOneClientChannel (pl);
        pl = next;
    }
    gPList = 0;
}

/*____________________________________________________________________________*/
void InitCommHandlers ()
{
    msStreamParseInitMthTbl (gParseMthTable);
    msStreamInitMthTbl (gStreamMthTable);
    gPList = 0;
    atexit (CloseAllClientChannels);
}

/*____________________________________________________________________________*/
void NewClientChannel (CommunicationChan cc)
{
    msThreadPtr thread; PipesListPtr pl;
    
    pl = (PipesListPtr)malloc (sizeof(PipesList));
    if (!pl) {
        CloseCommunicationChannel (cc);
        LogWrite ("NewClientChannel: PipesList memory allocation failed");
        return;
    }
    msStreamParseInit (&pl->parse, gParseMthTable, pl->buff, kParseBuffSize);
    msStreamInit (&pl->stream, gStreamMthTable, pl->buff, kParseBuffSize);
    pl->next = gPList;
    pl->comm = cc;
    thread = msThreadCreate (CommHandlerProc, pl, kServerHighPriority);
	if (!thread) {
        CloseCommunicationChannel (cc);
        free (pl);
        LogWrite ("NewClientChannel: msThreadCreate failed");
        return;
    }
    pl->thread = thread;
    gPList = pl;
}
