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
  grame@rd.grame.fr

*/

#ifndef __msSorter__
#define __msSorter__


/*______________________________________________________________*/
/*                   data structures  		     	  			*/
/*______________________________________________________________*/


/*--------------------- Time stamped event -------------------- */
typedef struct TDatedEv * TDatedEvPtr;
typedef struct TDatedEv {
  TDatedEvPtr  	link;		/* next event pointer (keep first) */
  unsigned long	date;		/* 32 bits time stamping           */
} TDatedEv;


/*--------------- Fifo of time stamped events ------------------ */

typedef struct TSFifo * TSFifoPtr;
typedef struct TSFifo {			/* events fifo structure    */
 	TDatedEvPtr	first;			/* first event (keep first) */
 	TDatedEvPtr	last;			/* last event               */
} TSFifo;


/*------------------- Table of 256 Fifo ------------------------ */

typedef struct TBuffer * TBufferPtr;
typedef struct TBuffer {			/* events fifo structure    */
 	TSFifo	fifo[256];			/* first event (keep first) */
} TBuffer;


/*----------------- Structure of a sorter level ----------- */

typedef struct TLevel * TLevelPtr;
typedef struct TLevel {		
  TLevelPtr			next;		/* pointer to the next level       	*/
  TSFifoPtr			fifo;		/* pointer to the fifo to resort  	*/
  TBufferPtr		pri;		/* pointer to the primary buffer   	*/
  TBufferPtr		alt;		/* pointer to the alternate buffer 	*/
  TBuffer			buf[2];		/* the actual buffers				*/
  unsigned char 	lev;		/* corresponding level             	*/
  unsigned char 	pos;		/* resort position                 	*/
} TLevel;


/*--------------------- Structure of a sorter ------------------*/

typedef struct TSorter * TSorterPtr;
typedef struct TSorter {	
  unsigned long sysDate;	/* current date of the system    */
  long			rSize;		/* amount of events to resort    */
  TLevel		level[4];	/* 0: date MSB .. 3: date LSB 	 */
  TSFifo		late;
} TSorter;

/*--------------------- the sorter interface -------------------*/
#ifdef __cplusplus
extern "C" {
#endif
		
void 		SorterInit		(TSorterPtr sb, long rs);
TDatedEvPtr	SorterClock		(TSorterPtr sb, TDatedEvPtr es);

#ifdef __cplusplus
}
#endif

#endif
