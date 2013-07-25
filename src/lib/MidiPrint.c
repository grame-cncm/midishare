/*
  MidiShare Project
  Copyright (C) Grame 2000

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "MidiPrint.h"
#ifdef MODULE
#include "MidiShareKernel.h"
#else
#include "MidiShare.h"
#endif

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
	char str[9]; short n=8;
	str[n--] = 0;
	while (n >= 0) {
		char c = val & 0x0f;
		str[n--] = (c > 9) ? 'a' + (c-10) : '0' + c;
		val >>= 4;
	}
	MidiPrintf(str);
}
