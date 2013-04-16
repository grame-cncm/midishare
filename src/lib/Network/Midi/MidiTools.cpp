/*
  MidiShare Project
  Copyright (C) Grame 2001
  Copyright (C) Mil Productions 2001

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/


#include "MidiTools.h"

//________________________________________________________________________
MidiEvPtr MidiTools::Text2Event (const char *text)
{
	MidiEvPtr e = MidiNewEv (typeTextual);
	if (e) {
		while (*text) MidiAddField (e, *text++);
	}
	return e;
}

//________________________________________________________________________
MidiEvPtr MidiTools::Text2Event (const unsigned char *text)
{
	MidiEvPtr e = MidiNewEv (typeTextual);
	if (e) {
		short n = *text++;
		while (n--) MidiAddField (e, *text++);
	}
	return e;
}

//________________________________________________________________________
void MidiTools::Event2Text (MidiEvPtr e, char *text, short bufsize)
{
	if (!e) return;
	long n = MidiCountFields (e);
	if (n > bufsize) n = bufsize;
	text[n] = 0;
	for (int i = 0; i < n; i++)
		*text++ = MidiGetField (e, i);
}

//________________________________________________________________________
void MidiTools::Event2Text (MidiEvPtr e, unsigned char *text, short bufsize)
{
	if (!e) return;
	long n = MidiCountFields (e);
	if (n > bufsize) n = bufsize;
	*text++ = n;
	for (int i = 0; i < n; i++)
		*text++ = MidiGetField (e, i);
}

//________________________________________________________________________
void MidiTools::Print (long val, short ref)
{
	MidiEvPtr e = MidiNewEv(typeTempo);
	if (e){
		Tempo(e)= val;
		MidiSendIm (ref, e);
	}
}

//________________________________________________________________________
void MidiTools::PrintH (long val, short ref)
{
	char str[9]; short n=8;
	str[n--] = 0;
	while (n >= 0) {
		char c = val & 0x0f;
		str[n--] = (c > 9) ? 'a' + (c-10) : '0' + c;
		val >>= 4;
	}
	Print(str);
}
