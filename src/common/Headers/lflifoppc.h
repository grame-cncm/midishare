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
  grame@rd.grame.fr

*/

#ifndef __LFLIFOPPC__
#define __LFLIFOPPC__

#define inline __inline__

static /*inline*/ void lfpush (register lifo * lf, register cell * cl) 
{
	asm (
		"# LFPUSH \n"
		"	addi	%0, %0, 4    \n"
        ".loop:               	 \n"
		"	lwarx	r9, 0, %0    \n"       
		"	stw	    r9, 0(%1)    \n"   
		"	sync                 \n"   
		"	stwcx.	%1, 0, %0    \n"  	                     
		"	bne-	.loop     	 \n"  
		"	addi	%0, %0, 4    \n"
	    ".inc:                    \n"
		"	lwarx	r9, 0, %0    \n"   
        "	addi	r9, r9, 1    \n"  
		"	sync                 \n"  
		"	stwcx.	r9, 0, %0    \n"
		"	bne-	.inc          \n"
        :  : "r" (lf), "r" (cl) : "r9", "r0"
	);
}

static /*inline*/ cell* lfpop (register lifo * lf) 
{
	register cell * result;
	asm (
        "# LFPOP				\n"
        "	addi	%1, %1, 4	\n"
        "poploop:				\n"
		"	lwarx	r9, 0, %1	\n"         /* creates a reservation on lf    */
		"	cmpwi	r9, 0		\n"         /* test if the lifo is empty      */
		"	beq-	empty		\n"
		"	lwz		r10, 0(r9)	\n"         /* next cell in b                */
        "	sync            	\n"         /* synchronize instructions       */
		"	stwcx.	r10, 0, %1	\n"         /* if the reservation is not altered */
                                            /* modify lifo top                */
		"	bne-	poploop  	\n"         /* otherwise: loop and try again  */
		"	addi	%1, %1, 4	\n"
        "dec:					\n"
		"	lwarx	r10, 0, %1	\n"         /* creates a reservation on lf->count */
        "	addi	r10, r10, -1	\n"         /* dec count                      */
		"	sync            	\n"         /* synchronize instructions       */
		"	stwcx.	r10, 0, %1	\n"         /* conditionnal store             */
		"	bne-	dec			\n"
        "empty:				\n"
		"	mr		%0, r9		\n"
        :"=r" (result) : "r" (lf) : "r9", "r10", "r0"
	);
	return result;
}

static inline unsigned long lfsize (lifo * lf) 
{
	return lf->oc;
}

#endif
