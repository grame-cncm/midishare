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

#ifndef __LFFIFO__
#define __LFFIFO__

#include "msTypes.h"
#include "msSync.h"

/*****************************************************************
 *****************************************************************
                       LOCK FREE FIFO STACK 

 Implements a lock-free shared FIFO stack made of a list of cells
 linked together. A cell can be anything provided it starts with
 a pointer available to link together the cells of the stack. 
   
                           OPERATIONS
 ------------------------------------------------------------------
 fifoinit(fifo* ff) 			: Initialise fifo ff to empty

 fifoput  (fifo* ff, cell* cl)	: append cell cl to fifo tail
 fifoget  (fifo* ff)-> cell*	: pop head cell of ff
 fifoclear(fifo* ff)-> cell*	: return the head cell and initialize
 fifosize (fifo* ff) -> n		: number of cells in fifo ff
 ------------------------------------------------------------------

 Warning : all operations expect non-null lifo and cell pointers.
 It is the caller responsability to check the arguments !
 *****************************************************************
 *****************************************************************/

/*****************************************************************
                           DATA STRUCTURES
 *****************************************************************/
#ifdef __Pentium__
# define vtype	volatile
#else
# define vtype
#endif

typedef struct fifo {
	vtype 		  cell*	head;		/* the fifo head */
	vtype unsigned long	count;		/* cells count   */
	vtype 		  cell*	tail;	    /* the fifo tail */
} fifo;


/****************************************************************
                          OPERATIONS
 ****************************************************************/
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

#ifdef __Macintosh__
# ifdef __POWERPC__
# else

static inline void fifoput (fifo * ff, cell * c) 
{
	INT_OFF();
	c->link = 0;
	ff->tail->link = c;
	ff->tail = c;
	ff->count++;
	INT_ON();
}

static inline cell* fifoget (fifo * ff) 
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

static inline cell* fifoclear (fifo * ff) 
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
#endif  /* __Macintosh__ */

#endif
