/*

  Copyright © Grame 1996-2006

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
//	TSyncInPlayer.h	   			 
// ===========================================================================

#ifndef __TSynchroPlayer__
#define __TSynchroPlayer__
 
#include "TTickPlayer.h"
#include "TChaserIterator.h"
#include "TPlayerSynchroniser.h"
#include "TPlayerInterface.h"

//---------------------
// Class TSyncInPlayer 
//---------------------
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


class TSyncInPlayer : public TPlayerInterface {
	
	 private:
	
	 	TPlayerSynchroniserPtr 	  fSynchroniser;
	 	TTickPlayerInterfacePtr   fTickPlayer;
		TChaserInterfacePtr 	  fChaser;
			 	 	
	 public:
	 	
	  	TSyncInPlayer (TPlayerSynchroniserPtr sy, TTickPlayerInterfacePtr pl, TChaserInterfacePtr ch);
	 	virtual ~TSyncInPlayer() {}
	 
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
