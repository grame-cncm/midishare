/*

  Copyright © Grame 1999-2003

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
  
  modifications history:
   [13-09-99] DF - inlining fifoinit in fifoclear

*/

#ifndef __lffifo__
#define __lffifo__

/*****************************************************************
 *****************************************************************
                       LOCK FREE FIFO STACK 

 Implements a lock-free shared FIFO stack made of a list of cells
 linked together. A cell can be anything provided it starts with
 a pointer available to link together the cells of the stack. 
   
                           OPERATIONS
 ------------------------------------------------------------------
 fifoinit(fifo* ff) 			: Initialise fifo ff to empty
 fifoput  (fifo* ff, cell* cl)	: append cell cl to fifo tail
 fifoget  (fifo* ff)-> cell*	: pop head cell of ff
 fifoavail(fifo* ff)-> cell*	: return the head cell
 fifoclear(fifo* ff)-> cell*	: return the head cell and initialize
 fifosize (fifo* ff)-> n		: number of cells in fifo ff
 ------------------------------------------------------------------

 Warning : all operations expect non-null lifo and cell pointers.
 It is the caller responsability to check the arguments !
 *****************************************************************
 *****************************************************************/

#include "msAtomic.h"

/*****************************************************************
                           DATA STRUCTURES
 *****************************************************************/

/*
 ATTENTION !!!!
 la taille de la structure fifocell devrait être au moins de 32 bytes
 pour prevenir les livelocks
 revoir l'implémentation de CAS et CASL pour traiter la taille
 spécifique des cellules midishare
*/
typedef struct fifocell {
	struct fifocell* link;	/* next cell in the list */
    long 	value[3];      /* cell size is midishare specific */
} fifocell;

typedef struct fifo {
	vtype fifocell * head;	/* pointer to the head cell */
#ifndef __ppc__
	unsigned long oc;		/* total count of get operations */
#else
    long reserved1[7]; 		/* to avoid livelocks */
#endif
    vtype fifocell * tail;	/* pointer to the tail cell */
#ifndef __ppc__
	unsigned long ic;		/* total count of push operations */
#else
    long reserved2[7]; 		/* to avoid livelocks */
#endif
	TAtomic	count;
} fifo;


/****************************************************************
                          OPERATIONS
 ****************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

 void 			fifoinit(fifo* ff, fifocell * dummy);
 unsigned long 	fifosize (fifo * ff);
 void 			fifoput (vtype fifo * ff, fifocell * cl);
 fifocell * 	fifoget (vtype fifo * ff);
// fifocell * 	fifoavail (vtype fifo * ff); 
// fifocell * 	fifoclear (vtype fifo * ff);

#ifdef __cplusplus
}
#endif

#endif
