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

#ifndef __UDPStreamToEvent__
#define __UDPStreamToEvent__

#ifdef MODULE
#include "MidiShareKernel.h"
#else
#include "MidiShare.h"
#endif

/*------------------------------------------------------------------------*/
typedef struct UDPStream  * UDPStreamPtr;
typedef MidiEvPtr   	 (* UDPParseMethodPtr)(UDPStreamPtr f, Byte c);

typedef UDPParseMethodPtr 	UDPParseMethodTbl[256];


/*------------------------------------------------------------------------*/
typedef struct UDPStream{
	 UDPParseMethodPtr * rcv;		/* parse methods table */
	 UDPParseMethodPtr   parse;		/* current parse state */
	 UDPParseMethodPtr   next;		/* next parse state    */
	 
	 MidiEvPtr ptrCur;    			/* current parsed event */
	 short     length;
	 short     index;				/* current event location write index */
	 
	 long *	   longDst;				/* parsed long value destination */
	 short *   shortDst;			/* parsed short value destination */
	 char      data[4];				/* buffer to parse short and long values */
} UDPStream;


/*------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

void UDPParseInit  (UDPStreamPtr f, UDPParseMethodTbl rcv);
void UDPParseReset (UDPStreamPtr f);
void UDPParseInitMthTbl (UDPParseMethodTbl tbl);

static inline MidiEvPtr UDPParseByte (UDPStreamPtr f, Byte c) { return (*f->parse)(f,c); }

#ifdef __cplusplus
}
#endif

#endif
