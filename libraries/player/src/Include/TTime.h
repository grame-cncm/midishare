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
//	TTime.h			    
// ===========================================================================


#ifndef __TTime__
#define __TTime__

 
#include "TPlayerConstants.h"
#include "GenericMidiShare.h"

//------------
// Class TPos
//------------

class TTime;

/*!
	\brief Position in Bar Beat Unit representation
*/

class TPos{

	friend class TTime;
	
	private:
	
		ULONG fBar;
		ULONG fBeat;
		ULONG fUnit;
	
	public :
	
		TPos (){Init();}
		TPos (ULONG bar, ULONG beat, ULONG unit):fBar(bar),fBeat(beat),fUnit(unit){}
		void Init(){fBar = fBeat = fUnit = 0;} 
		virtual ~TPos(){}
		
		ULONG GetBar()  {return fBar;}
		ULONG GetBeat() {return fBeat;}
		ULONG GetUnit() {return fUnit;}
};

typedef TPos FAR * TPosPtr;


//-------------
// Class TTime
//-------------
/*!
	\brief Algebra for Bar Beat Unit representation 
*/

class TTime{

	friend class TTimeConverter;

	private :
	
		ULONG fTick_per_beat;
		ULONG fBeat_per_bar;
		ULONG fTick_per_bar;
		ULONG fTicks_per_quarter; 
		
		const TPos Normalize (const TPos& p1);
		const TPos Normalize1 (const TPos& p1);
		
	public :
	
		TTime() {Init(kDefaultTpq);}
		virtual ~TTime() {}
	
		void Init (ULONG tick);
		
		const TPos ConvertTicksToBBUInBar (ULONG nb_ticks);
		ULONG ConvertBBUToTicksInBar(const TPos& pos);
		
		const TPos 	Add (const TPos& p1, const TPos& p2);
		const TPos 	Sub (const TPos& p1, const TPos& p2);
		
		Boolean Inf (const TPos& p1, const TPos& p2);
		Boolean Sup (const TPos& p1, const TPos& p2);
		Boolean InfEq (const TPos& p1, const TPos& p2);
		Boolean SupEq (const TPos& p1, const TPos& p2);

};

typedef TTime FAR * TTimePtr;

#endif
