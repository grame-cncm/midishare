/*

  Copyright © Grame 2002

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

*/

#ifndef __LFLIFOPPC__
#define __LFLIFOPPC__

#define inline __inline__


static inline void lfpush (register lifo * lf, register cell * cl) 
{
  register volatile long t1;
  register long t2=0;
  asm volatile (
	  "# LFPUSH \n"
	  "0: 				      \n"
	  "   lwarx   %0, %3, %1  \n"		
	  "   stw	  %0, 0(%2)   \n"	
	  "   sync  			  \n"	
	  "   stwcx.  %2, %3, %1  \n"						   
	  "   bne-    0b	      \n"  
	  "0:				      \n"
	  "   lwarx   %0, %3, %4  \n"		
	  "   addi    %0, %0, 1	  \n"  
	  "   sync  			  \n"  
	  "   stwcx.  %0, %3, %4  \n"
	  "   bne-    0b		  \n"
	  : "=r" (t1)
	  : "r" (&lf->top), "r" (cl), "r" (t2), "r" (&lf->oc), "0" (t1)
	  : "r0" 		/* prevents using r0 because of the ambiguity of 'addi' coding: */
	  				/* gcc version 2.95.3 20010315 (release - Linux-Mandrake 8.0 for PPC) */
					/* compiles the instruction "addi 0, 0, n" as li 0, n */
  );
}

static inline cell* lfpop (register lifo * lf) 
{
	register cell * result;
	register volatile long a, b;
	register long c=0;
	asm volatile (
       "# LFPOP					\n"
        "0:						\n"
		"	lwarx	%4, %1, %2	\n"         /* creates a reservation on lf    */
		"	cmpwi	%4, 0		\n"         /* test if the lifo is empty      */
		"	beq-	1f		\n"
		"	lwz		%5, 0(%4)	\n"         /* next cell in b                */
        "	sync            	\n"         /* synchronize instructions       */
		"	stwcx.	%5, %1, %2	\n"         /* if the reservation is not altered */
                                            /* modify lifo top                */
		"	bne-	0b  		\n"         /* otherwise: loop and try again  */
        "0:						\n"
		"	lwarx	%5, %1, %3	\n"         /* creates a reservation on lf->count */
        "	addi	%5, %5, -1	\n"         /* dec count                      */
		"	sync            	\n"         /* synchronize instructions       */
		"	stwcx.	%5, %1, %3	\n"         /* conditionnal store             */
		"	bne-	0b			\n"
        "1:						\n"
		"	mr		%0, %4		\n"
       :"=r" (result), "=r" (c)
	   : "r" (&lf->top), "r" (&lf->oc), "r" (a), "r" (b), "1" (c)
	   : "r0" 		/* prevents using r0 because of the ambiguity of 'addi' coding: */
	  				/* gcc version 2.95.3 20010315 (release - Linux-Mandrake 8.0 for PPC) */
					/* compiles the instruction "addi 0, 0, n" as li 0, n */
	);
	return result;
}

static inline unsigned long lfsize (lifo * lf) 
{
	return lf->oc;
}

#endif
