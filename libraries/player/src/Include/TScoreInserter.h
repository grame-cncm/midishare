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

class TScoreInserter : public TScoreObjFollower{

	private:
	
		TPlayerScorePtr fScore;
		
	public:
 
 		TScoreInserter(TPlayerScorePtr score, ULONG tpq):TScoreObjFollower(score,tpq),fScore(score){}
 		virtual ~TScoreInserter(){}
 		
 		long InsertAllTrack(MidiSeqPtr s);
 		long InsertTrack(short tracknum, MidiSeqPtr s);
};


typedef TScoreInserter FAR * TScoreInserterPtr;

#endif
