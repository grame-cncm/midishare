/*
  MidiShare Project
  Copyright (C) Grame 1999-2005

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/


#include <stdio.h>
#include <time.h>
#include "lffifo.h"

#define STEP		5
#define MAXCOUNT 	30
#define NSEC		20		// each test step duration (in sec)

fifo		gstack;
fifocell	tbl[MAXCOUNT+1];

//-----------------------------------------------------------------
// init stack with cells
static void initstack (fifo *f, long count) {
	int i;
	fifoinit (f);
	for(i=0; i < count; i++) {
		fifoput (f, &tbl[i]);
	}
}

static long stacktest (long n) {
	fifocell *c; 
	fifo *f = &gstack;
	clock_t t = clock();
	while (n--) {
		c = fifoget(f);
		if (!c) {
			printf("error: pop returned null\n");
			break;
		}
		else fifoput(f, c);
	}
	return clock() - t;
}

//-----------------------------------------------------------------
static long getlooplen ()
{
	double d; 
	long initlen=1000000;
	initstack (&gstack, 5);
	d = stacktest(initlen);
	d /= CLOCKS_PER_SEC;
	d = initlen * NSEC / d;
	return (long)d;	
}

//-----------------------------------------------------------------
void bench (int max, int step, long looplen)
{
	int		i;
	double	perf;
	
	for (i=0; i <= max; i+=step) {
		initstack (&gstack, i+1);
		perf = stacktest(looplen);
		perf /= looplen;
		perf *= 1000000 / CLOCKS_PER_SEC;
		printf ("stack size:\t %d\tperf (in us per pop/push):\t %2f\n", i, perf);
  	}
}

int main()
{
	printf("-------- Lock free fifo stack bench ----------\n");
	bench (MAXCOUNT, STEP, getlooplen());
	return 0;
}
