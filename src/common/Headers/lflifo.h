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
} lifo;



/****************************************************************
                          OPERATIONS
 ****************************************************************/
static inline void lfinit(lifo* lf)
{
	lf->ic = 0;		
	lf->top = 0;
	lf->oc = 0;
}


#ifdef __Pentium__

#ifdef __SMP__
#define LOCK "lock ; "
#else
#define LOCK ""
#endif

static inline cell* lfavail(lifo* lf) { 
	return (cell*)lf->top;
}

static inline void lfpush (lifo * lf, cell * cl) 
{
	__asm__ __volatile__ (
		"# LFPUSH					\n\t"
		"pushl	%%ebx				\n\t"
		"movl 0(%%esi), %%eax		\n\t"
		"movl 4(%%esi), %%edx		\n"	
		"1:\t"
		"movl %%eax, %%ebx			\n\t"
		"incl %%ebx					\n\t"
		"movl %%edx, (%%ecx)		\n\t"
		LOCK "cmpxchg8b (%%esi)		\n\t"
		"jnz	1b					\n\t"
		"popl	%%ebx				\n\t"
		:/* no output */
		:"S" (lf), "c" (cl)
		:"memory", "eax", "ebx", "ecx", "edx");
}

static inline cell* lfpop (lifo * lf) 
{
	cell*	v=0;
	__asm__ __volatile__ (
		"# LFPOP 					\n\t"
		"pushl	%%ebx				\n\t"
		"movl 	4(%%esi), %%edx		\n\t"
		"movl  	(%%esi), %%eax		\n\t"	
		"testl	%%eax, %%eax		\n\t"
		"jz		20f					\n"
		"10:\t"
		"movl 	(%%eax), %%ebx		\n\t"
		"movl	%%edx, %%ecx		\n\t"
		"incl	%%ecx				\n\t"
		LOCK "cmpxchg8b (%%esi)		\n\t"
		"jz		20f					\n\t"
		"testl	%%eax, %%eax		\n\t"
		"jnz	10b					\n"
		"20:\t"
		"popl	%%ebx				\n\t"
		:"=a" (v)
		:"S" (&lf->top)
		:"memory", "ecx", "edx", "ebx" );
	return v;
}

static inline unsigned long lfsize (lifo * lf) 
{
	unsigned long n;
	__asm__ __volatile__ (
		"# LFSIZE					\n\t"
		"movl 	8(%%esi), %%edx		\n\t"
		"movl  	(%%esi), %%eax		\n\t"	
		"subl 	%%edx, %%eax		\n\t"
		:"=a" (n)
		:"S" (lf)
		:"memory", "edx");
	return n;
}
#endif /* __Pentium__ */

#ifdef __Macintosh__
# ifdef __POWERPC__
# else

static inline void lfpush (register lifo * lf, register cell * cl) 
{
	INT_OFF();
	cl->link = lf->top;
	lf->top = cl;
	lf->ic++;
	INT_ON();
}

static inline cell* lfpop (register lifo * lf) 
{
	cell* v;
	INT_OFF();
	v = lf->top;
	if (v) {
		lf->top = v->link;
		lf->oc++;
	}
	INT_ON();
	return v;
}

static inline unsigned long lfsize (lifo * lf) 
{
	unsigned long oc = lf->oc;
	unsigned long ic = lf->ic;
	return ic - oc;
}

# endif
#endif /* __Macintosh__ */

#endif
