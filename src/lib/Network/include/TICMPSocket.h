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


#ifndef __TICMPSocket__
#define __TICMPSocket__

#if !macintosh
#include <pthread.h> 
#endif

#include "InetTypes.h"
#include "ICMPHeader.h"
#include "TICMPListener.h"

//____________________________________________________________
class TICMPSocket
{
	public:
		 	 TICMPSocket ();
	virtual ~TICMPSocket ();
	
		void			SetListener (TICMPListener *client)	{ fClient = client; }
		TICMPListener *	GetListener ()						{ return fClient; }
		
		Boolean 		Opened ()	{ return fRef != 0; }
		SocketRef		GetRefNum()	{ return fRef; }
		SocketStatus	Open ();
		void			Close ();
//		SocketStatus	Send (IPAddrPtr to, void *buff, short *len);
		SocketStatus	Listen (IPAddrPtr from, void *buff, short *len);
		
#if macintosh
		virtual void	NotifyProc (OTEventCode code, OTResult result, void* cookie);
#else
		virtual void	ThreadLoop ();
#endif
		
	private:
		TICMPListener *	fClient;
		SocketRef 		fRef;
		Boolean 		fBind;
		ICMPBuffer		fPacket;
#if macintosh
		TUnitData	fData;
#else
		int		ThreadCreate ();
		pthread_t	fThread;
#endif
};

#endif
