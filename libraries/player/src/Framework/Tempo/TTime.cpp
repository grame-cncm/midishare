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
//	TTime.cpp			    
// ===========================================================================
//
//	Algebra for BBU representation 
//

#include "TTime.h"

/*--------------------------------------------------------------------------*/

void TTime::Init (ULONG tpq)
{
	fTicks_per_quarter = fTick_per_beat = tpq;
	fBeat_per_bar = kDefaultBar; 
	fTick_per_bar = fTick_per_beat * fBeat_per_bar;
}

/*--------------------------------------------------------------------------*/

const TPos TTime::ConvertTicksToBBUInBar (ULONG nb_ticks)
{	
	ULONG bar,beat,unit;

	bar = nb_ticks / fTick_per_bar;
	nb_ticks = nb_ticks % fTick_per_bar;
	beat = nb_ticks / fTick_per_beat;
	unit = nb_ticks % fTick_per_beat;
	return TPos(bar,beat,unit);
}

/*--------------------------------------------------------------------------*/

ULONG TTime::ConvertBBUToTicksInBar(const TPos& pos)
{
	return pos.fBar * fTick_per_bar + pos.fBeat * fTick_per_beat + pos.fUnit;
}


// one always add a pos without beat and unit values with another one  
// or timesign representation is the same for both positions
/*--------------------------------------------------------------------------*/

 const TPos TTime::Add ( const TPos& p1, const TPos& p2)
 {
	ULONG temp;
	ULONG bar,beat,unit;
		
	temp = p1.fUnit + p2.fUnit;
 	unit = temp % fTick_per_beat;
 	
 	temp = p1.fBeat + p2.fBeat + temp/fTick_per_beat;
 	beat = temp % fBeat_per_bar;
 	
 	bar = p1.fBar + p2.fBar +  temp/fBeat_per_bar;
	return TPos(bar,beat,unit);
 }

/*--------------------------------------------------------------------------*/

const TPos TTime::Normalize( const TPos& p1)
{ 
	ULONG temp;
	ULONG bar,beat,unit;
	
	temp = p1.fUnit /fTick_per_beat  + p1.fBeat;
	unit =  p1.fUnit % fTick_per_beat;
	beat = temp % fBeat_per_bar;
	bar =  p1.fBar + temp / fBeat_per_bar;
	return TPos(bar,beat,unit);
}

/*--------------------------------------------------------------------------*/

const TPos TTime::Normalize1 (const TPos& p1)
{ 
	ULONG bar,beat,unit;
	
	bar = p1.fBar - 1;
	beat = p1.fBeat + fBeat_per_bar - 1;
	unit = p1.fUnit + fTick_per_beat;
	return TPos(bar,beat,unit);
}

/*--------------------------------------------------------------------------*/

const TPos TTime::Sub (const TPos& p1, const TPos& p2)
{
	Normalize1 (p1);
	return TPos ( p1.fBar - p2.fBar, p1.fBeat - p2.fBeat , p1.fUnit - p2.fUnit);
}

/*--------------------------------------------------------------------------*/

Boolean TTime::Inf (const TPos& p1, const  TPos& p2)
{
	if (p1.fBar == p2.fBar)  Normalize(p2); 

	if (p1.fBar < p2.fBar) {
		return 1;
	}else if (p1.fBar == p2.fBar) {
		if (p1.fBeat < p2.fBeat) {
			return 1;
		}else if (p1.fBeat == p2.fBeat) {
			return (p1.fUnit < p2.fUnit); 
		}else {
			return 0;
		}
	}else 
		return 0;
}	

/*--------------------------------------------------------------------------*/

Boolean TTime::InfEq (const TPos& p1, const TPos& p2)
{
	if (p1.fBar == p2.fBar)  Normalize(p2); 

	if (p1.fBar < p2.fBar) {
		return 1;
	}else if (p1.fBar == p2.fBar) {
		if (p1.fBeat < p2.fBeat) {
			return 1;
		}else if (p1.fBeat == p2.fBeat) {
			return (p1.fUnit <= p2.fUnit); 
		}else {
			return 0;
		}
	}else 
		return 0;
}	

/*--------------------------------------------------------------------------*/

Boolean TTime::Sup (const TPos& p1 , const TPos& p2)
{
	if (p1.fBar == p2.fBar)  Normalize(p2); 

	if (p1.fBar > p2.fBar) {
		return 1;
	}else if (p1.fBar == p2.fBar) {
		if (p1.fBeat > p2.fBeat){
			return 1;
		}else if (p1.fBeat == p2.fBeat) {
			return (p1.fUnit > p2.fUnit);  
		}else 
			return 0;
	}else 
		return 0;
}

/*--------------------------------------------------------------------------*/

Boolean TTime::SupEq (const TPos& p1 , const TPos& p2)
{
	if (p1.fBar == p2.fBar)  Normalize(p2); 

	if (p1.fBar > p2.fBar) {
		return 1;
	}else if (p1.fBar == p2.fBar) {
		if (p1.fBeat > p2.fBeat){
			return 1;
		}else if (p1.fBeat == p2.fBeat) {
			return (p1.fUnit >= p2.fUnit);  
		}else 
			return 0;
	}else 
		return 0;
}	
