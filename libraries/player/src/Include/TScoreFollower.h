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

class TScoreObjFollower : public TScoreFollower {

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
