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


#ifndef __TMidiPacket__
#define __TMidiPacket__

#include "TPacket.h"

#define kMaxMidiBuff	kMaxPacketBuff - sizeof(MidiHeader)

typedef struct {
	unsigned long	serial;		// packet serial number
	unsigned long	date;		// packet collection date
	short			count;		// data bytes count
	short			offset;		// first event offset: should be 0 for a regular packet
								// and non zero for a continuation packet
} MidiHeader, * MidiHeaderPtr;

typedef struct {
	msPacketHeader	header;
	MidiHeader		midi;
	char 	data[kMaxMidiBuff];
} MidiPacket, * MidiPacketPtr;

#endif
