/*

  Copyright © Grame 2002

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
  
  modifications history:

*/

#include <stdio.h>

#ifdef linux
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <sys/ioctl.h>
	#include <fcntl.h>
	#include <unistd.h> 
	#include <linux/rtc.h>
#endif

#include "msExtern.h"
#include "msKernel.h"
#include "msKernelPrefs.h"
#include "msMem.h"
#include "msServerInit.h"
#include "msTasks.h"
#include "msThreads.h"


msThreadPtr gTimeThread = 0;

//#include "dlfcn.h"
//#include "msPrefs.h"
//#include "portaudio.h"

#if defined(linux)

	typedef struct {
		unsigned long  freq;
		unsigned short rate;
	} rtcTimeInfo;

	typedef struct {
		float tick;
		float elapsed;
		short expected;
	} rtcState;

	static rtcTimeInfo rtcTimeTbl[] = {
		{ 1024, 1 },  // 1 ms time resolution
		{ 512,  1 },  // 2 ms time resolution
		{ 0,    0 },  // 3 ms time resolution (not suppported)
		{ 256,  1 },  // 4 ms time resolution
		{ 0,    0 },  // 5 ms time resolution (not suppported)
		{ 512,  3 },  // 6 ms time resolution
		{ 0,    0 },  // 7 ms time resolution (not suppported)
		{ 128,  1 },  // 8 ms time resolution
		{ 0,    0 },  // 9 ms time resolution (not suppported)
		{ 512,  5 }   // 10 ms time resolution
	};
		
	static void * RTCTimeHandler (void * ptr);
	static int open_rtc (rtcTimeInfo * t);
	static void close_rtc (int fd);

	static int gRTCfd = -1;
//____________________________________________________________
static int open_rtc (rtcTimeInfo * t)
{
	int fd, ret; unsigned long freq; char msg[512];

	fd = open ("/dev/rtc", O_RDONLY);
	if (fd == -1) {
		LogWriteErr ("Opening /dev/rtc failed");
		goto error;
	}

	ret = ioctl (fd, RTC_IRQP_SET, t->freq);
	if (ret == -1) {
		LogWriteErr ("Setting RTC freq. failed");
		goto error;
	}

	ret = ioctl (fd, RTC_IRQP_READ, &freq);
	if (ret == -1) {
		LogWriteErr ("Reading RTC freq. failed");
		goto error;
	}

	ret = ioctl (fd, RTC_PIE_ON, 0);
	if (ret == -1) {
		LogWriteErr ("Starting RTC clock failed (RTC_PIE_ON)");
		goto error;
	}
	sprintf (msg, "RTC time is running at %ld hz.", freq);
	LogWrite (msg);
	return fd;
	
error:
	if (fd != -1) close (fd);
	return -1;
}

//____________________________________________________________
static int RTCOpen (TMSGlobalPtr g)
{
	THorloge * h = &g->clock;
	rtcTimeInfo * t; int attempts = 3;
	int res;
	
	if ((h->timeRes < 1) || (h->timeRes > 10))
		h->timeRes = 2;
		
	res = h->timeRes;
	do {
		t = &rtcTimeTbl[res];
		if (!t->freq) 
			res = (res < 10) ? res + 1 : 10;
		else break;
	} while (attempts--);
	
	if (!t->freq) { 
		LogWrite ("Opening time interrupt in RTC mode failed: cannot find a supported freq.");
		return -1;
	}
	if (res != h->timeRes) {
		char msg [512];
		sprintf (msg, "Opening time interrupt in RTC mode: time resolution changed to %d", res);
		h->timeRes = res;
	}
	return open_rtc (t);
}

//____________________________________________________________
static void close_rtc (int fd)
{
	if (fd == -1) return;
	ioctl (fd, RTC_PIE_OFF, 0);
	close (fd);
}

//____________________________________________________________
static void * RTCTimeHandler (void * ptr)
{
	TMSGlobalPtr g = (TMSGlobalPtr)ptr;
	THorloge * h = &g->clock; 
	rtcTimeInfo * t = &rtcTimeTbl[h->timeRes];
	rtcState state;
	
	state.tick = 1000.0 / t->freq;
	state.elapsed = 0;
	state.expected = t->rate;
	while (true) {
		unsigned long tmp=0;
		int ret = read (gRTCfd, &tmp, sizeof(unsigned long));
		if (ret == -1) {
			LogWriteErr ("rtc read error");
			break;
		}
		state.elapsed += state.tick;
		while (state.elapsed > state.expected) {
			state.elapsed -= state.tick * t->rate;
			ClockHandler (g);
		}
	}
	return 0;
}
#endif

/*__________________________________________________________________________*/
/*      Interrupt handlers                                                                                                              */
/*__________________________________________________________________________*/
void OpenTimeInterrupts(TMSGlobalPtr g)
{
	THorlogePtr clock = &g->clock;
	char msg[512];
	
	switch (clock->timeMode) {
#ifdef linux
		case kTimeModeRTC:
			gRTCfd = RTCOpen (g);
			if (gRTCfd == -1) CloseTimeInterrupts (g);
			else {
				gTimeThread = msThreadCreate (RTCTimeHandler, g, kServerRTPriority);
				if (!gTimeThread) CloseTimeInterrupts (g);
			}
			break;
#endif
		case kTimeModeAudio:
			break;
#ifdef WIN32
		case kTimeModeMMSystem:
			break;
#endif
		default:
			sprintf (msg, "Can't open time interrupts: invalid time mode %d\n", (int)clock->timeMode);
			LogWrite (msg);
	}
}

/*_________________________________________________________________________*/
void CloseTimeInterrupts(TMSGlobalPtr g)
{
	THorlogePtr clock = &g->clock;
	char msg[512];
	
	switch (clock->timeMode) {
#ifdef linux
		case kTimeModeRTC:
			if (gTimeThread) msThreadDelete (gTimeThread);
			close_rtc (gRTCfd);
			break;
#endif
		case kTimeModeAudio:
			break;
#ifdef WIN32
		case kTimeModeMMSystem:
			break;
#endif
		default:
			sprintf (msg, "Can't open time interrupts: invalid time mode %d\n", (int)clock->timeMode);
			LogWrite (msg);
	}
}

