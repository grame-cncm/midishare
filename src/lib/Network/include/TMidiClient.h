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
			void			SetRcvAlarm (short refNum, bool on);
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
