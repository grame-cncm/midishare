/*

  Copyright © Grame 2001
  Copyright © Mil Productions 2001

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
  grame@grame.fr

*/

#ifndef __LFLIFO__
#define __LFLIFO__

#if macintosh

typedef unsigned long	atomic_t;
#define sizeField(lf)		(lf)->count

#else

#include <asm/atomic.h>
#define sizeField(lf)		(lf)->count.counter

#endif

/*****************************************************************
                           DATA STRUCTURES
 *****************************************************************/
#ifdef __Pentium__
# define vtype volatile
#else
# define vtype 
#endif

typedef struct cell {
	struct cell* link;		/*+ next cell in the list +*/
						    /*+ any data here		  +*/
} cell;


typedef struct lifo {
	vtype cell* top;		/* top of the stack          */
	vtype unsigned long oc;	/* used to avoid ABA problem */
	vtype atomic_t	count;
#ifdef __POWERPC__
	long 	unused [5];		/* lifo size must be at least 32 bytes */
							/* to avoid livelock in ppc multiprocessor */
#endif
} lifo;



/****************************************************************
                          OPERATIONS
 ****************************************************************/
static inline void lfinit(lifo* lf)
{
	lf->top = 0;
	lf->oc = 0;
	sizeField(lf) = 0;
}

static inline cell* lfavail(lifo* lf) 			{ return (cell*)lf->top; }
static inline unsigned long lfsize (lifo * lf)	{ return sizeField(lf); }


#ifdef __Pentium__

#ifdef __SMP__
#define LOCK "lock ; "
#else
#define LOCK ""
#endif

static void lfpush (vtype lifo * lf, cell * cl)
{
	__asm__ __volatile__ (
		"# LFPUSH \n\t"
		"1:\t"
		"movl %2, (%1)         \n\t"
		LOCK "cmpxchg %1, (%0) \n\t"
		"jnz  1b               \n\t"
		:
		:"c" (lf), "d" (cl), "a" (lf->top)
		);	
	atomic_inc (&lf->count);
}

static cell* lfpop (vtype lifo * lf)
{
	cell*	v=0;
	__asm__ __volatile__ (
		"# LFPOP 			 \n\t"
		"testl %%eax, %%eax  \n\t"
		"jz    20f           \n\t"
		"10: \t"
		"movl  (%%eax), %%ebx \n\t"
		"movl  %%edx, %%ecx	  \n\t"
		"incl  %%ecx          \n\t"
		LOCK  "cmpxchg8b %1   \n\t"
		"jz	   20f		      \n\t"
		"testl %%eax, %%eax   \n\t"
		"jnz   10b            \n"
		"20: \t"
		:"=a" (v)
		:"m" (*lf), "a" (lf->top), "d" (lf->oc)
		:"ecx", "ebx" );
	atomic_dec (&lf->count);
	return v;
}

static inline short CAS (vtype void * addr, void * value, void * n) 
{
	short ret;
	__asm__ __volatile__ (
		"# CAS \n\t"
		LOCK "cmpxchg %2, (%1) \n\t"
		"jnz  1f               \n\t"
		"movw $1, %0           \n\t"
		"jmp 2f                \n\t"
		"1:                    \n\t"
		"movw $0, %0           \n\t"
		"2:                    \n\t"
		:"=a" (ret)
		:"c" (addr), "d" (n), "a" (value)
	);
	return ret;
}

#endif /* __Pentium__ */

#if macintosh
static inline void lfpush (register lifo * lf, register cell * cl) 
{
	cl->link = lf->top;
	lf->top = cl;
	lf->count++;
}

static inline cell* lfpop (register lifo * lf) 
{
	cell* v;
	v = lf->top;
	if (v) {
		lf->top = v->link;
		lf->count--;
	}
	return v;
}

static inline long CAS (register void ** addr, register void *value, register void * n) 
{
	long retCode = true;
	if (*addr == value)
		*addr = n;
	else retCode = false;
	return retCode;
}

#endif /* __Macintosh__ */

#endif
