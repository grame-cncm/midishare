/*
  Copyright © Grame 2002,2003

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

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

#include "osglue.h"
#include "msServerInit.h"
#include "msServerContext.h"
#include "msLog.h"

static int gRun = 1;

//___________________________________________________________________
// global initialization
//___________________________________________________________________
static cdeclAPI(void) msExit ()
{
	msServerContextPtr c = ServerContext;
	gRun = 0;
	if (c->meetingPointThread) msThreadDelete (c->meetingPointThread);
	c->meetingPointThread = 0;
	if (c->meetingPoint) CloseMeetingPoint (c->meetingPoint);
	c->meetingPoint = 0;
	if (c->sharedmem) msSharedMemDelete (c->sharedmem);
	c->sharedmem = 0;
}

void CleanState ()
{
	void *ptr;
	SharedMemHandler shm = msSharedMemOpen  (kShMemId, &ptr);
	if (shm) msSharedMemForceDelete (shm);
	DeleteMeetingPoint ();
	CleanCommunicationChannels ();
}

void * InitShMem (int shmemSize)
{
	void * shmem = msSharedMemCreate(kShMemId, shmemSize);
	if (!shmem) {
		LogWriteErr ("Can't initialize the shared memory segment");
		return 0;
	}
	atexit (msExit);
	return shmem;
}

//___________________________________________________________________
// meeting point management thread
//___________________________________________________________________
static ThreadProc(mainServerCom, ptr)
{
	msServerContextPtr c = ServerContext;
	int tolerate = 3;
	NewClientProcPtr newclientproc = (NewClientProcPtr)ptr;

	c->meetingPoint = CreateMeetingPoint ();
	if (!c->meetingPoint) {
		LogWriteErr ("MeetingPoint thread: can't create the meeting point");
		goto err;
	}

	while (gRun) {
		CommunicationChan cc = HandleCommunicationChannelRequest(c->meetingPoint);
		if (cc) {
			newclientproc(cc);
		}
		else if (!tolerate--) goto err;
	}
	return 0;
err:
	if (gRun) LogWriteErr ("MeetingPoint thread: chan request error");
	msExit ();
	exit (1);
}

int InitMeetingPoint (TMSGlobalPtr g, NewClientProcPtr proc)
{
	msServerContextPtr c = (msServerContextPtr)g->context;
	if (!proc) return 0;

	c->meetingPointThread = msThreadCreate (mainServerCom, proc, kNormalPriority);
	if (!c->meetingPointThread) {
		LogWriteErr ("InitMeetingPoint: can't create a new thread");
		return 0;
	}
	return 1;
}

void msServerClose ()
{
	msExit();
}
