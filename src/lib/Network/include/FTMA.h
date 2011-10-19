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
