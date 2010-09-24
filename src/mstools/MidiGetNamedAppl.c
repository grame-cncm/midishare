/*====================== A MIDI SHARE TOOL  (� GRAME 99) =====================

NAME
      MidiGetNamedAppl -- a very simple MidiShare tool

SYNOPSIS
      MidiGetNamedAppl <name>

DESCRIPTION
      returns the reference number of an application from its name

===============================================================================*/
 
#include <stdio.h>
#include <stdlib.h>
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
void usage (char *name) 
{
	fprintf (stderr, "usage : %s <name>\n", name);
	fprintf (stderr, "        returns the reference of number of an application from its name\n");
	fprintf (stderr, "        <name> should be a MidiShare client application name\n");
	exit (1);
}

//_______________________________________________________________________
void ErrName (char *appl, char *name)
{
	fprintf (stderr, "%s : %s is not a valid MidiShare client name\n", appl, name);
	usage (appl);
}

//_______________________________________________________________________
Boolean CheckRef (int ref)
{
	return MidiGetName (ref) != 0;
}

//_______________________________________________________________________
int main (int argc, char *argv[])
{
	int ref;
	
	if (argc != 2) usage (argv[0]);
	CheckMidiShare (argv[0]);

	ref = MidiGetNamedAppl (argv[1]);
	if (!CheckRef (ref)) ErrName (argv[0], argv[1]);
	fprintf (stdout, "%d\n", ref);
	return 0;        
}

