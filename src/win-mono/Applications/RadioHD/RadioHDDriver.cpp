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


#include <Windows.h>

#include "MidiShare.h"
#include "msInetDriverAppl.h"

#include "TMidiServer.h"
#include "TInetControler.h"
#include "TWANControler.h"

#include "RadioHDFeedback.h"

/* ----------------------------------*/
/* constants definitions             */
char * radio_hd_server = "radio-hd.grame.fr";

TInetControler * gControl = 0;
TMidiServer *	 gServer = 0;

extern RadioHDFeedback * gFeedback;
TWANControler *  gWAN = 0;

/* -----------------------------------------------------------------------------*/
static strPtr TCPOpen ()
{
	int ret; WORD version; WSADATA data;
	version = MAKEWORD (2,2);
	if (WSAStartup (version, &data))
		return "cannot initialize Internet services";
	return 0;
}

/* -----------------------------------------------------------------------------*/
static MidiName InetDriverName (MidiName baseName, short port, short defaultPort)
{
	static char name [256];
	if (port == defaultPort) {
		wsprintf (name, "%s", baseName);
	}
	else {
		char num [32];
		itoa (port, num, 10);
		wsprintf (name, "%s:%s", baseName, num);
	}
	return name;
}

/* -----------------------------------------------------------------------------*/
static strPtr CheckName (MidiName name)
{
	static char msg[256];
	if (MidiGetNamedAppl (name) > 0) {
		wsprintf (msg, "%s is already running", name);
		return msg;
	}
	return 0;
}

/* -----------------------------------------------------------------------------*/
strPtr SetUpDriver (StatePtr params, FeedbackProvider * f)
{
	strPtr msg = TCPOpen ();
	if (!msg) msg = ReStart (params, f);
	return msg;
}

strPtr ReStart (StatePtr params, FeedbackProvider * f)
{
	strPtr msg;
	MidiName name = InetDriverName ("radio-hd-driver", params->net.port, kDefaultWANPort);
	msg = CheckName (name);
	if (msg) return msg;
	
	short mode = params->driverMode ? TInetControler::kDriverMode : TInetControler::kApplMode;
	if (gControl) delete gControl;	gControl = 0;
	if (gServer) delete gServer;	gServer = 0;
	gServer = new TMidiServer (params->net.port, 0);
	if (gServer) {
		gWAN = new TWANControler (gServer, &params->net, name, 0, mode);
		if (gWAN) {
			gWAN->SetFeedback (f);
			gControl = gWAN;
			if (!gWAN->Start ())
				return "cannot start Internet server";
		}
	}
	if (!gControl) {
		CloseInetDriver ();
		return strMemFail;
	}
	return 0;
}

/* -----------------------------------------------------------------------------*/
void Disconnect (long remoteID)
{
	if (gWAN && remoteID) {
		TMidiRemote * r = (TMidiRemote *)remoteID;
		gWAN->RemoveRemote (r->GetID());
	}
}

/* -----------------------------------------------------------------------------*/
void Connect (char * remote)
{
	if (gWAN) gWAN->Connect (remote);
}

/* -----------------------------------------------------------------------------*/
Boolean SetDriverMode (Boolean on)
{
	if (gControl) {
		gControl->SetMode (on ? TInetControler::kDriverMode : TInetControler::kApplMode);
		return true;
	}
	return false;
}

/* -----------------------------------------------------------------------------*/
void CloseInetDriver ()
{
	if (gControl) delete gControl;
	if (gServer)  delete gServer;
	gControl = 0;
	gServer = 0;
	WSACleanup ();
}

/* -----------------------------------------------------------------------------*/
void DoIdle()
{
	static Boolean connected = false;
	static long t = 0;
	if (!connected) {
		if (!t) t = MidiGetTime() + 1000;
		else if (MidiGetTime() > t) {
			Connect (radio_hd_server);
			connected = true;
		}
	}
	if (gControl)
		gControl->DoIdle();
	if (gFeedback)
		gFeedback->DoIdle();
}
