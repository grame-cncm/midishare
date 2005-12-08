/*******************************************************************************
 * C H A M E L E O N    S. D. K.                                               *
 *******************************************************************************
 *  $Archive:: /Chameleon.sdk/system/midishare/rtems/lffifo.h                  $
 *     $Date: 2005/12/08 13:39:26 $
 * $Revision: 1.1.2.1 $
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
  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public 
  License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

  Grame Research Laboratory, 9, rue du Garet 69001 Lyon - France
  research@grame.fr
  
  modifications history:
   [13-09-99] DF - inlining fifoinit in fifoclear

*/

#ifndef __LFFIFO_H__
#define __LFFIFO_H__

#include "msTypes.h"
#include "msSync.h"
#include <bsp.h>


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
 fifoavail(fifo* ff)-> cell*	: return the head cell
 fifoclear(fifo* ff)-> cell*	: return the head cell and initialize
 fifosize (fifo* ff)-> n		: number of cells in fifo ff
 ------------------------------------------------------------------

 Warning : all operations expect non-null lifo and cell pointers.
 It is the caller responsability to check the arguments !
 *****************************************************************
 *****************************************************************/

/*****************************************************************
                           DATA STRUCTURES
 *****************************************************************/

typedef struct fifo {
	cell*			head;		/* the fifo head */
	cell*			tail;	    /* the fifo tail */
	unsigned long	count;		/* cells count   */
} fifo;


/****************************************************************
                          OPERATIONS
 ****************************************************************/

static __inline__ cell* fifoavail (fifo * ff) 
{
	return ff->head;
}

static __inline__ void fifoinit(fifo* ff)
{
	ff->count = 0;
	ff->head = 0;		
	ff->tail = (cell*)&ff->head;
}

static __inline__ unsigned long fifosize (fifo * ff) 
{
	return ff->count;
}

static __inline__ void fifoput (register fifo * ff, register cell * c) 
{
	ISR_Level level;
	_ISR_Disable(level);
	c->link = 0;
	ff->tail->link = c;
	ff->tail = c;
	ff->count++;
	_ISR_Enable(level);
}

static __inline__ cell* fifoget (register fifo * ff) 
{
	cell* c;
	ISR_Level level;
	_ISR_Disable(level);
	c = ff->head;
	if (c && c->link) {
		ff->head = c->link;
		ff->count--;
	}
	else fifoinit (ff);
	_ISR_Enable(level);
	return c;
}

static __inline__ cell* fifoclear (register fifo * ff) 
{
	cell* v;
	ISR_Level level;
	_ISR_Disable(level);
	v = ff->head;
	ff->head = 0;		
	ff->count = 0;
	ff->tail = (cell*)&ff->head;
	_ISR_Enable(level);
	return v;
}

#endif
