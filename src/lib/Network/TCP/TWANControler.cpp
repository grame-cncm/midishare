/*
  MidiShare Project
  Copyright (C) Grame 2001
  Copyright (C) Mil Productions 2001

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <stdio.h>

#include "TWANControler.h"
#include "TInetAddress.h"
#include "TMidiServer.h"
#include "TMidiClient.h"
#include "FeedbackProvider.h"
#include "StrConstants.h"
#include "INetAlert.h"
#include "MidiTools.h"

//_________________________________________________________________________________
TWANControler::TWANControler (TMidiServer *server, NetConfigPtr net, MidiName name,
								short maxClients, short mode)
	: 	TInetControler (net, name, mode)
{
	fServer = server;
	fServer->SetControler (this);
	fFeedback = 0;
	fMaxClients = maxClients;
}

//_________________________________________________________________________________
TWANControler::~TWANControler ()
{
	Disconnect ();
	USleep ();
	fServer->Close ();
	fTCPRemote.RemoveAll();
}
	
//_________________________________________________________________________________
void TWANControler::DoIdle ()
{
#if macintosh
	fServer->AcceptLoop ();
#endif
	ExecDTasks ();
}
	
//_________________________________________________________________________________
void TWANControler::CallCheckRemote (TRemoteMgr *mgr)
{
}

//_________________________________________________________________________________
bool TWANControler::IsClient (IPNum ip)
{
	return FindUDPRemote (ip) != 0;
}

//_________________________________________________________________________________
bool TWANControler::Connect (strPtr remote)
{
	INetAlert alert; SocketStatus err; TMidiClient * client;

	TInetAddress * addr = new TInetAddress (fServer->GetInetRef ());
	if (!addr) {
		alert.Report (fDrvName, strConnectFailure, strMemFail, 0L);
		goto failed;
	}
	fFeedback->Lookup (remote);
	if (!addr->SetAddress (remote)) {				// looking for inet address
		alert.Report (fDrvName, strLookupFailure, remote, 0L);
		goto failed;
	}
	if (FindUDPRemote (addr->GetIP())) {	// check for existing connection
		alert.Report (fDrvName, strAlreadyOpened, remote, 0L);
		goto failed;
	}
	fFeedback->Connecting (addr->HostName());
	client = new TMidiClient (Net()->port, this, addr);	// creates TCP client
	if (!client) {
		alert.Report (fDrvName, strConnectFailure, strMemFail, 0L);
		goto failed;
	}

	err = client->ClientOpen (addr->GetAddress());		// opens TCP client
	if (err != noErr) {
		delete client;
		alert.Report (fDrvName, strConnectFailure, remote, 0L);
		goto failed;
	}

	if (!AddRemote (client)) {						// add client in TCP remote list
		alert.Report (fDrvName, strRemCreateFailure, strMemFail, 0L);
		goto failed;
	}
	fFeedback->CheckLatency (addr->HostName());
	client->Start (GetRefNum());				// start sending ID packets
	CallCheckCompletion (client->GetID(), kCompletionTimeout, false);
	return true;

failed:
	fFeedback->Failed (0);
	return false;
}

//_________________________________________________________________________________
void TWANControler::RemoveRemote (IPNum id)
{
	TMidiRemote * r = FindUDPRemote (id);
	if (r) fFeedback->Remove (r);
	TInetControler::RemoveRemote (id);
	fTCPRemote.Remove (id);
}

//_________________________________________________________________________________
void TWANControler::CloseClient (IPNum ip, CnxReportPtr report)
{
	TMidiRemote * r = FindUDPRemote (ip);
	if (r) {
		fFeedback->CloseReport (r, report);
	}
	CallRemoveRemote (ip);
}
	
//_________________________________________________________________________________
void TWANControler::CnxRefused (strPtr host)
{
	INetAlert alert;
	alert.Report (strConnectFailure, host, strBusyFailure, 0L);
	fFeedback->Failed (FeedbackProvider::kCnxRefused);
}

//_________________________________________________________________________________
void TWANControler::Disconnect ()
{
	RemotePtr next, rp = fTCPRemote.Head();
	while (rp) {
		next = rp->next;
		TMidiClient * client = (TMidiClient *)rp->remote;
		Disconnect (client->GetID(), false);
		rp = next;
	}
}

//_________________________________________________________________________________
void TWANControler::Disconnect (IPNum id, bool remove)
{
	TMidiClient * client = (TMidiClient *)fTCPRemote.Find (id);
	TMidiRemote * r = FindUDPRemote (id);
	if (client && r) {
		client->Bye (r->MaxLatency(), r->MaxLatencyOver(), r->Missing());
	}
	if (remove) CallRemoveRemote (id);
}

//_________________________________________________________________________________
char * TWANControler::RemoteName (IPNum ip, IDPacketPtr id)
{
	TMidiClient * client = (TMidiClient *)fTCPRemote.Find (ip);
	return client ? client->GetName() : 0;
}

//_________________________________________________________________________________
bool TWANControler::UWakeup (bool udpMode)
{
	INetAlert alert; SocketStatus err;
	if (!Sleeping()) return true;
	if (TInetControler::UWakeup (false)) {
		err = fServer->ServerOpen (fMaxClients);
		if (err != noErr) {
			alert.FatalError (fDrvName, strWakeUpFailure, strTCPSockOpenFailure, err);
			TInetControler::USleep ();
			return false;
		}
	}
	return true;
}

//_________________________________________________________________________________
void TWANControler::Bye ()
{
}

//_________________________________________________________________________________
void TWANControler::CheckCompletion  (IPNum id, bool silently)
{
	bool failed = false;
	TMidiRemote * r = FindUDPRemote (id);

	if (r) {
		if (!r->Opened()) {
			fTCPRemote.Remove (id);
			if (r) fRemoteMgr.Remove (id);
			failed = true;
		}
	}
	else {
		fTCPRemote.Remove (id);
	}
	if (failed) {
		if (!silently) {
			INetAlert alert;
			alert.Report (fDrvName, strConnectFailure, strNotResponding, 0L);
		}
		fFeedback->Failed (FeedbackProvider::kNotResponding);
	}
}

//_________________________________________________________________________________
TSocket *  TWANControler::GetSpecialSocket (IPNum ip)
{
#ifdef WIN32
	TSocket * s = (TSocket *)fTCPRemote.Find (ip);
#else
	TSocket * s = dynamic_cast<TSocket *>(fTCPRemote.Find (ip));
#endif
	return s;
}

//_________________________________________________________________________________
void TWANControler::OpenComplete  (IPNum id)
{
	TMidiRemote * r = FindUDPRemote (id);
	if (r) {
		TMidiClient * client = (TMidiClient *)fTCPRemote.Find (id);

		fFeedback->Complete (r);
		if (client) {
			r->SetListener (client);
			client->ReStart (GetRefNum());
		}
	}
	else RemoveRemote (id);
}

//_________________________________________________________________________________
bool TWANControler::AddRemote (TMidiClient * client)
{
	return (fTCPRemote.Add (dynamic_cast<TRemote *>(client)) != 0);
}

//_________________________________________________________________________________
bool TWANControler::CreateClient (SocketRef sok, TInetAddress * addr)
{
	TMidiClient * client = new TMidiClient (this, sok, addr);
	if (!client) return false;
	if (AddRemote (client)) {
		client->Start (GetRefNum());
		CallCheckCompletion (client->GetID(), kCompletionTimeout, true);
		return true;
	}
	delete client;
	return false;
}
