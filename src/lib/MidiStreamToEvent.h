/*
  MidiShare Project
  Copyright (C) Grame 1999

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
  
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
//typedef union TMidiFastEv {
//    MidiEvPtr std;
//    struct {
//        MidiEvPtr     link;        /* link to next event           */
//        unsigned long date;        /* event date (in ms)           */
//        long      	  common;
//        long      	  specific;
//    } * fast;
//} TMidiFastEv;

//#define Common(e)        (e).fast->common
//#define Specific(e)      (e).fast->specific


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
//		 long    fast;			// removed for 64 bits support
	 } common;

	 union {
		  char   data[4];       /* received datas    */
//		  long   fast;			// removed for 64 bits support
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

#ifdef WIN32
#define inline __inline
#endif
static inline MidiEvPtr MidiParseByte (StreamFifoPtr f, Byte c) { return (*f->parse)(f,c); }

#ifdef __cplusplus
}
#endif

#endif
