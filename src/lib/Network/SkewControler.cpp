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


#include "SkewControler.h"
#include "MidiTools.h"

//________________________________________________________________________
// Skew measurement
//________________________________________________________________________
SkewControler::SkewControler (SkewParams * params, AEFTMAParams * p)
	: fAEFTMA (p->histSize, p->retain, p->threshold, p->histlen)
{
	fFirst = true;
	fParams = *params;
}

//________________________________________________________________________
void SkewControler::GetFTMAParams (Boolean lowLatency, AEFTMAParams * p)
{
	if (lowLatency) {
/*		p->histSize	= 10;
		p->retain	= 6;
		p->threshold= 10;
		p->histlen	= 9;
*/
		p->histSize	= 10;
		p->retain	= 8;
		p->threshold= 10;
		p->histlen	= 2;
	}
	else {
		p->histSize	= 50;
		p->retain	= 10;
		p->threshold= 20;
		p->histlen	= 99;
	}
}

//________________________________________________________________________
Boolean SkewControler::Put (unsigned long localtime, long latency)
{
	long average;
	if (fAEFTMA.Put (latency, &average)) {
		if (fFirst) {
			fFirst = false;
			fLastAverage = average;
		}
		else if (average != fLastAverage) {
			long drift = average - fLastAverage;
			fParams.times->local += drift;
			fLastAverage = average;
		}
		return true;
	}
	return false;
}
