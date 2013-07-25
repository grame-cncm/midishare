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

#ifndef __UDPStreamToEvent__
#define __UDPStreamToEvent__

#ifdef MODULE
#include "MidiShareKernel.h"
#else
#include "MidiShare.h"
#endif

#ifdef WIN32
# ifndef inline
#	define inline	_inline
# endif
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
