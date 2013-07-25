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


#include "TPacket.h"
#include "HostTools.h"
#include "TSocket.h"

#define kMachineNameID	-16413		// systeme resource
#define kUserNameID		-16096		// systeme resource


//____________________________________________________________
// TPacket
//____________________________________________________________
TPacket::TPacket (char type, short len)
{
	fPacket.header.magic   = htons(kMagic);
	fPacket.header.version = 1;
	SetType (type);
	SetLength (len);
}

//____________________________________________________________
bool TPacket::Check (msPacketHeaderPtr p)
{
	return (p->magic == ntohs(kMagic));
}

//____________________________________________________________
SocketStatus TPacket::Send (TSocket *sok, IPAddrPtr to)
{
	short len = Length();
	return sok->Send (to, Buffer(), &len);
}

//____________________________________________________________
SocketStatus TPacket::Send (TSocket *sok)
{
	short len = Length();
	return sok->Send (0, Buffer(), &len);
}

//____________________________________________________________
// TTCPByePacket
//____________________________________________________________
TTCPByePacket::TTCPByePacket (short maxlat, short maxlatOver, unsigned long missing)
	: TPacket (kTCPBye, sizeof(short)*3 + sizeof(unsigned long))
{
	TCPByePacketPtr p = Packet();
	p->version = htons (kWANVersion);
	p->maxLatency = htons (maxlat);
	p->maxLatOver = htons (maxlatOver);
	p->missing = htonl (missing);
}

//____________________________________________________________
// TCnxRefusedPacket
//____________________________________________________________
TCnxRefusedPacket::TCnxRefusedPacket (short reason)
	: TPacket (kTCPCnxRefused, sizeof(short))
{
	CnxRefusedPacketPtr p = Packet();
	p->reason = htons (reason);
}

//____________________________________________________________
// TIDPacket
//____________________________________________________________
TIDPacket::TIDPacket () : TPacket (kHostID, 0)
{
	short i;
	char *dest, *s = HostTools::HostName ();
	IDPacketPtr p = Packet();
	dest = p->name;
	for (i=0; *s; i++) *dest++= *s++;
	*dest = 0;
	SetLength (sizeof(unsigned long) + i + 1);
}

//____________________________________________________________
SocketStatus TIDPacket::Send (TSocket *sok, IPAddrPtr to)
{
	SetTime();
	return TPacket::Send (sok, to);
}

//____________________________________________________________
// TEchoPacket
//____________________________________________________________
SocketStatus TEchoPacket::Send (TSocket *sok, IPAddrPtr to)
{
	SetTime();
	return TPacket::Send (sok, to);
}

//____________________________________________________________
long TEchoPacket::GetSecs ()
	{ return Packet()->time.sec; }

//____________________________________________________________
long TEchoPacket::GetUSecs ()
	{ return Packet()->time.usec; }

