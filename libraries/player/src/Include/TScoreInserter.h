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
//	TScoreInserter.cpp			    
// ===========================================================================


#ifndef __TScoreInserter__
#define __TScoreInserter__

 
#include "TScoreFollower.h"
#include "TPlayerScore.h"


//----------------------
// Class TScoreFollower 
//----------------------
/*!
\brief  A utility class to insert sequence of events in the score.
*/

class TScoreInserter  :public TScoreFollower{

	private:
	
		TPlayerScorePtr fScore;
		
	public:
 
 		TScoreInserter(TPlayerScorePtr score, ULONG tpq):TScoreFollower(score,tpq),fScore(score){}
 		virtual ~TScoreInserter(){}
 		
 		long InsertAllTrack(MidiSeqPtr s);
 		long InsertTrack(short tracknum, MidiSeqPtr s);
};


typedef TScoreInserter FAR * TScoreInserterPtr;

#endif
