/*====================== A MIDI SHARE TOOL  (© GRAME 99) =====================

NAME
      MidiCountDrivers -- a very simple MidiShare tool

SYNOPSIS
      MidiCountDrivers

DESCRIPTION
      returns the MidiShare client drivers count

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
	fprintf (stdout, "%d\n", MidiCountDrivers ());
	return 0;        
}

