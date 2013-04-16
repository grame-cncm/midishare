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
void SkewControler::GetFTMAParams (bool lowLatency, AEFTMAParams * p)
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
bool SkewControler::Put (unsigned long localtime, long latency)
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
