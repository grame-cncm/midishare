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

*/

#ifndef __LFLIFO__
#define __LFLIFO__

#include "msTypes.h"
#include "msSync.h"


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
#ifdef __Pentium__
# define vtype volatile
#else
# define vtype 
#endif

typedef struct lifo {
	vtype unsigned long	ic;		/*+ input (push) count	+*/
	vtype 		  cell*	top;	/*+ top of the stack	+*/
	vtype unsigned long	oc;		/*+ output (pop) count	+*/
#ifdef __POWERPC__
	long 	unused [5];		/* lifo size must be at least 32 bytes */
							/* to avoid livelock in multiprocessor */
#endif
} lifo;

#ifdef __Windows__
# define inline __inline
#endif



/****************************************************************
                          OPERATIONS
 ****************************************************************/
static inline void lfinit(lifo* lf)
{
	lf->ic = 0;		
	lf->top = 0;
	lf->oc = 0;
}

static inline cell* lfavail(lifo* lf) { 
	return (cell*)lf->top;
}

#if defined(__Linux__)
# if defined(__ppc__)
#  include "lflifoppc.h"
# else
#  include "lflifoLinux.h"
# endif

#elif defined(__Macintosh__)
# if defined(__ppc__) && defined(__GNUC__)
#  include "lflifoppc.h"
# else
#  include "lflifoMac.h"
# endif

#elif defined(__Windows__)
# include "lflifoWin.h"
#endif

#endif
