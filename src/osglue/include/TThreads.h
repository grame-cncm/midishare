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

#ifndef __TThreads__
#define __TThreads__

#ifdef WIN32

	#include <windows.h>
	#define ThreadAPI(proc, arg)	DWORD  WINAPI proc (LPVOID arg)

	typedef LPTHREAD_START_ROUTINE ThreadProcPtr;
	typedef HANDLE	ThreadHandle;
	
#else

	#include <pthread.h>
	#define ThreadAPI(proc, arg)	void * proc (void * arg)
	
	typedef void * ( * ThreadProcPtr) (void * ptr);
	typedef	pthread_t	ThreadHandle;

#endif

class TLog;
//___________________________________________________________________
// MidiShare oriented threads management:
//___________________________________________________________________
class TThreads
{
	public:
		// priorities are mapped to system priorities by the windows implementation
		// they directly represents pthreads priorities for other implementations
		enum { NormalPriority=0, 
			ClientHighPriority=49, ClientRTPriority=90, 
			ServerHighPriority=50, ServerRTPriority=99 };

		 	 TThreads (TLog * log = 0);
		 	 TThreads (ThreadProcPtr proc, void * arg, int priority, TLog * log = 0);
	virtual ~TThreads ()	{ Stop (); }
	
		int		Create (ThreadProcPtr proc, void * arg, int priority=NormalPriority);
		int		SetPriority (int priority);
		void	Stop();
		void	Join(int timeout=0);
		
		static int	MaskSigPipe ();

	private:
		int		MapPriority (int priority);

		ThreadHandle fThread;	// the thread handler
		TLog *		fLog;		// optionnal log capabilities
};

#ifndef WIN32
inline int	TThreads::MapPriority (int priority) { return priority; }
#endif

#endif
