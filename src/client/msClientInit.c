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
#include <stdlib.h>

#include "msFunctions.h"
#include "msExtern.h"

#include "osglue.h"
#include "msLibContext.h"
#include "msRTClientProc.h"


/*____________________________________________________________________________*/
static void DoCloseComm (void)
{
	msLibContextPtr c = LibContext;
	msThreadPtr rtThread = c->RTThread;
    CommunicationChan cc = c->cchan;
	c->RTThread = 0;
    c->cchan = 0;
    c->send  = NoCommSend;
	if (rtThread) msThreadDelete (rtThread);
    if (cc) CloseCommunicationChannel (cc);
}

/*____________________________________________________________________________*/
static cdeclAPI(void) msCommExit ()
{
	DoCloseComm ();
}

/*____________________________________________________________________________*/
static cdeclAPI(void) msShmExit ()
{
	msLibContextPtr c = LibContext;
	if (c->msMem) msSharedMemClose (c->msMem);
	c->msMem = 0;
}

/*____________________________________________________________________________*/
void CloseComm (TMSGlobalPtr g)
{
	msLibContextPtr c = (msLibContextPtr)g->context;
    if (!CCDec(c->cchan)) DoCloseComm ();
}

/*____________________________________________________________________________*/
Boolean InitComm (TMSGlobalPtr g)
{
	msLibContextPtr c = (msLibContextPtr)g->context;
    if (!c->cchan) {
		MeetingPointChan mp = OpenMeetingPoint ();
		if (!mp) return false;

        c->cchan = RequestCommunicationChannel (mp);
		CloseMeetingPoint (mp);
        if (!c->cchan) {
            DoCloseComm ();
            return false;
        }
		else {
			msStreamParseReset(&c->std.parse);
			msStreamReset(&c->std.stream);
            c->send  = StdSend;
			c->RTThread = msThreadCreate (RTClientProc, g, kClientRTPriority);
			if (!c->RTThread) {
				DoCloseComm ();
				return false;
			}
		}
		atexit (msCommExit);
    }
	CCInc (c->cchan);
    return true;
}

/*_________________________________________________________________________*/
Boolean _msCheckMidiShare (TMSGlobalPtr g)
{
	msLibContextPtr c = (msLibContextPtr)g->context;
	void * ptr;

	if (!c) return false;
	if (g->pub) return true;

	c->msMem = msSharedMemOpen (kShMemId, &ptr);
	if (c->msMem) {
		g->pub = ptr;
		atexit (msShmExit);
	}
	return g->pub ? true : false;
}
