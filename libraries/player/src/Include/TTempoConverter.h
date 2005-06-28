/*

  Copyright � Grame 1996-2004

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
//	TTempoConverter.h		    
// ===========================================================================

#ifndef __TTempoConverter__
#define __TTempoConverter__

#include "TPlayerConstants.h"
#include "PlayerDefs.h"
#include "MidiShare.h"

//-----------------------
// Class TTempoConverter
//-----------------------
/*!
 \brief Musical time (ticks) to absolute time (10 micro-sec and millisecond) conversion functions (and the contrary).
 Computation are done using integer values and managing remainders to be exact.
 Absolute time values are expressed in units of 10 micro-second, in order to compute
 larger date values before having overflow problems with integers.
*/ 

class TTempoConverter {
	
	protected:

		/*! Number of micro second per tick. */
		ULONG fTen_micro_sec_per_tick;   	
		/*! Number of micro second per tick (remainder) */	
		ULONG fTen_micro_sec_per_tick_remain; 	
	
		/*! Date of the last Tempo change (in ticks) */
		ULONG fLast_tempo;      
		/*! Date of the last Tempo change (in 10 micro sec) */   	  
		ULONG fLast_tempo_ten_micro ; 
		
		/*! Current Tempo in 10 micro-sec per quarter note */
		ULONG fTempo;	
		/*! Current tick per quarter value */			  
		ULONG fTicks_per_quarter; 	  
	
		ULONG ConvertTicksToMicroSecAux (ULONG ticks);
		ULONG ConvertMicroSecToTicksAux (ULONG ten_micro_sec);
	
	public:
	
		TTempoConverter(){Init(kDefaultTpq);}
		virtual ~TTempoConverter(){}
		
		void Init(ULONG tpq);
		void Init(); 		  
		
		ULONG ConvertTickToMicroSec(ULONG date_ticks); 
		ULONG ConvertMicroSecToTick(ULONG date_ms);	
	
		ULONG ConvertTickToMs(ULONG date_ticks); 		
		ULONG ConvertMsToTick(ULONG date_ms);			
	
		void Update(ULONG date_ticks, ULONG tempo);
		
		/*! Returns current tempo 
		\return The result is current tempo in micro-sec per quarter note.
		*/
		ULONG GetTempo() {return fTempo * 10;}
};

typedef TTempoConverter FAR * TTempoConverterPtr;

#endif
