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
//	TLoopManager.h			    
// ===========================================================================
//
//	Management of the Loop
// 


#ifndef __TLoopManager__
#define __TLoopManager__


#include "TPlayerConstants.h"
#include "TPlayerScore.h"
#include "TScoreFollower.h"
#include "TScoreMarker.h"

//-----------------------
// Class TLoopManager 
//-----------------------

class TLoopManager {

	private:
	
		TPlayerScorePtr 	fScore;
		TScoreFollowerPtr	fFollower;
		TScoreMarkerPtr		fLoopEndMarker;
		ULONG            	fLoopStart;
		short 				fLoopState;
		
	public :
		
		TLoopManager(TPlayerScorePtr score, ULONG tpq);
		~TLoopManager ();
		
		void SetLoop (short state) {fLoopState = state;}
		short GetLoop () {return fLoopState;}
		Boolean IsLoopOn(){ return fLoopState == kLoopOn; }
		Boolean IsLoopPlaced(){ return fLoopEndMarker->IsInserted(); }
		
		long SetLoopStartTicks (ULONG date_ticks);
		long SetLoopStartBBU (const TPos& pos);
		long SetLoopStartMs(ULONG date_ms);
		long SetLoopEndTicks (ULONG date_ticks);
		long SetLoopEndBBU (const TPos& pos);
		long SetLoopEndMs(ULONG date_ms);
		
		ULONG GetLoopEndTicks() 	{return fLoopEndMarker->GetDateTicks();}
		ULONG GetLoopStartTicks() 	{return fLoopStart;}
	
		
};


typedef TLoopManager FAR * TLoopManagerPtr;

#endif
