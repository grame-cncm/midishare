/*====================== A MIDI SHARE TOOL  (© GRAME 2003) =====================

NAME
      MidiShare -- a very simple MidiShare tool

SYNOPSIS
      MidiShare

DESCRIPTION
      returns 0 if MidiShare is MidiShare is running otherwise returns 1

===============================================================================*/
 
#include <stdio.h>
#include <MidiShare.h>

//_______________________________________________________________________
int main (int argc, char *argv[])
{
	if (!MidiShare ()) {
		fprintf (stderr, "MidiShare is not installed\n");
		exit (1);
	}
	return 0;        
}

