/*

  Copyright © Grame 2001 - 2002
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
