/*

  Copyright © Grame 2001

  Grame Research Laboratory, 9, rue du Garet 69001 Lyon - France
  grame@grame.fr

*/


#ifndef __TInetAddress__
#define __TInetAddress__

#include "InetTypes.h"
#include "MidiShare.h"

//____________________________________________________________
class TInetAddress
{
	public:
		 TInetAddress ();
		 TInetAddress (char *host);
		 TInetAddress (IPNum ip);
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
		
		IPAddr 			fAddr;
		InetDomainName	fHostName;
		InetDomainName	fRealName;
};

#endif
