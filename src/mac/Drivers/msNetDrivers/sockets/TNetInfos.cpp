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


#include "TNetInfos.h"

//____________________________________________________________
TNetInfos::TNetInfos (short port)
{
	InetInterfaceInfo info;
	fError = OTInetGetInterfaceInfo (&info, kDefaultInetInterface);
	if (fError == noErr) {
		IPNum mask = 0xffffffff & ~info.fNetmask;
		OTInitInetAddress (&fLocal, port, info.fAddress);
		OTInitInetAddress (&fBroadcast, port, info.fAddress | mask);
		fMTU = info.fIfMTU;
	}
	else {
		fLocal.fHost = fBroadcast.fHost = 0;
		fMTU = 0;
	}
}
