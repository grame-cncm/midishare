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

#include "msFunctions.h"
#include "msKernel.h"
//#include "msKernelPrefs.h"
#include "msInit.h"
#include "msServerInit.h"
#include "msServerContext.h"
#include "msLog.h"
#include "msCommHandler.h"

//extern TMSGlobalPtr gMem;
//msKernelPrefs * gPrefs = 0;
static msServerContext gContext;

/*____________________________________________________________________________*/
static void MainClientServerProc (CommunicationChan cc)
{
    if (!gMem->running) MidiShareWakeup (gMem);
	NewClientChannel (cc);
}

/*____________________________________________________________________________*/
static void contextInit (msServerContextPtr c)
{
	c->sharedmem = 0;
	c->RTThread = 0;
	c->meetingPointThread = 0;
	c->meetingPoint = 0;
	c->prefs = 0;
	msStreamParseInitMthTbl (c->parseMthTable);
	msStreamInitMthTbl (c->streamMthTable);
	msStreamParseInit (&c->RT.parse, c->parseMthTable, c->RT.buff, kCommBuffSize);
	msStreamInit 	  (&c->RT.stream, c->streamMthTable, c->RT.buff, kCommBuffSize);
}

/*____________________________________________________________________________*/
static msKernelPrefs * init (int argc, char *argv[])
{
	msCmdLinePrefs args;
	msKernelPrefs * prefs;
	
	ReadArgs (&args, argc, argv);
	prefs = ReadPrefs (args.conffile ? args.conffile : 0, &args);
	if (prefs->logfile[0]) OpenLog (prefs->logfile);
	LogWrite ("MidiShare is starting...");
	CheckPrefs (prefs);
	return prefs;
}

/*____________________________________________________________________________*/
int main (int argc, char *argv[])
{
	contextInit (&gContext);
	gContext.prefs = init (argc, argv);
	LogPrefs (gContext.prefs);
	InitSignal (); 
	gContext.sharedmem = InitShMem (sizeof(TMSGlobalPublic));
	if (gContext.sharedmem) {
		/* never call any MidiShare function before MidiShareSpecialInit */
		MidiShareSpecialInit (40000, gContext.sharedmem);

		/* don't publish the server context before MidiShareSpecialInit: */
		/* the initialization proc set this context to nil */
        gMem->context = &gContext;

		/* the main communication server proc needs an initialized kernel */
        InitCommHandlers ();
        if (InitMeetingPoint (gMem, MainClientServerProc)) {
			int version = MidiGetVersion();
			LogWrite ("MidiShare Server v.%d.%02d is running", version/100, version%100);
			printf ("press return to quit\n");
			getc(stdin);
		}
		msServerClose ();
	}
	else {
		LogWrite ("Cannot launch MidiShare server: initialization failed!");
		return 1;
	}
	return 0;
}
