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
			Boolean 		Put (MidiPacketPtr p, long rcvDate);
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
