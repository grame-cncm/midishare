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
//	TTimeConverter.h		    
// ===========================================================================

#ifndef __TTimeConverter__
#define __TTimeConverter__

#include "GenericMidiShare.h"
#include "TTime.h"

//----------------------
// Class TTimeConverter 
//----------------------
/*!
	\brief Ticks to Bar Beat Unit conversion functions (and the contrary)
*/

class TTimeConverter {

	private:

		/*! Date of the last timesign change (in ticks) */
		ULONG 	fLast_timesign ;    
		/*! Position of the last timesign change */
		TPos   	fLast_pos;         
		short   fNumerator;         
		short 	fDenominator;
		short  	fNClocks;
		short 	fN32nd;
		TTime 	fInt;     			// Time Signature 
		
		ULONG 	CalcBeat (ULONG val, short r) { return (r < 0) ? val<<(- r) : val>>r;}
		
	public:
			
		TTimeConverter():fInt() {Init (kDefaultTpq);} 
		virtual ~TTimeConverter() {}
		
		void Init(ULONG tpq);  // Initialize with a new TPQ value
		void Init(); 		   // Initialize keeping the current TPQ
		
		TPos ConvertTickToBBU  (ULONG date); 	
		ULONG ConvertBBUToTick  (const TPos& pos);	

		void Update (MidiEvPtr ts) ;
		void Update (ULONG date, short num, short denom, short clock , short N32nd);
		
		short GetNum() 		{return fNumerator;}
		short GetDenom() 	{return fDenominator;}
		short GetNClocks() 	{return fNClocks;}
		short GetN32nd() 	{return fN32nd;}
		
		Boolean Inf (const TPos& p1, const TPos& p2) {return fInt.Inf(p1,p2);}
		Boolean Sup (const TPos& p1, const TPos& p2) {return fInt.Sup(p1,p2);}
		Boolean InfEq (const TPos& p1, const TPos& p2) {return fInt.InfEq(p1,p2);}
		Boolean SupEq (const TPos& p1, const TPos& p2) {return fInt.SupEq(p1,p2);}
};

typedef TTimeConverter FAR * TTimeConverterPtr;

#endif
