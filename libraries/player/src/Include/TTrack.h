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
//	TTrack.h			    
// ===========================================================================


#ifndef __TTrack__
#define __TTrack__

 
#include "GenericMidiShare.h"
#include "TPlayerConstants.h"


//--------------
// Class TTrack 
//--------------
/*! 
	\brief Tracks management.
*/

class TTrack{

	friend class TTrackTable;
	
		short fParam[kMaxParam];   // Parameter table for one track
	
	public :
 	
 		TTrack() {Init();}	
 		virtual ~TTrack(){}
 		
 		void Init() 
 		{
 			fParam[kSolo] = kSoloOff; 
 			fParam[kMute] = kMuteOff;
 		}
 	
 };
	
typedef  TTrack FAR * TTrackPtr;
	

#endif
