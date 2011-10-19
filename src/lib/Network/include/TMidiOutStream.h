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
