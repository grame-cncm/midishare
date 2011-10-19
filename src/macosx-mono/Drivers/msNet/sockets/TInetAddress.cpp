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
#include <arpa/inet.h>
#include <netdb.h>

#include "TInetAddress.h"

//____________________________________________________________
TInetAddress::TInetAddress (InetServiceRef ref)
{
	fRefNum = ref;
	Initialize();
}

//____________________________________________________________
TInetAddress::TInetAddress (char *host, InetServiceRef ref)
{
	fRefNum = ref;
	SetAddress (host);
}

//____________________________________________________________
TInetAddress::TInetAddress (IPNum ip, InetServiceRef ref)
{
	fRefNum = ref;
	SetAddress (ip);
}

//____________________________________________________________
bool TInetAddress::SetAddress (IPNum ip)
{
	struct in_addr ipa;
	ipa.s_addr = ip;
	Initialize();
	return SetAddress (inet_ntoa (ipa));
}

//____________________________________________________________
void TInetAddress::MakeAddress (char *name, char *real, void *ipa)
{
	fAddr.sin_family = AF_INET;
	fAddr.sin_addr = *(struct in_addr *)ipa;
	strcpy (fHostName, name);
	if (real) strcpy (fRealName, real);
}

//____________________________________________________________
bool TInetAddress::SetAddress (unsigned char *host)
{
	host[*host + 1] = 0;
	return SetAddress ((char *)&host[1]);
}

//____________________________________________________________
bool TInetAddress::SetAddress (char *host)
{
	struct hostent * info;
	struct in_addr ipa;
	
	Initialize();
	if ((ipa.s_addr = inet_addr(host)) != INADDR_NONE) {
		info = gethostbyaddr ((const char *)&ipa, sizeof(ipa), AF_INET);
		if (!info) MakeAddress (host, NULL, &ipa);
		else MakeAddress (info->h_name, info->h_name, info->h_addr);
	} 
	else {
		info = gethostbyname(host);
		if (info) MakeAddress (host, info->h_name, info->h_addr);
		else return false;
	}
	return true;
}

//____________________________________________________________
char * TInetAddress::IP2String (IPNum num)
{
	struct in_addr ipa;
	ipa.s_addr = num;
	return inet_ntoa (ipa);	
}

//____________________________________________________________
IPNum TInetAddress::String2IP (char * str)
{
	struct in_addr addr;
	return inet_aton (str, &addr) ? addr.s_addr : 0;
}

//____________________________________________________________
void TInetAddress::Initialize ()
{
	fHostName[0] = 0;
	fRealName[0] = 0;
	memset (&fAddr, 0, sizeof(fAddr));
}
