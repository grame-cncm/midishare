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
		 	 TMidiServer (short port, InetSvcRef ref = 0);
	virtual ~TMidiServer () {}
	
		// TTCPListener methods
		void	Accept 	  (SocketRef sok, IPAddrPtr from);
		void	Disconnect (long reason);
		void	RcvPacket (GenericPacketPtr p, short len);
		void	SockError (SocketStatus err);
		void	NetDisconnected ();
		
		void	RefuseCnx (SocketRef sok, short reason);
		void	SetControler (TWANControler * ctrl) { fControler = ctrl; }
		InetSvcRef GetInetRef ()	{ return fRefNum; }
		
	private:
		InetSvcRef 	fRefNum;
		TWANControler * fControler;
};

#endif
