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
#include "msCommHandler.h"
#include "msServerInit.h"
#include "msThreads.h"
#include "msSharedMem.h"

#include "EventToStream.h"
#include "StreamToEvent.h"

#ifdef WIN32
#define windecl	__cdecl
#else
#define windecl
#endif

#define kCommBuffSize	2048
#define kParseBuffSize	2048

typedef struct PipesList * PipesListPtr;
typedef struct PipesList{
    PipesListPtr 	next;
    PipesPair		comm;
    msThreadPtr 	thread;
    msStreamBuffer 	parse;
    Ev2StreamRec	stream;
    char 			buff[kParseBuffSize];
}PipesList;

PipesListPtr gPList = 0;
msStreamParseMethodTbl 	gParseMthTable;
msStreamMthTbl			gStreamMthTable;

/*____________________________________________________________________________*/
static void Event2Text (MidiEvPtr e, char *buff, short len)
{
    long i, n = MidiCountFields (e);
    len -= 1;
    n = (n > len) ? len : n;
    for (i=0; i<n; i++) {
        *buff++ = (char)MidiGetField (e, i);
    }
    *buff = 0;
}

/*____________________________________________________________________________*/
static void SetFilter (short ref, MidiEvPtr e)
{
    ShMemID id; SharedMemHandler memh; FilterInfoPtr fsptr;
#ifdef WIN32
    char fid[20];
    Event2Text (e, fid, 20);
    id = *fid ? fid : 0;
#else
    id = e->info.longField;
#endif
    if (id) {
        memh = msSharedMemOpen (id, (void **)&fsptr);
        if (memh) {
            MidiSetFilter (ref, (MidiFilterPtr)++fsptr);
        }
        else  printf ("msSharedMemOpen failed\n");
    }
    else  MidiSetFilter (ref, 0);
}

/*____________________________________________________________________________*/
static MidiEvPtr EventHandlerProc (MidiEvPtr e, int *count)
{
    char msg[256]; MidiEvPtr res = 0;
    char name[256]; short ref;
    static char refmap[32];

    if (EvType(e) >= typeReserved)
        goto unexpected;

    switch (EvType(e)) {
        case typeRcvAlarm:
        case typeApplAlarm:
        case typeMidiOpenRes:
            goto unexpected;

        case typeMidiOpen:
            Event2Text (e, name, 256);
            ref = MidiOpen (name);
//    printf ("typeMidiOpen received: %s (ref:%d)\n", name, ref);
            if (ref > 0) {
                *count += 1;
                refmap[ref>>3] |= (1<<(ref&7));
            }
            MidiFreeEv (e);
            res = MidiNewEv (typeMidiOpenRes);
            if (res) RefNum(res) = (unsigned char)ref;
            else goto memfail;
            break;
        case typeMidiClose:
            ref = RefNum(e);
            MidiFreeEv (e);
            if (refmap[ref>>3] & (1<<(ref&7))) {
                MidiClose (ref);
                *count -= 1;
                refmap[ref>>3] &= ~(1<<(ref&7));
            }
//    printf ("typeMidiClose received (ref:%d)\n", ref);
            res = MidiNewEv (typeMidiCommSync);
            if (!res) goto memfail;
            break;
        case typeMidiConnect:
            MidiConnect (e->info.cnx.src, e->info.cnx.dst, CnxState(e));
            MidiFreeEv (e);
            res = MidiNewEv (typeMidiCommSync);
            if (!res) goto memfail;
           break;
        case typeMidiSetName:
            Event2Text (e, name, 256);
            MidiSetName (RefNum(e), name);
            MidiFreeEv (e);
            res = MidiNewEv (typeMidiCommSync);
            if (!res) goto memfail;
            break;
        case typeMidiSetInfo:
            MidiSetInfo (RefNum(e), (void *)e->info.longField);
            MidiFreeEv (e);
            res = MidiNewEv (typeMidiCommSync);
            if (!res) goto memfail;
            break;
        case typeMidiSetFilter:
            SetFilter (RefNum(e), e);
            MidiFreeEv (e);
            res = MidiNewEv (typeMidiCommSync);
            if (!res) goto memfail;
            break;
        default:
            MidiSend (RefNum(e), e);
    }
    return res;

memfail:
    LogWrite ("EventHandlerProc: MidiShare memory allocation failed\n");
    return 0;
unexpected:
    sprintf (msg, "EventHandlerProc: unexpected event type %d\n", EvType(e));
    LogWrite (msg);
    MidiFreeEv (e);
    return 0;
}

/*____________________________________________________________________________*/
static Boolean SendEvent (MidiEvPtr e, PipesListPtr pl)
{
    Ev2StreamPtr stream = &pl->stream;
    long n; short len; char msg[256];

    msStreamStart (stream);
    if (!msStreamPutEvent (stream, e)) {
        do {
            len = msStreamSize(stream);
            n = PPWrite (pl->comm, pl->buff, len);
            if (n != len) goto failed;
        } while (!msStreamContEvent (stream));
    }
    else {
        len = msStreamSize(stream);
        n = PPWrite (pl->comm, pl->buff, len);
        if (n != len) goto failed;
    }
    return true;
    
failed:
    sprintf (msg, "PPWrite failed (%ld)\n", n);
	LogWrite (msg);
    return false;
}

/*____________________________________________________________________________*/
static ThreadProc(PipeHandlerProc, p)
{
	PipesListPtr pl = (PipesListPtr)p;
    char msg[256]; int refcount = 0;
	sprintf (msg, "New PipeHandlerProc: pipes id = %d\n", (int)PPID(p));
	LogWrite (msg);

    do {
        long n = PPRead (pl->comm, pl->buff, kCommBuffSize);
        if (n > 0) {
            int ret;
            MidiEvPtr e = msStreamGetEvent (&pl->parse, &ret);
            if (e) {
                e = EventHandlerProc(e, &refcount);
                if (e && !SendEvent (e, pl))
                    break;
            }
            else if (ret != kStreamNoMoreData) {
                sprintf (msg, "msStreamGetEvent read error (%d)\n", ret);
                LogWrite (msg);
                break;
            }
        }
        else if (n < 0) {
            perror ("PPRead error");
            sprintf (msg, "PipeHandlerProc read error (%ld)\n", n);
            LogWrite (msg);
            break;
        }
    } while (refcount);
    fprintf (stderr, "PipeHandlerProc exit: ClosePipesPair (refcount %d)\n", refcount);
    ClosePipesPair (pl->comm);
    pl->comm = 0;
    pl->thread = 0;
    return 0;
}

/*____________________________________________________________________________*/
static void CloseOneClientChannel (PipesListPtr pl)
{
    if (pl->comm) ClosePipesPair (pl->comm);
    if (pl->thread) msThreadDelete (pl->thread);
    free (pl);
}

/*____________________________________________________________________________*/
void windecl CloseAllClientChannels (void)
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
void NewClientChannel (PipesPair * p)
{
    msThreadPtr thread; PipesListPtr pl;
    
    pl = (PipesListPtr)malloc (sizeof(PipesList));
    if (!pl) {
        ClosePipesPair (p);
        LogWrite ("NewClientChannel: PipesList memory allocation failed\n");
        return;
    }
    msStreamParseInit (&pl->parse, gParseMthTable, pl->buff, kParseBuffSize);
    msStreamInit (&pl->stream, gStreamMthTable, pl->buff, kParseBuffSize);
    pl->next = gPList;
    pl->comm = p;
    thread = msThreadCreate (PipeHandlerProc, pl, kServerHighPriority);
	if (!thread) {
        ClosePipesPair (p);
        free (pl);
        LogWrite ("NewClientChannel: msThreadCreate failed\n");
        return;
    }
    pl->thread = thread;
    gPList = pl;
}
