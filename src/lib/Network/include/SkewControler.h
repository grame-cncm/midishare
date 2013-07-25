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


#ifndef __SkewControler__
#define __SkewControler__

#include "InetTypes.h"
#include "FTMA.h"

class TUDPSocket;
class TIDPacket;

//__________________________________________________________________________
typedef struct {
	short histSize;
	short retain;
	short threshold;
	short histlen;
} AEFTMAParams;

typedef struct {
	PeerTimes * times;
	IPNum 		ip;
	TUDPSocket * socket;
	TIDPacket *	packet;
} SkewParams;

//__________________________________________________________________________
// SkewControler
//__________________________________________________________________________
class SkewControler
{
	public:
		 	 SkewControler (SkewParams * params, AEFTMAParams * p);
	virtual ~SkewControler () {}
	
				bool	Put	(unsigned long localtime, long latency);
		
		static void GetFTMAParams (bool lowLatency, AEFTMAParams * p);

	private:
		long		fLastAverage;
		bool		fFirst;
		SkewParams	fParams;
		AEFTMA		fAEFTMA;
};

#endif
