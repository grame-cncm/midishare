/*

  Copyright © Grame 1999-2002

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

#ifndef __msAtomicPPC_CW__
#define __msAtomicPPC_CW__

//----------------------------------------------------------------
// Compare and swap
//----------------------------------------------------------------
static inline int CAS (register vtype void * addr, register void * value, register void * newvalue) 
{
	register int result;
	register long tmp;
	asm {
    loop:
        lwarx	tmp, 0, addr       /* creates a reservation on addr  */
        cmpw	tmp, value         /* test value at addr             */
        bne-	failed
        sync          	           /* synchronize instructions       */
        stwcx.	newvalue, 0, addr  /* if the reservation is not altered */
                                   /* stores the new value at addr   */
        bne-	failed
        li      result, 1
        bl		exit
	failed:
        li      result, 0
    exit:
	}
	return result;
}

//----------------------------------------------------------------
// Compare and swap link
//----------------------------------------------------------------
static inline int CASL (register vtype void * addr, register void * value) 
{
	register int result;
	register long tmp, next;
	asm {
    loop:
        lwarx	tmp, 0, addr       /* creates a reservation on addr  */
        cmpw	tmp, value         /* test value at addr             */
        bne-	failed
		lwzx	next, 0, tmp
        sync          	           /* synchronize instructions       */
        stwcx.	next, 0, addr      /* if the reservation is not altered */
                                   /* stores the new value at addr   */
        bne-	failed
        li      result, 1
        bl		exit
	failed:
        li      result, 0
    exit:
	}
	return result;
}

#endif
