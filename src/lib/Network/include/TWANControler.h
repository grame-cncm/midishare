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


#ifndef __TWANControler__
#define __TWANControler__

#include "TInetControler.h"
#include "TRemoteMgr.h"
#include "FeedbackProvider.h"

class TMidiServer;
class TWANRemote;
class TMidiClient;
class TInetAddress;
//_________________________________________________________________________________
class TWANControler : public TInetControler
{
	public:
		enum { kCompletionTimeout = 10000 };
		
			 	 TWANControler (TMidiServer *server, NetConfigPtr net, MidiName name, 
			 	 					short maxClients, short mode=kApplMode);
		virtual ~TWANControler ();
	
		virtual void 	DoIdle ();
		virtual void 	RemoveRemote (IPNum id);
		virtual void	OpenComplete  (IPNum id);
				void 	CheckCompletion	(IPNum ip, Boolean silently);
		
				void 	SetFeedback (FeedbackProvider * f)	{ fFeedback = f; }
				void 	Connect 	(strPtr remote);
				void 	Disconnect	(IPNum ip, Boolean remove=true);
				Boolean	CreateClient (SocketRef sok, TInetAddress * addr);
				Boolean	IsClient 	 (IPNum ip);
				Boolean	AddRemote 	 (TMidiClient *client);
				void	CloseClient (IPNum ip, CnxReportPtr report);
				void	CnxRefused  (strPtr host);
				void	RefuseCnx   (strPtr host, short reason)	{ fFeedback->CnxRefused (host, reason); }
	
				Boolean	Start ()	{ return UWakeup (); }

	protected:
		virtual Boolean UWakeup (Boolean udpMode = false);	// deferred wake up
		virtual void 	Bye ();
		virtual char *	RemoteName (IPNum ip, IDPacketPtr id);
		virtual IPNum 	GetSpecialIP ()				{ return 0; }
		virtual TSocket * GetSpecialSocket (IPNum ip);
		virtual short	LocalMTU ()	{ return kWANPacketBuff; }
		virtual void	CallCheckRemote  (TRemoteMgr *mgr);
				void 	Disconnect	();
	
	private:
		TMidiServer	*		fServer;
		FeedbackProvider * 	fFeedback;
		TRemoteList 		fTCPRemote;
		short				fMaxClients;
};

#endif
