/*
  MidiShare Project
  Copyright (C) Grame 2001-2002
  Copyright (C) Mil Productions 2001

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

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

# if defined(__MACH__) && (defined(__ppc__) || defined(__i386__))  && defined(__GNUC__)
# define __MacOSX__
#endif

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
typedef InetSvcRef		InetServiceRef;
typedef InetDomainName	INetDomainName;

#define IPField(a)		(a).fHost
#define IPType(a)		(a).fAddressType
#define IPPort(a)		(a).fPort
#define PIPField(a)		(a)->fHost
#define PPortField(a)	(a)->fPort
#define ntohl(v)	(v)
#define ntohs(v)	(v)
#define htonl(v)	(v)
#define htons(v)	(v)

#elif WIN32
//________________________________________________________________________
// Windows types definition
//________________________________________________________________________
#include <Winsock.h>

#define IPField(a)		(a).sin_addr.s_addr
#define IPType(a)		(a).sin_family
#define IPPort(a)		(a).sin_port
#define PIPField(a)		(a)->sin_addr.s_addr
#define PPortField(a)	(a)->sin_port
#define kMaxHostNameLen 255
#define noErr			0

typedef char 	INetDomainName[kMaxHostNameLen + 1];
typedef short 	InetServiceRef;
typedef char 	ErrString[255];
typedef char 	NameStr[32];
typedef char * 	strPtr;

//typedef unsigned char Boolean;
typedef unsigned long		IPNum;
typedef struct sockaddr_in 	IPAddr, * IPAddrPtr;
typedef SOCKET				SocketRef;
typedef int 				SocketStatus;
typedef long 				UPDEvent;
typedef long 				SockResult;

#else
//________________________________________________________________________
// linux and MacOS X types definition
//________________________________________________________________________
# ifdef __MacOSX__
#	include <sys/types.h>
#	include <sys/socket.h>
# else
//	typedef int 				Boolean;
#endif

#include <netinet/in.h>

#define IPField(a)		(a).sin_addr.s_addr
#define IPType(a)		(a).sin_family
#define IPPort(a)		(a).sin_port
#define PIPField(a)		(a)->sin_addr.s_addr
#define PPortField(a)	(a)->sin_port
#define kMaxHostNameLen 255
#define noErr			0

typedef char 	INetDomainName[kMaxHostNameLen + 1];
typedef short 	InetServiceRef;
typedef char 	ErrString[255];
typedef char 	NameStr[32];
typedef char * 	strPtr;

typedef unsigned long		IPNum;
typedef struct sockaddr_in 	IPAddr, * IPAddrPtr;
typedef int					SocketRef;
typedef int 				SocketStatus;
typedef long 				UPDEvent;
typedef long 				SockResult;

#endif

#endif
