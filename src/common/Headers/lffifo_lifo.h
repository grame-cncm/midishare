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
  grame@grame.fr
  
  modifications history:
   [13-09-99] DF - inlining fifoinit in fifoclear

*/

#ifndef __LFFIFO_LIFO__
#define __LFFIFO_LIFO__

#include "lflifo.h"

/*****************************************************************
                           DATA STRUCTURES
 *****************************************************************/
typedef struct fifo {
	lifo in;
	lifo out;	
} fifo;

/****************************************************************
                          OPERATIONS
 ****************************************************************/
static inline unsigned long fifosize (fifo * ff) 
{
	return lfsize(&ff->in) + lfsize(&ff->out);
}

static inline void fifoinit(fifo* ff)
{
	lfinit(&ff->in);		
	lfinit(&ff->out);	
}

static inline void fifoput (fifo * ff, cell * cl) 
{
	lfpush(&ff->in, cl);
}

static inline cell* fifoget (fifo * ff) 
{
	cell * v1, * v2;
	lifo * in = &ff->in;
	lifo * out = &ff->out;
	
	v1 = lfpop(out);
	
	if (!v1){
		v1 = lfpop(in);
		if (v1) {
			while ((v2 = lfpop(in))) { 
				lfpush(out, v1); 
				v1 = v2;
			}
		}
	}
	return v1;
}

static inline cell* fifoavail (fifo * ff) 
{
	cell * v1, * v2;
	lifo * in = &ff->in;
	lifo * out = &ff->out;
	
	v1 = lfavail(out);
	
	if (!v1){
		while ((v2 = lfpop(in))) { 
			v1 = v2;
			lfpush(out, v1); 
		}
	}
	return v1;
}


static inline cell* fifoclear (fifo * ff) 
{
	cell* next, *cur;
	cell* first = fifoget(ff);
	
	if (first==0) return 0;
	
	cur = first;
	
	while ((next = fifoget(ff))) {
		cur->link = next;
		cur = next;
	}
	cur->link = 0;
	
	fifoinit (ff);
	return first;
}

#endif
