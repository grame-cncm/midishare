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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "benchs.h"

#define kMaxValues		10100
#define kIgnoreValues	100

static long gCount=0;
static long gTable[kMaxValues];

#ifndef RTC
//____________________________________________________________
long elapsed (TimeVal t1, TimeVal t2)
{
	long sec = t1->tv_sec - t2->tv_sec;
	long usec = t1->tv_usec - t2->tv_usec;
	return sec ? (sec * 1000000) + usec : usec;
}
#endif

//____________________________________________________________
void _storeTime (TimeVal t1, TimeVal t2)
{
    long d = elapsed(t2, t1);
	if ((gCount < kMaxValues) && (d > 0))
		gTable[gCount++] = d;
}

//____________________________________________________________
static long std_dev (long start, long limit, long *table, long avg)
{
	double sum = 0; long dev, i;
        for (i=start; i < limit; i++) {
		dev = table[i] - avg;
		sum += dev * dev;
 	}
        sum /= limit - start;
        return (long)sqrt(sum);
}

#if defined(__APPLE__) && defined(__MACH__)
//____________________________________________________________
static float time_ratio ()
{
        mach_timebase_info_data_t info; kern_return_t ret;
        float ratio;

        ret = mach_timebase_info(&info);
        ratio = (float)info.numer/info.denom;
        return ratio / 1000;
}
#else
#define time_ratio() 1.0
#endif

//____________________________________________________________
static int _print_result (FILE* fd, long start, long limit, long *table)
{
 	long i, d, min=0xffff, max=0, sum=0, total;
    float avg;
    float ratio = time_ratio();

	if (!limit) return 0;
	for (i=start; i < limit; i++) {
		table[i] = (long)(table[i]*ratio);
		d = table[i];
		if (d < min) min = d;
		else if (d > max) max = d;
		sum += d;
		fprintf (fd, "%ld\n", d);
	}
	total = limit - start;
	fprintf (fd, "total calls achieved: %ld\n", total);
	fprintf (fd, "min time: %ld\n", min);
	fprintf (fd, "max time: %ld\n", max);
	fprintf (fd, "average : %f\n", avg = (float)sum / total);
	fprintf (fd, "std dev : %ld\n", std_dev(start, limit, table, (long)avg));
	return 1;
}

//____________________________________________________________
int print_bench ()
{
	static char  buff [1024]; FILE * fd;
	const char* home = getenv("HOME");
	int ret = 0;

	if (home)
		sprintf (buff, "%s/midishare-bench.txt", home);
	else
		sprintf (buff, "/midishare-bench.txt");

    fd = fopen(buff, "w");
	if (fd) {
		ret = _print_result (fd, kIgnoreValues, gCount, gTable);
		fclose(fd);
	}
	return ret;
}

//____________________________________________________________
void bench_reset ()
{
	gCount = 0;
}

//____________________________________________________________
int bench_done ()
{
    return (gCount < kMaxValues) ? 0 : 1;
}
