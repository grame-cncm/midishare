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


#ifndef __TTCPSocket__
#define __TTCPSocket__

#if !macintosh
#if WIN32
#include <windows.h>
#else
#include <pthread.h> 
#endif
#endif

#include "TSocket.h"
#include "TTCPListener.h"

//____________________________________________________________
class TTCPSocket : public TSocket
{
	public:
	typedef enum { kTCPClientMode=1, kTCPServerMode } TCPSocketMode;

		 	 TTCPSocket (short port);
		 	 TTCPSocket (short port, TTCPListener * client);
	virtual ~TTCPSocket ();
	
		void			SetListener (TTCPListener *client)	{ fTCPClient = client; }
		TTCPListener *	GetListener ()						{ return fTCPClient; }
		
		virtual SocketStatus	Open ();
		virtual void			Close ();
		virtual SocketStatus	Send (void *buff, short *len);
		virtual SocketStatus	Send (IPAddrPtr to, void *buff, short *len) 
								{ return Send (buff, len); }
		virtual SocketStatus	Listen (IPAddrPtr from, void *buff, short *len);

#if macintosh
			void	NotifyProc (OTEventCode code, OTResult result, void* cookie);
#endif

	protected:
		virtual SocketStatus	SetOptions ();
		TTCPListener *	fTCPClient;
		
#if macintosh
		OSStatus	Disconnect	(Boolean orderly);
		OSStatus	Listen 		(IPAddrPtr from, SocketRef ref, void *buff, short *len);
#elif WIN32
		DWORD	ThreadCreate (TCPSocketMode mode);
		HANDLE	fThread;
		DWORD	fThreadID;
		BOOL	fRunFlag;
#else
		int			ThreadCreate (TCPSocketMode mode);
		pthread_t	fThread;
#endif
};

//____________________________________________________________
class TTCPServer : public TTCPSocket
{
	public:
		 	 TTCPServer (short port) : TTCPSocket(port) {}
	virtual ~TTCPServer () {}
		
	virtual SocketStatus	ServerOpen (short maxClients);
			short			MaxClients ()	{ return fMaxClients; }
			void			AcceptLoop ();
	virtual SocketStatus	SetOptions ();
			SocketStatus	SendTo (SocketRef to, void *buff, short *len);

	protected:
#if macintosh
			OSStatus	Accept (IPAddrPtr addr, SocketRef *ref);
#endif
		short			fMaxClients;
};

//____________________________________________________________
class TTCPClient : public TTCPSocket
{
	public:
		 	 TTCPClient (short port) : TTCPSocket(port) { fMTU = kDefaultMTU; }
		 	 TTCPClient (SocketRef ref, IPAddrPtr addr, TTCPListener * client);
	virtual ~TTCPClient () {}
		
			SocketStatus	ClientOpen (IPAddrPtr addr);
			short			GetMTU ()		{ return fMTU; }
#if !macintosh
			void	ListenLoop ();
#endif

	protected:
		enum { kDefaultMTU = 512 };
		short	fMTU;
		void	SetMTU ();
#if macintosh
		OSStatus	Connect (IPAddrPtr to);
#endif
};

#endif
