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

#ifdef __WIN16__
#define FAR far
#else
#define FAR 
#endif

/*_____________________________________________________________________*/
/* data structures                                                     */

typedef struct TDatedEv FAR *TDatedEvPtr;
typedef struct TDatedEv {		/* events common structure         */
	TDatedEvPtr		link;		/* next event pointer (keep first) */
	unsigned long	date;		/* 32 bits time stamping           */
} TDatedEv;

typedef struct Tfifo FAR *TfifoPtr;
typedef struct Tfifo {			/* events fifo structure    */
 	TDatedEvPtr	first;			/* first event (keep first) */
 	TDatedEvPtr	last;			/* last event               */
} Tfifo;

typedef Tfifo 	 Tbuffer[256];  /* a 256 fifos table */
typedef TfifoPtr TbufferPtr;

typedef struct TLevel FAR *TLevelPtr;
typedef struct TLevel {		/* Structure of a sorting level    */
	TLevelPtr	next;		/* pointer to the next level       */
	TfifoPtr	fifo;		/* pointer to the fifo to resort   */
	TbufferPtr	buf;		/* pointer to the main buffer      */
	TbufferPtr	alt;		/* pointer to the alternate buffer */
	unsigned char 	lev;	/* corresponding level             */
	unsigned char 	pos;	/* resort position                 */
} TLevel;

typedef struct Tsorter FAR *TsorterPtr;
typedef struct Tsorter {	/* Structure of an events sorter */
	unsigned long sysDate;	/* current date of the system    */
	TLevel		level[4];	/* level 0, the date MSB 	     */
							/* level 1, LSB of the MS word   */
							/* level 2, MSB of the LS word   */
							/* level 3, the date LSB         */
	Tfifo		ready;		/* events ready                  */
	Tfifo		input;		/* events to put in the sorter   */
	short		rSize;		/* amount of events to resortp   */
} Tsorter;

typedef struct TsorterBlock FAR *TsorterBlockPtr;
typedef struct TsorterBlock {
    Tsorter  sorter;            /* the sorter datas    */
    Tbuffer  buf[4];            /* its buffers         */
    Tbuffer  alt[3];            /* alternate buffers   */
} TsorterBlock;

				
#define SorterDate(sb)     (sb->sorter.sysDate)

/*_____________________________________________________________________*/
#ifdef __cplusplus
extern "C" {
#endif
		
void 		SorterInit	(TsorterBlockPtr sb);
void 		SorterRSize	(TsorterBlockPtr sb, short Rsize);
void 		SorterPut	(TsorterBlockPtr sb, TDatedEvPtr event);
void 		SorterClock	(TsorterBlockPtr sb);
TDatedEvPtr	SorterGet	(TsorterBlockPtr sb);

#ifdef __cplusplus
}
#endif

#endif
