/*

  Copyright © Grame 2001

  Grame Research Laboratory, 9, rue du Garet 69001 Lyon - France
  grame@grame.fr

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "MidiShare.h"
#include "TInetAddress.h"
#include "server.h"

static short		gSrc;
static TInetAddress	gAddr;

extern char *		gCurFile;

//_______________________________________________________________________
static void rcvalarm (short ref) 
{
	MidiFlushEvs (ref);
}

//_______________________________________________________________________
static Boolean connect (short ref)
{
	MidiName name = MidiGetName(ref);
	if (!name) return false;
	if (gAddr.SetAddress (name)) {
		MidiConnect (gSrc, ref, true);
		return true;
	}
	if (strlen(name) >= 31) {
		MidiConnect (gSrc, ref, true);
		return true;
	}
	return false;
}

//_______________________________________________________________________
static void start ()
{
	int n = MidiCountAppls ();
	for (int i = 1; i<=n; i++) {
		short ref = MidiGetIndAppl (i);
		if (ref > 0)
			connect (ref);
	}
}

//_______________________________________________________________________
static void applalarm (short refnum, long code)
{
	short changeCode = code & 0xffff;
	short srcRef = code >> 16;
	switch (changeCode) {
		case MIDIOpenAppl:
			if (connect (srcRef) && gCurFile) {
				sendinfos (gSrc, gCurFile);
			}
			break;
		default:
			break;
	}
}

//_______________________________________________________________________
void CnxInit (short ref)
{	
	gSrc = ref;
	MidiSetRcvAlarm (ref, rcvalarm);
	MidiSetApplAlarm (ref, applalarm);
	start ();
}
