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

#include <stdlib.h>

#include "msFunctions.h"
#include "msExtern.h"

#include "msSharedMem.h"
#include "osglue.h"
#include "msCommChans.h"
#include "msSendToServer.h"

static SharedMemHandler 	gShMem = 0;
StreamDesc 			gStream = { { 0 } };
CommunicationChan 	gComm = 0;

/*____________________________________________________________________________*/
static void DoCloseComm (void)
{
printf ("DoCloseComm\n");
    if (gComm) CloseCommunicationChannel (gComm);
    gComm = 0;
    if (gStream.gParseMthTable) free (gStream.gParseMthTable);
    gStream.gParseMthTable = 0;
    if (gStream.gStreamMthTable) free (gStream.gStreamMthTable);
    gStream.gStreamMthTable = 0;
}

/*____________________________________________________________________________*/
static cdeclAPI(void) msCommExit ()
{
	DoCloseComm ();
}

/*____________________________________________________________________________*/
static cdeclAPI(void) msShmExit ()
{
	if (gShMem) msSharedMemClose (gShMem);
	gShMem = 0;
}

/*____________________________________________________________________________*/
void CloseComm (TMSGlobalPtr g)
{
    if (!CCDec(gComm)) DoCloseComm ();
}

/*____________________________________________________________________________*/
Boolean InitComm (TMSGlobalPtr g)
{
    if (!gComm) {
		MeetingPointChan mp = OpenMeetingPoint ();
		if (!mp) return false;

		gStream.gParseMthTable = (msStreamParseMethodPtr *)malloc (sizeof(msStreamParseMethodTbl));
        if (!gStream.gParseMthTable) return false;
        gStream.gStreamMthTable = (msStreamMthPtr *)malloc (sizeof(msStreamMthTbl));
        if (!gStream.gStreamMthTable) return false;

        msStreamParseInitMthTbl (gStream.gParseMthTable);
        msStreamInitMthTbl (gStream.gStreamMthTable);
        msStreamParseInit (&gStream.parse, gStream.gParseMthTable, gStream.buff, kParseBuffSize);
        msStreamInit (&gStream.stream, gStream.gStreamMthTable, gStream.buff, kParseBuffSize);

        gComm = RequestCommunicationChannel (mp);
		CloseMeetingPoint (mp);
        if (!gComm) {
            DoCloseComm ();
            return false;
        }
		atexit (msCommExit);
    }
	else CCInc (gComm);
    return true;
}

/*_________________________________________________________________________*/
Boolean CheckMidiShare (TMSGlobalPtr g)
{
	void * ptr;
	if (g->pub) return true;
	gShMem = msSharedMemOpen (kShMemId, &ptr);
	if (gShMem) {
		g->pub = ptr;
		atexit (msShmExit);
	}
	return g->pub ? true : false;
}
