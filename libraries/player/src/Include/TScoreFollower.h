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


#ifndef __TScoreFollower__
#define __TScoreFollower__


#include "TScoreIterator.h"
#include "TTempoMapVisitor.h"


//----------------------
// Class TScoreFollower 
//----------------------
/*!
\brief  Generic score follower : uses a TScoreIterator to move a TScoreVisitor on a TScore object.
*/

class TScoreFollower {

	private:
	
		void SetPosBBUForward (const TPos& pos);
		void SetPosBBUBackward (const TPos& pos);
		
		void SetPosMicroForward (ULONG date_ten_micro);
		void SetPosMicroBackward (ULONG date_ten_micro);
		
		void SetPosTicksForward (ULONG date_ticks);
		void SetPosTicksBackward (ULONG date_ticks);
	
	protected:
	
                virtual TTempoMapVisitor& GetVisitor() = 0;
                virtual TScoreIterator& GetIterator() = 0;
		
	public:
	
 		TScoreFollower(){}
 		virtual ~TScoreFollower (){}
 		
		void Init();
		
 		TEventPtr SetPosTicks (ULONG date_ticks); 
 		TEventPtr SetPosBBU (const TPos& pos);
 		TEventPtr SetPosMs (ULONG date_ms);
 		
 		ULONG GetPosTicks();
};

typedef TScoreFollower FAR * TScoreFollowerPtr;

//-------------------------
// Class TScoreObjFollower 
//--------------------------
/*!
\brief  A score follower that use TTempoMapVisitor and TScoreIterator objects.
*/

class TScoreObjFollower : public TScoreFollower{

	private:
	
                TScoreIterator		fIterator;
		TTempoMapVisitor 	fTempoVisitor;
	 
        protected:
                 
                TTempoMapVisitor& GetVisitor() {return fTempoVisitor;}
                TScoreIterator& GetIterator() {return fIterator;}
		
	public:
	
 		TScoreObjFollower(TScorePtr score, ULONG tpq):fIterator(score),fTempoVisitor(tpq){}
 		virtual ~TScoreObjFollower(){}
};

typedef TScoreObjFollower FAR * TScoreObjFollowerPtr;


//-------------------------
// Class TScoreRefFollower 
//--------------------------
/*!
\brief  A score follower that use TTempoMapVisitor and TScoreIterator references.
*/

class TScoreRefFollower : public TScoreFollower{

	private:
	
                TScoreIterator&		fIterator;
		TTempoMapVisitor& 	fTempoVisitor;
	   
        protected:
                 
                TTempoMapVisitor& GetVisitor() {return fTempoVisitor;}
                TScoreIterator& GetIterator() {return fIterator;}
		
	public:
	
 		TScoreRefFollower(TScoreIterator& iterator, TTempoMapVisitor& visitor):fIterator(iterator),fTempoVisitor(visitor){}
 		virtual ~TScoreRefFollower(){}
};


typedef TScoreRefFollower FAR * TScoreRefFollowerPtr;

#endif
