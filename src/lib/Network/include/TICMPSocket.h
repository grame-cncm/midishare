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
