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
/*!
	\brief Conversion functions: clocks, songpos <===> ticks.
*/


#ifndef __TClockConverter__
#define __TClockConverter__

#include "TPlayerConstants.h"
#include <math.h>

//-----------------------
// Class TClockConverter
//-----------------------

class TClockConverter {

	private:
	
		float   fTicks_per_quarter;
	
	public: 
	
		TClockConverter(ULONG tpq) {Init (tpq);}
		~TClockConverter(){}
			
		void Init (ULONG tpq) {fTicks_per_quarter = (float)tpq;}
				
		ULONG ConvertDeltaToTempo (ULONG delta_ms) {return delta_ms * 24000;}
		
		float ConvertTickToClock (float date_ticks) {return round((24.0f * date_ticks)/fTicks_per_quarter);}
		float ConvertClockToTick (float clocks) {return round((clocks * fTicks_per_quarter)/24.0f);}
		
		float ConvertSongPosToTick (float pos) {return round((pos * fTicks_per_quarter)/4.0f);}
		float ConvertTickToSongPos (float date_ticks) {return round((date_ticks * 4.0f)/fTicks_per_quarter);}
		
		float ConvertTickToTickAtPrevSP (float date_ticks) 
		{ 
			return ConvertSongPosToTick(ConvertTickToSongPos(date_ticks));
		}
		
		float ConvertTickToTickAtPrevClock (float date_ticks) 
		{ 
			return ConvertClockToTick(ConvertTickToClock(date_ticks));
		}
		
};


typedef TClockConverter FAR * TClockConverterPtr;


#endif
