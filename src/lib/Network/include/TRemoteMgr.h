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

			Boolean       Refresh      (IPNum id, PeerTimesPtr times);
			TMidiRemote * FindOutSlot  (short slotRef);
			TMidiRemote * AddRemote (UPDParamsPtr params, RemoteCtrlParams * rp, short drvRef=0);

	private:	
		StreamMethods	fMethods;
};

//____________________________________________________________
inline TMidiRemote * TRemoteMgr::FindRemote (IPNum id)	
				{ return (TMidiRemote *)Find(id); }

#endif
