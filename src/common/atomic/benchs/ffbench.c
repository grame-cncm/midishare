/*

  Copyright © Grame 2001-2002

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
#include <time.h>
#include "pthread.h"
//#include <unistd.h> 

#include "lffifo.h"

#define MAXTHREADS	10
#define MAXCOUNT 6*MAXTHREADS

typedef struct pont {
	long 	limit;
	long	stopped;
	long	duration;
} pont;

#define pile fifo
#ifndef POP
	typedef fifocell * testcell;
#	define POP(f,c) c=fifoget(f)
#	define CHECK(f,c,msg) POP(f,c); if(c==0) printf (msg)
#endif
#define PUSH fifoput
#define LASTCELL(c)		((c) == fifo_end(&gstack))

#define LIMIT	500000
#define cell fifocell;
#define fifo_end(ff)	(fifocell *)ff

/*
 *	Global variables
 */
pile	gstack;
fifocell	tbl[MAXCOUNT];
fifocell	dummy;

// init stack with cells
void initstack (pile *f)
{
	int i;
	fifoinit (f, &dummy);
	for(i=0; i < MAXCOUNT; i++) {
		PUSH (f, &tbl[i]);
	}
}

long stacktest (long n)
{
	testcell temp[6];
	int i; clock_t t;
	
	t = clock();
	while (n--) {
		for (i=0; i<6; i++) {
			CHECK(&gstack, temp[i], "error: pop returned null\n");
		}
		for (i=0; i<6; i++) PUSH(&gstack, temp[i]);
	}
	return clock() - t;
}

void* stacktestthread(void* pp)
{
	pont* p = (pont*) pp;
	p->duration = stacktest(p->limit);
	p->stopped = 1;
	return 0;
}


//-----------------------------------------------------------------
#ifndef COUNT
static long count (pile * ff)
{
	long count = 0;
	fifocell * cl;
	cl = ff->head->link;
	while (cl != fifo_end(ff)) {
		count++;
		cl = cl->link;
		if (count > MAXCOUNT)
			return -1;
	}
	return count;
}
#endif

void checkstack (pile * f, int n)
{
	int i = count(f);
	if (i != n) { 
		printf("error checking stack %p : count=%d, (should be %d)\n", f, i, n);
		printf("       fifo size: %ld\n", fifosize(f));
	}
}

void bench (int max)
{
	pthread_t	fils[MAXTHREADS];
	pont		bridge[MAXTHREADS]; 
	int			i, end, th;
	float		perf;
	
	for (th=1; th <= max; th++) {
		initstack (&gstack);
	  	printf("threads count:\t %d \t", th); fflush (stdout);
	  	for (i=0; i<th; i++) {
		  	bridge[i].limit = LIMIT;
		  	bridge[i].stopped = 0; 
		  	pthread_create(&fils[i], NULL, stacktestthread, &bridge[i]);
	  	}
	  	do {
		  	end = 1; for (i=0; i<th; i++)  end &= bridge[i].stopped;
	  	} while ( end == 0 );
		perf = 0;
		for (i=0; i<th; i++) { perf += bridge[i].duration; }
		perf = (perf * 1000000) / (6 * CLOCKS_PER_SEC);
		perf /= (th * LIMIT);
		printf (" perf (in us per pop/push):\t %2f\n", perf); fflush (stdout);
		checkstack(&gstack, MAXCOUNT);
  	}
}


#define THREADSCOUNT	8

int main()
{
	printf("\n-------- Lock free fifo stack bench ----------\n");
	bench (THREADSCOUNT);
	return 0;
}
