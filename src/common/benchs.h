/*
  MidiShare Project
  Copyright (C) Grame 2004

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#ifndef __benchs__
#define __benchs__

#if defined(__APPLE__) && defined(__MACH__)
#include <mach/mach_time.h>
#define RTC
#elif defined(WIN32)
#include <windows.h>
#else
#include <sys/time.h>
#endif

#ifdef RTC
typedef double TimeVal;
typedef double TimeType;
#define elapsed(t2, t1)	(t2 - t1)
#define gettimeofday(t,n)	t=GetRTC()
//static __inline__ GetRTC() { asm ("mftb r3");}
#define GetRTC mach_absolute_time
#define storeTime(t1,t2)	_storeTime(t1, t2)

#elif defined(WIN32)

long elapsed (LARGE_INTEGER t1, LARGE_INTEGER t2);
typedef LARGE_INTEGER TimeVal;
#define gettimeofday(t,n)	gettime(t)

#else

typedef struct timeval * TimeVal;
typedef struct timeval TimeType;
long elapsed (TimeVal t1, TimeVal t2);
#define gettimeofday(t,n)	gettimeofday(&t, n)
#define storeTime(t1,t2)	_storeTime(&t1, &t2)
#endif

void _storeTime (TimeVal first, TimeVal second);
int print_bench ();
int bench_done ();
void bench_reset ();


#endif
