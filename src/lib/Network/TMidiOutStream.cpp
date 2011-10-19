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


#include "TMidiOutStream.h"
#include "TSocket.h"

//____________________________________________________________
TMidiOutStream::TMidiOutStream (UDPLinearizeMthTbl tbl, 
		TSocket *sok, IPNum ip, short MTU)
 : TPacket (kMidi, 0)
{
	fSerial = fPackDate = 0;
	fIndex = fOffset = 0;
	fMTU = MTU;
	fThroughput = fOverFlow = 0;
	fSocket = sok;
	IPType(fDest) = AF_INET;
	IPField(fDest) = ip;
	UDPStreamInit (&fStream, tbl);
}

//____________________________________________________________
short TMidiOutStream::Length ()
{
	SetLength (sizeof(MidiHeader) + fIndex);
	return TPacket::Length ();
}

//____________________________________________________________
void TMidiOutStream::Set (long collectTime)
{
	fPackDate = collectTime;
	fOffset = 0;
}

//____________________________________________________________
SocketStatus TMidiOutStream::Put (MidiEvPtr ev)
{
	SocketStatus err=noErr;
	MidiPacketPtr packet = Packet(); bool continuation = false;
	
	UDPStreamPutEvent (&fStream, ev);
	do {
		Byte c;
		if (UDPStreamGetByte (&fStream, &c)) {
			if (!FreeSpace()) {
				err = Flush ();
				fOffset = 0;
				fOverFlow++;
				fPackDate = 0;
				continuation = true;
				if (err != noErr) return err;
			}
			if (continuation) fOffset++;
			packet->data[fIndex++] = c;
		}
		else break;
	} while (true);
	return noErr;
}

//____________________________________________________________
short TMidiOutStream::Throughput (short * overflow)
{
	short out = fThroughput;
	*overflow = fOverFlow;
	fThroughput = fOverFlow = 0;
	return out;
}

//____________________________________________________________
SocketStatus TMidiOutStream::Flush ()
{
	SocketStatus err = noErr;
	if (fIndex) {
		MidiPacketPtr packet = Packet();
		packet->midi.serial = htonl(++fSerial);
		packet->midi.date   = htonl(fPackDate);
		packet->midi.offset = htons(fOffset);
		packet->midi.count  = htons(fIndex);
		err = Send (fSocket, &fDest);
		fThroughput = fIndex;
		fIndex = 0;
	}
	return err;
}
