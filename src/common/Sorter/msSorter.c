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

#include "msSorter.h"
#include "msSync.h"


/*-------------------------------------------------------------------------*/

#ifdef __LittleEndian__
enum { kLev3, kLev2, kLev1, kLev0 };
#else
enum { kLev0, kLev1, kLev2, kLev3 };
#endif

typedef union {
	unsigned long date;
	unsigned char part[4];
} SorterDate;

#define Next(e)          e->link
#define SorterData(sb)   sb->sorter
#define SorterBuf(sb)    sb->buf
#define SorterAlt(g)     sb->alt


/*===========================================================================
  Internal macros
=========================================================================== */
#ifdef __LittleEndian__
  #define GetFifo(sorter, date, lev) (sorter->level[3-lev].buf[date.part[lev]])
#else
  #define GetFifo(sorter, date, lev) (sorter->level[lev].buf[date.part[lev]])
#endif

#define SWAP(level) { TbufferPtr tmp; tmp=level->alt; level->alt=level->buf; level->buf=tmp; }


/*===========================================================================
  Internal functions prototypes
  =========================================================================== */
  
static void 		FifoInit	( TfifoPtr fifo);
static void 		BufferInit	( TbufferPtr buffer);
static void 		LevelInit	( TsorterBlockPtr sb, TLevelPtr level, unsigned char index);
static void 		FifoPut		( TfifoPtr fifo, TDatedEvPtr event);
static TDatedEvPtr	BufferPut	( TbufferPtr buff, TDatedEvPtr event, unsigned char index);
static void 		Resort2		( TLevelPtr level, TfifoPtr fifo);
static void 		ResortAll	( TLevelPtr level, TfifoPtr fifo);
static void 		LevelClock	( TLevelPtr level, unsigned char date);

static void HandleInput	(TsorterPtr sorter, TfifoPtr fifo);
static void PutEvent 	(TsorterPtr sorter, TDatedEvPtr event);



/*===========================================================================
  External initialization functions implementation
  =========================================================================== */

/*-------------------------------------------------------------------------*/
/* - SorterInit - Initialize the sorter */
/*-------------------------------------------------------------------------*/
void  SorterInit (TsorterBlockPtr sb)
{
	unsigned char i;
	TsorterPtr sorter = &SorterData(sb);
	sorter->sysDate = 0;
	FifoInit(&sorter->ready);
	FifoInit(&sorter->input);
	for (i=0;i<4;i++) {
		LevelInit (sb, &sorter->level[i], i);
	}
}	

/*===========================================================================
  External functions implementation
  =========================================================================== */

/*-------------------------------------------------------------------------*/
/* - SorterRSize - setup the amount of events to resort at every clock     */
/*-------------------------------------------------------------------------*/
void SorterRSize (TsorterBlockPtr sb, short Rsize)
{
	SorterData(sb).rSize= Rsize;
}

/*-------------------------------------------------------------------------*/
/* - SorterGet - returns the ready events */
/*-------------------------------------------------------------------------*/
TDatedEvPtr SorterGet (TsorterBlockPtr sb)
{
	TsorterPtr sorter = &SorterData(sb);
	TfifoPtr fifo = &sorter->ready;
	TDatedEvPtr	ev = fifo->first;
	FifoInit (fifo);
	return ev;
}

/*-------------------------------------------------------------------------*/
/* - SorterPut - insert an event in the sorter */
/*-------------------------------------------------------------------------*/
void SorterPut (TsorterBlockPtr sb, TDatedEvPtr event)
{
	TsorterPtr sorter = &SorterData(sb);
	TfifoPtr fifo = (event->date > sorter->sysDate) ? &sorter->input : &sorter->ready;
	FifoPut (fifo, event);
}

/*-------------------------------------------------------------------------*/
/* - SorterClock - sort the events and update the list of ready events     */
/*-------------------------------------------------------------------------*/
void SorterClock (TsorterBlockPtr sb)
{
	TLevelPtr	lastLevel;
	TfifoPtr	fifo;
	SorterDate	date;
	TsorterPtr sorter = &SorterData(sb);

	HandleInput (sorter, &sorter->input);
	
	date.date = sorter->sysDate += 1;
	LevelClock(&sorter->level[0],date.part[kLev0]);
	LevelClock(&sorter->level[1],date.part[kLev1]);
	LevelClock(&sorter->level[2],date.part[kLev2]);

	lastLevel = &sorter->level[3];			/* last level processing  */
	if ( !date.part[kLev3] ) {				/* check if we need to invert the buffers */
		SWAP(lastLevel);
		lastLevel->fifo=lastLevel->buf;		/* initialize Fifo to buf[0] */
	}
	fifo = lastLevel->fifo++;				/* current fifo = next fifo */
	if ( fifo->first ) {					/* if not empty */
		Next(sorter->ready.last) = fifo->first;
		sorter->ready.last = fifo->last;	/* add current fifo to ready */
		fifo->first = 0;
		fifo->last = (TDatedEvPtr)fifo;
	}
}
		
/*===========================================================================
  Internal functions implementation
  =========================================================================== */
static void HandleInput (TsorterPtr sorter, TfifoPtr fifo)
{
	TDatedEvPtr next, ev = fifo->first;
	FifoInit (fifo);
	while (ev) {
		next = ev->link;
		PutEvent (sorter, ev);
		ev = next;
	}
}
	
/*-------------------------------------------------------------------------*/
static void PutEvent (TsorterPtr sorter, TDatedEvPtr event)
{
	if ( event->date > sorter->sysDate ) {
		SorterDate evDate,sysDate;
		evDate.date = event->date;
		sysDate.date = sorter->sysDate;
		if ( evDate.part[kLev0] > sysDate.part[kLev0] ) {
			FifoPut(&GetFifo(sorter,evDate,kLev0), event);
		} else if ( evDate.part[kLev1] > sysDate.part[kLev1] ) {
			FifoPut(&GetFifo(sorter,evDate,kLev1), event);
		} else if ( evDate.part[kLev2] > sysDate.part[kLev2] ) {
			FifoPut(&GetFifo(sorter,evDate,kLev2), event);
		} else {
			FifoPut(&GetFifo(sorter,evDate,kLev3), event);
		}
	}
	else FifoPut(&sorter->ready, event);
}

/*-------------------------------------------------------------------------*/
static void FifoInit (TfifoPtr fifo)
{
  fifo->first = 0;
  fifo->last = (TDatedEvPtr)fifo;
}

/*-------------------------------------------------------------------------*/
static void BufferInit (TbufferPtr buffer)
{
  short i = 256;

  while ( i-- ) FifoInit(buffer++);
}

/*-------------------------------------------------------------------------*/
static void LevelInit (TsorterBlockPtr sb, TLevelPtr level, unsigned char index)
{
#ifdef __LittleEndian__
	level->lev = 3-index;
#else
	level->lev = (unsigned char)index;
#endif
	level->buf = SorterBuf(sb)[index];

	/* level 0, corresponds to a date MSB - no alternate buffer */
	level->alt = index ? SorterAlt(sb)[index-1] : level->buf;

	/* level 3, corresponds to a date LSB - no next level */
	level->next = ( index == 3 ) ? 0 : level + 1;

	level->fifo = &level->buf[1];
	level->pos = 1;
	BufferInit(level->buf);
	BufferInit(level->alt);
}
	
/*-------------------------------------------------------------------------*/
static void FifoPut (TfifoPtr fifo, TDatedEvPtr event)
{
	Boolean done = false;
	event->link = 0;
	while (!done) {
		done = CompareAndSwap (&fifo->last->link, 0, event);
	}
	fifo->last = event;
}

/*-------------------------------------------------------------------------*/
static TDatedEvPtr BufferPut (TbufferPtr buff, TDatedEvPtr event, unsigned char index)
{
	TDatedEvPtr	nextEv;
	TfifoPtr	fifo;
	SorterDate	date;

	date.date = event->date;
	fifo = &buff[date.part[index]];
	Next(fifo->last) = event;
	fifo->last = event;
	nextEv = Next(event);
	Next(event) = 0;
	return nextEv;
}

/*-------------------------------------------------------------------------*/
static void Resort2 (TLevelPtr level, TfifoPtr fifo)
{
  TDatedEvPtr event;	

  if ( fifo->first ) {
    event = BufferPut(level->alt,fifo->first,level->lev);
    if ( event ) event = BufferPut(level->alt,event,level->lev);
    fifo->first = event;
    if ( !event ) fifo->last = (TDatedEvPtr)fifo;
  }
}

/*-------------------------------------------------------------------------*/
static void ResortAll (TLevelPtr level, TfifoPtr fifo)
{
  TDatedEvPtr event;	

  event = fifo->first;
  if ( event ) {
    do {
		event = BufferPut(level->alt,event,level->lev);
    } while ( event );
    fifo->first = 0;
    fifo->last = (TDatedEvPtr)fifo;
  }
}

/*-------------------------------------------------------------------------*/
static void LevelClock (TLevelPtr level, unsigned char date)
{
  if ( date != level->pos ) Resort2(level->next,level->fifo);
  else {
    ResortAll(level->next,level->fifo);
    if ( !level->pos ) SWAP(level);
    if ( !++level->pos ) level->fifo = level->alt;
    else level->fifo++;
  }
}
