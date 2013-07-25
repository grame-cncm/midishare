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

#include "TPacket.h"
#include "TMidiClient.h"
#include "TWANControler.h"
#include "HostTools.h"
#include "MidiTools.h"

//________________________________________________________________________
// TMidiClient
//________________________________________________________________________
TMidiClient::TMidiClient (short port, TWANControler *ctrl, TInetAddress * addr) 
	: TTCPClient(port), 
	  fSkewTask(ctrl->GetIDPacket(), addr->GetIP())
{
	SetListener (this);
	fControler = ctrl;
	fAddr = addr;
	fSkewTask.Initialize (ctrl->GetSocket());
}

//________________________________________________________________________
TMidiClient::TMidiClient (TWANControler *ctrl, SocketRef ref, TInetAddress * addr) 
	: TTCPClient(ref, addr->GetAddress(), this), 
	  fSkewTask(ctrl->GetIDPacket(), addr->GetIP())
{
	SetListener (this);
	fControler = ctrl;
	fAddr = addr;
	fSkewTask.Initialize (ctrl->GetSocket());
}

//________________________________________________________________________
TMidiClient::~TMidiClient () 
{
	SetListener (0);
	fSkewTask.Forget();
	delete fAddr;
}

//________________________________________________________________________
void TMidiClient::SetRcvAlarm (short refNum, bool on)
{
	if (on) {
		fSkewTask.ReSchedule (refNum);
	}
	else {
		fSkewTask.Forget ();
	}
}

//________________________________________________________________________
SocketStatus TMidiClient::Bye (short maxlat, short maxLatOver, unsigned long missing)
{
	TTCPByePacket bye (maxlat, maxLatOver, missing);
	SocketStatus err = bye.Send (this);
	return err;
}

//________________________________________________________________________
void TMidiClient::Disconnect (long reason) 
{
	fSkewTask.Forget();
	if (fControler) fControler->Disconnect (GetID());
	if (reason == kTCPCnxRefused) {
		fControler->CallCnxRefused (HostTools::HostName (fAddr->HostName()));
	}
}

//________________________________________________________________________
void TMidiClient::CloseClient (TCPByePacketPtr p)
{
	CnxReport report;
	report.version = ntohs(p->version);
	report.maxlat = ntohs(p->maxLatency);
	report.maxLatOver = ntohs(p->maxLatOver);
	report.missing = ntohl(p->missing);
	if (fControler) fControler->CloseClient (GetID(), &report);
}

//________________________________________________________________________
void TMidiClient::RcvPacket (GenericPacketPtr p, short len) 
{
	if (TPacket::Check (&p->header)) {	// check the packet header
		switch (p->header.type) {
			case kTCPBye:	CloseClient ((TCPByePacketPtr)p);
				break;
			case kMidi:		fControler->DispatchTCP (fAddr->GetIP(), p, len);
				break;
			case kTCPCnxRefused: 	Disconnect (kTCPCnxRefused);
				break;
			default:
				;
		}
	}
}

//________________________________________________________________________
void TMidiClient::RcvEvent  (UPDEvent ev, SockResult result, void * infos)
{
}

//________________________________________________________________________
void TMidiClient::SockError (SocketStatus err)
{
}
