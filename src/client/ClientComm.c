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
  
  modifications history:

*/

#ifdef WIN32
#	include <windows.h>
#	include <stdio.h>
#define windecl	__cdecl
#else
#	include <stdio.h>
#	include <stdlib.h>
#define windecl
#endif

#include "msCommInit.h"
#include "msExtern.h"
#include "msFunctions.h"

#include "EventToStream.h"
#include "StreamToEvent.h"

#define kCommBuffSize	2048
#define kParseBuffSize	2048

typedef struct CommDesc * CommDescPtr;
typedef struct CommDesc {
    short			refCount;
    CInitHandler 	inith;
    PipesPair		comm;
    msStreamBuffer 	parse;
    Ev2StreamRec	stream;
    char 			buff[kParseBuffSize];
    msStreamParseMethodPtr 	* gParseMthTable;
    msStreamMthPtr			* gStreamMthTable;
}CommDesc;

CommDesc gComm = { 0 };


/*____________________________________________________________________________*/
void windecl DoCloseComm (void)
{
    if (gComm.comm) ClosePipesPair (gComm.comm);
    gComm.comm = 0;
    if (gComm.inith) EndClientInit (gComm.inith);
    gComm.inith = 0;
    if (gComm.gParseMthTable) free (gComm.gParseMthTable);
    gComm.gParseMthTable = 0;
    if (gComm.gStreamMthTable) free (gComm.gStreamMthTable);
    gComm.gStreamMthTable = 0;
}

/*____________________________________________________________________________*/
Boolean InitComm (TMSGlobalPtr g)
{
    if (!gComm.refCount) {
        gComm.gParseMthTable = (msStreamParseMethodPtr *)malloc (sizeof(msStreamParseMethodTbl));
        if (!gComm.gParseMthTable) return false;
        gComm.gStreamMthTable = (msStreamMthPtr *)malloc (sizeof(msStreamMthTbl));
        if (!gComm.gStreamMthTable) return false;
        
        msStreamParseInitMthTbl (gComm.gParseMthTable);
        msStreamInitMthTbl (gComm.gStreamMthTable);
        msStreamParseInit (&gComm.parse, gComm.gParseMthTable, gComm.buff, kParseBuffSize);
        msStreamInit (&gComm.stream, gComm.gStreamMthTable, gComm.buff, kParseBuffSize);
        gComm.inith = CreateClientInit ();
        if (!gComm.inith) return false;
        gComm.comm = StartClientInit (gComm.inith);
        if (!gComm.comm) {
            EndClientInit (gComm.inith);
            return false;
        }
        atexit (DoCloseComm);
    }
//    else printf ("InitComm: gComm.refCount %d\n", gComm.refCount);
    gComm.refCount++;
    return true;
}

/*____________________________________________________________________________*/
void CloseComm (TMSGlobalPtr g)
{
    if (gComm.refCount > 0) gComm.refCount--;
    if (!gComm.refCount)
        DoCloseComm ();
//    else printf ("CloseComm: gComm.refCount %d\n", gComm.refCount);
}

/*____________________________________________________________________________*/
static MidiEvPtr ReadFromServer ()
{
    MidiEvPtr e = 0;
    while (!e) {
        long n = PPRead (gComm.comm, gComm.buff, kCommBuffSize);
        if (n > 0) {
            int ret;
            e = msStreamGetEvent (&gComm.parse, &ret);
//fprintf (stderr, "ReadFromServer %ld: ev %lx type %d\n", n, (long)e, EvType(e));
            if (e) break;
            else if (ret != kStreamNoMoreData) {
                fprintf (stderr, "ReadFromServer read error (%d)\n", ret);
                break;
            }
        }
    }
    return e;
}

/*____________________________________________________________________________*/
Boolean SendToServer (MidiEvPtr e, TMSGlobalPtr g)
{
    Ev2StreamPtr stream = &gComm.stream;
    long n; short len;

//fprintf (stderr, "SendToServer %lx: type %d\n", (long)e, EvType(e));
    msStreamStart (stream);
    if (!msStreamPutEvent (stream, e)) {
        do {
            len = msStreamSize(stream);
            n = PPWrite (gComm.comm, gComm.buff, len);
            if (n != len) goto failed;
//fprintf (stderr, "SendToServer: %ld bytes written\n", n);
        } while (!msStreamContEvent (stream));
    }
    else {
        len = msStreamSize(stream);
        n = PPWrite (gComm.comm, gComm.buff, len);
        if (n != len) goto failed;
//fprintf (stderr, "SendToServer: %ld bytes written\n", n);
    }
    MidiFreeEv (e);
    return true;
failed:
    MidiFreeEv (e);
    fprintf (stderr, "PPWrite failed (%ld)\n", n);
    return false;
}

/*____________________________________________________________________________*/
MidiEvPtr SendToServerSync (MidiEvPtr e, TMSGlobalPtr g)
{
    if (SendToServer (e, g)) {
        return ReadFromServer ();
    }
    return 0;
}
