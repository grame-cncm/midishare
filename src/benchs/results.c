
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "results.h"

#define kIgnoreValues	0

static long gMax=0;
static long gCount=0;
static long * gTable = 0;

//____________________________________________________________
int bench_init (long benchlen)
{
	gCount = 0;
    gTable = (long *)malloc(sizeof(long) * benchlen);
    if (gTable) {
        gMax = benchlen;
        return 1;
    }
    else gMax = 0;
    return 0;
}

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
	if ((gCount < gMax) && (d > 0))
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
static int _print_result (FILE * to, FILE * sumfd, long start, long limit, long *table)
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
		fprintf (to, "%ld\n", d);
	}
	total = limit - start;
	fprintf (sumfd, "total calls achieved: %ld\n", total);
	fprintf (sumfd, "min time: %ld\n", min);
	fprintf (sumfd, "max time: %ld\n", max);
	fprintf (sumfd, "average : %f\n", avg = (float)sum / total);
	fprintf (sumfd, "std dev : %ld\n", std_dev(start, limit, table, (long)avg));
	return 1;
}

//____________________________________________________________
int print_result (FILE * to, FILE * sum)
{
    return _print_result (to, sum, kIgnoreValues, gCount, gTable);
}

//____________________________________________________________
int bench_done ()
{
    return (gCount < gMax) ? 0 : 1;
}
