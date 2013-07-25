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


#ifndef __TMidiServer__
#define __TMidiServer__

#include "TTCPListener.h"
#include "TTCPSocket.h"

class TMidiClient;
class TWANControler;
//____________________________________________________________
class TMidiServer : public TTCPListener, public TTCPServer
{
	public:
		 	 TMidiServer (short port, InetServiceRef ref = 0);
	virtual ~TMidiServer () {}
	
		// TTCPListener methods
		void	Accept 	  (SocketRef sok, IPAddrPtr from);
		void	Disconnect (long reason);
		void	RcvPacket (GenericPacketPtr p, short len);
		void	SockError (SocketStatus err);
		void	NetDisconnected ();
		
		void	RefuseCnx (SocketRef sok, short reason);
		void	SetControler (TWANControler * ctrl) { fControler = ctrl; }
		InetServiceRef GetInetRef ()	{ return fRefNum; }
		
	private:
		InetServiceRef 	fRefNum;
		TWANControler * fControler;
};

#endif
