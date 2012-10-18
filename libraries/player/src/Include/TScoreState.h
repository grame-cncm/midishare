/*

  Copyright (C) Grame 1996-2012

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
//	TScoreState.h			    
// ===========================================================================

#ifndef __TScoreState__
#define __TScoreState__

#include "TScoreFollower.h"
#include "CStruct.h"

//-------------------
// Class TScoreState 
//-------------------
/*!
	\brief For displaying purpose : get the current state.
*/ 

class TScoreState : public TScoreObjFollower {
	
	public:
 
 		TScoreState(TScorePtr score, ULONG tpq):TScoreObjFollower(score,tpq) {}
 		virtual ~TScoreState () {}
 	
		void FillState(PlayerStatePtr state, ULONG date_ticks) 
		{
			SetPosTicks(date_ticks);
	
			TPos pos = GetVisitor().CurDateBBU();
			state->date = GetVisitor().CurDateMicro() / 100;
			state->ticks = date_ticks;
			
			state->bar = (short)pos.GetBar() + 1; 
			state->beat = (short)pos.GetBeat() + 1;
			state->unit = (short)pos.GetUnit();
			
			state->tsnum = GetVisitor().GetNum();
			state->tsdenom = GetVisitor().GetDenom();
			state->tsclick= GetVisitor().GetNClocks();
			state->tsquarter = GetVisitor().GetN32nd();
		} 
};

typedef TScoreState FAR * TScoreStatePtr;

#endif
