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
