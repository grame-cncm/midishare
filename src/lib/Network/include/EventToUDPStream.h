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
