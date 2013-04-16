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
				void 	CheckCompletion	(IPNum ip, bool silently);
		
				void 	SetFeedback (FeedbackProvider * f)	{ fFeedback = f; }
				bool	Connect 	(strPtr remote);
				void 	Disconnect	(IPNum ip, bool remove=true);
				bool	CreateClient (SocketRef sok, TInetAddress * addr);
				bool	IsClient 	 (IPNum ip);
				bool	AddRemote 	 (TMidiClient *client);
				void	CloseClient (IPNum ip, CnxReportPtr report);
				void	CnxRefused  (strPtr host);
				void	RefuseCnx   (strPtr host, short reason)	{ fFeedback->CnxRefused (host, reason); }
	
				bool	Start ()	{ return UWakeup (); }

	protected:
		virtual bool	UWakeup (bool udpMode = false);	// deferred wake up
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
