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
		virtual	SocketStatus	Open (Boolean local);
		virtual	void			Close ();
		virtual	SocketStatus	Send (IPAddrPtr to, void *buff, short *len);
		virtual	SocketStatus	Listen (IPAddrPtr from, void *buff, short *len);

#if macintosh
		virtual void	NotifyProc (OTEventCode code, OTResult result, void* cookie);
#else
		virtual void	ThreadLoop ();
#endif
		
	private:
		SocketStatus	SetOptions (Boolean local);
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
