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

#ifndef WIN32
#include <signal.h>
#include <stdlib.h>
#endif

#include "TLog.h"
#include "msServerInit.h"
#include "msCommInit.h"
#include "TShMem.h"

#ifdef WIN32
#	define kDirSep		'\\'
#	define kLogName		"MidiShared.log"
#else
#	define kDirSep		'/'
#	define kLogName		"MidiShared.log"
#endif

typedef struct {
	ShMem * shmem;
	void * shmemPtr;
} ServerEnv, *ServerEnvPtr;

TLog 		gLog;
ServerEnv 	gEnv = { 0 };


//___________________________________________________________________
// global initialization
//___________________________________________________________________
#ifndef WIN32
static int siglist [] = {
	SIGHUP,
	SIGINT,
	SIGQUIT,
	SIGILL,
	SIGABRT,
	SIGBUS,
	SIGFPE,
	SIGSEGV,
	SIGTERM,
	0
};

static void sigActions (int sig)
{
	char buff[256];
	sprintf (buff, "signal %d received", sig);
	gLog.Write (buff);
	gLog.Write ("server exit...");
	exit (1);
}

static void msExit ()
{
	ServerMainComStop (0);
	msServerClose ();
}

static void setSigActions ()
{
	int * sigs = siglist;
	
	while (*sigs) {
		struct sigaction sa;
		sa.sa_handler = sigActions;
		sigemptyset (&sa.sa_mask);
		sa.sa_flags = 0; //SA_RESETHAND;
		sigaction (*sigs++, &sa, 0);
	}
}
#endif

int msServerInit (int shmemSize, int debug)
{
#ifndef WIN32
	setSigActions();
	atexit (msExit);
#endif
	if (!debug) {
		char buff[512], dir[400];
		sprintf (buff, "%s%c%s", TLog::DefaultDir(dir, 400), kDirSep, kLogName);
		gLog.Open (buff);
	}
	gEnv.shmem = new ShMem (&gLog);
	if (!gEnv.shmem) goto err;
	gEnv.shmemPtr = gEnv.shmem->Create (kShMemId, shmemSize);
	if (!gEnv.shmemPtr) goto err;
	return true;
	
err:
	msServerClose ();
	return false;
}

void msServerClose()
{
	if (gEnv.shmem) delete gEnv.shmem;
	gEnv.shmem = 0;
}

//___________________________________________________________________
// log facilities
//___________________________________________________________________
void LogWrite (const char *msg)
{
	gLog.Write (msg);
}

void LogWriteErr (const char *msg)
{
	gLog.WriteErr (msg);
}


//___________________________________________________________________
// shared memory
//___________________________________________________________________
void *	GetSharedMemory ()
{
	return gEnv.shmemPtr;
}
