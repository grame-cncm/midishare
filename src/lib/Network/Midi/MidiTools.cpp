/*

  Copyright © Grame 2001
  Copyright © Mil Productions 2001

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
