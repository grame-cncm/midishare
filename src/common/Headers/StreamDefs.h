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


#ifndef __StreamDefs__
#define __StreamDefs__

#include "msTypes.h"

/*------------------------------------------------------------------------*/
/* storage of values common to all the events typology                    */
typedef struct EvCommonPart {
	unsigned long date;
	union {
		struct {
			Byte        evType;         /* event type              */
			Byte        refNum;         /* sender reference number */
			Byte        port;           /* Midi port               */
			Byte        chan;           /* Midi channel            */
		} common;
		long	lcommon;	  /* fast access to common information */
	}u;
} EvCommonPart, * EvCommonPartPtr;

/*------------------------------------------------------------------------*/
/* header of all the midishare buffers                                    */
typedef struct msStreamHeader {
	unsigned long 	serial;  /* buffer serial number                            */
	short 			magic;   /* intended to discriminate non conformant buffers */
	unsigned short 	len;     /* total length of the buffer                      */
	unsigned short 	cont;    /* optionnal event continuation length             */
} msStreamHeader, * msStreamHeaderPtr;

/*------------------------------------------------------------------------*/
typedef struct msStreamVarLenData {  /* var len events structure    */
	long	len;                     /* event length in cells count */
	Byte	data[1];                 /* data chunk begins here      */
} msStreamVarLenData, * msStreamVarLenDataPtr;

/*------------------------------------------------------------------------*/
/* some macros                                                            */
#define kStreamMagic			0xbe1
#define kMinStreamBufferSize	sizeof(msStreamHeader)+sizeof(EvCommonPart)+16
#define VLCellSize 		(sizeof(long) * 3) /* data size of var len events cells */

#define EventCommonPart(e)		(EvCommonPartPtr)(&(e)->date)
#define EventData(e)	        (e)->info.data

#define StreamLength(buff)		((msStreamHeaderPtr)(buff))->len
#define StreamContLength(buff)	((msStreamHeaderPtr)(buff))->cont

#endif
