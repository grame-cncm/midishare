/*

  Copyright � Grame 2001-2005

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
  research@grame.fr

*/

#include "lffifo.h"

#define fifo_end(ff)	(fifocell *)ff

//----------------------------------------------------------------
void fifoinit (fifo* ff, fifocell *dummy)
{
	ff->count.value = 0;
	/*
		raw initialization may be used to control a fifo state 
		when no dummy cell is available
	*/
	if (dummy) dummy->link = fifo_end(ff);
	ff->head = ff->tail = dummy;
}

//----------------------------------------------------------------
unsigned long fifosize (fifo * ff)
{
    return ff->count.value;
}

//----------------------------------------------------------------
void fifoput (fifo * ff, fifocell * cl) 
{
	fifocell * volatile tail;

    cl->link = fifo_end(ff);	/* set the cell next pointer to the end marker */
	while (1) {
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
	CASLNE (&ff->tail, (void *)tail, fifo_end(ff));
	msAtomicInc (&ff->count);
}

//----------------------------------------------------------------
fifocell * fifoget (fifo * ff) 
{
	fifocell * volatile head;
	fifocell * next; 
	long val[3];
	short done = 0;
	
	do {
		LWARX (&ff->head);
        head = ff->head;				/* read the head cell */
		next = head->link;				/* read the next cell */
		/*
		  WARNING: the next pointer still needs to be checked before reading its value 
		*/
		if (head == ff->tail) {			/* is queue empty or tail falling behind? */
			if (head->link == fifo_end(ff) && STWCX (&ff->head, (void *)head, (void *)head)) /* is queue really empty? */
				return 0;				/* queue is empty; return NULL */
			/* tail is pointing to head in a non empty queue, */
			/* try to set tail to the next cell               */
			CASLNE (&ff->tail, (void *)head, fifo_end(ff));
		}
		else if (next != fifo_end(ff)) { /* if we are not competing on the dummy cell */
			val[0] = next->value[0];	 /* we read the next cell value */
			val[1] = next->value[1];
			val[2] = next->value[2];
			/* and we try to set head to the next cell */
			done = STWCX (&ff->head, (void *)head, next);
		}
	} while (!done);
	msAtomicDec (&ff->count);
	head->value[0] = val[0]; 	/* and finally copy the value to the deqeued cell */
	head->value[1] = val[1];
	head->value[2] = val[2];
	return (fifocell *)head;
}

//----------------------------------------------------------------
/*	fifoavail returns a pointer to the "first" cell in the fifo
	if it is not empty. This is meaningful if there is only one
	reader for the fifo. The pointer returned is actually the 
	second cell of the fifo because it contains the right content.
	This means that : fifoavail(ff) != fifoget(ff) but we have
	content(fifoavail(ff)) == content(fifoget(ff))
*/
fifocell* fifoavail (fifo* ff) 
{
	/* simulated atomic read of the required fields*/
	while (1) {
		LWARX (&ff->head);
		fifocell * 	hd  = ff->head;
		fifocell*	n   = hd->link;
		fifocell*	tail= ff->tail;
		
		if (STWCX (&ff->head, (void *)hd, hd)) {
			/*	no cells were removed during reading, therefore
				we have coherent (but maybe outdated) data*/
			return (hd==tail) ? 0 : n;
		}
	}
	return 0;	/* never used !*/
}

//----------------------------------------------------------------
fifocell* fifoflush (fifo* ff) 
{
	fifocell	*next, *cur;
	fifocell* first;
	if (ff->head==0) return 0;	
	
	first = fifoget(ff);
	if (first==0) return 0;	
	cur = first;	
	while ((next = fifoget(ff))) {
		cur->link = next;
		cur = next;
	}
	cur->link = 0;
	return first;
}

//----------------------------------------------------------------
fifocell* fifoclear (fifo* ff) 
{
	fifocell* head = ff->head;
	fifocell* tail = ff->tail;
	fifoinit(ff, 0);
	if (tail) tail->link = 0;	
	return head;
}
