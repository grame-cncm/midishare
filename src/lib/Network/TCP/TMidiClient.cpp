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
	fSkewTask.Forget();
	delete fAddr;
}

//________________________________________________________________________
void TMidiClient::SetRcvAlarm (short refNum, Boolean on)
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
