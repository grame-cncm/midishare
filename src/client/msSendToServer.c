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

#include <stdio.h>

#include "msCommChans.h"
#include "msExtern.h"
#include "msFunctions.h"

#include "msLibContext.h"

/*____________________________________________________________________________*/
static MidiEvPtr ReadFromServer (TMSGlobalPtr g)
{
    msLibContextPtr c = (msLibContextPtr)g->context;
    MidiEvPtr e = 0;
    while (!e) {
        long n = CCRead (c->cchan, c->std.buff, kCommBuffSize);
        if (n > 0) {
            int ret;
            e = msStreamGetEvent (&c->std.parse, &ret);
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
Boolean StdSend (MidiEvPtr e, TMSGlobalPtr g)
{
    msLibContextPtr c = (msLibContextPtr)g->context;
	Ev2StreamPtr stream = &c->std.stream;
    long n; short len;

    msStreamStart (stream);
    if (!msStreamPutEvent (stream, e)) {
        do {
            len = msStreamSize(stream);
            n = CCWrite (c->cchan, c->std.buff, len);
            if (n != len) goto failed;
        } while (!msStreamContEvent (stream));
    }
    else {
        len = msStreamSize(stream);
        n = CCWrite (c->cchan, c->std.buff, len);
        if (n != len) goto failed;
    }
    MidiFreeEv (e);
    return true;
failed:
    MidiFreeEv (e);
    fprintf (stderr, "CCWrite failed (%ld)\n", n);
    return false;
}

/*____________________________________________________________________________*/
Boolean SendToServer (MidiEvPtr e, TMSGlobalPtr g)
{
    msLibContextPtr c = (msLibContextPtr)g->context;
	return c->send(e, g);
}

/*____________________________________________________________________________*/
MidiEvPtr SendToServerSync (MidiEvPtr e, TMSGlobalPtr g)
{
    if (SendToServer (e, g)) {
        return ReadFromServer (g);
    }
    return 0;
}
