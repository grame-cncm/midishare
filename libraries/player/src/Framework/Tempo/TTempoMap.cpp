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
//	TTempoMapVisitor.cpp		    
// ===========================================================================
//
// TTempoMapVisitor: an object used to maintain the current Tempo and TimeSign state
// and give conversion functions 


#include "TTempoMap.h"
#include "UTools.h"

/*--------------------------------------------------------------------------*/

TTempoMap::TTempoMap(ULONG tpq)
{
	fTempoConverter.Init(tpq); 
	fTimeConverter.Init(tpq);
	fCur_pos.Init();
	fCurdate_ticks = 0;
	fCur_date_ten_micro = 0;
}

/*--------------------------------------------------------------------------*/

void TTempoMap::Init () 
{
	fTempoConverter.Init(); 
	fTimeConverter.Init();
	fCur_pos.Init();
	fCurdate_ticks = 0;
	fCur_date_ten_micro = 0;
}

/*--------------------------------------------------------------------------*/

void TTempoMap::UpdateBBU (const TPos& pos) 
{
	fCurdate_ticks = fTimeConverter.ConvertBBUToTick (pos);
	fCur_date_ten_micro = fTempoConverter.ConvertTickToMicroSec(fCurdate_ticks);	
	fCur_pos = pos;
}

/*--------------------------------------------------------------------------*/

void TTempoMap::UpdateTicks (ULONG date_ticks)
{
	fCur_date_ten_micro = fTempoConverter.ConvertTickToMicroSec(date_ticks);	
	fCurdate_ticks =  date_ticks;
	fCur_pos = fTimeConverter.ConvertTickToBBU (fCurdate_ticks);
}

/*--------------------------------------------------------------------------*/

void TTempoMap::UpdateTenMicroSec (ULONG date_ten_micro) 
{
	fCur_date_ten_micro =  date_ten_micro;	
	fCurdate_ticks =  fTempoConverter.ConvertMicroSecToTick (date_ten_micro);
	fCur_pos = fTimeConverter.ConvertTickToBBU (fCurdate_ticks);
}

/*--------------------------------------------------------------------------*/

void TTempoMap::UpdateMs (ULONG date_ms) { UpdateTenMicroSec(date_ms*100);}
	
