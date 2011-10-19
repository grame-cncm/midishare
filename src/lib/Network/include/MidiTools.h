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
