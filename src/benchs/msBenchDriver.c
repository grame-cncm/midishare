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

/* ----------------------------------*/
/* constants definitions             */
#define kProfileName  "msBenchDriver.ini"
#define kDriverName   "msBenchDriver"
#define kDriverVersion 100

#define kOutFileName  "msBenchDriver.out.txt"
#define kSumFileName  "msBenchDriver.sum.txt"

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
/* Driver required callbacks                                                    */
/* -----------------------------------------------------------------------------*/
static void WakeUpBench (short r)
{
	MidiSetRcvAlarm (r, FlushReceivedEvents);
	gTask = MidiTask (InitTask, MidiGetTime()+1, r, 0, 0, 0);
}

/* -----------------------------------------------------------------------------*/
static void SleepBench (short r)
{
	MidiForgetTask (&gTask);
}

/* -----------------------------------------------------------------------------*/
/* MidiShare part                                                               */
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
Boolean SetUpMidi ()
{
	TDriverInfos infos = { kDriverName, kDriverVersion, 0};
	short refNum; TDriverOperation op = { WakeUpBench, SleepBench }; 
		
	refNum = MidiRegisterDriver(&infos, &op);
	if (refNum == MIDIerrSpace) return false;
    gRef = refNum;
    return true;
}

/* -----------------------------------------------------------------------------*/
void CloseMidi ()
{
	if (gRef > 0) {
		MidiUnregisterDriver (gRef);
        gRef = 0;
	}
}

Boolean Start()
{
    if (bench_init(kBenchLen))
		return SetUpMidi();
	else fprintf (stderr, "cannot initialize bench memory\n");
	return false;
}

void Stop()
{
	FILE * out, *sum;
	CloseMidi();
	out = fopen(kOutFileName, "w");
	sum = fopen(kSumFileName, "w");
	print_result (out ? out : stdout, sum ? sum : stderr);
	if (out) fclose(out);
	if (sum) fclose(sum);
}
