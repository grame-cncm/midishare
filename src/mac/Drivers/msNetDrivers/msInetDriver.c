/*

  Copyright © Grame 2001
  Copyright © Mil Productions 2001

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

#include <Gestalt.h>
#include <OpenTransport.h>

#include "MidiShare.h"
#include "msInetDriverAppl.h"
#include "HostTools.h"

#include "TMidiServer.h"
#include "TInetControler.h"
#include "TWANControler.h"

/* ----------------------------------*/
/* constants definitions             */
#define StateFile	"\pmsInet Driver State"	
enum { StateCreator = 'MsIn', StateType = 'stIn' };	

TInetControler * gControl = 0;
TMidiServer *	 gServer = 0;
InetSvcRef		 gRef = 0;
Boolean * 		 gAutoQuit;

#define kMaxTCPClients	32

/* -----------------------------------------------------------------------------*/
static strPtr TCPOpen ()
{
	OSStatus err = InitOpenTransport();
	if (err != noErr) return "\pcannot initialize Open Transport";

	gRef = OTOpenInternetServices(kDefaultInternetServicesPath, 0, &err);
	if (err != noErr) {
		CloseOpenTransport ();
		return "\pcannot initialize Internet services";
	}
	return 0;
}

/* -----------------------------------------------------------------------------*/
static MidiName InetDriverName (MidiName baseName, short port, short defaultPort)
{
	static Str255 name;
	if (port == defaultPort) {
		HostTools::PStrCat (baseName, "\p", name);
	}
	else {
		Str32 num;
		NumToString (port, num);
		HostTools::PStrCat (baseName, "\p:", name);
		HostTools::PStrCat (name, num, name);
	}
	return name;
}

/* -----------------------------------------------------------------------------*/
static strPtr CheckName (MidiName name)
{
	static Str255 msg;
	if (MidiGetNamedAppl (name) > 0) {
		HostTools::PStrCat (name, "\p is already running", msg);
		return msg;
	}
	return 0;
}

#ifdef __WANInet__
#include "MacFeedback.h"

extern MacFeedback * gFeedback;
TWANControler *  gWAN = 0;
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
	MidiName name = InetDriverName (strWANName, params->net.port, kDefaultWANPort);
	msg = CheckName (name);
	if (msg) return msg;
	
	short mode = params->driverMode ? TInetControler::kDriverMode : TInetControler::kApplMode;
	if (gControl) delete gControl;	gControl = 0;
	if (gServer) delete gServer;	gServer = 0;
	gServer = new TMidiServer (params->net.port, gRef);
	if (gServer) {
		gWAN = new TWANControler (gServer, &params->net, name, kMaxTCPClients, mode);
		if (gWAN) {
			gWAN->SetFeedback (f);
			gControl = gWAN;
			if (!gWAN->Start ())
				return "\pcannot start Internet server";
		}
	}
	if (!gControl) {
		CloseInetDriver ();
		return strMemFail;
	}
	gAutoQuit = &params->autoQuit;
	return 0;
}

/* -----------------------------------------------------------------------------*/
void Disconnect (long remoteID)
{
	if (gWAN && remoteID) {
		TMidiRemote * r = (TMidiRemote *)remoteID;
		gWAN->Disconnect (r->GetID());
	}
}

/* -----------------------------------------------------------------------------*/
void Connect (Str255 remote)
{
	if (gWAN) gWAN->Connect (remote);
}

#else

/* -----------------------------------------------------------------------------*/
strPtr SetUpDriver (StatePtr params)
{
	strPtr msg = TCPOpen ();
	if (!msg) msg = ReStart (params);
	return msg;
}

strPtr ReStart (StatePtr params)
{
	strPtr msg;
	MidiName name = InetDriverName (strLANName, params->net.port, kDefaultLANPort);
	msg = CheckName (name);
	if (msg) return msg;

	short mode = params->driverMode ? TInetControler::kDriverMode : TInetControler::kApplMode;
	if (gControl) delete gControl;
	gControl = new TInetControler(&params->net, name, mode);
	if (!gControl) {
		CloseInetDriver ();
		return strMemFail;
	}
	gAutoQuit = &params->autoQuit;
	return 0;
}

#endif

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
	if (gRef) {
		OTCloseProvider (gRef);
		CloseOpenTransport();
	}
}

/* -----------------------------------------------------------------------------*/
void DoIdle()
{
	if (gControl) {
		gControl->DoIdle();
#ifdef __WANInet__
		gFeedback->DoIdle();
#endif
		if (gControl->Sleeping() && *gAutoQuit) doneFlag = true;
	}
}

/* -----------------------------------------------------------------------------*/
/* Check for Open Transport                                                     */
/* -----------------------------------------------------------------------------*/
Boolean CheckOT ()
{
	long reply;
	OSErr err = Gestalt (gestaltOpenTpt, &reply);
	if ((err == noErr) && reply) {
		if (reply & gestaltOpenTptTCPPresentMask) {
			// gestaltOpenTptVersions implemented starting at OT version 1.1
			return Gestalt (gestaltOpenTptVersions, &reply) == noErr;
		}
	}
	return false;
}
