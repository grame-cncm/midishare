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


#ifndef __TMidiClient__
#define __TMidiClient__

#include "TPacket.h"
#include "TRemote.h"
#include "TInetAddress.h"
#include "TInetTasks.h"
#include "TTCPListener.h"
#include "TTCPSocket.h"

class TWANControler;
//__________________________________________________________________________
class TMidiClient : public TTCPListener, public TTCPClient, 
					public TRemote, public TRemoteListener
{
	public:
		 	 TMidiClient (short port, TWANControler *ctrl, TInetAddress * addr);
		 	 TMidiClient (TWANControler *ctrl, SocketRef ref, TInetAddress * addr);
	virtual ~TMidiClient ();
	
			IPNum			GetID ()				{ return fAddr->GetIP(); }
			void			Start (short refNum)	{ fSkewTask.Start (refNum); }
			void			ReStart (short refNum)	{ fSkewTask.ReSchedule (refNum); }
		TCPActiveSensing *  SkewTask()				{ return &fSkewTask; }
			char *			GetName ()				{ return fAddr->HostName (); }
			void			SetRcvAlarm (short refNum, Boolean on);
			SocketStatus	Bye (short maxlat, short maxLatOver, unsigned long missing);

		// TTCPListener methods
		void	Disconnect (long reason);
		void	RcvPacket (GenericPacketPtr p, short len);
		void	RcvEvent  (UPDEvent ev, SockResult result, void * infos);
		void	SockError (SocketStatus err);
		void	NetDisconnected () {};

	private:
		void	CloseClient (TCPByePacketPtr p);

		TWANControler * fControler;
		TInetAddress *	fAddr;
		TCPActiveSensing fSkewTask;
};

#endif
