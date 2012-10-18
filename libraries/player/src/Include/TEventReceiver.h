/*

  Copyright (C) Grame 1996-2012

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
//	TEventReceiver.h			    
// ===========================================================================

#ifndef __TEventReceiver__
#define __TEventReceiver__

#include "TEventReceiverInterface.h"
#include "TLoopManager.h"
#include "TPlayerInterface.h"
#include "TClockConverter.h"

//-----------------------
// Class TEventReceiver 
//-----------------------
/*!
	\brief A TEventReceiver object handles incoming events.
*/

class TEventReceiver : public TEventDispatcher {

	protected:
		
		TPlayerInterfacePtr  fPlayer;
		TLoopManagerPtr      fLoopManager;
	
	 	void ReceiveLoopEnd(MidiEvPtr e);
	 	void ReceiveStop(MidiEvPtr e);

	public:
	
		TEventReceiver(TPlayerInterfacePtr  player, TLoopManagerPtr loopmanager, TEventDispatcherPtr successor)
			:TEventDispatcher(successor),fPlayer(player),fLoopManager(loopmanager) {}
		virtual ~TEventReceiver() {}
	
		virtual void ReceiveEvents(MidiEvPtr e);
};


//--------------------------
// Class TClockEventReceiver 
//--------------------------
/*!
	\brief A TEventReceiver object used for clock synchronization.
*/

class TClockEventReceiver : public TEventReceiver {

	private:
		
		TClockConverterPtr fClockConverter;
	
		void ReceiveSongPos(MidiEvPtr e);
		void ReceiveClock(MidiEvPtr e);
		void ReceiveStart(MidiEvPtr e);
		void ReceiveContinue(MidiEvPtr e);
		void ReceiveStop(MidiEvPtr e);

	public:
	
		TClockEventReceiver(TPlayerInterfacePtr  player, TLoopManagerPtr loopmanager, TClockConverterPtr converter, TEventDispatcherPtr successor)
			:TEventReceiver(player,loopmanager,successor),fClockConverter(converter) {}
		virtual ~TClockEventReceiver() {}
	
		void ReceiveEvents(MidiEvPtr e);
};


//-----------------------------
// Class TExternalEventReceiver 
//-----------------------------
/*!
	\brief A TEventReceiver object used for external tempo synchronization.
*/

class TExtEventReceiver : public TEventReceiver {

	private:
	
		void ReceiveTempo(MidiEvPtr e);

	public:
	
		TExtEventReceiver(TPlayerInterfacePtr player, TLoopManagerPtr loopmanager,TEventDispatcherPtr successor)
			:TEventReceiver(player,loopmanager,successor) {}
		virtual ~TExtEventReceiver() {}
	
		void ReceiveEvents (MidiEvPtr e);
};

//--------------------------
// Class TSMPTEEventReceiver 
//--------------------------
/*!
	\brief A TEventReceiver object used for SMPTE synchronization.
*/

class TSMPTEEventReceiver : public TEventReceiver {

	private:
	
		void ReceiveStart(MidiEvPtr e);
		void ReceiveStop(MidiEvPtr e);

	public:
	
		TSMPTEEventReceiver(TPlayerInterfacePtr player, TLoopManagerPtr loopmanager,TEventDispatcherPtr successor)
			:TEventReceiver(player,loopmanager,successor) {}
		virtual ~TSMPTEEventReceiver() {}
	
		void ReceiveEvents(MidiEvPtr e);
};

#endif
