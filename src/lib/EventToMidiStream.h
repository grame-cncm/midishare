/*

  Copyright © Grame 1999

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


#ifndef __EventToMidiStream__
#define __EventToMidiStream__

#include "MidiShare.h"

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

void MidiStreamInitMthTbl 	(LinearizeMthTbl lin);
void MidiStreamInit 		(Ev2StreamPtr f, LinearizeMthTbl lin);
void MidiStreamReset 		(Ev2StreamPtr f);

MidiEvPtr 	MidiStreamPutEvent	(Ev2StreamPtr f, MidiEvPtr e);
Boolean		MidiStreamGetByte	(Ev2StreamPtr f, Byte *code);

static inline short MidiStreamCountByte (Ev2StreamPtr f) { return f->count;}

#endif
