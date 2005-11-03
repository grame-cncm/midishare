/*
	Copyright: 	© Copyright 2002 Apple Computer, Inc. All rights reserved.
 
	Disclaimer:	IMPORTANT:  This Apple software is supplied to you by Apple Computer, Inc.
	("Apple") in consideration of your agreement to the following terms, and your
	use, installation, modification or redistribution of this Apple software
	constitutes acceptance of these terms.  If you do not agree with these terms,
	please do not use, install, modify or redistribute this Apple software.
 
	In consideration of your agreement to abide by the following terms, and subject
	to these terms, Apple grants you a personal, non-exclusive license, under Apple’s
	copyrights in this original Apple software (the "Apple Software"), to use,
	reproduce, modify and redistribute the Apple Software, with or without
	modifications, in source and/or binary forms; provided that if you redistribute
	the Apple Software in its entirety and without modifications, you must retain
	this notice and the following text and disclaimers in all such redistributions of
	the Apple Software.  Neither the name, trademarks, service marks or logos of
	Apple Computer, Inc. may be used to endorse or promote products derived from the
	Apple Software without specific prior written permission from Apple.  Except as
	expressly stated in this notice, no other rights or licenses, express or implied,
	are granted by Apple herein, including but not limited to any patent rights that
	may be infringed by your derivative works or by other works in which the Apple
	Software may be incorporated.
 
	The Apple Software is provided by Apple on an "AS IS" basis.  APPLE MAKES NO
	WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION THE IMPLIED
	WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A PARTICULAR
	PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND OPERATION ALONE OR IN
	COMBINATION WITH YOUR PRODUCTS.
 
	IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL OR
	CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
	GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
	ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION, MODIFICATION AND/OR DISTRIBUTION
	OF THE APPLE SOFTWARE, HOWEVER CAUSED AND WHETHER UNDER THEORY OF CONTRACT, TORT
	(INCLUDING NEGLIGENCE), STRICT LIABILITY OR OTHERWISE, EVEN IF APPLE HAS BEEN
	ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*
  Copyright © Grame 1999-2005

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

#include "msKernel.h"
#include "msPrefs.h"
#include "msTimeImpl.h"
#include <pthread.h>
#include <mach/mach_time.h>
#include <mach/thread_policy.h>
#include <mach/thread_act.h>
#include <CoreAudio/HostTime.h>

#define THREAD_SET_PRIORITY         0
#define THREAD_SCHEDULED_PRIORITY   1

extern TMSGlobalPtr gMem;
static mach_timebase_info_data_t gTBI;
static double gDateNanos, gInvRatio;
static unsigned long gTimeRes;
static pthread_t gThread = NULL;

static UInt32 GetThreadSetPriority(pthread_t thread);
static UInt32 GetThreadPriority(pthread_t thread, int inWhichPriority);
static int SetThreadToPriority(pthread_t thread, UInt32 inPriority, Boolean inIsFixed, UInt64 period, UInt64 computation, UInt64 constraint);

static int SetThreadToPriority(pthread_t thread, UInt32 inPriority, Boolean inIsFixed, UInt64 period, UInt64 computation, UInt64 constraint)
{
    if (inPriority == 96) {
        // REAL-TIME / TIME-CONSTRAINT THREAD
        thread_time_constraint_policy_data_t	theTCPolicy;
		theTCPolicy.period = AudioConvertNanosToHostTime(period);
        theTCPolicy.computation = AudioConvertNanosToHostTime(computation);
        theTCPolicy.constraint = AudioConvertNanosToHostTime(constraint);
        theTCPolicy.preemptible = true;
        kern_return_t res = thread_policy_set(pthread_mach_thread_np(thread), THREAD_TIME_CONSTRAINT_POLICY, (thread_policy_t) & theTCPolicy, THREAD_TIME_CONSTRAINT_POLICY_COUNT);
        return (res == KERN_SUCCESS) ? 0 : -1;
    } else {
        // OTHER THREADS
        thread_extended_policy_data_t theFixedPolicy;
        thread_precedence_policy_data_t thePrecedencePolicy;
        SInt32 relativePriority;
		theFixedPolicy.timeshare = !inIsFixed;
        thread_policy_set(pthread_mach_thread_np(thread), THREAD_EXTENDED_POLICY, (thread_policy_t)&theFixedPolicy, THREAD_EXTENDED_POLICY_COUNT);
		relativePriority = inPriority - GetThreadSetPriority(pthread_self());
		thePrecedencePolicy.importance = relativePriority;
        kern_return_t res = thread_policy_set(pthread_mach_thread_np(thread), THREAD_PRECEDENCE_POLICY, (thread_policy_t) & thePrecedencePolicy, THREAD_PRECEDENCE_POLICY_COUNT);
        return (res == KERN_SUCCESS) ? 0 : -1;
    }
}

static UInt32 GetThreadSetPriority(pthread_t thread)
{
    return GetThreadPriority(thread, THREAD_SET_PRIORITY);
}

static UInt32 GetThreadPriority(pthread_t thread, int inWhichPriority)
{
    thread_basic_info_data_t	threadInfo;
    policy_info_data_t	thePolicyInfo;
    unsigned int	count;

    // get basic info
    count = THREAD_BASIC_INFO_COUNT;
    thread_info(pthread_mach_thread_np(thread), THREAD_BASIC_INFO, (thread_info_t)&threadInfo, &count);

    switch (threadInfo.policy) {
        case POLICY_TIMESHARE:
            count = POLICY_TIMESHARE_INFO_COUNT;
            thread_info(pthread_mach_thread_np(thread), THREAD_SCHED_TIMESHARE_INFO, (thread_info_t)&(thePolicyInfo.ts), &count);
            if (inWhichPriority == THREAD_SCHEDULED_PRIORITY) {
                return thePolicyInfo.ts.cur_priority;
            } else {
                return thePolicyInfo.ts.base_priority;
            }
            break;

        case POLICY_FIFO:
            count = POLICY_FIFO_INFO_COUNT;
            thread_info(pthread_mach_thread_np(thread), THREAD_SCHED_FIFO_INFO, (thread_info_t)&(thePolicyInfo.fifo), &count);
            if ( (thePolicyInfo.fifo.depressed) && (inWhichPriority == THREAD_SCHEDULED_PRIORITY) ) {
                return thePolicyInfo.fifo.depress_priority;
            }
            return thePolicyInfo.fifo.base_priority;
            break;

        case POLICY_RR:
            count = POLICY_RR_INFO_COUNT;
            thread_info(pthread_mach_thread_np(thread), THREAD_SCHED_RR_INFO, (thread_info_t)&(thePolicyInfo.rr), &count);
            if ( (thePolicyInfo.rr.depressed) && (inWhichPriority == THREAD_SCHEDULED_PRIORITY) ) {
                return thePolicyInfo.rr.depress_priority;
            }
            return thePolicyInfo.rr.base_priority;
            break;
    }

    return 0;
}

static void* TimerTask(void* unused) 
{
	gDateNanos = mach_absolute_time() / gInvRatio;
	unsigned long call;
	while (1) {
		pthread_testcancel();
		call = gTimeRes;
		while (call--)
			ClockHandler(gMem); 
		gDateNanos += gTimeRes * 1000000; // In nanoseconds
		mach_wait_until(gDateNanos * gInvRatio);
	}
	return 0;
}

void OpenTimer(TMSGlobalPtr g)
{
	gTimeRes = LoadTimeRes();
	if (gTimeRes < 1) gTimeRes = 1;
	mach_timebase_info(&gTBI);
	gInvRatio = ((double) gTBI.denom) / ((double) gTBI.numer);  // Takes the inverse to use a * instead of a / in the TimerTask
	
	if (pthread_create(&gThread, NULL, TimerTask, NULL) == 0) {
		SetThreadToPriority(gThread, 96, true, gTimeRes * 1000000, 500 * 100, gTimeRes * 1000000); // Computation value is set to 500 us (like CoreMidi RT threads)
		ReportN("MidiShare", "open time interrupt using a timer - time resolution is", gTimeRes);
	} else {
		Report("MidiShare", "cannot create real-time thread","");
	}	
}

void CloseTimer(TMSGlobalPtr g)
{
	void* status;
	if (gThread) {
		pthread_cancel(gThread);
		pthread_join(gThread, &status);
	}
}
