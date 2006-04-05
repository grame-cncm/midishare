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
//	TSyncOutPlayer.h	   			 
// ===========================================================================

#ifndef __TSyncOutPlayer__
#define __TSyncOutPlayer__

#include "TSyncInPlayer.h"
#include "TClockSender.h"

//----------------------
// Class TSyncOutPlayer
//----------------------
/*!
  \brief A TSyncOutPlayer object send Clock, Start, Stop, Continue and SongPos events.
  A TSyncOutPlayer use the Decorator pattern. Additionnal behaviours are added 
  dynamically on the TPlayerInterface object being decorated.
*/

class TSyncOutPlayer :public TPlayerInterface {
	
	 private:
	 
	 	TClockConverterPtr  fClockConverter;
	 	TClockSenderPtr	 	fClocksender;
	 	TPlayerInterfacePtr fPlayer;
	 	Boolean             fNeedSongPos;
	 	
	 public:
	  	
	 	TSyncOutPlayer (TPlayerInterfacePtr player, TClockSenderPtr clock,TClockConverterPtr converter);
	 	virtual ~TSyncOutPlayer() {}
	  
		void Start();
		void Stop();
		void Pause();
		void Cont() ;
		
		void PlaySliceForward();
 		void PlaySliceBackward() ;

		void SetPosTicks(ULONG date_ticks) ;
		ULONG GetPosTicks();
		
		void SetTempo(ULONG tempo);
		ULONG GetTempo();
		void RcvClock(ULONG date_ms);
}; 

#endif
