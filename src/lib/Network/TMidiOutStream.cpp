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
