/*====================== A MIDI SHARE TOOL  (© GRAME 2003) =====================

NAME
      MidiGetDriverInfos -- a very simple MidiShare tool

SYNOPSIS
      MidiGetDriverInfos <refnum>

DESCRIPTION
       returns a driver information from is order number.
      <refnum> should be a valid driver reference number

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
	fprintf (stderr, "        returns driver information from its reference number\n");
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
    TDriverInfos infos = { 0 }; int ref;

	if (argc != 2) usage (argv[0]);
	CheckMidiShare (argv[0]);
    ref = GetRef (argv[1]);
    if (ref <  0) ErrRef (argv[0], argv[1]);
    if (MidiGetDriverInfos 	( ref, &infos)) {
        fprintf (stdout, "%s v%d.%02d: %d slots\n", infos.name, (int)infos.version/100,
        (int)infos.version%100, (int)infos.slots);
    }
    else {
       fprintf (stderr, "Can't get infos for driver %d\n", ref);
     }
	return 0;        
}

