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

#include "msFunctions.h"
#include "msKernel.h"
#include "msKernelPrefs.h"
#include "msServerInit.h"
#include "msCommInit.h"

/*____________________________________________________________________________*/
static void MainClientServerProc (PipesPair p)
{
	char msg[512];
	sprintf (msg, "New contact: pipes id = %d\n", (int)PPID(p));
	LogWrite (msg);
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
	TMSGlobalPublic * pubMem;
	msKernelPrefs * prefs;
	
	OpenLog (0);
	prefs = init (argc, argv);
	LogPrefs (prefs);
	pubMem = msServerInit (sizeof(TMSGlobalPublic));
	if (pubMem) {
		int version;
		char msg[512];

		/* never call any MidiShare function before MidiShareSpecialInit */
		MidiShareSpecialInit (40000, pubMem);

		/* the main communication server proc needs an initialized kernel */
		if (ServerMainComInit (MainClientServerProc)) {
			version = MidiGetVersion();
			sprintf (msg, "MidiShare Server v.%d.%02d is running", version/100, version%100);
			LogWrite (msg);
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