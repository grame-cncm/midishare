// ===========================================================================
// The Player Library is Copyright (c) Grame, Computer Music Research Laboratory 
// 1996-2003, and is distributed as Open Source software under the Artistic License;
// see the file "Artistic" that is included in the distribution for details.
//
// Grame : Computer Music Research Laboratory
// Web : http://www.grame.fr/Research
// E-mail : MidiShare@rd.grame.fr
// ===========================================================================


// ===========================================================================
//	TTimeConverter.cpp			    
// ===========================================================================
//
//	Ticks <===> BBU convertion functions
//

#include "TTimeConverter.h"

/*--------------------------------------------------------------------------*/

void TTimeConverter::Init (ULONG tpq) 
{
 	fLast_timesign = 0;
 	fLast_pos.Init();
	fInt.Init(tpq);
	fNumerator = kDefaultNum;
	fDenominator = kDefaultDenom;
	fNClocks = kDefaultClocks;
	fN32nd = kDefaultN32;	
}	

/*--------------------------------------------------------------------------*/

void TTimeConverter::Init () 
{
 	fLast_timesign = 0;
 	fLast_pos.Init();
	fNumerator = kDefaultNum;
	fDenominator = kDefaultDenom;
	fNClocks = kDefaultClocks;
	fN32nd = kDefaultN32;	
}	

/*--------------------------------------------------------------------------*/

void TTimeConverter::Update (MidiEvPtr ts)
{
	Update(Date(ts), TSNum(ts), TSDenom(ts), TSClocks(ts), TS32nd(ts));
}

/*--------------------------------------------------------------------------*/

void TTimeConverter::Update (ULONG date_ticks, short num, short denom, short clock , short N32nd)
{
	if ((num != fNumerator)   	// Update values only if the TimeSign has changed
		|| (denom != fDenominator)
		|| (clock != fNClocks)
		|| (N32nd != fN32nd)) {

		fLast_pos = ConvertTickToBBU (date_ticks);

		// Check arguments validity
		if ((num == 0) || (CalcBeat(fInt.fTicks_per_quarter, denom - 2) == 0)) {
			// Set default values
			num = kDefaultNum;
			denom = kDefaultDenom;
			clock = kDefaultClocks;
			N32nd = kDefaultN32;
		}

		fInt.fBeat_per_bar =  num;
		fInt.fTick_per_beat = CalcBeat(fInt.fTicks_per_quarter, denom - 2);
		fInt.fTick_per_bar = fInt.fBeat_per_bar * fInt.fTick_per_beat;

		fNumerator = num;
		fDenominator = denom;
		fNClocks = clock;
		fN32nd = N32nd;

		fLast_timesign = date_ticks;
	}
}

/*--------------------------------------------------------------------------*/
 
 TPos TTimeConverter::ConvertTickToBBU (ULONG date_ticks) 	
 {
 	if (date_ticks >= fLast_timesign) {
		return fInt.Add (fLast_pos, fInt.ConvertTicksToBBUInBar(date_ticks - fLast_timesign));
	}else{
		return fInt.ConvertTicksToBBUInBar(fInt.ConvertBBUToTicksInBar(fLast_pos) - (fLast_timesign - date_ticks));
	}
 }
 
/*--------------------------------------------------------------------------*/

ULONG TTimeConverter::ConvertBBUToTick (const TPos& pos)	
{
	if (fInt.SupEq(pos,fLast_pos))  {
		return fLast_timesign + fInt.ConvertBBUToTicksInBar(fInt.Sub (pos, fLast_pos));
	}else{
		return fLast_timesign - fInt.ConvertBBUToTicksInBar(fInt.Sub (fLast_pos ,pos));
	}
}