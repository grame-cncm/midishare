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


#ifndef __TInetAddress__
#define __TInetAddress__

#include "InetTypes.h"

//____________________________________________________________
class TInetAddress
{
	public:
		 TInetAddress (InetServiceRef ref=0);
		 TInetAddress (char *host, InetServiceRef ref=0);
		 TInetAddress (IPNum ip, InetServiceRef ref=0);
	virtual ~TInetAddress () {}
	
			IPAddrPtr	GetAddress () 	{ return &fAddr; }
			IPNum		GetIP () 	{ return IPField(fAddr); }
			char *		HostName ()	{ return fHostName; }
			char *		RealName ()	{ return fRealName; }
			Boolean		Named ()	{ return fRealName[0]; }
			char *		IP2String () { return IP2String (GetIP()); }
	static	char *		IP2String (IPNum num);
	static	IPNum		String2IP (char * str);

		virtual Boolean	SetAddress (unsigned char *host);
		virtual Boolean	SetAddress (char *host);
		virtual Boolean	SetAddress (IPNum ip);
		
	private:
		void	Initialize ();
		void	MakeAddress (char *name, char *real, void * ipa);
		void	NameCopy (char *dst, char *src);
		
		IPAddr 			fAddr;
		INetDomainName	fHostName;
		INetDomainName	fRealName;
		InetServiceRef	fRefNum;
};

#endif
