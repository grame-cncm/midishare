/*

  Copyright © Grame 2001

  Grame Research Laboratory, 9, rue du Garet 69001 Lyon - France
  grame@grame.fr

*/


#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "TInetAddress.h"

//____________________________________________________________
TInetAddress::TInetAddress ()
{
	Initialize();
}

//____________________________________________________________
TInetAddress::TInetAddress (char *host)
{
	SetAddress (host);
}

//____________________________________________________________
TInetAddress::TInetAddress (IPNum ip)
{
	SetAddress (ip);
}

//____________________________________________________________
Boolean TInetAddress::SetAddress (IPNum ip)
{
	struct in_addr ipa;
	ipa.s_addr = ip;
	Initialize();
	SetAddress (inet_ntoa (ipa));
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
Boolean TInetAddress::SetAddress (unsigned char *host)
{
	host[*host + 1] = 0;
	return SetAddress ((char *)&host[1]);
}

//____________________________________________________________
Boolean TInetAddress::SetAddress (char *host)
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
