/*====================== A MIDI SHARE TOOL  (© GRAME 99) =====================

NAME
      MidiGetTime -- a very simple MidiShare tool

SYNOPSIS
      MidiGetTime

DESCRIPTION
      returns the MidiShare current time in mls

===============================================================================*/
 
#include <stdio.h>
#include "MidiShare.h"

//_______________________________________________________________________
void CheckMidiShare (char *name) 
{
	if (!MidiShare ()) {
		fprintf (stderr, "%s : MidiShare is not installed\n", name);
		exit (1);
	}
}

//_______________________________________________________________________
int main (int argc, char *argv[])
{
	CheckMidiShare (argv[0]);
	fprintf (stdout, "%ld\n", MidiGetTime ());
	return 0;        
}

