// ===========================================================================
// The Player Library is Copyright (c) Grame, Computer Music Research Laboratory 
// 1996-1999, and is distributed as Open Source software under the Artistic License;
// see the file "Artistic" that is included in the distribution for details.
//
// Grame : Computer Music Research Laboratory
// Web : http://www.grame.fr/Research
// E-mail : MidiShare@rd.grame.fr
// ===========================================================================


// ===========================================================================
//	TTempoConverter.h		    
// ===========================================================================
//
// Tempo management: musical time (ticks) to absolute time (millisecond) 
// conversion functions (and vice versa)
//
// Computation are done using integer values and managing remainders to be exact
// 


#ifndef __TTempoConverter__
#define __TTempoConverter__

#include "TPlayerConstants.h"
#include "GenericMidiShare.h"

//---------------------------
// Class TTempoConverter
//---------------------------

class TTempoConverter  {
	
	protected:

		ULONG fTen_micro_sec_per_tick;   		// Number of micro second per tick
		ULONG fTen_micro_sec_per_tick_remain; 	// Number of micro second per tick (remainder)
	
		ULONG fLast_tempo;         	  // Date of the last Tempo change (in ticks)
		ULONG fLast_tempo_ten_micro ; // Date of the last Tempo change (in 10 micro sec)
		
		ULONG fTempo;				  // Current Tempo in micro second per quarter note
		ULONG fTicks_per_quarter; 	  // Current tick per quarter value
	
		ULONG ConvertTicksToMicroSecAux (ULONG ticks);
		ULONG ConvertMicroSecToTicksAux (ULONG ten_micro_sec);
	
	public:
	
		TTempoConverter() {Init(kDefaultTpq);}
		~TTempoConverter() {}
		
		void Init(ULONG tpq);
		void Init(); 		  
		
		ULONG ConvertTickToMicroSec(ULONG date_tick); 
		ULONG ConvertMicroSecToTick(ULONG date_ms);	
	
		ULONG ConvertTickToMs(ULONG date_tick); 		
		ULONG ConvertMsToTick(ULONG date_ms);			
	
		void Update(MidiEvPtr tempo);
		void Update(ULONG date_ticks, ULONG tempo);
		
		ULONG GetTempo() {return fTempo * 10;}
		
};

typedef TTempoConverter FAR * TTempoConverterPtr;


#endif
