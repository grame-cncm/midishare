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


#ifndef __TMidiInStream__
#define __TMidiInStream__

#include "MidiShare.h"
#include "InetTypes.h"
#include "TMidiPacket.h"
#include "UDPStreamToEvent.h"

class TUDPSocket;

typedef struct {
	unsigned long 	last;		// last packet date
	unsigned long	current;	// current packet date
	unsigned long	offset;		// current transmission offset
} NetTimes;

//____________________________________________________________
class TMidiInStream
{
	public:
		 	 TMidiInStream (UDPParseMethodTbl tbl, PeerTimes * init, short maxLatency);
	virtual ~TMidiInStream () {}
	
			void 			Reset ()	{ fPacket = 0; }
			bool			Put (MidiPacketPtr p, long rcvDate);
			MidiEvPtr		GetEv ();

			unsigned long	Missed ()		{ return fMissed; }
			short			MaxLatency ()	{ return fMaxLatency; }
			short			MaxLatencyOver ()	{ return fMaxOver; }
			void			SetLatency (short lat)	{ fMaxLatency = lat; }
			short 			Throughput (short * overflow);

	private:
		unsigned long	Serial (MidiPacketPtr p)	{ return ntohl(p->midi.serial); }
		unsigned long	PacketDate () 	{ return ntohl(fPacket->midi.date); }
		short			PacketCount () 	{ return ntohs(fPacket->midi.count); }
		
		long			fDelay;			// current packet delay
		unsigned long	fLastSerial;	// last packet serial number
		unsigned long	fMissed;		// missing packets count
		unsigned long	fCurrentPackDate; // current packet reception date
		PeerTimes *		fOffsets;	// local & remote time offsets

		short			fMaxLatency; // estimated maximum latency
		short			fCount;		// packet data bytes count
		short			fIndex;		// current packet reading position
		MidiPacket	*	fPacket;	// current packet pointer, 
									// set to nil when the packet is completely parsed
		UDPStream		fStream;

		// the following fields are just for indication purpose
		short			fThroughput;// current out stream throughput
		short			fOverFlow;  // overflow indicator
		short			fMaxOver;	// max latency over count
};

#endif
