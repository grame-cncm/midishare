/*

  Copyright © Grame 2001

  Grame Research Laboratory, 9, rue du Garet 69001 Lyon - France
  grame@grame.fr

*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "MidiShare.h"
#include "server.h"

short gRefNum, gXmtPort=1, doneFlag;
char * gCurFile = 0;

#define ApplName	"msControlSignal"
#define kSpeed		500

//_______________________________________________________________________
static void FatalError (char * name, const char *msg) 
{
	fprintf (stderr, "Cannot launch %s: %s\n", name, msg);
	exit (1);
}

//_______________________________________________________________________
static void usage (const char *name) 
{
	fprintf (stderr, "usage : %s [options] [files]\n", name);
	fprintf (stderr, "        option: -p <port> dest. MidiShare port number\n");
	exit (1);
}

//_______________________________________________________________________
static int getvalue (int argc, char *argv[], int i)
{
	if (i < argc)
		return atoi(argv[i]);
	usage (argv[0]);
	return 0;
}

//_______________________________________________________________________
// returns the input files index or 0 when unspecified
static int getopts (int argc, char *argv[])
{
	int i = 1;
	char * arg = argv[i];
	if (*arg == '-') {
		if (!strcmp (arg, "-p")) {
			gXmtPort = getvalue (argc, argv, ++i);
			i++;
		}
		else usage (argv[0]);
	}
	return i;
}

//_______________________________________________________________________
static void Run (long date, short refNum, long a1,long a2,long a3)
{
	MidiEvPtr e = MidiNewEv (typeNote);
	if (e) {
		Dur(e)  = 100;
		Pitch(e)= 60;
		Vel(e)  = 100;
		Chan(e) = 0;
		Port(e) = gXmtPort;
		MidiSendIm (refNum, e);
		MidiTask (Run, date + kSpeed, refNum, a1, a2, a3);
	}
	else doneFlag = true;
}

//_______________________________________________________________________
void sendinfos (short ref, char * file)
{
}

//_______________________________________________________________________
main (int argc, char *argv[])
{
	int i;
	if (argc > 3) usage (argv[0]);
	else if (argc == 1) gXmtPort = 1;
	else getopts (argc, argv);

	if (!MidiShare()) FatalError (argv[0], "MidiShare is not installed");

	gRefNum = MidiOpen (ApplName);
	if (gRefNum < 0) FatalError (argv[0], "can't open a MidiShare client");
	
	printf ("Control signal is running on port %d\n", (int)gXmtPort);
	CnxInit (gRefNum);
	doneFlag = false;
	Run (MidiGetTime (), gRefNum, 0, 0, 0);
	while (!doneFlag) {
		sleep (10);
	}
	MidiClose (gRefNum);
	exit (0);
}
