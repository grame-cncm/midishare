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


#include "FTMA.h"

//__________________________________________________________________________
// AEFTMA implementation
//__________________________________________________________________________
AEFTMA::AEFTMA (short histSize, short retain, short threshold, short histLen)
	: FTMA (histSize, retain, threshold), fAverage (histLen)
{
}

//__________________________________________________________________________
bool AEFTMA::Put (long value, long *average)
{
	long m;
	if (FTMA::Put (value, &m)) {
		*average = fAverage.Average (m);
		return true;
	}
	return false;
}

//__________________________________________________________________________
// FTMA implementation
//__________________________________________________________________________
FTMA::FTMA (short histSize, short retain, short threshold)
{
	fIndex = 0;
	fSize  =  (histSize > kMaxSize) ? kMaxSize : histSize;
	fRetain = retain;
	fThreshold = threshold < 4 ? 4 : threshold;
	fEmpty = true;
}

//__________________________________________________________________________
bool FTMA::Put (long value, long *average)
{
	fValues[fIndex++] = value;
	if (fIndex == fSize) {
		fIndex = 0;
		fEmpty = false;
	}
	if (!Empty()) {
		*average = Average (fValues, fSize, fRetain);
		return true;
	}
	else if (fIndex > fThreshold) {
		short retain = (fRetain * fThreshold)/fIndex;
		if (retain < 1) retain = 1;
		*average = Average (fValues, fIndex-1, retain);
		return true;
	}
	return false;
}

//__________________________________________________________________________
double FTMA::Sum  (long * tbl, short size)
{
	double v = 0;
	for (short i = 0; i < size; i++)
		v += *tbl++;
	return v;
}

//__________________________________________________________________________
long FTMA::Average (long * tbl, short size, short retain)
{
	short offset = (size - retain)/2;
	long tmp[kMaxSize];
	Sort (tbl, tmp, size);
	double sum = Sum (&tmp[offset], retain);
	return (long)(sum / retain);
}

//__________________________________________________________________________
void FTMA::Sort (long * tbl, short size)
{
	short t;
	do {
		t = 0;
		for (short i = 1; i < size; i++) {
			if (tbl[i-1] > tbl[i]) {
				long tmp = tbl[i-1];
				tbl[i-1] = tbl[i];
				tbl[i] = tmp;
				t = i;
			}
		}
	} while (t);
}

//__________________________________________________________________________
void FTMA::Sort (long * tbl,  long *dst, short size)
{
	for (short i = 0; i < size; i++) {
		dst[i] = tbl[i];
	}
	Sort (dst, size);
}

//__________________________________________________________________________
// Exponential average class
//__________________________________________________________________________
EAverage::EAverage (short histlen)
{
	fHistoryLen = histlen;
	fAverage = 0;
	fCurrentHistLen = 0;
	fDivider = ((histlen / 10) + 1) * 10;
}

//__________________________________________________________________________
long EAverage::Average (long value)
{
	fAverage = ((fCurrentHistLen * fAverage) + ((fDivider - fCurrentHistLen) * value)) / fDivider;
	if (fCurrentHistLen < fHistoryLen) fCurrentHistLen += 1;
	return (long)fAverage;
}
