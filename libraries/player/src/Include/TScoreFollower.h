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
//	TScoreFollower.h		    
// ===========================================================================
/*!
\brief  Uses a TScoreIterator to move a TScoreVisitor on a TScore object.
*/


#ifndef __TScoreFollower__
#define __TScoreFollower__


#include "TScoreIterator.h"
#include "TTempoMapVisitor.h"


//-----------------------
// Class TScoreFollower 
//-----------------------

class TScoreFollower {

	private:
	
		void SetPosBBUForward (const TPos& pos);
		void SetPosBBUBackward (const TPos& pos);
		
		void SetPosMicroForward (ULONG date_ten_micro);
		void SetPosMicroBackward (ULONG date_ten_micro);
		
		void SetPosTicksForward (ULONG date_ticks);
		void SetPosTicksBackward (ULONG date_ticks);
	
	protected:
	
		TTempoMapVisitorPtr fTempoVisitor;
		TScoreIteratorPtr	fIterator;
		
	public :
	
 		TScoreFollower() { fTempoVisitor = 0; fIterator = 0;}
 		TScoreFollower(TScorePtr score,ULONG tpq);
 		TScoreFollower(TScoreIteratorPtr iterator, TTempoMapVisitorPtr tempovisitor);
 		virtual ~TScoreFollower ();
 		
		void Init (){ fIterator->Init(); fTempoVisitor->Init();}
		
 		TEventPtr SetPosTicks (ULONG date_ticks); 
 		TEventPtr SetPosBBU (const TPos& pos);
 		TEventPtr SetPosMs (ULONG date_ms);
 		
 		ULONG GetPosTicks() { return fTempoVisitor->CurDateTicks();}
};


typedef TScoreFollower FAR * TScoreFollowerPtr;

#endif
