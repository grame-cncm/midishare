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


#include <string.h>
#include <stdio.h>

#include "TInetAddress.h"

//____________________________________________________________
TInetAddress::TInetAddress (InetSvcRef ref)
{
	fRefNum = ref;
	Initialize();
}

//____________________________________________________________
TInetAddress::TInetAddress (char *host, InetSvcRef ref)
{
	fRefNum = ref;
	SetAddress (host);
}

//____________________________________________________________
TInetAddress::TInetAddress (IPNum ip, InetSvcRef ref)
{
	fRefNum = ref;
	SetAddress (ip);
}

//____________________________________________________________
Boolean TInetAddress::SetAddress (IPNum ip)
{
	Initialize();
	return SetAddress (IP2String(ip));
}

//____________________________________________________________
void TInetAddress::NameCopy (char *dst, char *src)
{
	short i = 0;
	while (*src && (i < kMaxHostNameLen)) {
		*dst++ = *src++;
		i++;
	}
	*dst = 0;
}

//____________________________________________________________
void TInetAddress::MakeAddress (char *name, char *real, void *ipa)
{
	fAddr.fAddressType = AF_INET;
	fAddr.fPort = 0;
	fAddr.fHost = *(IPNum *)ipa;
	NameCopy (fHostName, name);
	if (real) NameCopy (fRealName, real);
}

//____________________________________________________________
Boolean TInetAddress::SetAddress (unsigned char *host)
{
	char str[256];
	short i, n = *host;
	for (i=0; i < n; i++) {
		str[i] = host[i+1];
	}
	str[i] = 0;
	return SetAddress (str);
}

//____________________________________________________________
Boolean TInetAddress::SetAddress (char *host)
{
	IPNum ip;
	
	Initialize();
	ip = String2IP(host);
	if (ip) {
		InetDomainName name; 
		if (OTInetAddressToName (fRefNum, ip, name) == noErr)
			MakeAddress (name, name, &ip);
		else
			MakeAddress (host, 0, &ip);
	}
	else {
		InetHostInfo infos = { 0};
		OSStatus err = OTInetStringToAddress(fRefNum, host, &infos);
		if (err == noErr)
			MakeAddress (host, infos.name, &infos.addrs[0]);
		else return false;
	}
	return true;
}

//____________________________________________________________
char * TInetAddress::IP2String (IPNum num)
{
	static char str[32];
	OTInetHostToString (num,  str);
	return str;	
}

//____________________________________________________________
IPNum TInetAddress::String2IP (char * str)
{
	InetHost host;
	OSErr err = OTInetStringToHost(str, &host);
	return (err == noErr) ? host : 0;
}

//____________________________________________________________
void TInetAddress::Initialize ()
{
	IPAddr ad = { 0 };
	fHostName[0] = 0;
	fRealName[0] = 0;
	fAddr = ad;
	fAddr.fAddressType = AF_INET;
}
