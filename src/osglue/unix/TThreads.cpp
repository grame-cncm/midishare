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

#include <pwd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include "TThreads.h"

#define RTSchedPolicy SCHED_RR

typedef struct {
	void * 			arg;
	ThreadProcPtr	proc;
} BaseThreadArg, * BaseThreadArgPtr;

#ifndef linux
#include <mach/thread_act.h>

//_____________________________________________________________________
static void sigActions (int sig)
{
printf ("signal %d received: pthread_exit\n", sig);
	pthread_exit (0);
}

static void setSigStop ()
{
	struct sigaction sa;
	sa.sa_handler = sigActions;
	sigemptyset (&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction (SIGSTOP, &sa, 0);
}
#endif

//_____________________________________________________________________
static void * baseThreadProc (void * ptr)
{
	int old;
	BaseThreadArgPtr argPtr = (BaseThreadArgPtr)ptr;
	ThreadProcPtr proc = argPtr->proc;
	void * arg = argPtr->arg;

	free (ptr);
	pthread_setcancelstate (PTHREAD_CANCEL_ENABLE, &old);
	pthread_setcanceltype (PTHREAD_CANCEL_ASYNCHRONOUS, &old);
	return proc (arg);
}

//_____________________________________________________________________
TThreads::TThreads ()
{
	fThread = 0;
}

//_____________________________________________________________________
TThreads::TThreads (ThreadProcPtr proc, void * arg, int priority)
{
	fThread = 0;
	Create (proc, arg, priority);
}

//_____________________________________________________________________
int	TThreads::SetPriority (int priority)
{
	if (fThread) {
		struct sched_param param; int ret;
		
		param.sched_priority = priority;
		if (priority) {
			struct passwd *p;
			uid_t saved_uid = getuid ();

			p = getpwnam ("root");
			setuid (p->pw_uid); 
		  	ret = pthread_setschedparam (fThread, RTSchedPolicy, &param);
			setuid (saved_uid);
		  	if (!ret) return true;
		}
		return true;
	}
	return false;
}

//_____________________________________________________________________
int	TThreads::Create (ThreadProcPtr proc, void * arg, int priority)
{
	int ret;
	BaseThreadArgPtr argPtr = (BaseThreadArgPtr)malloc (sizeof(BaseThreadArg));
	if (!argPtr) {
		return MemAllocFailed;
	}
	argPtr->arg = arg;
	argPtr->proc= proc;
	ret = pthread_create(&fThread, NULL, baseThreadProc, argPtr);
	if (!ret) {
        return SetPriority (MapPriority(priority)) ? ThreadNoErr : SetPriorityFailed;
	}
	free (argPtr);
	return CreateFailed;	
}

//_____________________________________________________________________
void TThreads::Join(int timeout)
{
	if (fThread) {
		void *threadRet; 
		pthread_join (fThread, &threadRet);
		fThread = 0;
	}
}

//_____________________________________________________________________
int TThreads::MaskSigPipe ()
{
	sigset_t set; int ret;
	
	ret = sigemptyset (&set);
	if (ret == -1)
		return false;
	ret = sigaddset (&set, SIGPIPE);
	if (ret == -1)
		return false;
	ret = sigprocmask (SIG_BLOCK, &set, 0);
	if (ret == -1)
		return false;
	return true;
}

//_____________________________________________________________________
void TThreads::Stop ()
{
	if (fThread) {
#ifdef linux
		void *threadRet; 
		pthread_cancel (fThread);
		pthread_join (fThread, &threadRet);
#else
	// MacOSX pthread_cancel not implemented in Darwin 5.5, 6.4
		mach_port_t machThread = pthread_mach_thread_np (fThread);
		thread_terminate (machThread);
#endif
		fThread = 0;
	}
}
