/*
  MidiShare Project
  Copyright (C) Grame 1999-2005

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#ifndef __LFFIFO_SEM__
#define __LFFIFO_SEM__

#include <pthread.h>
#include "lffifo.h"

#define fifo_end(ff)	0
pthread_mutex_t 	gSemaphore;

unsigned long fifosize (fifo * ff) {
	return ff->count.value;
}

void fifoinit (fifo* ff)
{
	static int init=0;
	ff->count.value = 0;
	ff->head = 0;
	ff->tail = (fifocell*)&ff->head;
	if (!init) {
		pthread_mutex_init (&gSemaphore, 0);
		init=1;
	}
}

void fifoput (fifo * ff, fifocell* cl) 
{
	cl->link = fifo_end(ff);
	pthread_mutex_lock(&gSemaphore);
	ff->tail->link = cl;
	ff->tail = cl;
	ff->count.value++;
	pthread_mutex_unlock(&gSemaphore);
}

fifocell * fifoget (fifo * ff) 
{
	fifocell * head, *next;
	pthread_mutex_lock(&gSemaphore);
	head = ff->head;
	if (head == fifo_end(ff)) {
		pthread_mutex_unlock(&gSemaphore);
		return 0;
	}	
	next = head->link;
	if (next == fifo_end(ff)) {
		ff->tail = (fifocell*)&ff->head;
	}	
	ff->head = next;
	ff->count.value--;
	pthread_mutex_unlock(&gSemaphore);
	return head;
}

#endif
