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
#include "msKernelPrefs.h"
#include "msServerInit.h"

/*____________________________________________________________________________*/
void init (int argc, char *argv[])
{
	msCmdLinePrefs args;
	msKernelPrefs * prefs;
	
	ReadArgs (&args, argc, argv);
	prefs = ReadPrefs (args.conffile ? args.conffile : 0);
	OpenLog (args.logfile ? args.logfile : prefs->logfile);
	AdjustPrefs (prefs, &args);
	LogPrefs (prefs);
}

/*____________________________________________________________________________*/
int main (int argc, char *argv[])
{
	TMSGlobalPublic * pubMem;
	OpenLog (0);
	init (argc, argv);
	pubMem = msServerInit (sizeof(TMSGlobalPublic), true);
	if (pubMem) {
		char msg[512]; int version;
		MidiShareSpecialInit (40000, pubMem);
		version = MidiGetVersion ();
		sprintf (msg, "MidiShare Server v.%d.%02d is running", version/100, version%100);
		LogWrite (msg);
		printf ("press return to quit\n");
		getc(stdin);
		msServerClose ();
	}
	else {
		fprintf (stderr, "Cannot launch MidiShare server: initialization failed!\n");
		return 1;
	}
	return 0;
}
