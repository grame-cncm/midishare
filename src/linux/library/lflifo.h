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

  modifications history:
   [08-09-99] DF - adaptation to the new memory management

*/

#ifndef __LFLIFO__
#define __LFLIFO__

/*===============================================================*
 *****************************************************************
 *===============================================================*
                       LOCK FREE LIFO STACK 

 Implements a lock-free shared LIFO stack made of a list of cells 
 linked together. A cell can be anything provided it starts with  
 a pointer available to link together the cells of the stack. 
 
 All the operations are based on the following schema :
 	a) READ CURRENT STACK STATE <top:ref>, 
 	b) COMPUTE NEXT STATE <top':ref+1> 
 	c) CMPXCHG8B <top:ref> with <top':ref+1>
 	d) loop until CMPXCHG8B succeed
 
 By using ref we can avoid the ABA problem.
 
                           OPERATIONS
 ------------------------------------------------------------------
 lfinit(lifo* lf) 				: Initialise lifo lf to empty

 lfpush(lifo* lf, cell* cl)		: push cell cl on top of lifo lf
 lfpop(lifo* lf)-> cell*		: pop top cell of lf
 lfsize(lifo* lf) -> n			: number of cells in lifo lf
 ------------------------------------------------------------------

 Warning : all operations expect non-null lifo and cell pointers.
 lf is a non-null pointer to a lifo stack, cl is a non-null pointer
 to a cell, fc is a non-null pointer to the first cell of a list 
 of cells.
 
 *===============================================================*
 *****************************************************************
 *===============================================================*/

#include "msTypes.h"

/*****************************************************************
                           DATA STRUCTURES
 *****************************************************************/
/*
typedef struct cell {
	struct cell*	link;
	// some data here
} cell;
*/

typedef struct lifo {
	volatile unsigned long	ic;		// input (push) count
	volatile 		  cell*	top;	// top of the stack
	volatile unsigned long	oc;		// output (pop) count
} lifo;



/****************************************************************
                          IMPLEMENTATION
 ****************************************************************/

#ifdef __SMP__
#define LOCK "lock ; "
#else
#define LOCK ""
#endif


static inline void lfinit(lifo* lf)
{
	lf->ic = 0;		
	lf->top = 0;
	lf->oc = 0;
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

#endif
