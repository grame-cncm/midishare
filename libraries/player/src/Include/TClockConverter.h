// ===========================================================================
// The Player Library is Copyright (c) Grame, Computer Music Research Laboratory 
// 1996-2003, and is distributed as Open Source software under the Artistic License;
// see the file "Artistic" that is included in the distribution for details.
//
// Grame : Computer Music Research Laboratory
// Web : http://www.grame.fr/Research
// E-mail : MidiShare@rd.grame.fr
// ===========================================================================


// ===========================================================================
//	TClockConverter.h			    
// ===========================================================================

#ifndef __TClockConverter__
#define __TClockConverter__

#include "TPlayerConstants.h"
#include <math.h>

#ifndef round
//static float round(float val){return (float)floor(val+0.5f);}
#endif

//-----------------------
// Class TClockConverter
//-----------------------
/*!
	\brief Conversion functions between values in clocks, songpos to ticks (and vice-versa).
*/


class TClockConverter {

	private:
	
		float   fTicks_per_quarter;
	
	public: 
	
		TClockConverter(ULONG tpq) {Init (tpq);}
		virtual ~TClockConverter(){}
			
		void Init (ULONG tpq) {fTicks_per_quarter = (float)tpq;}
				
		ULONG ConvertDeltaToTempo (ULONG delta_ms) {return delta_ms * 24000;}
		
		float ConvertTickToClock (float date_ticks) {return round((24.0f * date_ticks)/fTicks_per_quarter);}
		float ConvertClockToTick (float clocks) {return round((clocks * fTicks_per_quarter)/24.0f);}
		
		float ConvertSongPosToTick (float pos) {return round((pos * fTicks_per_quarter)/4.0f);}
		float ConvertTickToSongPos (float date_ticks) {return round((date_ticks * 4.0f)/fTicks_per_quarter);}
		
		float ConvertTickToTickAtPrevSP (float date_ticks) 
		{ 
			float prev_songpos = floor((date_ticks * 4.0f)/fTicks_per_quarter);
			return ConvertSongPosToTick(prev_songpos);
		}
		
		float ConvertTickToTickAtPrevClock (float date_ticks) 
		{ 
			float prev_clock = floor((24.0f * date_ticks)/fTicks_per_quarter);
			return 	ConvertClockToTick(prev_clock);
		}
};


typedef TClockConverter FAR * TClockConverterPtr;


#endif
