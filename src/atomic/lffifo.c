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

#include <stdio.h>

#define CW_OSX
#include "lffifo.h"

#define fifo_end(ff)	(fifocell *)ff

//----------------------------------------------------------------
void fifoinit (fifo* ff, fifocell *dummy)
{
	ff->count.value = 0;
#ifndef __ppc__
	ff->oc = ff->ic = 0;
#endif
	dummy->link = fifo_end(ff);
	ff->head = ff->tail = dummy;
}

void pfifo (fifo *ff)
{
    printf ("fifo %lx : head %lx, tail %lx end %lx",
    (long)ff, (long)ff->head, (long)ff->tail, (long)ff->tail->link);
	if (ff->tail->link == fifo_end(ff)) printf ("\n");
	else printf (" (%lx)\n", (long)ff->tail->link->link);
	fflush (stdout);
}

//----------------------------------------------------------------
unsigned long fifosize (fifo * ff)
{
    return ff->count.value;
}

//----------------------------------------------------------------
#ifdef __ppc__

void fifoput (vtype fifo * ff, fifocell * cl) 
{
	vtype fifocell * tail;
//printf (">> fifoput in %lx\n", (long)cl);
//pfifo (ff);
    cl->link = fifo_end(ff);	/* set the cell next pointer to the end marker */
	while (1) {
//        LWARX (&ff->tail);
		tail = ff->tail;		/* read the tail cell */
		/* try to link the cell to the tail cell */
		if (CAS (&tail->link, fifo_end(ff), cl)) {
			break;		
        }
        else {
            /* tail was not pointing to the last cell, try to set tail to the next cell */
            CASLNE (&ff->tail, (void *)tail, fifo_end(ff));
        }
	} 
    if (!CASLNE (&ff->tail, (void *)tail, fifo_end(ff)))
//printf ("====> fifoput CASLNE failed\n");
//pfifo (ff);
	msAtomicInc (&ff->count);
//printf ("<< fifoput out\n");
}

#else

void fifoput (vtype fifo * ff, fifocell * cl) 
{
    long ic;
	vtype fifocell * tail;

	cl->link = fifo_end(ff);	/* set the cell next pointer to the end marker */
	while (1) {
		ic = ff->ic;			/* read the tail modification count */
		tail = ff->tail;		/* read the tail cell */
		/* try to link the cell to the tail cell */
        if (CAS (&tail->link, fifo_end(ff), cl))
			break;
		else
		/* tail was not pointing to the last cell, try to set tail to the next cell */
            CAS2 (&ff->tail, tail, ic, tail->link, ic+1);
	} 
    /* enqeue is done, try to set tail to the enqueued cell */
	CAS2 (&ff->tail, tail, ic, cl, ic+1);
	msAtomicInc (&ff->count);
}

#endif

//----------------------------------------------------------------
#ifdef __ppc__

fifocell * fifoget (vtype fifo * ff) 
{
	vtype fifocell * head;
	fifocell * next; 
	long ic, oc, val[3];
	short done = 0;
	
//printf (">> fifoget in\n");
	do {
		LWARX (&ff->head);
        head = ff->head;				/* read the head cell */
		next = head->link;				/* read the next cell */
        if (head == ff->tail) {			/* is queue empty or tail falling behind? */
            printf ("---> fifoget: queue empty or ? (%lx %lx %ld)\n", (long)head, (long)next, fifosize(ff));
			pfifo(ff);
            if (next == fifo_end(ff))	/* is queue empty?             */
                return 0;				/* queue is empty; return NULL */
            /* tail is pointing to head in a non empty queue, */
            /* try to set tail to the next cell               */
            if (!CASLNE (&ff->tail, (void *)head, fifo_end(ff)))
				pfifo(ff);
        }
        else if (next != fifo_end(ff)) { /* if we are not competing on the dummy cell */
            val[0] = next->value[0];	 /* we read the next cell value */
            val[1] = next->value[1];
            val[2] = next->value[2];
            /* and we try to set head to the next cell */
            done = STWCX (&ff->head, (void *)head, next);
        }
        else printf ("---> fifoget: uncatched test\n");
	} while (!done);
	msAtomicDec (&ff->count);
	head->value[0] = val[0]; 	/* and finally copy the value to the deqeued cell */
	head->value[1] = val[1];
	head->value[2] = val[2];
//printf ("<< fifoget out (%lx)\n", (long)head);
	return (fifocell *)head;
}

fifocell * oldfifoget (vtype fifo * ff) 
{
	vtype fifocell * head;
	fifocell * next; 
	long ic, oc, val[3];
	short done = 0;
	
	do {
//		LWARX (&ff->head);
//		LWARX (&ff->tail);
        head = ff->head;				/* read the head cell */
		next = head->link;				/* read the next cell */
//		if (oc == ff->oc) {				/* ensures that next is a valid pointer */
		if (next) {				/* ensures that next is a valid pointer */
                                        /* to avoid failure when reading next value */
            LWARX (&ff->tail);
			if (head == ff->tail) {			/* is queue empty or tail falling behind? */
				if (next == fifo_end(ff))	/* is queue empty?             */
					return 0;				/* queue is empty; return NULL */
                /* tail is pointing to head in a non empty queue, */
                /* try to set tail to the next cell               */
				STWCX (&ff->tail, (void *)head, next);
			}
			else if (next != fifo_end(ff)) { /* if we are not competing on the dummy cell */
                LWARX (&ff->head);
				val[0] = next->value[0];	 /* we read the next cell value */
				val[1] = next->value[1];
				val[2] = next->value[2];
                /* and we try to set head to the next cell */
				done = STWCX (&ff->head, (void *)head, next);
			}
		}
	} while (!done);
	msAtomicDec (&ff->count);
	head->value[0] = val[0]; 	/* and finally copy the value to the deqeued cell */
	head->value[1] = val[1];
	head->value[2] = val[2];
	return (fifocell *)head;
}

#else

fifocell * fifoget (vtype fifo * ff) 
{
	fifocell * head;
	fifocell * next; 
	long ic, oc, val[3];
	short done = 0;
	
	do {
		oc = ff->oc;					/* read the head modification count */
		ic = ff->ic;					/* read the tail modification count */
		head = ff->head;				/* read the head cell */
		next = head->link;				/* read the next cell */
		if (oc == ff->oc) {				/* ensures that next is a valid pointer */
                                        /* to avoid failure when reading next value */
			if (head == ff->tail) {			/* is queue empty or tail falling behind? */
				if (next == fifo_end(ff))	/* is queue empty?             */
					return 0;				/* queue is empty; return NULL */
                /* tail is pointing to head in a non empty queue, */
                /* try to set tail to the next cell               */
				CAS2 (&ff->tail, head, ic, next, ic+1);
			}
			else if (next != fifo_end(ff)) { /* if we are not competing on the dummy cell */
				val[0] = next->value[0];	 /* we read the next cell value */
				val[1] = next->value[1];
				val[2] = next->value[2];
                /* and we try to set head to the next cell */
				done = CAS2 (&ff->head, head, oc, next, oc+1);
			}
		}
	} while (!done);
	msAtomicDec (&ff->count);
	head->value[0] = val[0]; 	/* and finally copy the value to the deqeued cell */
	head->value[1] = val[1];
	head->value[2] = val[2];
	return head;
}

#endif

//----------------------------------------------------------------
fifocell * fifoavail (vtype fifo * ff)
{
    return 0;
}

//----------------------------------------------------------------
fifocell * fifoclear (vtype fifo * ff)
{
    return 0;
}
