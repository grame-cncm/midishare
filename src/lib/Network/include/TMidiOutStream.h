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


#ifndef __TMidiOutStream__
#define __TMidiOutStream__

#include "MidiShare.h"
#include "InetTypes.h"
#include "TMidiPacket.h"
#include "EventToUDPStream.h"

class TUDPSocket;
//____________________________________________________________
class TMidiOutStream : public TPacket
{
	public:
		 	 TMidiOutStream (UDPLinearizeMthTbl tbl, TSocket *sok, IPNum ip, short MTU);
	virtual ~TMidiOutStream () {}
	
			MidiPacketPtr 	Packet () 		{ return (MidiPacketPtr)Buffer(); }
	virtual short			Length();
			short 			MTU ()			{ return fMTU; }
			bool			Ready ()		{ return fSocket != 0; }
			IPNum 			IP ()			{ return IPField(fDest); }
			short 			Throughput (short * overflow);
			void 			Set (long collectTime);	
			SocketStatus 	Put (MidiEvPtr ev);	
			SocketStatus 	Flush ();

	private:
		short			FreeSpace ()	{ return fMTU - fIndex  - sizeof(MidiHeader); }

		short			fMTU;		// maximum transit unit size
		short			fIndex;		// packet current write index
		short			fOffset;	// first event start offset in the packet
		unsigned long	fSerial;	// current packet serial number
		unsigned long	fPackDate;	// current packet starting date
		Ev2UDPStreamRec	fStream;
		TSocket *		fSocket;
		IPAddr 			fDest;
		// the following fields are just for indication purpose
		short			fThroughput;// current out stream throughput
		short			fOverFlow;  // overflow indicator
};

#endif
