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
			bool		Named ()	{ return fRealName[0]; }
			char *		IP2String () { return IP2String (GetIP()); }
	static	char *		IP2String (IPNum num);
	static	IPNum		String2IP (char * str);

		virtual bool	SetAddress (unsigned char *host);
		virtual bool	SetAddress (char *host);
		virtual bool	SetAddress (IPNum ip);
		
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
