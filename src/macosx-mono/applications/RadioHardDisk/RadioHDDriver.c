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


#include <Gestalt.h>
#include <OpenTransport.h>

#include "MidiShare.h"
#include "msInetDriverAppl.h"
#include "HostTools.h"

#include "TMidiServer.h"
#include "TInetControler.h"
#include "TWANControler.h"

#include "RadioHDFeedback.h"

/* ----------------------------------*/
/* constants definitions             */
unsigned char * radio_hd_server = "\pradio-hd.grame.fr";

TInetControler * gControl = 0;
TMidiServer *	 gServer = 0;
InetSvcRef		 gRef = 0;

RadioHDFeedback * gFeedback;
TWANControler *  gWAN = 0;

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
	MidiName name = InetDriverName ("\pradio-hd-driver", params->net.port, kDefaultWANPort);
	msg = CheckName (name);
	if (msg) return msg;
	
	short mode = params->driverMode ? TInetControler::kDriverMode : TInetControler::kApplMode;
	if (gControl) delete gControl;	gControl = 0;
	if (gServer) delete gServer;	gServer = 0;
	gServer = new TMidiServer (params->net.port, gRef);
	if (gServer) {
		gWAN = new TWANControler (gServer, &params->net, name, 0, mode);
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
void Connect (Str255 remote)
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
	short ref = MidiGetNamedAppl ("\pQuickTime Driver");
	if (ref > 0) {
#if defined(macintosh) && defined(MidiSharePPC_68k)
		long ref = MidiGetIndSlot (ref, 1);
		SlotRefNum sref = *(SlotRefNum *)&ref;
#else
		SlotRefNum sref = MidiGetIndSlot (ref, 1);
#endif
		if (MidiIsSlotConnected (0, sref)) {
			long t = MidiGetTime() + 15;
			MidiConnect (gControl->GetRefNum(), 0, true);
			MidiSendIm (gControl->GetRefNum(), MidiNewEv (typeReset));
			while (MidiGetTime() < t)
				;
		}
	}
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
	static Boolean connected = false;
	static long t = 0;
#ifndef CustomRadio
	if (!connected) {
		if (!t) t = MidiGetTime() + 1000;
		else if (MidiGetTime() > t) {
			Connect (radio_hd_server);
			connected = true;
		}
	}
#endif
	if (gControl)
		gControl->DoIdle();
	if (gFeedback)
		gFeedback->DoIdle();
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
