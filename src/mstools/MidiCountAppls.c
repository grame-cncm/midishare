/*====================== A MIDI SHARE TOOL  (© GRAME 99) =====================

NAME
      MidiCountAppls -- a very simple MidiShare tool

SYNOPSIS
      MidiCountAppls

DESCRIPTION
      returns the MidiShare client applications count

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
	fprintf (stdout, "%d\n", MidiCountAppls ());
	return 0;        
}

