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
//	TSyncInPlayer.h	   			 
// ===========================================================================
/*!
  \brief A TSyncInPlayer is the basic synchronisable player.

  A TSyncInPlayer uses three components :
  <UL>
 	<LI> a tickplayer : plays the score where dates are in ticks
 	<LI> a synchroniser : provides ticks ==> millisecond conversion
  	<LI> a chaser : maintain the "event to chase" state and send events when necessary (start, stop)
  </UL>
  Changing the synchronizer object allows to change the synchronization behaviour of the TSyncInPlayer.
*/  


#ifndef __TSynchroPlayer__
#define __TSynchroPlayer__

 
#include "TTickPlayer.h"
#include "TChaserIterator.h"
#include "TPlayerSynchroniser.h"
#include "TPlayerInterface.h"


//-----------------------
// Class TSyncInPlayer 
//-----------------------

class TSyncInPlayer :public TPlayerInterface{
	
	 private:
	
	 	TPlayerSynchroniserPtr 	  fSynchroniser;
	 	TTickPlayerInterfacePtr   fTickPlayer;
		TChaserInterfacePtr 	  fChaser;
			 	 	
	 public:
	 	
	  	TSyncInPlayer (TPlayerSynchroniserPtr sy, TTickPlayerInterfacePtr pl, TChaserInterfacePtr ch);
	 	virtual ~TSyncInPlayer (){}
	 
		void Start();
		void Stop();
		void Pause();
		void Cont();
		
		void PlaySliceForward();
 		void PlaySliceBackward();

		void SetPosTicks(ULONG date_ticks);
		ULONG GetPosTicks();
		
		void  SetTempo(ULONG tempo);
		ULONG GetTempo();
		
		void RcvClock (ULONG date_ms);
};

typedef TSyncInPlayer FAR * TSynchroPlayerPtr;



#endif
