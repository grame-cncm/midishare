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


#ifndef __InetTypes__
#define __InetTypes__

//________________________________________________________________________
typedef struct {
	unsigned long local;
	unsigned long remote;
} PeerTimes, *PeerTimesPtr;

typedef struct {
	short	port;
	short	maxlat;
	short	groupTime;
} NetConfig, *NetConfigPtr;

typedef struct {
	short version;
	short maxlat;
	short maxLatOver;
	unsigned long missing;
} CnxReport, *CnxReportPtr;

enum {  kDefaultLANPort=4950, kDefaultLANLatency = 10, kDefaultLANGroupTime = 10,
	kDefaultWANPort=4951, kDefaultWANLatency = 1500, kDefaultWANGroupTime = 200 };

#define kWANVersion		100

#if macintosh
//________________________________________________________________________
// macintosh types definition
//________________________________________________________________________
#include <OpenTptInternet.h>

typedef InetHost		IPNum;
typedef InetAddress 	IPAddr, * IPAddrPtr;
typedef EndpointRef 	SocketRef;
typedef OSStatus 		SocketStatus;
typedef OTEventCode 	UPDEvent;
typedef OTResult 		SockResult;
typedef unsigned char 	ErrString[255];
typedef unsigned char 	NameStr[kMaxHostNameLen];
typedef unsigned char * strPtr;

#define IPField(a)		(a).fHost
#define IPType(a)		(a).fAddressType
#define IPPort(a)		(a).fPort
#define PIPField(a)		(a)->fHost
#define PPortField(a)	(a)->fPort
#define ntohl(v)	(v)
#define ntohs(v)	(v)
#define htonl(v)	(v)
#define htons(v)	(v)

#else
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
typedef short 	InetSvcRef;
typedef char 	ErrString[255];
typedef char 	NameStr[32];
typedef char * 	strPtr;

typedef int 				Boolean;
typedef unsigned long		IPNum;
typedef struct sockaddr_in 	IPAddr, * IPAddrPtr;
typedef int					SocketRef;
typedef int 				SocketStatus;
typedef long 				UPDEvent;
typedef long 				SockResult;

#endif

#endif
