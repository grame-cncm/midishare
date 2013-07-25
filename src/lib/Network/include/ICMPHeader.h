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


#ifndef __ICMPHeader__
#define __ICMPHeader__

#include "IPHeader.h"

//__________________________________________________________________________________________
// ICMP packet structure
//__________________________________________________________________________________________
typedef struct ICMPPb { 		// ICMP datas returned in case of problem
	IPHeader	header;
	char		original[8];
} ICMPPb, * ICMPPbPtr;

//_______________________________
typedef struct ICMPTimeStamp {	// TimeStamp or TimeStamp messages
	long	originate;
	long	rcv;
	long 	transmit;
} ICMPTimeStamp, * ICMPTimeStampPtr;

//_______________________________
typedef struct ICMPInfo {
	short 	id;				// identifier
	short	seqNum;			// sequence number
} ICMPInfo, * ICMPInfoPtr;

//_______________________________
typedef struct ICMPPacket {
	unsigned char	type;		// ICMP packet type
	unsigned char	code;		// information code
	short			checksum;	//
	
	union {
		long		lvalue;		// long value 
		char		ptr;		// octet identifier for parametere pb message
		ICMPInfo	info;		// 
	} part1;
	
	union {
		ICMPPb			retData;
		ICMPTimeStamp	tStamp;
		char			data[1];
	} part2;
	
} ICMPPacket, * ICMPPacketPtr;

//__________________________________________________________________________________________
// ICMP packet types
enum { 	kICMPUnreachableDest 	= 3,
		kICMPTimeExceeded		= 11,
		kICMPParamPb			= 12,
		kICMPSrcQuench			= 4,
		kICMPRedirect			= 5,
		kICMPEcho				= 8,
		kICMPEchoReply			= 0,
		kICMPTimeStamp			= 13,
		kICMPTimeStampReply		= 14,
		kICMPInfoRequest		= 15,
		kICMPInfoReply			= 16
	};

//__________________________________________________________________________________________
#define kMaxICMPData	512
typedef struct {
	IPHeader	ip;
	ICMPPacket	icmp;
	char		data[kMaxICMPData];
} ICMPBuffer, *ICMPBufferPtr;



#endif
