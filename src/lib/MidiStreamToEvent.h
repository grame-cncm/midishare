/*******************************************************************************
 * C H A M E L E O N    S. D. K.                                               *
 *******************************************************************************
 *  $Archive:: /Chameleon.sdk/system/midishare/lib/Stream/MidiStreamToEvent.h  $
 *     $Date: 2005/12/08 13:36:18 $
 * $Revision: 1.4.2.1 $
 *-----------------------------------------------------------------------------*
 * This file is part of the Chameleon Software Development Kit                 *
 *                                                                             *
 * Copyright (C) 2001 soundart                                                 *
 * www.soundart-hot.com                                                        *
 * codemaster@soundart-hot.com                                                 *
 ******************************************************************************/

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

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _MSC_VER
# define inline __inline
#else
# define inline __inline__
#endif

/*------------------------------------------------------------------------*/
typedef struct StreamFifo  * StreamFifoPtr;
typedef MidiEvPtr   	  (* ParseMethodPtr)(StreamFifoPtr f, char c);

typedef ParseMethodPtr 	ParseMethodTbl[256];
typedef BYTE 			Status2TypeTbl[256];

//__________________________________________________________________________
// status byte offset in the type table
#define offset(c) ((BYTE)c-(BYTE)0x80)
// type corresponding to a status byte
#define type(c, f) (f->typesTbl[offset(c)])

/*__________________________________________________________________________*/
/*      fields fast access structure            							*/
typedef union TMidiFastEv {
    MidiEvPtr std;
    struct {
        MidiEvPtr     link;        /* link to next event           */
        DWORD		  date;        /* event date (in ms)           */
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
	 BYTE * 	      typesTbl;
	 MidiEvPtr        ptrCur;    /* current sysex event  	*/
	 DWORD		      date;

	 union {
	 	struct {
		 	BYTE msType;
		 	BYTE refNum;
		 	BYTE port;
		 	BYTE chan;
		 } field;
		 long    fast;
	 } common;

	 union {
		  char   data[4];       /* received datas    */
		  long   fast;
	 } infos;
} StreamFifo;


/*------------------------------------------------------------------------*/
void	 	MidiParseError(StreamFifoPtr f);
void 		MidiParseInit (StreamFifoPtr f, ParseMethodTbl rcv, BYTE * typesTbl);
void 		MidiParseReset(StreamFifoPtr f);
void 		MidiParseInitMthTbl (ParseMethodTbl tbl);
void 		MidiParseInitTypeTbl(Status2TypeTbl table);

static inline MidiEvPtr MidiParseByte (StreamFifoPtr f, BYTE c) { return (*f->parse)(f,c); }

#ifdef __cplusplus
}
#endif

#endif
