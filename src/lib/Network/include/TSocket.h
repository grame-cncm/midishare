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


#ifndef __TSocket__
#define __TSocket__

#include "InetTypes.h"
#include "TPacket.h"

//____________________________________________________________
class TSocket
{
	public:
		 	 TSocket (short port)	{ fPort=port; fRef=0; fBind=false; }
	virtual ~TSocket ()				{}
			
				bool			Opened ()	{ return fRef != 0; }
				short			GetPort () 	{ return fPort; }
				SocketRef		GetRefNum()	{ return fRef; }
		virtual	SocketStatus	Open () 	= 0;
		virtual	void			Close ()	= 0;
		virtual	SocketStatus	Send (IPAddrPtr to, void *buff, short *len)		= 0;
		virtual	SocketStatus	Listen (IPAddrPtr from, void *buff, short *len)	= 0;

	protected:
		SocketRef 		fRef;
		short			fPort;
		bool			fBind;
		GenericPacket	fPacket;
};

#endif
