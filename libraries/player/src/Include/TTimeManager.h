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
//	TTimeManager.cpp			    
// ===========================================================================
/*!
 \brief Uses a TIterator object to move a TVisitor object on a TScore object.
 Provides time conversion functions.
*/ 

#ifndef __TTimeManager__
#define __TTimeManager__

 
#include "TScoreFollower.h"


//-----------------------
// Class TTimeManager 
//-----------------------

class TTimeManager : public TScoreFollower{

	private:
	
		TScorePtr fScore;
			
	public :
 
 		TTimeManager(TScorePtr sc, ULONG tpq);
 		~TTimeManager(){}
 		
 		// Conversions functions
 		const TPos ConvertTickToBBU(ULONG date_tick);
		ULONG  ConvertBBUToTick(const TPos& pos);
		
		ULONG ConvertTickToMs  (ULONG date_tick);
		ULONG ConvertMsToTick  (ULONG date_ms) ;
		
		// Get Tempo and TimeSign
		ULONG GetTempo (ULONG date_tick) ;
		short GetNum (ULONG date_tick);
		short GetDenom (ULONG date_tick);
		short GetNClocks (ULONG date_tick);
		short GetN32nd (ULONG date_tick);
	
};


typedef TTimeManager FAR * TTimeManagerPtr;

#endif
