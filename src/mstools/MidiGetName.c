/*====================== A MIDI SHARE TOOL  (© GRAME 99) =====================

NAME
      MidiGetName -- a very simple MidiShare tool

SYNOPSIS
      MidiGetName <refnum>

DESCRIPTION
      returns an application name from its reference number.
      <refnum> should be a valid reference number

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
void usage (char *name) 
{
	fprintf (stderr, "usage : %s <refnum>\n", name);
	fprintf (stderr, "        returns an application name from its reference number\n");
	fprintf (stderr, "        <refnum> should be a valid reference number\n");
	exit (1);
}

//_______________________________________________________________________
void ErrRef (char *appl, char *index)
{
	fprintf (stderr, "%s : %s is not a valid reference number\n", appl, index);
	usage (appl);
}

//_______________________________________________________________________
Boolean CheckRef (int ref)
{
	return MidiGetName (ref) != 0;
}

//_______________________________________________________________________
int GetRef (char * str)
{
	int ref;
	if (sscanf (str, "%d", &ref)) return ref;
	return -1;	
}

//_______________________________________________________________________
int main (int argc, char *argv[])
{
	int ref;
	
	if (argc != 2) usage (argv[0]);
	CheckMidiShare (argv[0]);
	
	ref = GetRef (argv[1]);
	if (!CheckRef (ref)) ErrRef (argv[0], argv[1]);
	fprintf (stdout, "%s\n", MidiGetName (ref));
	return 0;        
}

