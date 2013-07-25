/*
  MidiShare Project
  Copyright (C) Grame 2001-2002
  Copyright (C) Mil Productions 2001

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/


#ifndef __HostTools__
#define __HostTools__

#if macintosh
#include <Resources.h>
#include <TextUtils.h>

#define kMachineNameID	-16413		// systeme resource
#define kUserNameID		-16096		// systeme resource

#elif WIN32
#include <winsock.h>

#else
#include <unistd.h>

#endif

#include "InetTypes.h"

//________________________________________________________________________
class HostTools
{
	public:
		enum { kMaxName	= 256 };

		static	char * HostName ();
		static	strPtr HostName (char *host);
#if macintosh
		static StringPtr PStrCat (Str255 s1, Str255 s2, Str255 dst);
#endif
};

#endif
