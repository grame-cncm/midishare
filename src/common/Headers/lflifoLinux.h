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

#ifndef __LFLIFOLINUX__
#define __LFLIFOLINUX__

#ifdef __Pentium__

#ifndef LOCK
# ifdef __SMP__
#  define LOCK "lock ; "
# else
#  define LOCK ""
# endif
#endif

static inline void lfpush (lifo * lf, cell * cl) 
{
	__asm__ __volatile__ (
		"# LFPUSH					\n\t"
		"pushl	%%ebx				\n\t"
		"pushl	%%ecx				\n\t"
		"movl 0(%%esi), %%eax		\n\t"
		"movl 4(%%esi), %%edx		\n"	
		"1:\t"
		"movl %%eax, %%ebx			\n\t"
		"incl %%ebx					\n\t"
		"movl %%edx, (%%ecx)		\n\t"
		LOCK "cmpxchg8b (%%esi)		\n\t"
		"jnz	1b					\n\t"
		"popl	%%ecx				\n\t"
		"popl	%%ebx				\n\t"
		:/* no output */
		:"S" (lf), "c" (cl)
		:"memory", "eax", "edx");
}

static inline cell* lfpop (lifo * lf) 
{
	cell*	v=0;
	__asm__ __volatile__ (
		"# LFPOP 					\n\t"
		"pushl	%%ebx				\n\t"
		"pushl	%%ecx				\n\t"
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
		"popl	%%ecx				\n\t"
		"popl	%%ebx				\n\t"
		:"=a" (v)
		:"S" (&lf->top)
		:"memory", "edx");
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

#endif
