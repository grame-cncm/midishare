/*

  Copyright © Grame 1996-2004

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
  research@grame.fr

*/


// ===========================================================================
//	TTime.cpp			    
// ===========================================================================
//
//	Algebra for BBU representation 
//

#include "TTime.h"

/*--------------------------------------------------------------------------*/

void TTime::Init(ULONG tpq)
{
	fTicks_per_quarter = fTick_per_beat = tpq;
	fBeat_per_bar = kDefaultBar; 
	fTick_per_bar = fTick_per_beat * fBeat_per_bar;
}

/*--------------------------------------------------------------------------*/

const TPos TTime::ConvertTicksToBBUInBar(ULONG nb_ticks)
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

 const TPos TTime::Add(const TPos& p1, const TPos& p2)
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

const TPos TTime::Normalize(const TPos& p1)
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

const TPos TTime::Normalize1(const TPos& p1)
{ 
	ULONG bar,beat,unit;
	
	bar = p1.fBar - 1;
	beat = p1.fBeat + fBeat_per_bar - 1;
	unit = p1.fUnit + fTick_per_beat;
	return TPos(bar,beat,unit);
}

/*--------------------------------------------------------------------------*/

const TPos TTime::Sub(const TPos& p1, const TPos& p2)
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

Boolean TTime::InfEq(const TPos& p1, const TPos& p2)
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

Boolean TTime::Sup(const TPos& p1, const TPos& p2)
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

Boolean TTime::SupEq(const TPos& p1, const TPos& p2)
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
