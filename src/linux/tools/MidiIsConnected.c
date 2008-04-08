/*====================== A MIDI SHARE TOOL  (� GRAME 99) =====================

NAME
      MidiIsConnected -- a very simple MidiShare tool

SYNOPSIS
      MidiConnect <src> <dest>

DESCRIPTION
      returns the connection state of 2 MidiShare client applications
      src and dest should be MidiShare client application names or reference numbers

===============================================================================*/
 
#include <stdio.h>
#include <stdlib.h>
#include "MidiShare.h"

#define nil     0
#define true    1
#define false   0

//_______________________________________________________________________
void usage (char *name) 
{
	fprintf (stderr, "usage : %s <src> <dest>\n", name);
	fprintf (stderr, "        returns the connection state of 2 MidiShare client applications\n");
	fprintf (stderr, "        src and dest should be MidiShare client application names or reference numbers\n");
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
		 !strcmp (state, "True"));	return 1;

	if ( !strcmp (state, "false") ||
		 !strcmp (state, "FALSE") ||
		 !strcmp (state, "False"));	return 0;
	return -1;
}

//_______________________________________________________________________
int main (int argc, char *argv[])
{
	int src, dst, state;
	if (argc != 3) usage (argv[0]);
	
	CheckMidiShare (argv[0]);
	
	src= GetAppl (argv[1]);
	if (!CheckRef (src)) ErrClient (argv[0], argv[1]);

	dst= GetAppl (argv[2]);
	if (!CheckRef (dst)) ErrClient (argv[0], argv[2]);

	fprintf (stdout, "%d\n", MidiIsConnected(src, dst) ? 1 : 0);
	return 0;        
}

