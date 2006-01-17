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
//	TChaserIterator.cpp			    
// ===========================================================================
//
//	The chaser iterator use a chaser visitor to keep the state of KeyOn, CtrlChange, 
//  ProgChange, KeyPress, ChanPress, PitchBend, Sysex and Tune events when moving 
//  along the score.
//  The chaser iterator is used when starting or stopping the Player and will send 
//  events that occurred previously in the tracks.
//  


#include "TChaserIterator.h"

/*--------------------------------------------------------------------------*/

void TChaserIterator::GoBeginDateTicks(ULONG date_ticks) 
{
	while (!fIterator.IsLastEv() && fIterator.CurDate() < date_ticks){ 
		fIterator.CurEv()->Accept(&fChaser,true);
		fIterator.NextEv();
	}
}	

/*--------------------------------------------------------------------------*/

void TChaserIterator::Init()
{
	fChaser.Init();
	fIterator.Init();
	fCurdate_ticks = 0;
}

/*--------------------------------------------------------------------------*/

void TChaserIterator::SetPosTicks (ULONG date_ticks)
{
	if (date_ticks > fCurdate_ticks) {	
		GoBeginDateTicks(date_ticks);
	}else if (date_ticks < fCurdate_ticks){
		Init();
		GoBeginDateTicks(date_ticks);
	}
	fCurdate_ticks = date_ticks;
}

/*--------------------------------------------------------------------------*/

void TChaserIterator::ChaseOn (ULONG date_ticks)
{
	SetPosTicks(date_ticks);
	fChaser.ChaseOn(date_ticks);
}

/*--------------------------------------------------------------------------*/

void TChaserIterator::ChaseOff (ULONG date_ticks)
{
	SetPosTicks(date_ticks);
	fChaser.ChaseOff(date_ticks);
}

