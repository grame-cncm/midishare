/*

  Copyright © Grame 2001-2002

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

#include <stdio.h>

#include "MidiShare.h"
#include "msInetDriverAppl.h"
#include "HostTools.h"
#include "NetTools.h"

#include "TMidiServer.h"
#include "TInetControler.h"
#include "TWANControler.h"

#include "RadioHDFeedback.h"

/* ----------------------------------*/
/* constants definitions             */
const char * radio_hd_server = "radio-hd.grame.fr";

TInetControler * gControl = 0;
TMidiServer *	 gServer = 0;
InetServiceRef		 gRef = 0;

RadioHDFeedback * gFeedback;
TWANControler *  gWAN = 0;

/* -----------------------------------------------------------------------------*/
strPtr SetUpDriver (StatePtr params, FeedbackProvider * f)
{
	strPtr msg;
	MidiName name = InetDriverName ("radio-hd-driver", params->net.port, kDefaultWANPort);
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
void Connect (const char * remote)
{
	if (gWAN) gWAN->Connect ((char *)remote);
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
	short ref = MidiGetNamedAppl ("QuickTime Driver");
	if (ref > 0) {
		SlotRefNum sref = MidiGetIndSlot (ref, 1);
		if (MidiIsSlotConnected (0, sref)) {
			MidiConnect (gControl->GetRefNum(), 0, true);
			MidiSendIm (gControl->GetRefNum(), MidiNewEv (typeReset));
			usleep (10000);
		}
	}
	if (gControl) delete gControl;
	if (gServer)  delete gServer;
	gControl = 0;
	gServer = 0;
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
