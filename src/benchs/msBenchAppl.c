/*

  Copyright © Grame 2003

  This library is free software; you can redistribute it and modify it under 
  the terms of the GNU Library General Public License as published by the 
  Free Software Foundation version 2 of the License, or any later version.

  This library is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public License 
  for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

  Grame Research Laboratory, 9, rue du Garet 69001 Lyon - France
  grame@rd.grame.fr

*/

#include <stdio.h>
#include <MidiShare.h>

#include "results.h"

#ifdef WIN32
# include <windows.h>
# define sleep(n)  Sleep(n)
#else
# include <unistd.h>
# define sleep(n)  usleep(n*1000)
#endif


/* ----------------------------------*/
/* constants definitions             */
#define kApplName     "msBenchAppl"

#define kOutFileName  "msBenchAppl.out.txt"
#define kSumFileName  "msBenchAppl.sum.txt"

#define kBenchLen		1500
#define kTimeRes		1

/* ----------------------------------*/
/* functions declarations            */
static void FlushReceivedEvents (short r);
static void InitTask( long date, short refNum, long a1,long a2,long a3 );
static void TimeTask( long date, short refNum, long a1,long a2,long a3 );

short gRef = 0;
MidiEvPtr gTask = 0;
TimeType  gLastCall;

/* -----------------------------------------------------------------------------*/
static void InitTask (long date, short refNum, long a1,long a2,long a3 )
{
	getTime(gLastCall);
	gTask = MidiTask (TimeTask, date+kTimeRes, refNum, 0, 0, 0);
}

/* -----------------------------------------------------------------------------*/
static void TimeTask (long date, short refNum, long a1,long a2,long a3)
{
	TimeType t;
	getTime(t);
	storeTime (gLastCall, t);
	gLastCall = t;
	gTask = MidiTask (TimeTask, date+kTimeRes, refNum, 0, 0, 0);
}

/* -----------------------------------------------------------------------------*/
static void FlushReceivedEvents (short r)
{
	MidiFlushEvs (r);
}

/* -----------------------------------------------------------------------------*/
int main (int argc, char *argv[])
{
	short ref;
	
	if (!bench_init(kBenchLen)) {
		fprintf (stderr, "Can't launch msBenchAppl: initialization failed\n");
		return 1;
	}
	
	if (!MidiShare()) {
		fprintf (stderr, "Can't launch msBenchAppl: MidiShare is not running\n");
		return 1;
	}
	
	ref = MidiOpen (kApplName);
	if (ref < 0) {
		fprintf (stderr, "Can't launch msBenchAppl: can't open a MidiShare client (%d)\n", ref);
		return 1;
	}

	MidiSetRcvAlarm (ref, FlushReceivedEvents);
	gTask = MidiTask (InitTask, MidiGetTime()+1, ref, 0, 0, 0);
	while (!bench_done())
		sleep (100);
	MidiForgetTask (&gTask);
	MidiClose (ref);
	print_result (stdout, stderr);
	return 0;
}
