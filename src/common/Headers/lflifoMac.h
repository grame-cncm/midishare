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

*/

#ifndef __LFLIFOMAC__
#define __LFLIFOMAC__


# ifdef __POWERPC__

static void lfpush (register lifo * lf, register cell * cl) 
{
	register long tmp;
	asm {
		addi	lf, lf, 4
	loop:
		lwarx	tmp, 0, lf       /* creates a reservation on lf    */
		stw		tmp, 0(cl)       /* link the new cell to the lifo  */
		sync                     /* synchronize instructions       */
		stwcx.	cl, 0, lf        /* if the reservation is not altered */
		                         /* modify lifo top               */
		bne-	loop             /* otherwise: loop and try again */

		addi	lf, lf, 4
	inc:
		lwarx	tmp, 0, lf       /* creates a reservation on lf->count */
		addi	tmp, tmp, 1      /* inc count                      */
		sync                     /* synchronize instructions       */
		stwcx.	tmp, 0, lf       /* conditionnal store             */
		bne-	inc 
	}
}
	
static cell* lfpop (register lifo * lf) 
{
	register cell * result;
	register long a, b;
	asm {
		addi	lf, lf, 4
	loop:
		lwarx	a, 0, lf         /* creates a reservation on lf    */
		cmpwi	a, 0             /* test if the lifo is empty      */
		beq-	empty
		lwz		b, 0(a)          /* next cell in b                */
		sync                     /* synchronize instructions       */
		stwcx.	b, 0, lf         /* if the reservation is not altered */
		                         /* modify lifo top                */
		bne-	loop             /* otherwise: loop and try again  */

		addi	lf, lf, 4
	dec:
		lwarx	b, 0, lf         /* creates a reservation on lf->count */
		addi	b, b, -1         /* dec count                      */
		sync                     /* synchronize instructions       */
		stwcx.	b, 0, lf         /* conditionnal store             */
		bne-	dec
		 
	empty:
		mr		result, a
	}
	return result;
}

static inline unsigned long lfsize (lifo * lf) 
{
	return lf->oc;
}

# else

#include "msSync.h"

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

#endif 

#endif
