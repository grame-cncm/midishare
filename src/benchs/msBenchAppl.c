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
#include <stdlib.h>
#include <MidiShare.h>

#include "results.h"

#ifdef WIN32
# include <windows.h>
# define sleep(n)  Sleep(n)
#else
# include <signal.h>
# include <unistd.h>
# define sleep(n)  usleep(n*1000)
#endif

#define kTimeout	1500


static int siglist [] = {
	SIGABRT,
	SIGFPE,
	SIGILL,
	SIGINT,
	SIGSEGV,
	SIGTERM,
#ifndef WIN32
	SIGHUP,
	SIGQUIT,
	SIGBUS,
#endif
	0
};

/* ----------------------------------*/
/* constants definitions             */
#define kApplName     "msBenchAppl"

#define kOutFileName  "msBenchAppl.out.txt"
#define kSumFileName  "msBenchAppl.sum.txt"

/* ----------------------------------*/
/* functions declarations            */
static void FlushReceivedEvents (short r);
static void InitTask( long date, short refNum, long a1,long a2,long a3 );
static void TimeTask( long date, short refNum, long a1,long a2,long a3 );

short gRef = 0;
MidiEvPtr gTask = 0;
TimeType  gFirstCall, gLastCall;
int gCount  = 1500;
long gLastTime = 0;
long gFirstTime = 0;
long gFlag = 0;

static void TimeTask (long date, short refNum, long a1,long a2,long a3);

//___________________________________________________________________
static void benchSigHandler(int sig)
{
	fprintf (stderr, "client %d: signal %d received\n", (int)gRef, sig);
	exit (1);
}

//___________________________________________________________________
static void sigInit ()
{
#ifndef WIN32
	int * sigs = siglist;
	struct sigaction sa;

	while (*sigs) {
		sa.sa_handler = benchSigHandler;
		sigemptyset (&sa.sa_mask);
		sa.sa_flags = 0; 			//SA_RESETHAND;
		sigaction (*sigs++, &sa, 0);
	}
	sa.sa_handler = SIG_IGN;
	sigemptyset (&sa.sa_mask);
	sa.sa_flags = 0; 			//SA_RESETHAND;
	sigaction (SIGPIPE, &sa, 0);
#endif
}

/* -----------------------------------------------------------------------------*/
static void InitTask (long date, short refNum, long a1,long a2,long a3 )
{
//	getTime(gFirstCall);
    gFirstTime = MidiGetTime();
	getTime(gLastCall);
	gTask = MidiTask (TimeTask, date+kTimeRes, refNum, 0, 0, 0);
	if (!gTask)
		fprintf (stderr, "InitTask: MidiTask failed\n");
}

/* -----------------------------------------------------------------------------*/
static void TimeTask (long date, short refNum, long a1,long a2,long a3)
{
	TimeType t;

	if (gFlag)
		fprintf (stderr, "%d: TimeTask called with gFlag=%ld\n", refNum, gFlag);
	gTask = 0;
	getTime(t);
	storeTime (gLastCall, t);
	gLastCall = t;
    gLastTime = MidiGetTime();
	if (bench_done()) return;
	gTask = MidiTask (TimeTask, date+kTimeRes, refNum, 0, 0, 0);
	if (!gTask)
		fprintf (stderr, "TimeTask %d: MidiTask failed\n", refNum);
}

/* -----------------------------------------------------------------------------*/
static void NoteTask (long date, short refNum, long a1,long a2,long a3)
{
	MidiEvPtr e  = MidiNewEv(typeNote);
	if (e) {
		Pitch(e) = 64;
		Dur(e) = 100;
		Vel(e) = 127;
		MidiSendIm (refNum, e);
		gTask = MidiTask (NoteTask, date+500, refNum, 0, 0, 0);
		if (!gTask)
			fprintf (stderr, "NoteTask %d: MidiTask failed\n", refNum);
	}
	else
		fprintf (stderr, "TimeTask %d: MidiTask failed\n", refNum);
}

/* -----------------------------------------------------------------------------*/
static void FlushReceivedEvents (short r)
{
	MidiFlushEvs (r);
}

/* -----------------------------------------------------------------------------*/
int main (int argc, char *argv[])
{
	long t;
	sigInit ();
	if (!bench_init(kBenchLen)) {
		fprintf (stderr, "Can't launch msBenchAppl: initialization failed\n");
		return 1;
	}
	
	if (!MidiShare()) {
		fprintf (stderr, "Can't launch msBenchAppl: MidiShare is not running\n");
		return 1;
	}
	
	gRef = MidiOpen (kApplName);
	if (gRef < 0) {
		fprintf (stderr, "Can't launch msBenchAppl: can't open a MidiShare client (%d)\n", gRef);
		return 1;
	}

	MidiSetRcvAlarm (gRef, FlushReceivedEvents);
//	MidiConnect (gRef, 0, 1);
//	NoteTask (MidiGetTime(), gRef, 0, 0, 0);
//	gTask = MidiTask (InitTask, MidiGetTime()+1, gRef, 0, 0, 0);
	gTask = MidiTask (InitTask, kStartTime, gRef, 0, 0, 0);

	sleep (1500);
	while (!bench_done()) {
		sleep (100);
		t = MidiGetTime();
		if ((t - gLastTime) > kTimeout) {
			fprintf (stderr, "%d time out\n", gRef); 
			break;
		}
	}
/*
	fprintf (stdout, "press return to quit");
	getc(stdin);
*/
	MidiForgetTask (&gTask);
//	fprintf (stderr, "%d MidiForgetTask done (%lx)\n", gRef, (long)gTask);
    gFlag = 1;
	MidiClose (gRef);
	print_result (stdout, stderr);
	fprintf (stderr, "first and last ms times: %ld %ld elapsed: %ld\n", 
        gFirstTime, gLastTime, gLastTime - gFirstTime);
//	fprintf (stderr, "real time elapsed: %ld\n", micro(elapsed(gLastCall, gFirstCall)));
	return 0;
}
