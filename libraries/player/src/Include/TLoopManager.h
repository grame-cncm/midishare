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
