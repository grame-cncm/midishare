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
//	TTrackTable.h			    
// ===========================================================================
//
//	Tracks management
//


#ifndef __TTrackTable__
#define __TTrackTable__

 
#include "TTrack.h"
#include "TPlayerConstants.h"

//-----------------------
// Class TTrackTable 
//-----------------------

class TTrackTable {

	friend class TTrack;

	private:	
		
		TTrackPtr 	fTracktable[kMaxTrack];   		// Track table
		long 		fSolo;							// Global Solo state
		
		Boolean	IsValid (short tracknum , short param)  
		{ 
			return ((tracknum >= 0) && (tracknum < kMaxTrack) && (param >= 0) && (param <kMaxParam));
		}
		
		Boolean	IsNewValue(short tracknum, short param,short value) { return (value != fTracktable[tracknum]->fParam[param]);}
		
	public :
 	
 		TTrackTable();
 		~TTrackTable();
 	
		void		SetParam (short tracknum, short param, short value);
		short		GetParam (short tracknum, short param);
	
		MidiEvPtr 	IsPlayable (MidiEvPtr e);
		void 		Clear();
		
		Boolean IsOnTrack(short tracknum) 
		{ 
			return ((fSolo > 0) &&  (fTracktable[tracknum]->fParam[kSolo]))
				| ((fSolo == 0) && (!fTracktable[tracknum]->fParam[kMute]));
		}
				
		Boolean IsOffTrack( short tracknum) { return (!IsOnTrack(tracknum));}
	
};

typedef  TTrackTable FAR * TTrackTablePtr;

#endif
