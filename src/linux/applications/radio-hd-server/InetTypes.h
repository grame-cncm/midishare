/*

  Copyright © Grame 2001

  Grame Research Laboratory, 9, rue du Garet 69001 Lyon - France
  grame@grame.fr

*/


#ifndef __InetTypes__
#define __InetTypes__

//________________________________________________________________________
// linux types definition
//________________________________________________________________________
#include <netinet/in.h>

#define IPField(a)		(a).sin_addr.s_addr
#define IPType(a)		(a).sin_family
#define IPPort(a)		(a).sin_port
#define PIPField(a)		(a)->sin_addr.s_addr
#define PPortField(a)	(a)->sin_port
#define kMaxHostNameLen 255
#define noErr			0

typedef char 	InetDomainName[kMaxHostNameLen + 1];
typedef unsigned long		IPNum;
typedef struct sockaddr_in 	IPAddr, * IPAddrPtr;

#endif
