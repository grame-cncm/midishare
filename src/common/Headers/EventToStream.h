/*

  Copyright © Grame 2002

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


#ifndef __EventToStream__
#define __EventToStream__

#include "msDefs.h"
#include "StreamDefs.h"

/*------------------------------------------------------------------------*/
typedef struct Ev2StreamRec * Ev2StreamPtr;

typedef Boolean   	(*msStreamMthPtr)(Ev2StreamPtr f, MidiEvPtr e);
typedef msStreamMthPtr msStreamMthTbl[256];

typedef struct Ev2StreamRec {
   	void * 			buff;         	/* the linearization buffer ptr	*/
	unsigned short  size;         	/* the buffer size	            */
    msStreamMthPtr * lin;			/* linearization methods	    */

	void *  		loc;         	/* the current write location   */
	msStreamMthPtr 	cont;         	/* linearization continuation   */
    MidiEvPtr     	nextCell;     	/* current continuation part	*/
	long			nextCount; 		/* nextCell remaining count to write */
	unsigned long	serial; 		/* current buffer serial number */
} Ev2StreamRec;

/*___________________________________*/

#ifdef __cplusplus
extern "C" {
#endif

void msStreamInitMthTbl (msStreamMthTbl lin);
void msStreamInit 		(Ev2StreamPtr f, msStreamMthTbl lin);
void msStreamReset 		(Ev2StreamPtr f);

/* msStreamStart starts to write a new buffer: write header informations */
void msStreamStart     (Ev2StreamPtr f, void *buffer, unsigned short size);

/*
   msStreamPutEvent writes the event e into the buffer, 
   returns true if the event fit into the buffer,
   otherwise returns false and the caller must call 
   msStreamContEvent until it returns true.
   The buffer content is supposed to be used between successive
   calls of msStreamStart and/or msStreamContEvent
*/
Boolean msStreamPutEvent  (Ev2StreamPtr f, MidiEvPtr e);

/* msStreamContEvent writes a new buffer with a previously uncompleted event */
Boolean msStreamContEvent (Ev2StreamPtr f);

#ifdef WIN32
#define inline __inline
#endif
static inline unsigned short msStreamSize  (Ev2StreamPtr f) { return StreamLength(f->buff); }

#ifdef __cplusplus
}
#endif

#endif
