
#ifndef __results__
#define __results__

#if defined(__APPLE__) && defined(__MACH__)
# include <mach/mach_time.h>
# define RTC
#else 
# include <sys/time.h>
#endif

#ifdef RTC

typedef double TimeVal;
typedef double TimeType;
#define elapsed(t2, t1)	(t2 - t1)
#define getTime(t)	t=mach_absolute_time()
#define storeTime(t1,t2)	_storeTime(t1, t2)

#elif defined(WIN32)

long elapsed (LARGE_INTEGER * t1, LARGE_INTEGER *t2);
typedef LARGE_INTEGER * TimeVal;

#else

typedef struct timeval * TimeVal;
typedef struct timeval TimeType;
long elapsed (TimeVal t1, TimeVal t2);
#define getTime(t)	gettimeofday(&t, 0)
#define storeTime(t1,t2)	_storeTime(&t1, &t2)

#endif

// common declarations to define the bench length and the start time
#define kBenchLen		1500
#define kStartTime		1500
#define kTimeRes		1

void _storeTime (TimeVal t1, TimeVal t2);
int print_result (FILE * to, FILE * sum);
int bench_init (long benchlen);
int bench_done ();
long micro (long value);

#endif
