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


#ifndef __TPacket__
#define __TPacket__

#include "InetTypes.h"
#include "TimeTools.h"
#include "MidiShare.h"

enum {
	kHostID=1,	// host identification packet
	kBye,		// host leaving the network
	kEcho,
	kEchoReply,
	kMidi,
	kTCPBye,
	kTCPCnxRefused
};

#define kMaxHostName	256
#define kMaxMTU 		1500
#define kWANMTU 		512
#define kIPHeaderSize 	24
#define kUDPHeaderSize 	8
#define kPacketHeaderSize		kIPHeaderSize + kUDPHeaderSize
#define kMaxPacketSize	kMaxMTU - kPacketHeaderSize
#define kWANPacketSize	kWANMTU - kPacketHeaderSize
#define kMaxPacketBuff	kMaxPacketSize - sizeof(msPacketHeader)
#define kWANPacketBuff	kWANPacketSize - sizeof(msPacketHeader)

#define kMagic	0x1001

// reasons for refused connexions
enum { kTooMuchClients = 1, kAlreadyConnected, kServerError };

class TSocket;

//____________________________________________________________
typedef char msPacketType;
typedef struct {
	short 			magic;		// protocole id
	unsigned char	version;	// protocole version
	msPacketType	type;		// packet id
} msPacketHeader, *msPacketHeaderPtr;

typedef struct {
	msPacketHeader	header;
	char			buff[kMaxPacketSize];
} GenericPacket, * GenericPacketPtr;

typedef struct {
	msPacketHeader	header;
	unsigned long	time;
	char			name[kMaxHostName];
} IDPacket, * IDPacketPtr;

typedef struct {
	msPacketHeader	header;
	TimeInfo		time;
} EchoPacket, * EchoPacketPtr;

typedef struct {
	msPacketHeader	header;
	short			reason;
} CnxRefusedPacket, * CnxRefusedPacketPtr;

typedef struct {
	msPacketHeader	header;
	short			version;
	short			maxLatency;
	short			maxLatOver;
	unsigned long	missing;
} TCPByePacket, * TCPByePacketPtr;

//____________________________________________________________
class TPacket
{
	public:
		 TPacket (msPacketType type, short len);
	virtual ~TPacket () {}
	
			msPacketType	Type()		{ return fPacket.header.type; }
	virtual short		Length()	{ return fLength; }
			void *		Buffer()	{ return &fPacket; }

	virtual SocketStatus Send (TSocket *sok, IPAddrPtr to);
	virtual SocketStatus Send (TSocket *sok);

	static Boolean		Check (msPacketHeaderPtr p);

	protected:
		void	SetType (msPacketType type) 
			{ fPacket.header.type = type; }
		void	SetLength(short len) 
			{ fLength = len + sizeof(msPacketHeader); }

		GenericPacket  	fPacket;
		short		fLength;
};

//____________________________________________________________
class TByePacket : public TPacket
{
	public:
		 TByePacket () : TPacket (kBye, 0) {};
	virtual ~TByePacket () {}

};

//____________________________________________________________
class TCnxRefusedPacket : public TPacket
{
	public:
		 TCnxRefusedPacket (short reason);
	virtual ~TCnxRefusedPacket () {}
	
			CnxRefusedPacketPtr Packet () 	{ return (CnxRefusedPacketPtr)Buffer(); }
};

//____________________________________________________________
class TTCPByePacket : public TPacket
{
	public:
		 TTCPByePacket (short maxlat, short maxlatOver, unsigned long missing);
	virtual ~TTCPByePacket () {}

			TCPByePacketPtr Packet () 	{ return (TCPByePacketPtr)Buffer(); }
};

//____________________________________________________________
class TIDPacket : public TPacket
{
	public:
		 TIDPacket ();
	virtual ~TIDPacket () {}

	virtual SocketStatus Send (TSocket *sok, IPAddrPtr to);

			IDPacketPtr Packet () 	{ return (IDPacketPtr)Buffer(); }
			char * 		Name ()  	{ return Packet()->name; }
			void 		SetTime ()	{ Packet()->time = htonl(MidiGetTime()); }
};

//____________________________________________________________
class TEchoPacket : public TPacket
{
	public:
		 TEchoPacket () : TPacket (kEcho, sizeof(TimeInfo)) {}
	virtual ~TEchoPacket () {}
	

	virtual SocketStatus Send (TSocket *sok, IPAddrPtr to);

			EchoPacketPtr 	Packet () { return (EchoPacketPtr)Buffer(); }

			void 	SetTime ()		{ TimeTools::GetTime (&Packet()->time); }
			long	GetSecs ();
			long 	GetUSecs ();
};

#endif
