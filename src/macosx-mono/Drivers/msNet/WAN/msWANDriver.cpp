/*

  Copyright © Grame 2001

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

*/

#include "MidiShare.h"
#include "msNetDriver.h"
#include "NetTools.h"
#include "TWANControler.h"
#include "TMidiRemote.h"
#include "TMidiServer.h"
#include "OSXFeedback.h"
#include "profport.h"
#include <unistd.h>

/* ----------------------------------*/
TWANControler * gControl = 0;
Boolean * 		gAutoQuit;
pthread_t		gIdleThread = 0;
OSXFeedback *	gFeedback = 0;
TMidiServer *	gServer = 0;
Boolean doneFlag = false, launched=false;

extern char * Targets;
extern char * NetSectionName;
extern char * profileName;

//_________________________________________________________
char * NextCnx (char *ptr, Boolean first);
static void LaunchTargets()
{
	char buff[1024], *next, *ptr;
	int n = get_private_profile_string (NetSectionName, Targets,
					"", buff, 1024, MakeMSFullName (profileName));
	if (n) {
		while (MidiGetTime() <  10) usleep (10000);
		ptr = NextCnx (buff, true);
		while (ptr) {
			next = NextCnx (ptr, false);
			if (next) {
				next[-1] = 0;
				Connect (ptr);
				next[-1] = ' ';
			}
			else Connect (ptr);
			ptr = next;
		}
	}
}

//_______________________________________________________________________
static void * IdleThread (void * ptr)
{
	int old;
	TInetControler * inet = (TInetControler *)ptr;
	pthread_setcancelstate (PTHREAD_CANCEL_DEFERRED, &old);
	pthread_setcanceltype (PTHREAD_CANCEL_ASYNCHRONOUS, &old);
	while (!doneFlag) {
		if (!launched && (MidiGetTime() > 10)) {
			launched = true;
			LaunchTargets ();
		}
		inet->DoIdle();
		usleep(2000);
	}
	return 0;
}

//_______________________________________________________________________
static pthread_t ThreadCreate (TInetControler * inet)
{
	pthread_t thread;
	int  ret = pthread_create(&thread, NULL, IdleThread, inet);
	if (ret) {
		INetAlert alert;
		alert.Report ("Cant initalize driver","pthread create failed", "", ret);
	}
	return thread;
}

/* -----------------------------------------------------------------------------*/
strPtr SetUpDriver (StatePtr params)
{
	strPtr msg;
	MidiName name = InetDriverName (strWANName, params->net.port, kDefaultWANPort);
	msg = CheckName (name);
	if (msg) return msg;

	short mode = params->driverMode ? TInetControler::kDriverMode : TInetControler::kApplMode;
	gAutoQuit = &params->autoQuit;
	if (gControl) delete gControl;	gControl = 0;
	if (gServer) delete gServer;	gServer = 0;
	gServer = new TMidiServer (params->net.port, 0);
	gFeedback = new OSXFeedback ();
	if (gServer && gFeedback) {
		gControl = new TWANControler (gServer, &params->net, name, params->maxClients, mode);
		if (gControl) {
			gControl->SetFeedback (gFeedback);
			if (!gControl->Start ()) {
				CloseInetDriver ();
				return "cannot start Internet server";
			}
			else {
				gIdleThread = ThreadCreate (gControl);
				if (!gIdleThread) {
					CloseInetDriver ();
					return "CreateThread failed";
		}
			}
		}
	}
	if (!gControl || !gServer || !gFeedback) {
		CloseInetDriver ();
		return strMemFail;
	}
	return 0;
}

/* -----------------------------------------------------------------------------*/
void CloseInetDriver ()
{
	doneFlag = true;
	if (gIdleThread) {
		void *threadRet;
		pthread_cancel (gIdleThread);
		usleep (10000);
		pthread_join (gIdleThread, &threadRet);
		gIdleThread = 0;
	}
	if (gControl) delete gControl;
	if (gServer)  delete gServer;
	if (gFeedback)  delete gFeedback;
	gControl = 0;
	gServer = 0;
	gFeedback = 0;
}

/* -----------------------------------------------------------------------------*/
void Disconnect (long remoteID)
{
	if (gControl && remoteID) {
		TMidiRemote * r = (TMidiRemote *)remoteID;
		gControl->Disconnect (r->GetID());
	}
}

/* -----------------------------------------------------------------------------*/
void Connect (char * remote)
{
	if (gControl) gControl->Connect (remote);
}
