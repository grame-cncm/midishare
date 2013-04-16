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

#ifndef __EventToUDPStream__
#define __EventToUDPStream__


#ifdef MODULE
#include "MidiShareKernel.h"
#else
#include "MidiShare.h"
#endif

#ifdef WIN32
# ifndef inline
#   define inline _inline
# endif
#endif

/*------------------------------------------------------------------------*/
typedef struct Ev2UDPStreamRec * Ev2UDPStreamPtr;

typedef void	(*Continuation) (Ev2UDPStreamPtr f);
typedef void   	(*UDPLinearizeMthPtr)(MidiEvPtr e, Ev2UDPStreamPtr f);
typedef UDPLinearizeMthPtr UDPLinearizeMthTbl[256];

enum { 	
	kDateSize = 2, kHeaderSize = 3, kLenSize = 2, 
	kCommonSize = kDateSize + kHeaderSize + kLenSize
};

typedef struct Ev2UDPStreamRec {
   	Continuation 	cont;         	/* xmt continuation        	*/
    UDPLinearizeMthPtr * lin;		/* linearization methods	*/
    MidiSEXPtr     	nextCell;     	/* current var length event part */
    Byte           	data[16+kCommonSize];
    short           count;
} Ev2UDPStreamRec;

/*___________________________________*/

#ifdef __cplusplus
extern "C" {
#endif

void UDPStreamInitMthTbl 	(UDPLinearizeMthTbl lin);
void UDPStreamInit 			(Ev2UDPStreamPtr f, UDPLinearizeMthTbl lin);
void UDPStreamReset 		(Ev2UDPStreamPtr f);

MidiEvPtr 	UDPStreamPutEvent	(Ev2UDPStreamPtr f, MidiEvPtr e);
Boolean		UDPStreamGetByte	(Ev2UDPStreamPtr f, Byte *code);

static inline short UDPStreamCountByte (Ev2UDPStreamPtr f) { return f->count;}

#ifdef __cplusplus
}
#endif

#endif
