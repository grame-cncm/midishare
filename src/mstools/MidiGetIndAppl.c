/*====================== A MIDI SHARE TOOL  (© GRAME 99) =====================

NAME
      MidiGetIndAppl -- a very simple MidiShare tool

SYNOPSIS
      MidiGetIndAppl <index>

DESCRIPTION
      returns the reference of number of an application from is order number.
      <index> should be a number between 1 and MidiCountAppls()

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
void usage (char *name) 
{
	fprintf (stderr, "usage : %s <index>\n", name);
	fprintf (stderr, "        returns the reference of number of an application from is order number\n");
	fprintf (stderr, "        <index> should be a number between 1 and MidiCountAppls\n");
	exit (1);
}

//_______________________________________________________________________
void ErrIndex (char *appl, char *index)
{
	fprintf (stderr, "%s : %s is not a valid index\n", appl, index);
	usage (appl);
}

//_______________________________________________________________________
Boolean CheckRef (int ref)
{
	return MidiGetName (ref) != 0;
}

//_______________________________________________________________________
int GetIndex (char * str)
{
	int index;
	if (sscanf (str, "%d", &index)) return index;
	return -1;	
}

//_______________________________________________________________________
int main (int argc, char *argv[])
{
	int ref;
	
	if (argc != 2) usage (argv[0]);
	CheckMidiShare (argv[0]);
	
	ref = MidiGetIndAppl (GetIndex (argv[1]));
	if (!CheckRef (ref)) ErrIndex (argv[0], argv[1]);
	fprintf (stdout, "%d\n", ref);
	return 0;        
}

