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


#ifndef __TTCPListener__
#define __TTCPListener__

#include "InetTypes.h"
#include "TPacket.h"

class TTCPSocket;
//____________________________________________________________
class TTCPListener
{
	public:
		 	 TTCPListener () {}
	virtual ~TTCPListener () {}
	
		virtual void	Accept 	  (SocketRef sok, IPAddrPtr from) {};
		virtual void	Disconnect (long reason) = 0;
		virtual void	RcvPacket (GenericPacketPtr p, short len) = 0;
		virtual void	RcvEvent  (UPDEvent ev, SockResult result, void * infos) {}
		virtual void	SockError (SocketStatus err) {}
		virtual void	NetDisconnected () = 0;
};

#endif
