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
#include "TLog.h"

#define RTSchedPolicy SCHED_RR

typedef struct {
	void * 			arg;
	ThreadProcPtr	proc;
} BaseThreadArg, * BaseThreadArgPtr;


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
TThreads::TThreads (TLog * log)
{
	fThread = 0;
	fLog = log;
}

//_____________________________________________________________________
TThreads::TThreads (ThreadProcPtr proc, void * arg, int priority, TLog * log)
{
	fThread = 0;
	fLog = log;
	Create (proc, arg, priority);
}

//_____________________________________________________________________
int	TThreads::SetPriority (int priority)
{
	if (fThread) {
		struct sched_param param; int ret;
		char * msg = "";
		
		param.sched_priority = priority;
		if (priority) {
			struct passwd *p;
			uid_t saved_uid = getuid ();

			p = getpwnam ("root");
			setuid (p->pw_uid); 
		  	ret = pthread_setschedparam (fThread, RTSchedPolicy, &param);
			setuid (saved_uid);
		  	if (!ret) return true;
			msg = "pthread_setschedparam RTSchedPolicy failed:";
		}
		else {
			if (!pthread_setschedparam (fThread, SCHED_OTHER, &param)) 
				return true;
			msg = "pthread_setschedparam SCHED_OTHER failed:";
		}
		if (fLog) fLog->WriteErr (msg);
	}
	return false;
}

//_____________________________________________________________________
int	TThreads::Create (ThreadProcPtr proc, void * arg, int priority)
{
	int ret;
	BaseThreadArgPtr argPtr = (BaseThreadArgPtr)malloc (sizeof(BaseThreadArg));
	if (!argPtr) {
		if (fLog) fLog->WriteErr ("malloc in TThreads::Create failed:");
		return false;
	}
	argPtr->arg = arg;
	argPtr->proc= proc;
	ret = pthread_create(&fThread, NULL, baseThreadProc, argPtr);
	if (!ret) {
		SetPriority (MapPriority(priority));
		return true;	
	}
	free (argPtr);
	if (fLog) fLog->WriteErr ("pthread_create failed:");
	return false;	
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
	// MacOSX pthread_cancel not implemented in Darwin 5.5
#endif
		fThread = 0;
	}
}
