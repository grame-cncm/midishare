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
#include "msServerContext.h"
#include "msMem.h"
#include "msLog.h"
#include "msTasks.h"
#include "msPortAudio.h"

#if defined(linux)
#include "msThreads.h"
msThreadPtr gTimeThread = 0;

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
		{ 0,    0 },  // 0 ms time resolution
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
	int fd, ret; unsigned long freq;

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
	LogWrite ("RTC time is running at %ld hz.", freq);
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
		h->timeRes = res;
		LogWrite ("Opening time interrupt in RTC mode: time resolution changed to %d", res);
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

#ifdef WIN32

typedef struct {
	 UINT 	wTimerID;
	 UINT 	wTimerRes;
} WinTimer, * WinTimerPtr;
WinTimer gWinTimer;
/*__________________________________________________________________________*/
/*	Interrupt handlers														*/
/*__________________________________________________________________________*/
static void CALLBACK TimerProc(UINT wID,UINT wMsg,DWORD dwUser,DWORD dw1,DWORD dw2)
{
	ClockHandler((TMSGlobalPtr)dwUser);
}

void OpenMMTimeInterrupts(TMSGlobalPtr g)
{
	TIMECAPS tc;
	MMRESULT res;
	WinTimerPtr t = &gWinTimer;

	if ( timeGetDevCaps(&tc,sizeof(TIMECAPS)) == TIMERR_NOERROR ) {
		t->wTimerRes = 1;
		res = timeBeginPeriod (t->wTimerRes);	

		if (res != TIMERR_NOERROR) return;
		t->wTimerID = timeSetEvent(1,t->wTimerRes,TimerProc,(DWORD)g,TIME_PERIODIC);
	}
}

/*_________________________________________________________________________*/
void CloseMMTimeInterrupts(TMSGlobalPtr g)
{
	MMRESULT res;
	WinTimerPtr t = &gWinTimer;

	if (t->wTimerID ) {
		res = timeKillEvent(t->wTimerID);
		t->wTimerID = 0;
	}
	timeEndPeriod (t->wTimerRes);
}

#endif

/*__________________________________________________________________________*/
/*      Interrupt handlers                                                                                                              */
/*__________________________________________________________________________*/
void OpenTimeInterrupts(TMSGlobalPtr g)
{
	msServerContextPtr c = (msServerContextPtr)g->context;
	msKernelPrefs * prefs = c->prefs;
	THorlogePtr clock = &g->clock;
	
    clock->timeMode = prefs->timeMode;
    clock->timeRes = prefs->timeRes;
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
#ifndef WIN32
		case kTimeModeAudio:
            OpenPortAudio (g, prefs ? prefs->audioDev : 0);
			break;
#else
		case kTimeModeMMSystem:
			OpenMMTimeInterrupts (g);
			break;
#endif
		default:
			LogWrite ("Can't open time interrupts: invalid time mode %d\n", (int)clock->timeMode);
	}
}

/*_________________________________________________________________________*/
void CloseTimeInterrupts(TMSGlobalPtr g)
{
	THorlogePtr clock = &g->clock;
	
	switch (clock->timeMode) {
#ifdef linux
		case kTimeModeRTC:
			if (gTimeThread) msThreadDelete (gTimeThread);
			close_rtc (gRTCfd);
			break;
#endif
#ifndef WIN32
		case kTimeModeAudio:
            ClosePortAudio ();
			break;
#else
		case kTimeModeMMSystem:
			CloseMMTimeInterrupts (g);
			break;
#endif
		default:
			LogWrite ("Can't close time interrupts: invalid time mode %d\n", (int)clock->timeMode);
	}
}

