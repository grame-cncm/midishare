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


#include <stdio.h>
#include <unistd.h>

#include "TNetInfos.h"
#include "TInetAddress.h"

extern int errno, h_errno;
//____________________________________________________________
TNetInfos::TNetInfos (short port)
{
	char name[256]; TInetAddress addr;
	int ret = gethostname(name, 255); 
	if (ret == -1) {
		fError = errno;
		return;
	}

	if (!addr.SetAddress (name)) {
		fError = h_errno;
		return;
	}
	IPField(fLocal) = addr.GetIP();
	fLocal.sin_family = fBroadcast.sin_family = addr.GetAddress()->sin_family;
	fLocal.sin_port = fBroadcast.sin_port = port;
	IPField(fLocal) = addr.GetIP();
	IPField(fBroadcast) = IPField(fLocal) | 0x000000ff;

	fMTU = 1500;
	fError = noErr;
}
