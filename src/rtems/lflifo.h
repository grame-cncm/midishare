/*******************************************************************************
 * C H A M E L E O N    S. D. K.                                               *
 *******************************************************************************
 *  $Archive:: /Chameleon.sdk/system/midishare/rtems/lflifo.h                  $
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

*/

#ifndef __LFLIFO_H__
#define __LFLIFO_H__

#include "msTypes.h"
#include "msSync.h"
#include <bsp.h>


/*****************************************************************
 *****************************************************************
                       LOCK FREE LIFO STACK 

 Implements a lock-free shared LIFO stack made of a list of cells
 linked together. A cell can be anything provided it starts with
 a pointer available to link together the cells of the stack. 
 
 A LIFO stack <ic, [a b ...], oc> is made of 3 parts : an
 input counter 'ic' incremented every time a cell is added (lfpush)
 to the lifo stack, a linked list of cells [a b ...], and
 an output counter 'oc' incremented every time a cell is removed
 (lfpop) from the lifo stack. The 'lfpush' and 'lfpop' operations
 use 'CMPXCHG8B' to update the LIFO stack. The 'ic' and 'oc' 
 counters are used to avoid the ABA problem.
  
                           OPERATIONS
 ------------------------------------------------------------------
 lfinit(lifo* lf) 				: Initialise lifo lf to empty

 lfpush(lifo* lf, cell* cl)		: push cell cl on top of lifo lf
 lfpop(lifo* lf)-> cell*		: pop top cell of lf
 lfsize(lifo* lf) -> n			: number of cells in lifo lf
 ------------------------------------------------------------------

 Warning : all operations expect non-null lifo and cell pointers.
 It is the caller responsability to check the arguments !
 
 *****************************************************************
 *****************************************************************/



/*****************************************************************
                           DATA STRUCTURES
 *****************************************************************/
typedef struct lifo {
	unsigned long	ic;		/*+ input (push) count	+*/
			 cell*	top;	/*+ top of the stack	+*/
	unsigned long	oc;		/*+ output (pop) count	+*/
} lifo;



/****************************************************************
                          OPERATIONS
 ****************************************************************/
static __inline__ void lfinit(lifo* lf)
{
	lf->ic = 0;		
	lf->top = 0;
	lf->oc = 0;
}

static __inline__ cell* lfavail(lifo* lf) { 
	return (cell*)lf->top;
}

static __inline__ void lfpush (register lifo * lf, register cell * cl) 
{
	ISR_Level level;
	_ISR_Disable(level);
	cl->link = lf->top;
	lf->top = cl;
	lf->ic++;
	_ISR_Enable(level);
}

static __inline__ cell* lfpop (register lifo * lf) 
{
	cell* v;
	ISR_Level level;
	_ISR_Disable(level);
	v = lf->top;
	if (v) {
		lf->top = v->link;
		lf->oc++;
	}
	_ISR_Enable(level);
	return v;
}

static __inline__ unsigned long lfsize (lifo * lf) 
{
	unsigned long oc = lf->oc;
	unsigned long ic = lf->ic;
	return ic - oc;
}

#endif
