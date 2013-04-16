/*
  MidiShare Project
  Copyright (C) Grame 1999

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
  
*/


#ifndef __EventToMidiStream__
#define __EventToMidiStream__


#ifdef MODULE
#include "MidiShareKernel.h"
#else
#include "MidiShare.h"
#endif

/*------------------------------------------------------------------------*/
typedef struct Ev2StreamRec * Ev2StreamPtr;

typedef void	(*Continuation) (Ev2StreamPtr f);
typedef void   	(*LinearizeMthPtr)(MidiEvPtr e, Ev2StreamPtr f);
typedef LinearizeMthPtr LinearizeMthTbl[256];


typedef struct Ev2StreamRec {
   	Continuation 	cont;         	/* xmt continuation        	*/
    LinearizeMthPtr * lin;			/* linearization methods	*/
    MidiSEXPtr     	nextCell;     	/* current sysex part		*/
    Byte           	data[16];
    short           count;
    Byte          	runStat;      	/* running status           */
} Ev2StreamRec;

/*___________________________________*/

#ifdef __cplusplus
extern "C" {
#endif

void MidiStreamInitMthTbl 	(LinearizeMthTbl lin);
void MidiStreamInit 		(Ev2StreamPtr f, LinearizeMthTbl lin);
void MidiStreamReset 		(Ev2StreamPtr f);

MidiEvPtr 	MidiStreamPutEvent	(Ev2StreamPtr f, MidiEvPtr e);
Boolean		MidiStreamGetByte	(Ev2StreamPtr f, Byte *code);

#ifdef WIN32
#define inline __inline
#endif
static inline short MidiStreamCountByte (Ev2StreamPtr f) { return f->count;}

#ifdef __cplusplus
}
#endif

#endif
