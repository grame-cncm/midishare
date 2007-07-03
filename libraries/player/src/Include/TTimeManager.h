/*

  Copyright © Grame 1996-2006

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
//	TTimeManager.cpp			    
// ===========================================================================

#ifndef __TTimeManager__
#define __TTimeManager__

#include "TScoreFollower.h"

//--------------------
// Class TTimeManager 
//--------------------
/*!
 \brief Uses a TScoreIterator object to move a TScoreVisitor object on a TScore object.
 Provides time conversion functions.
*/ 

class TTimeManager : public TScoreObjFollower {

	private:
	
		TScorePtr fScore;
			
	public:
 
 		TTimeManager(TScorePtr sc, ULONG tpq);
 		virtual ~TTimeManager() {}
 		
 		// Conversions functions
 		const TPos ConvertTickToBBU(ULONG date_tick);
		ULONG  ConvertBBUToTick(const TPos& pos);
		
		ULONG ConvertTickToMs(ULONG date_tick);
		ULONG ConvertMsToTick(ULONG date_ms) ;
		
		// Get Tempo and TimeSign
		ULONG GetTempo(ULONG date_tick) ;
		short GetNum(ULONG date_tick);
		short GetDenom(ULONG date_tick);
		short GetNClocks(ULONG date_tick);
		short GetN32nd(ULONG date_tick);
};

typedef TTimeManager FAR * TTimeManagerPtr;

#endif
