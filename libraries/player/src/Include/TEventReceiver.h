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

class TEventReceiver :public TEventDispatcher {

	protected:
		
		TPlayerInterfacePtr  fPlayer;
		TLoopManagerPtr      fLoopManager;
	
	 	void ReceiveLoopEnd (MidiEvPtr e);
	 	void ReceiveStop (MidiEvPtr e);

	public:
	
		TEventReceiver(TPlayerInterfacePtr  player, TLoopManagerPtr loopmanager, TEventDispatcherPtr successor)
			:TEventDispatcher(successor),fPlayer(player),fLoopManager(loopmanager){}
		virtual ~TEventReceiver(){}
	
		virtual void ReceiveEvents (MidiEvPtr e);
};


//--------------------------
// Class TClockEventReceiver 
//--------------------------
/*!
	\brief A TEventReceiver object used for clock synchronization.
*/

class TClockEventReceiver :public TEventReceiver {

	private:
		
		TClockConverterPtr fClockConverter;
	
		void ReceiveSongPos (MidiEvPtr e);
		void ReceiveClock (MidiEvPtr e);
		void ReceiveStart (MidiEvPtr e);
		void ReceiveContinue (MidiEvPtr e);
		void ReceiveStop (MidiEvPtr e);

	public:
	
		TClockEventReceiver(TPlayerInterfacePtr  player, TLoopManagerPtr loopmanager, TClockConverterPtr converter, TEventDispatcherPtr successor)
			:TEventReceiver(player,loopmanager,successor),fClockConverter(converter){}
		virtual ~TClockEventReceiver(){}
	
		void ReceiveEvents (MidiEvPtr e);
};


//-----------------------------
// Class TExternalEventReceiver 
//-----------------------------
/*!
	\brief A TEventReceiver object used for external tempo synchronization.
*/

class TExtEventReceiver :public TEventReceiver {

	private:
	
		void ReceiveTempo (MidiEvPtr e);

	public:
	
		TExtEventReceiver(TPlayerInterfacePtr player, TLoopManagerPtr loopmanager,TEventDispatcherPtr successor)
			:TEventReceiver(player,loopmanager,successor){}
		virtual ~TExtEventReceiver(){}
	
		void ReceiveEvents (MidiEvPtr e);
		
};

//--------------------------
// Class TSMPTEEventReceiver 
//--------------------------
/*!
	\brief A TEventReceiver object used for SMPTE synchronization.
*/

class TSMPTEEventReceiver :public TEventReceiver {

	private:
	
		void ReceiveStart (MidiEvPtr e);
		void ReceiveStop (MidiEvPtr e);

	public:
	
		TSMPTEEventReceiver(TPlayerInterfacePtr player, TLoopManagerPtr loopmanager,TEventDispatcherPtr successor)
			:TEventReceiver(player,loopmanager,successor){}
		virtual ~TSMPTEEventReceiver(){}
	
		void ReceiveEvents (MidiEvPtr e);
		
};

 
#endif
