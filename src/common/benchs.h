/*

  Copyright © Grame 2004

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
  research@grame.fr

*/

#ifndef __benchs__
#define __benchs__

#if defined(__APPLE__) && defined(__MACH__)
#include <mach/mach_time.h>
#define RTC
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

long elapsed (LARGE_INTEGER * t1, LARGE_INTEGER *t2);
typedef LARGE_INTEGER * TimeVal;

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
