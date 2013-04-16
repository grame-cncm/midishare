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
