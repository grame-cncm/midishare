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


#ifndef __TNetInfos__
#define __TNetInfos__

#include "InetTypes.h"

//____________________________________________________________
class TNetInfos
{
	public:
		 	 TNetInfos (short port);
	virtual ~TNetInfos () {}
	
			bool		Available () 		{ return fError == noErr; }
			SocketStatus Error ()			{ return fError; }
			IPAddrPtr	LocalAddress () 	{ return &fLocal; }
			IPNum		LocalIP () 			{ return IPField(fLocal); }
			IPAddrPtr	BroadcastAddress () { return &fBroadcast; }
			IPNum		BroadcastIP () 		{ return IPField(fBroadcast); }
			short		MTU () 				{ return fMTU; }
		
	private:		
		IPAddr 	fLocal;
		IPAddr 	fBroadcast;
		short	fMTU;
		SocketStatus fError;
};

#endif
