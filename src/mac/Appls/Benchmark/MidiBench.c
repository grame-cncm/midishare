/*

  Copyright © Grame 2000

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
  grame@grame.fr

*/


#include "OSUtils.h"

#include "MidiShare.h"
#include "MidiBench.h"

#if macintosh
#define ApplName "\pMidiBench"
#define GenName  "\pGenerator"
#else
#define ApplName "MidiBench"
#define GenName  "Generator"
#endif

static MidiEvPtr genTask;
static short genRef;
static long benchLoop;

static char * infos [] = {
	" Bench1: reference loop measurement (MidiShare sleeping).",
	" Bench2: like bench1, with MidiShare running.",
	" Bench3: like bench2, with a task scheduled every 10 ms.",
	" Bench4: like bench2, with a task scheduled every 1 ms.",
	" Bench5: like bench2, with 500 tasks, 1 ms shifted, every 500 ms.",
	" Bench6: like bench2, with 500 tasks, 1 ms shifted, every 500 ms and 10 tasks every 10 ms.",
	" Bench7: with 3 different tasks per ms, every 500, 176 and 48ms.",
	" Bench8: 16 tasks every 16ms.",
	" Bench9: like bench8, with every task playing a keyOn using a different MIDI channel without output connected.",
	" Bench10: like bench9, with output connected.",
	" Bench11: gives 3 minutes measurement using MidiShare time.",
	" Bench12: playing 16 notes chords, 16 ms duration, every 32 ms (full MIDI stream)."
};


/* -----------------------------------------------------------------------------*/
static float ref1(void)
{
  float a = 500, b = 1254;
  return (a + b) / b * a;
}
static float ref2(void)
{
  float a = 999999, b = 725.45;
  return (b - a) * b / a;
}

/* -----------------------------------------------------------------------------*/
static void reference(void)
{
  long refi, refj; double res;

  for (refj = 0; refj <= benchLoop; refj++) {
    for (refi = 1; refi <= 150000L; refi++) {
      res = ref1 () + ref2 ();
      res = ref1 () - ref2 ();
      res = ref1 () * ref2 ();
      res = ref1 () / ref2 ();
      res = ref2 () + ref1 ();
      res = ref2 () - ref1 ();
    }
  }
}

char * GetBenchInfos(short benchNum)
{
	return ((benchNum > 0) && (benchNum < 13)) ? infos[benchNum - 1] : " ";
}

/* -----------------------------------------------------------------------------*/
/* tasks     */
/* -----------------------------------------------------------------------------*/

static pascal void emptyProcess(long d, short refNum, long a1, long a2, long a3)
{
  MidiTask (emptyProcess, d + a1, refNum, a1, a2, a3);
}

static pascal void keyOnProcess(long d, short refNum, long delay, long model, long a3)
{
  MidiSendAt(refNum, MidiCopyEv((MidiEvPtr)model), d + 7);
  MidiTask (keyOnProcess, d + delay, refNum, delay, model, a3);
}

static pascal void AccProcess(long d, short refNum, long a1, long a2, long a3)
{
  short i;
  for (i = 0; i <= 15; i++) {
    MidiEvPtr e = MidiNewEv(typeNote);
    if (e) {
      Pitch(e) = 60;
      Vel(e) = 80;
      Dur(e) = 16;
      Chan(e) = i;
    }
    MidiSend (refNum, e);
  }
  MidiTask (AccProcess, d + 32, refNum, a1, a2, a3);
}

/* -----------------------------------------------------------------------------*/
/* generators     */
/* -----------------------------------------------------------------------------*/
static pascal void GenProcess(long d, short ref, long start, long num, long freq)
{
  short i, adv = 5;

  for (i = 0; i <= 15; i++) {
    MidiEvPtr e = MidiNewEv (typeKeyPress);
    if (e) {
      Pitch(e) = 60;
      Vel(e) = 80;
      Chan(e) = i;
      MidiSendAt (ref, e, start + adv + num * 1000 / freq);
      num++;
    }
  }
  genTask = MidiTask (GenProcess, start + num * 1000 / freq, ref, start, num, freq);
}

void StartGen (short f)
{
  genRef = MidiOpen(GenName);
  MidiConnect (genRef, 0, true);
  GenProcess (MidiGetTime(), genRef, MidiGetTime(), 0, f * 100);
}

void StopGen(void)
{
  if (genRef) {
	  MidiForgetTask(&genTask);
	  MidiClose(genRef);
	  genRef = 0;
  }
}

/* -----------------------------------------------------------------------------*/
/* benchMarks     */
/* -----------------------------------------------------------------------------*/
static void benchMark1(long *elapsed)
{
  unsigned long d1, d2;

  GetDateTime (&d1);
  reference();
  GetDateTime (&d2);
  *elapsed = (d2 - d1) * 1000;
}

typedef void (* BenchProcPtr)(short refnum);
static void benchMark(long *elapsed, BenchProcPtr f)
{
  unsigned long d1, d2;
  short refNum;

  refNum = MidiOpen(ApplName);
  if (f) f(refNum);
  GetDateTime(&d1);
  reference();
  GetDateTime(&d2);
  MidiClose(refNum);
  *elapsed = (d2 - d1) * 1000;
}


static void __benchMark3(short refNum)
	{ MidiTask (emptyProcess, MidiGetTime(), refNum, 10, 0, 0); }

static void __benchMark4(short refNum)
	{ MidiTask (emptyProcess, MidiGetTime(), refNum, 1, 0, 0); }

static void __benchMark5(short refNum)
{
  long i, d = MidiGetTime() - 500;
  for (i = 1; i <= 500; i++) {
    MidiTask (emptyProcess, d + i, refNum, 500, 0, 0);
  }
}

static void __benchMark6(short refNum)
{
  long i, d;

  d = MidiGetTime() - 500;
  for (i = 1; i <= 500; i++) {
    MidiTask(emptyProcess, d + i, refNum, 500, 0, 0);
  }
  d = MidiGetTime() - 10;
  for (i = 1; i <= 10; i++) {
    MidiTask(emptyProcess, d + i, refNum, 10, 0, 0);
  }
}

static void __benchMark7 (short refNum)
{
  long i, d;

  d = MidiGetTime() - 250;
  for (i = 1; i <= 250; i++) {
    MidiTask(emptyProcess, d + i, refNum, 250, 0, 0);
  }
  d = MidiGetTime() - 176;
  for (i = 1; i <= 176; i++) {
    MidiTask(emptyProcess, d + i, refNum, 176, 0, 0);
  }
  d = MidiGetTime() - 48;
  for (i = 1; i <= 48; i++) {
    MidiTask(emptyProcess, d + i, refNum, 48, 0, 0);
  }
}

static void __benchMark8(short refNum)
{
  long i, dx = MidiGetTime() - 16;
  for (i = 1; i <= 16; i++) {
    MidiTask(emptyProcess, dx + i, refNum, 16, 0, 0);
  }
}

static void __benchMark9(short refNum)
{
  long i, d = MidiGetTime() - 16;

  for (i = 1; i <= 16; i++) {
    MidiEvPtr e = MidiNewEv(typeKeyOn);
    if (e) {
      Chan(e) = i - 1;
      MidiTask(keyOnProcess, d + i, refNum, 16, (long)e, 0);
    }
  }
}

static void __benchMark10(short refNum)
{
  long i, d = MidiGetTime() - 16;

  MidiConnect(refNum, 0, true);
  for (i = 1; i <= 16; i++) {
    MidiEvPtr e = MidiNewEv (typeKeyOn);
    if (e) {
      Chan(e) = i - 1;
      MidiTask (keyOnProcess, d + i, refNum, 16, (long)e, 0);
    }
  }
}

static void __benchMark12(short refNum)
{
  MidiConnect(refNum, 0, true);
  MidiTask (AccProcess, MidiGetTime(), refNum, 0, 0, 0);
}

static inline void benchMark2(long *elapsed) { benchMark(elapsed, 0); }
static inline void benchMark3(long *elapsed) { benchMark(elapsed, __benchMark3); }
static inline void benchMark4(long *elapsed) { benchMark(elapsed, __benchMark4); }
static inline void benchMark5(long *elapsed) { benchMark(elapsed, __benchMark5); }
static inline void benchMark6(long *elapsed) { benchMark(elapsed, __benchMark6); }
static inline void benchMark7(long *elapsed) { benchMark(elapsed, __benchMark7); }
static inline void benchMark8(long *elapsed) { benchMark(elapsed, __benchMark8); }
static inline void benchMark9(long *elapsed) { benchMark(elapsed, __benchMark9); }
static inline void benchMark10(long *elapsed) { benchMark(elapsed, __benchMark10); }
static inline void benchMark12(long *elapsed) { benchMark(elapsed, __benchMark12); }

static void benchMark11(long *elapsed)
{
  unsigned long d1, d2, d3, t1, t2;
  short refNum;

  refNum = MidiOpen(ApplName);
  GetDateTime(&d1);
  d1++;
  do {
    GetDateTime(&d2);
  } while (d2 < d1);   /* synch with a new second */
  t1 = MidiGetTime();
  d2 += 180;
  do {
    GetDateTime(&d3);
  } while (d3 < d2);   /* wait 3 minutes*/
  t2 = MidiGetTime();
  MidiClose(refNum);
  *elapsed = t2 - t1;
}


long DoBench (short benchNum)
{
	long elapsed;
	switch (benchNum) {
		case 1: 	benchMark1(&elapsed);
					break;
		case 2: 	benchMark2(&elapsed);
					break;
		case 3: 	benchMark3(&elapsed);
					break;
		case 4: 	benchMark4(&elapsed);
					break;
		case 5: 	benchMark5(&elapsed);
					break;
		case 6: 	benchMark6(&elapsed);
					break;
		case 7: 	benchMark7(&elapsed);
					break;
		case 8: 	benchMark8(&elapsed);
					break;
		case 9: 	benchMark9(&elapsed);
					break;
		case 10: 	benchMark10(&elapsed);
					break;
		case 11: 	benchMark11(&elapsed);
					break;
		case 12: 	benchMark12(&elapsed);
					break;
	}
	return elapsed;
}


void InitBenchLoop(void)
{
	long elapsed=0;

	benchLoop = 1;
	do {
	  	benchMark1 (&elapsed);
	  	if (elapsed) break;
		else benchLoop++;
	} while (benchLoop < 20); 
	benchLoop *= 180000 / (elapsed ? elapsed : 1);
}

void CloseBenchs (void)
{
	StopGen ();
}
