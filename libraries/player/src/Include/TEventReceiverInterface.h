// ===========================================================================
// The Player Library is Copyright (c) Grame, Computer Music Research Laboratory 
// 1996-1999, and is distributed as Open Source software under the Artistic License;
// see the file "Artistic" that is included in the distribution for details.
//
// Grame : Computer Music Research Laboratory
// Web : http://www.grame.fr/Research
// E-mail : MidiShare@rd.grame.fr
// ===========================================================================


// ==============================
//	TEventReceiverInterface.h		
// ==============================
//
//	Abstract class for an object which reveive incoming events


#ifndef __TEventReceiverInterface__
#define __TEventReceiverInterface__

#include "GenericMidiShare.h"

//-------------------------------
// Class TEventReceiverInterface 
//-------------------------------

class TEventReceiverInterface  {

	public:
	
		virtual ~TEventReceiverInterface(){}
		virtual void ReceiveEvents(MidiEvPtr e) = 0;
			
};

typedef TEventReceiverInterface FAR * TEventReceiverInterfacePtr;

//------------------------
// Class TEventDispatcher 
//------------------------
// Chain of responsability pattern

class TEventDispatcher  :public TEventReceiverInterface{

	private:
	
		TEventDispatcher* fSuccessor;

	public:
	
		TEventDispatcher() { fSuccessor = 0;}
		TEventDispatcher(TEventDispatcher* successor){ fSuccessor = successor;}
		virtual ~TEventDispatcher(){}
		
		void SetSuccessor(TEventDispatcher* successor){ fSuccessor = successor;}
	
		virtual void ReceiveEvents(MidiEvPtr e)
		{ 
			if (fSuccessor) { fSuccessor->ReceiveEvents(e); } else { MidiFreeEv(e);}
		}
		
};


typedef TEventDispatcher FAR * TEventDispatcherPtr;

#endif
