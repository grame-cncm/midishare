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
	
