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
//	TLoopManager.h			    
// ===========================================================================


#ifndef __TLoopManager__
#define __TLoopManager__


#include "TPlayerConstants.h"
#include "TPlayerScore.h"
#include "TScoreFollower.h"
#include "TScoreMarker.h"

//--------------------
// Class TLoopManager 
//--------------------

#define kLoopEndOffset 2

/*!
	\brief Management of the Loop.
*/

class TLoopManager {

	private:
		
        TScoreObjFollower	fFollower;
        TScoreMarker		fLoopEndMarker;
        TPlayerScorePtr		fScore;
        Boolean			fLoopState;
        ULONG			fLoopStart;
	
	public:
		
		TLoopManager(TPlayerScorePtr score, ULONG tpq)
			:fFollower(score,tpq),fLoopEndMarker(typeLoopEnd),fScore(score),fLoopState(kLoopOff),fLoopStart(0){}
		virtual ~TLoopManager () {fLoopEndMarker.Remove(fScore);}
		
		void SetLoop (Boolean state) {fLoopState = state;}
		Boolean GetLoop () {return fLoopState;}
		Boolean IsLoopOn(){ return fLoopState == kLoopOn; }
		Boolean IsLoopPlaced(){ return fLoopEndMarker.IsInserted(); }
		
		long SetLoopStartTicks (ULONG date_ticks);
		long SetLoopStartBBU (const TPos& pos);
		long SetLoopStartMs(ULONG date_ms);
		long SetLoopEndTicks (ULONG date_ticks);
		long SetLoopEndBBU (const TPos& pos);
		long SetLoopEndMs(ULONG date_ms);
		
		ULONG GetLoopEndTicks() 	{return fLoopEndMarker.GetDateTicks()+kLoopEndOffset;}
		ULONG GetLoopStartTicks() 	{return fLoopStart;}
	
};


typedef TLoopManager FAR * TLoopManagerPtr;

#endif
