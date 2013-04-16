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


#ifndef __FTMA__
#define __FTMA__

#include "InetTypes.h"
//__________________________________________________________________________
// FTMA class
//__________________________________________________________________________
class FTMA
{
	public:
		enum { kMaxSize = 50 };
		
		 	 FTMA (short histSize, short retain, short threshold);
	virtual ~FTMA () {}

	virtual bool Put (long value, long *average);
		bool 	Empty ()	{ return fEmpty; }
		
	protected:
		virtual long	Average (long * tbl, short size, short retain);
		double	Sum  (long * tbl, short size);
		void	Sort (long * tbl, short size);
		void	Sort (long * tbl, long *dst, short size);

	private:
		long	fValues[kMaxSize];
		short	fIndex, fSize, fRetain, fThreshold;
		bool fEmpty;
};

//__________________________________________________________________________
// Exponential average class
//__________________________________________________________________________
class EAverage
{
	public:
		 	 EAverage (short histlen);
	virtual ~EAverage () {}

		long 	Average (long value);

	private:
		short 	fHistoryLen, fCurrentHistLen;
		short 	fDivider;
		float	fAverage;
};

//__________________________________________________________________________
// AEFTMA class
//__________________________________________________________________________
class AEFTMA : public FTMA
{
	public:
		 	 AEFTMA (short histSize, short retain, short threshold, short histlen);
	virtual ~AEFTMA () {}

	virtual bool Put (long value, long *average);
	
	private:
		EAverage	fAverage;
};

#endif
