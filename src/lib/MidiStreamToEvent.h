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


#ifndef __MidiStreamToEvent__
#define __MidiStreamToEvent__

#ifdef MODULE
#include "MidiShareKernel.h"
#else
#include "MidiShare.h"
#endif

/*------------------------------------------------------------------------*/
typedef struct StreamFifo  * StreamFifoPtr;
typedef MidiEvPtr   	  (* ParseMethodPtr)(StreamFifoPtr f, char c);

typedef ParseMethodPtr 	ParseMethodTbl[256];
typedef Byte 			Status2TypeTbl[256];

//__________________________________________________________________________
// status byte offset in the type table
#define offset(c) ((unsigned char)c-(unsigned char)0x80)
// type corresponding to a status byte
#define type(c, f) (f->typesTbl[offset(c)])

/*__________________________________________________________________________*/
/*      fields fast access structure            							*/
typedef union TMidiFastEv {
    MidiEvPtr std;
    struct {
        MidiEvPtr     link;        /* link to next event           */
        unsigned long date;        /* event date (in ms)           */
        long      	  common;
        long      	  specific;
    } * fast;
} TMidiFastEv;

#define Common(e)        (e).fast->common
#define Specific(e)      (e).fast->specific


/*------------------------------------------------------------------------*/
typedef struct StreamFifo{
	 ParseMethodPtr   parse;
	 ParseMethodPtr * rcv;
	 Byte * 	      typesTbl;
	 MidiEvPtr        ptrCur;    /* current sysex event  	*/
	 unsigned long    date;

	 union {
	 	struct {
		 	Byte msType;
		 	Byte refNum;
		 	Byte port;
		 	Byte chan;
		 } field;
		 long    fast;
	 } common;

	 union {
		  char   data[4];       /* received datas    */
		  long   fast;
	 } infos;
} StreamFifo;


/*------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

void	 	MidiParseError(StreamFifoPtr f);
void 		MidiParseInit (StreamFifoPtr f, ParseMethodTbl rcv, Byte * typesTbl);
void 		MidiParseReset(StreamFifoPtr f);
void 		MidiParseInitMthTbl (ParseMethodTbl tbl);
void 		MidiParseInitTypeTbl(Status2TypeTbl table);

static inline MidiEvPtr MidiParseByte (StreamFifoPtr f, Byte c) { return (*f->parse)(f,c); }

#ifdef __cplusplus
}
#endif

#endif
