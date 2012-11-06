/*

  Copyright � Grame 2000

  This program is free software; you can redistribute it and modify it under 
  the terms of the GNU General Public License as published by the 
  Free Software Foundation version 2 of the License.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for 
  more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

  Grame Research Laboratory, 9, rue du Garet 69001 Lyon - France
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
