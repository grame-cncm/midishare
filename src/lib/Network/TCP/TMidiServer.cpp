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

#include "StrConstants.h"
#include "TMidiClient.h"
#include "TMidiServer.h"
#include "TUDPListener.h"
#include "TInetAddress.h"
#include "TWANControler.h"
#include "HostTools.h"
#include "TPacket.h"

//________________________________________________________________________
// TMidiServer
//________________________________________________________________________
TMidiServer::TMidiServer (short port, InetServiceRef ref)
	: TTCPServer (port)
{
	fRefNum = ref;
	SetListener (this);
}

//________________________________________________________________________
void TMidiServer::RefuseCnx (SocketRef sok, short reason) 
{
	TCnxRefusedPacket p(reason);
	short len = p.Length();
	SendTo (sok, p.Buffer(), &len);
}

//________________________________________________________________________
void TMidiServer::Accept (SocketRef sok, IPAddrPtr from) 
{
	TInetAddress * addr = new TInetAddress (PIPField(from), fRefNum);
	if (!fControler || !addr) {
		RefuseCnx (sok, kServerError);
	}
	else if (fControler->RemoteCount() >= MaxClients()) {
		RefuseCnx (sok, kTooMuchClients);
		fControler->RefuseCnx (HostTools::HostName (addr->HostName()), kTooMuchClients);
		delete addr;
	}
	else if (fControler->IsClient(PIPField(from))) {
		RefuseCnx (sok, kAlreadyConnected);
		fControler->RefuseCnx (HostTools::HostName (addr->HostName()), kAlreadyConnected);
		delete addr;
	}
	else if (!fControler->CreateClient (sok, addr)) {
		fControler->CreateRemoteFailed (strMemFail);
	}
}

//________________________________________________________________________
void TMidiServer::SockError (SocketStatus err) 
{
}

//________________________________________________________________________
void TMidiServer::Disconnect (long reason) 
{
}

//________________________________________________________________________
void TMidiServer::NetDisconnected ()
{
	fControler->CallFatalError (strNetDisconnect, 0);
}

//________________________________________________________________________
void TMidiServer::RcvPacket (GenericPacketPtr p, short len) 
{
	if (TPacket::Check (&p->header)) {	// check the packet header
		switch (p->header.type) {
			default:
				;
		}
	}
}
