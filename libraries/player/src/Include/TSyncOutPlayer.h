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
//	TSyncOutPlayer.h	   			 
// ===========================================================================
//
//	A TSyncOutPlayer object send Clock,Start,Stop,Continue and SongPos events
//  
//  A TSyncOutPlayer use the Decorator pattern: additionnal behaviours are added 
//  dynamically on the TPlayerInterface object being decorated.


#ifndef __TSyncOutPlayer__
#define __TSyncOutPlayer__

#include "TSyncInPlayer.h"
#include "TClockSender.h"


//-----------------------
// Class TSyncOutPlayer
//-----------------------

class TSyncOutPlayer :public TPlayerInterface {
	
	 private:
	 
	 	TClockConverterPtr  fClockConverter;
	 	TClockSenderPtr	 	fClocksender;
	 	TPlayerInterfacePtr fPlayer;
	 	Boolean             fNeedSongPos;
	 	
	 public:
	  	
	 	TSyncOutPlayer (TPlayerInterfacePtr player, TClockSenderPtr clock,TClockConverterPtr converter);
	 	~TSyncOutPlayer (){}
	  
		void Start();
		void Stop();
		void Pause();
		void Cont() ;
		
		void PlaySliceForward();
 		void PlaySliceBackward() ;

		void SetPosTicks (ULONG date_ticks) ;
		ULONG GetPosTicks();
		
		void SetTempo (ULONG tempo);
		ULONG GetTempo ();
		void RcvClock (ULONG date_ms);
};
 

#endif
