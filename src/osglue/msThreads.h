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

#ifndef __msThreads__
#define __msThreads__

#include "osglue.h"

#ifdef WIN32
#include <windows.h>
	typedef LPTHREAD_START_ROUTINE ThreadProcPtr;
#	define ThreadProc(proc, arg)	DWORD WINAPI proc(LPVOID arg)
#else
	typedef void * ( * ThreadProcPtr) (void * ptr);
#	define ThreadProc(proc, arg)	void * proc(void * arg)
#endif

typedef void * msThreadPtr;
typedef cdeclAPI(void) (* SigProcPtr)(int sig);


enum { 
	kNormalPriority=0, 
	kClientHighPriority=49, 
	kClientRTPriority=90, 
	kServerHighPriority=50,
	kServerLRTPriority=98,
	kServerRTPriority=99 
};

#ifdef __cplusplus
extern "C" {
#endif

msThreadPtr msThreadCreate (ThreadProcPtr proc, void * arg, int priority);
void        msThreadDelete (msThreadPtr thread);
void        msThreadSigInit(SigProcPtr proc);  // only to be called in server context

#ifdef __cplusplus
}
#endif

#endif
