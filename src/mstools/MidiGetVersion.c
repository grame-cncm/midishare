/*====================== A MIDI SHARE TOOL  (© GRAME 99) =====================

NAME
      MidiGetVersion -- a very simple MidiShare tool

SYNOPSIS
      MidiGetVersion

DESCRIPTION
      returns the MidiShare version number

===============================================================================*/
 
#include <stdio.h>
#include <MidiShare.h>

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
	fprintf (stdout, "%d\n", MidiGetVersion ());
	return 0;        
}

