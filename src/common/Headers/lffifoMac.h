/*

  Copyright © Grame 1999

  This library is free software; you can redistribute it and modify it under 
  the terms of the GNU Library General Public License as published by the 
  Free Software Foundation version 2 of the License, or any later version.

  This library is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public 
  License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

  Grame Research Laboratory, 9, rue du Garet 69001 Lyon - France
  grame@rd.grame.fr
  
  modifications history:
   [13-09-99] DF - inlining fifoinit in fifoclear

*/

#ifndef __LFFIFOMAC__
#define __LFFIFOMAC__

/*****************************************************************
                           DATA STRUCTURES
 *****************************************************************/

#ifdef __Macintosh__
# ifdef __POWERPC__
# include "lflifo.h"
# else
typedef struct fifo {
	cell*			head;		/* the fifo head */
	cell*			tail;	    /* the fifo tail */
	unsigned long	count;		/* cells count   */
} fifo;
# endif
#endif

/****************************************************************
                          OPERATIONS
 ****************************************************************/
# ifdef __POWERPC__

# else

static inline cell* fifoavail (fifo * ff) 
{
	return ff->head;
}

static inline void fifoinit(fifo* ff)
{
	ff->count = 0;
	ff->head = 0;		
	ff->tail = (cell*)&ff->head;
}

static inline unsigned long fifosize (fifo * ff) 
{
	return ff->count;
}

static inline void fifoput (register fifo * ff, register cell * c) 
{
	INT_OFF();
	c->link = 0;
	ff->tail->link = c;
	ff->tail = c;
	ff->count++;
	INT_ON();
}

static inline cell* fifoget (register fifo * ff) 
{
	cell* c;
	INT_OFF();
	c = ff->head;
	if (c && c->link) {
		ff->head = c->link;
		ff->count--;
	}
	else fifoinit (ff);
	INT_ON();
	return c;
}

static inline cell* fifoclear (register fifo * ff) 
{
	cell* v;
	INT_OFF();
	v = ff->head;
	ff->head = 0;		
	ff->count = 0;
	ff->tail = (cell*)&ff->head;
	INT_ON();
	return v;
}

# endif

#endif
