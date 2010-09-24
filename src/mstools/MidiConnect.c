/*====================== A MIDI SHARE TOOL  (� GRAME 99) =====================

NAME
      MidiConnect -- a very simple MidiShare tool

SYNOPSIS
      MidiConnect <src> <dest> <state>

DESCRIPTION
      connect or disconnect 2 MidiShare client applications
      state should be in 0 or 1
      src and dest should be MidiShare client application names or reference numbers

===============================================================================*/
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MidiShare.h"

#define nil     0
#define true    1
#define false   0

//_______________________________________________________________________
void usage (char *name) 
{
	fprintf (stderr, "usage : %s <src> <dest> <state>\n", name);
	fprintf (stderr, "        connect or disconnect 2 MidiShare client applications\n");
	fprintf (stderr, "        src and dest should be MidiShare client application names or reference numbers\n");
	fprintf (stderr, "        state should be in 0, 1 or true, false\n");
	exit (1);
}


//_______________________________________________________________________
void ErrClient (char *appl, char *client)
{
	fprintf (stderr, "%s : %s is not a valid MidiShare client application name or reference number\n",
				appl, client);
	usage (appl);
}

//_______________________________________________________________________
void ErrState (char *appl, char *state)
{
	fprintf (stderr, "%s : %s is not a valid state\n", appl, state);
	usage (appl);
}

//_______________________________________________________________________
void CheckMidiShare (char *name) 
{
	if (!MidiShare ()) {
		fprintf (stderr, "%s : MidiShare is not installed\n", name);
		exit (1);
	}
}

//_______________________________________________________________________
Boolean CheckRef (int ref)
{
	return MidiGetName (ref) != 0;
}

//_______________________________________________________________________
Boolean CheckState (int state)
{
	return (state == 0) || (state == 1);
}

//_______________________________________________________________________
int GetAppl (MidiName name)
{
	int ref;
	if (sscanf (name, "%d", &ref)) return ref;
	return MidiGetNamedAppl (name);	
}

//_______________________________________________________________________
int GetState (char * state)
{
	int val;
	if (sscanf (state, "%d", &val)) return val;

	if ( !strcmp (state, "true") ||
		 !strcmp (state, "TRUE") ||
		 !strcmp (state, "True"))	return 1;

	if ( !strcmp (state, "false") ||
		 !strcmp (state, "FALSE") ||
		 !strcmp (state, "False"))	return 0;
	return -1;
}

//_______________________________________________________________________
int main (int argc, char *argv[])
{
	int src, dst, state;

	if (argc != 4) usage (argv[0]);
	CheckMidiShare (argv[0]);
	
	src= GetAppl (argv[1]);
	if (!CheckRef (src)) ErrClient (argv[0], argv[1]);

	dst= GetAppl (argv[2]);
	if (!CheckRef (dst)) ErrClient (argv[0], argv[2]);

	state = GetState (argv[3]);
	if (!CheckState (state)) ErrState (argv[0], argv[3]);

	MidiConnect(src, dst, state);
	return 0;        
}

