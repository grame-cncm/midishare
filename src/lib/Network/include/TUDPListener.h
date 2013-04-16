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


#ifndef __TUDPListener__
#define __TUDPListener__

#include "InetTypes.h"
#include "TPacket.h"

//____________________________________________________________
class TUDPListener
{
	public:
		 	 TUDPListener () {}
	virtual ~TUDPListener () {}
	
		virtual void	RcvPacket (IPAddrPtr from, GenericPacketPtr p, short len) = 0;
		virtual void	RcvEvent  (UPDEvent ev, SockResult result, void * infos) {}
		virtual void	SockError (SocketStatus err) {}
};

#endif
