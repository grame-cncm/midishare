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
