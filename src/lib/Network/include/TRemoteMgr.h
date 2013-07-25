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


#ifndef __TRemoteMgr__
#define __TRemoteMgr__

#include "InetTypes.h"
#include "lflifo.h"
#include "TMidiRemote.h"
#include "TRemote.h"

class TWANRemote;
class TMidiClient;
class InetCtrlDTask;

//____________________________________________________________
typedef struct RemoteList RemoteList, * RemotePtr;
struct RemoteList {
	RemotePtr		next;
	TRemote *		remote;
	unsigned long	timeOut;
};

//____________________________________________________________
class TRemoteList
{
	public:
			 	 TRemoteList ()	{ lfinit (&fRemote); }
		virtual ~TRemoteList ()	{ Clear (); }
	
		virtual TRemote *	Find		(IPNum id);
		virtual void 		Remove		(IPNum id);
		virtual void 		RemoveAll   ()		{ Clear(); }
		virtual RemotePtr	Add	(TRemote * remote);
		
				RemotePtr	Head()		{ return (RemotePtr)lfavail (&fRemote); }

	protected:	

	private:	
		void 		Clear ();
		void		Delete (RemotePtr r);
		RemotePtr 	Find (IPNum id, RemotePtr *previous);
		void		Free (RemotePtr r);
		RemotePtr	Allocate();		

		lifo		fRemote;
};

//____________________________________________________________
class TRemoteMgr : public TRemoteList
{
	public:
		enum { kInitTTL = 1500, kRemoteTTL = 500 };

			 	 TRemoteMgr ();
		virtual ~TRemoteMgr () {}
	
			TMidiRemote * FindRemote (IPNum id);
		
			void 	RcvAlarmLoop (short refNum);				
			void 	SetMode 	 (short mode, short refNum);				
			void 	CheckTimeOut (unsigned long date);				
			short 	RemoteCount ();

			bool		Refresh      (IPNum id, PeerTimesPtr times);
			TMidiRemote * FindOutSlot  (short slotRef);
			TMidiRemote * AddRemote (UPDParamsPtr params, RemoteCtrlParams * rp, short drvRef=0);

	private:	
		StreamMethods	fMethods;
};

//____________________________________________________________
inline TMidiRemote * TRemoteMgr::FindRemote (IPNum id)	
#ifdef WIN32
				{ return (TMidiRemote *)Find(id); }
#else
				{ return dynamic_cast<TMidiRemote *>(Find(id)); }
#endif

#endif
