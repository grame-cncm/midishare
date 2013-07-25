/*
  MidiShare Project
  Copyright (C) Grame 2000

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <stdio.h>
#include "MidiShare.h"
#include "EventToMidiStream.h"
#include "MidiStreamToEvent.h"

LinearizeMthTbl gLinMethods;
ParseMethodTbl	gParseTbl;
Status2TypeTbl	gTypeTbl;

/*____________________________________________________________________*/
void print(MidiEvPtr e)
{
	printf("date: %ld\n", Date(e));
	printf("ev	type: %d\n", (int)EvType(e));
	printf("	chan: %d\n", (int)Chan(e));
	printf("	data 0: %d\n", (int)Data(e)[0]);
	printf("	data 1: %d\n", (int)Data(e)[1]);
}

/*____________________________________________________________________*/
int main()
{
	printf ("\nMidiShare stream to event functions test.\n");
	printf ("==========================================\n");
	
	if( MidiShare()) {
		StreamFifo f; MidiEvPtr e;
		short ref;
		ref = MidiOpen("toto");
		printf("application ref: %d\n", (int)ref);
		MidiStreamInitMthTbl (gLinMethods);
		MidiParseInitMthTbl	 (gParseTbl);
		MidiParseInitTypeTbl (gTypeTbl);
		MidiParseInit (&f, gParseTbl, gTypeTbl);
		e = MidiParseByte (&f, 177);		// ctrl change channel 1
		if (e) printf ("got unexpected event !\n");
		e = MidiParseByte (&f, 7);			// volume
		if (e) printf ("got unexpected event !\n");
		e = MidiParseByte (&f, 120);		// value
		if (!e) printf ("no event received !\n");
		else print(e);
	}
	else printf ("MidiShare is not installed !\n");
	printf ("\nEnd of test.\n");
	return 0;
}
