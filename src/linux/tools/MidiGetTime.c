/*====================== A MIDI SHARE TOOL  (� GRAME 99) =====================

NAME
      MidiGetTime -- a very simple MidiShare tool

SYNOPSIS
      MidiGetTime

DESCRIPTION
      returns the MidiShare current time in mls

===============================================================================*/
 
#include <stdio.h>
#include <stdlib.h>
#include "MidiShare.h"

//_______________________________________________________________________
static void usage (char *name) 
{
	fprintf (stderr, "usage : %s [options]\n", name);
	fprintf (stderr, "        %s gives the current MidiShare date\n");
	fprintf (stderr, "        option is: -h output in human readable form\n");
	exit (1);
}

//_______________________________________________________________________
static void checkOpt (int argc, char *argv[]) 
{
	if (strcmp ("-h", argv[1]))
		usage (argv[0]);
}

//_______________________________________________________________________
static void CheckMidiShare (char *name) 
{
	if (!MidiShare ()) {
		fprintf (stderr, "%s : MidiShare is not installed\n", name);
		exit (1);
	}
}

//_______________________________________________________________________
static void HTime (unsigned long time) 
{
	long mls, sec, min, hrs, days; 
	
	mls = time % 1000;
	time /= 1000;
	sec = time % 60;
	time /= 60;
	min = time % 60;
	time /= 60;
	hrs = time % 24;
	days = time / 24;
	fprintf (stdout, "%02ld days %02ld hrs %02ld min %02ld sec %03ld mls\n", 
		days, hrs, min, sec, mls);
}

//_______________________________________________________________________
int main (int argc, char *argv[])
{
	CheckMidiShare (argv[0]);
	if (argc > 2) usage (argv[0]);
	else if (argc == 2) {
		checkOpt(argc, argv);
		HTime (MidiGetTime());
	}
	else fprintf (stdout, "%ld\n", MidiGetTime ());
	return 0;        
}

