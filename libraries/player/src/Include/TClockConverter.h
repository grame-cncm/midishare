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
//	TClockConverter.h			    
// ===========================================================================
//
//	Conversion functions: cloks <===> ticks
// 


#ifndef __TClockConverter__
#define __TClockConverter__

#include "TPlayerConstants.h"


//---------------------------
// Class TClockConverter
//---------------------------

class TClockConverter {

	private:
	
		ULONG   fTicks_per_quarter;
	
	public: 
	
		TClockConverter(ULONG tpq) { Init (tpq); }
		~TClockConverter(){}
			
		void Init (ULONG tpq) { fTicks_per_quarter = tpq; }
		
		ULONG ConvertDeltaToTempo(ULONG delta_ms) {return delta_ms * 24000;}
		
		ULONG ConvertTickToClock (ULONG date_ticks) { return (24 * date_ticks)/fTicks_per_quarter;}
		ULONG ConvertClockToTick (ULONG clocks) { return (clocks * fTicks_per_quarter)/24;}
		
		ULONG ConvertSongPosToTick (ULONG pos) { return (pos * fTicks_per_quarter)/4;}
		ULONG ConvertTickToSongPos (ULONG date_ticks) { return (date_ticks * 4)/fTicks_per_quarter;}
		
		ULONG ConvertTickToTickAtPrevSP (ULONG date_ticks) 
		{ 
			return ConvertSongPosToTick(ConvertTickToSongPos(date_ticks));
		}
		
		ULONG ConvertTickToTickAtPrevClock (ULONG date_ticks) 
		{ 
			return ConvertClockToTick(ConvertTickToClock(date_ticks));
		}
};

typedef TClockConverter FAR * TClockConverterPtr;


#endif
