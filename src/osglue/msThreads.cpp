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

#include "TThreads.h"
#include "msThreads.h"

static int siglist [] = {
	SIGABRT,
	SIGFPE,
	SIGILL,
	SIGINT,
	SIGSEGV,
	SIGTERM,
#ifndef WIN32
	SIGHUP,
	SIGQUIT,
	SIGBUS,
#endif
	0
};

//___________________________________________________________________
msThreadPtr msThreadCreate (ThreadProcPtr proc, void * arg, int priority)
{
	TThreads * thread = new TThreads ();
	if (thread) {
        int ret = thread->Create (proc, arg, priority);
		switch (ret) {
			case TThreads::SetPriorityFailed:
			case TThreads::ThreadNoErr:
				break;
			default:
				delete thread;
				thread = 0;
		}
	}
	return (msThreadPtr)thread;
}

//___________________________________________________________________
void msThreadDelete (msThreadPtr thread)
{
	delete (TThreads *)thread;
}

//___________________________________________________________________
void msThreadSigInit (SigProcPtr proc)
{
#ifndef WIN32
	int * sigs = siglist;
	struct sigaction sa;

	while (*sigs) {
		sa.sa_handler = proc;
		sigemptyset (&sa.sa_mask);
		sa.sa_flags = 0; 			//SA_RESETHAND;
		sigaction (*sigs++, &sa, 0);
	}
	sa.sa_handler = SIG_IGN;
	sigemptyset (&sa.sa_mask);
	sa.sa_flags = 0; 			//SA_RESETHAND;
	sigaction (SIGPIPE, &sa, 0);
#endif
}
