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
  grame@rd.grame.fr

*/

#include "MidiPrint.h"
#include "MidiShare.h"


//____________________________________________________________________________________
void MidiPrintf (char *s)
{
	MidiEvPtr e = MidiNewEv( typeTextual);
	if( e) {
		while( *s)
			MidiAddField( e, *s++);
		MidiSendIm( 0, e);
	}
}

//____________________________________________________________________________________
void MidiPrintp (unsigned char *s)
{
	MidiEvPtr e = MidiNewEv( typeTextual);
	if( e) {
		short size = *s++;
		while(size--)
			MidiAddField( e, *s++);
		MidiSendIm( 0, e);
	}
}

//____________________________________________________________________________________
void MidiPrintl (long val)
{
	MidiEvPtr e = MidiNewEv(typeTempo);
	if (e){
		Tempo(e)= val;
		MidiSendIm (0, e);
	}
}

//____________________________________________________________________________________
void MidiPrintH (long val)
{
	char str[9], n=8;
	str[n--] = 0;
	while (n >= 0) {
		char c = val & 0x0f;
		str[n--] = (c > 9) ? 'a' + (c-10) : '0' + c;
		val >>= 4;
	}
	MidiPrintf(str);
}
