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


#include "TMidiInStream.h"
#include "TUDPSocket.h"

//____________________________________________________________
TMidiInStream::TMidiInStream (UDPParseMethodTbl tbl, PeerTimes * init, short maxLatency)
{
	fLastSerial = 0;
	fCurrentPackDate = 0;
	fMissed = 0;
	fPacket = 0;
	fIndex = 0;
	fDelay = fMaxLatency = maxLatency;
	fOffsets = init;
	fThroughput = fOverFlow = fMaxOver = 0;
	UDPParseInit (&fStream, tbl);
}

//____________________________________________________________
short TMidiInStream::Throughput (short * overflow)
{
	short out = fThroughput;
	*overflow = fOverFlow;
	fThroughput = fOverFlow = 0;
	return out;
}

//____________________________________________________________
bool TMidiInStream::Put (MidiPacketPtr p, long rcvDate)
{
	unsigned long serial, date; long serialDiff;
	
	// prevent an uncompletely parsed packet to be overrided
	if (!p || fPacket) return false;

	serial = Serial(p);
	serialDiff = serial - fLastSerial - 1;
	if (serialDiff < 0) {				// duplicate packet received
		return true;					// ignored
	}

	if (serialDiff && fLastSerial) {	// check for missing packets
		fMissed += serialDiff;			
		UDPParseReset (&fStream);		// reset the parse state
		fIndex = ntohs(p->midi.offset);
	}
	else fIndex = 0;

	fPacket = p;
	fThroughput = fCount = PacketCount();
	fLastSerial = serial;			// stores the current packet serial number
	date = PacketDate();
	if (date) {						// check if not a continuation packet 
		fCurrentPackDate = rcvDate;
		short curLatency = (rcvDate - date) - (fOffsets->local - fOffsets->remote);
		if (curLatency > fMaxLatency) {
			fMaxOver++;
			fDelay = 0;
		}
		else fDelay = fMaxLatency - curLatency;
	}
	else fOverFlow++;
	return true;
}

//____________________________________________________________
MidiEvPtr TMidiInStream::GetEv ()
{
	MidiEvPtr e = 0;
	if (!fPacket) return 0;
	
	while (fIndex < fCount) {
		e = UDPParseByte (&fStream, fPacket->data[fIndex++]);
		if (e) {
			Date(e) += fCurrentPackDate + fDelay;
			break;
		}
	}
	if (fIndex == fCount) {					// packet parsing is complete
		fPacket = 0;						// and clear the packet pointer
	}
	return e;
}
