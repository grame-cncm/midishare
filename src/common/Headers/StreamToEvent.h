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


#ifndef __StreamToEvent__
#define __StreamToEvent__

#include "msDefs.h"

/*------------------------------------------------------------------------*/
/* constants defining values returned by msStreamGetEvent                */
#define kStreamNoError			 1 	/* no error 		*/
#define kStreamNoMoreData		 0	/* last event is partialy linearized */
#define kStreamInvalidHeader	-1	/* buffer header is invalid */
#define kStreamParseError		-2	/* a parsing error occured */
#define kStreamMemoryError		-3	/* unable to allocate a new MidiShare event */
#define kStreamInvalidParameter -4	/* invalid parameter */
#define kStreamUnknowParseMeth  -5	/* unknow event encountered */

/*------------------------------------------------------------------------*/
typedef struct msStreamBuffer  * msStreamBufferPtr;
typedef int   (* msStreamParseMethodPtr)(msStreamBufferPtr f, MidiEvPtr e);

typedef msStreamParseMethodPtr 	msStreamParseMethodTbl[256];

/*------------------------------------------------------------------------*/
typedef struct msStreamBuffer {
  	void * 			buff;         	/* the input buffer ptr	   */
	unsigned short  size;         	/* the buffer size	       */
	unsigned short  slen;         	/* the stream actual data length */
	unsigned short  len;         	/* the buffer actual data length */
	unsigned short  expected;       /* possible remaining expected data */
	msStreamParseMethodPtr * parse; /* parse methods table     */

  	void * 			loc;         	/* current read location   */
	unsigned short  read;         	/* currently read bytes count  */
	unsigned short  varLen;         /* current varlen event length */
	MidiEvPtr		curEv;			/* currently parsed event      */
} msStreamBuffer;

/*------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

#define	msStreamParseRewind(f) 	(f)->loc=(f)->buff; (f)->read=0

void	msStreamParseInitMthTbl (msStreamParseMethodTbl tbl);
void	msStreamParseInit  (msStreamBufferPtr f, msStreamParseMethodTbl methTbl,
							void * buffer, unsigned short size);
void	msStreamParseReset (msStreamBufferPtr f);
/*
	msStreamGetEvent returns a parsed MidiShare event or nil 
	in case of failure. At function exit, the retcode parameter
	contains an error code: kStreamNoError in case of succes
	kStreamNoMoreData if the last event is partially linearized
	in this case, the function should be called with a new buffer
	to complete the event.
	other error codes are mentionned above
*/
MidiEvPtr msStreamStartBuffer(msStreamBufferPtr f, int buflen, int * retcode);
MidiEvPtr msStreamGetEvent   (msStreamBufferPtr f, int * retcode);
int 	  msStreamGetSize    (msStreamBufferPtr f);
const char * msStreamGetErrorText    (int errcode);

#ifdef __cplusplus
}
#endif

#endif
