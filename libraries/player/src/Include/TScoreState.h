// ===========================================================================
// The Player Library is Copyright (c) Grame, Computer Music Research Laboratory 
// 1996-1999, and is distributed as Open Source software under the Artistic License;
// see the file "Artistic" that is included in the distribution for details.
//
// Grame : Computer Music Research Laboratory
// Web : http://www.grame.fr/Research
// E-mail : MidiShare@rd.grame.fr
// ===========================================================================


// ===========================================================================
//	TScoreState.h			    
// ===========================================================================
//
//	For displaying purpose: get the current state
// 

#ifndef __TScoreState__
#define __TScoreState__

#include "TScoreFollower.h"
#include "CStruct.h"

//-----------------------
// Class TScoreState 
//-----------------------

class TScoreState :public TScoreFollower {
	
	public :
 
 		TScoreState(TScorePtr score,ULONG tpq):TScoreFollower(score,tpq) {}
 		~TScoreState (){}
 	
		void FillState(PlayerStatePtr state, ULONG date_ticks) 
		{
			SetPosTicks(date_ticks);
	
			TPos pos = fTempoVisitor->CurDateBBU();
			state->date_ms =  fTempoVisitor->CurDateMicro() / 100;
			state->date_ticks =  date_ticks;
			
			state->bar  = (short)pos.GetBar() + 1; 
			state->beat = (short)pos.GetBeat() + 1;
			state->unit = (short)pos.GetUnit() + 1;
			
			state->tsnum = fTempoVisitor->GetNum ();
			state->tsdenom = fTempoVisitor->GetDenom ();
			state->tsclick= fTempoVisitor->GetNClocks();
			state->tsquarter = fTempoVisitor->GetN32nd();
		} 
};


typedef TScoreState FAR * TScoreStatePtr;

#endif
