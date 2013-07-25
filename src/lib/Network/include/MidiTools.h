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


#ifndef __MidiTools__
#define __MidiTools__

#include "MidiShare.h"

//________________________________________________________________________
class MidiTools
{
	public:
		static	MidiEvPtr Text2Event (const char *text);
		static	MidiEvPtr Text2Event (const unsigned char *text);
		static	void 	  Event2Text (MidiEvPtr e, char *text, short bufsize);
		static	void 	  Event2Text (MidiEvPtr e, unsigned char *text, short bufsize);

		static	void 	  Print (const char *s, short ref = 0)	{ MidiSendIm (ref, Text2Event (s)); }
		static	void 	  Print (const unsigned char *s, short ref = 0)	{ MidiSendIm (ref, Text2Event (s)); }
		static	void 	  Print (long val, short ref = 0);
		static	void 	  PrintH (long val, short ref = 0);
		static	void 	  PrintH (void *ptr, short ref = 0)	{ PrintH ((long)ptr, ref); }
};

#endif
