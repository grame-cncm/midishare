/*

  Copyright © Grame 2000

  This library is free software; you can redistribute it and modify it under 
  the terms of the GNU Library General Public License as published by the 
  Free Software Foundation version 2 of the License, or any later version.

  This library is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public License 
  for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

  Grame Research Laboratory, 9, rue du Garet 69001 Lyon - France
  grame@grame.fr

*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "MidiShare.h"
#include "MidiBench.h"

int main(void)
{
	int version, i, ref, c;
	
	if (!MidiShare()) {
		printf ("MidiShare is not installed!\n\n");
		return 1;
	}
	version = MidiGetVersion ();
	printf ("MidiShare version %d.%02d\n", version/100, version%100);
	printf ("-------------------------------------------------------\n");
	printf ("Press enter to launch the benchs or 'q' to quit: ");
	c = getc(stdin);
	if ((c=='q') || (c=='Q')) {
		printf ("\n------------------ Benchs cancelled -------------------\n");
		return 0;
	}
	InitBenchLoop ();
	printf ("\n%d\t%ld\t%s\n", 1, DoBench(1), GetBenchInfos(1));
	ref = MidiOpen ("\pbenchs");
	for (i=2; i<=kBenchCount; i++) {
		printf ("%d\t%ld\t%s\n", i, DoBench(i), GetBenchInfos(i));
	}

	printf ("\nMIDI stream transmission and reception:\n");
	StartGen (5);
	printf ("1:\t%ld\t1/2 full MIDI stream\n", DoBench(1));
	StopGen ();

	StartGen (10);
	printf ("2:\t%ld\tfull MIDI stream\n", DoBench(1));
	StopGen ();

	MidiClose (ref);
	printf ("-------------------------------------------------------\n\n");
	return 0;
}

