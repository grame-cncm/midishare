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
