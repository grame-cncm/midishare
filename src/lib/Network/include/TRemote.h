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


#ifndef __TRemote__
#define __TRemote__

#include "InetTypes.h"
//____________________________________________________________
class TRemote
{
	public:
			 	 TRemote () {}
		virtual ~TRemote () {}
	
		virtual IPNum	GetID () = 0;
};

//____________________________________________________________
class TRemoteListener
{
	public:
			 	 TRemoteListener () {}
		virtual ~TRemoteListener () {}
	
		virtual void	SetRcvAlarm (short refNum, bool on) = 0;
};

#endif
