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
//	TRunningPlayer.h		    
// ===========================================================================

/*!
\brief  A TRunningPlayer uses the Decorator pattern : additionnal behaviours are added 
 dynamically on the TPlayerInterface object being decorated.
 A TRunningPlayer calls the decorated player methods (Start, Stop, SetPosTicks)
 depending of the current running state, read in the TRunningState object. 
 */


#ifndef __TRunningPlayer__
#define __TRunningPlayer__


#include "TSyncInPlayer.h"
#include "TRunningState.h"


//----------------------
// Class TRunningPlayer
//----------------------

class TRunningPlayer : public TPlayerInterface {
	
	 private:
	 
	 	TRunningStatePtr  	fRunningState;
	 	TPlayerInterfacePtr fPlayer;
	 	
	  public:
	  	
	 	TRunningPlayer (TPlayerInterfacePtr player, TRunningStatePtr state);
	 	~TRunningPlayer () {}
	  
		void Start();
		void Stop();
		void Pause();
		void Cont();
		
		void PlaySliceForward();
 		void PlaySliceBackward();

		void SetPosTicks(ULONG date_ticks) ;	
		ULONG GetPosTicks();
		
		void  SetTempo(ULONG tempo);
		ULONG GetTempo();
		
		void  RcvClock(ULONG date_ms);
};
 

#endif
