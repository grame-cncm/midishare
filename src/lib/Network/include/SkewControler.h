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
	
				Boolean	Put	(unsigned long localtime, long latency);
		
		static void GetFTMAParams (Boolean lowLatency, AEFTMAParams * p);

	private:
		long		fLastAverage;
		Boolean		fFirst;
		SkewParams	fParams;
		AEFTMA		fAEFTMA;
};

#endif
