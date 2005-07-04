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
//	TClockConverter.h			    
// ===========================================================================

#ifndef __TClockConverter__
#define __TClockConverter__

#include "TPlayerConstants.h"
#include <math.h>

#ifdef __MSWindows__
static float round(float val){return (float)floor(val+0.5f);}
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
			float prev_songpos = (float)floor((date_ticks * 4.0f)/fTicks_per_quarter);
			return ConvertSongPosToTick(prev_songpos);
		}
		
		float ConvertTickToTickAtPrevClock (float date_ticks) 
		{ 
			float prev_clock = (float)floor((24.0f * date_ticks)/fTicks_per_quarter);
			return 	ConvertClockToTick(prev_clock);
		}
};

typedef TClockConverter FAR * TClockConverterPtr;

#endif
