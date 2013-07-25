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


#ifndef __TUDPSocket__
#define __TUDPSocket__

#if !macintosh
#if WIN32
#include <winsock.h>
#else
#include <pthread.h>
#endif
#endif

#include "TSocket.h"
#include "TUDPListener.h"

//____________________________________________________________
class TUDPSocket : public TSocket
{
	public:
		 	 TUDPSocket (short port);
	virtual ~TUDPSocket ();
	
		void			SetListener (TUDPListener *client)	{ fClient = client; }
		TUDPListener *	GetListener ()						{ return fClient; }
		
		virtual	SocketStatus	Open ()		{ return Open (true); }
		virtual	SocketStatus	Open (bool local);
		virtual	void			Close ();
		virtual	SocketStatus	Send (IPAddrPtr to, void *buff, short *len);
		virtual	SocketStatus	Listen (IPAddrPtr from, void *buff, short *len);

#if macintosh
		virtual void	NotifyProc (OTEventCode code, OTResult result, void* cookie);
#else
		virtual void	ThreadLoop ();
#endif
		
	private:
		SocketStatus	SetOptions (bool local);
		TUDPListener *	fClient;
#if macintosh
//		TUnitData	fData;
#elif WIN32
		DWORD	ThreadCreate ();
		HANDLE	fThread;
		DWORD	fThreadID;
		BOOL	fRunFlag;
#else
		int		ThreadCreate ();
		pthread_t	fThread;
#endif
};

#endif
