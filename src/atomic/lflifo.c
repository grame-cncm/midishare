/*

  Copyright © Grame 2001-2003

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

#define CW_OSX
#include "lflifo.h"

void lfinit(lifo* lf)
{
	lf->top = 0;
	lf->count.value = 0;
#ifndef __ppc__
	lf->oc = 0;
#endif
}

cell* lfavail(lifo* lf) 			{ return (cell*)lf->top; }
unsigned long lfsize (lifo * lf) 	{ return lf->count.value; }

void lfpush (vtype lifo * lf, cell * cl)
{
	cell * top;
	do {
		top = (cell *)lf->top;
		cl->link = top;
	} while (!CAS (&lf->top, top, cl));
	msAtomicInc (&lf->count);
}

#ifdef __ppc__
/* 
   on ppc architecture, the ABA problem is naturaly catched
   by the memory reservation operated by CASL
*/
cell* lfpop (vtype lifo * lf)
{
	cell* top;
	do {
		top = (cell *)lf->top;
		if (!top) return 0;
	} while (!CASL (&lf->top, top));
	msAtomicDec (&lf->count);
	return top;
}

#else
/* 
   on intel architecture, the ABA problem is catched
   using oc (out count) and a CAS2
*/
cell* lfpop (vtype lifo * lf)
{
	long oc;
	cell* top;
	do {
		oc =  lf->oc;
		top = (cell *)lf->top;
		if (!top) return 0;
	} while (!CAS2 (&lf->top, top, oc, top->link, oc+1));
	msAtomicDec (&lf->count);
	return top;
}
#endif
