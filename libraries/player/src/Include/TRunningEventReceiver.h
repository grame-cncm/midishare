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
// TRunningEventReceiver.h			    
// ===========================================================================
//

#ifndef __TRunningEventReceiver__
#define __TRunningEventReceiver__


#include "TEventReceiverInterface.h"
#include "UTools.h"


class TRunningEventReceiver :public TEventReceiverInterface {
	
	private :
	
		TEventReceiverInterfacePtr 	fReceiver; 		// Decorated receiver
		TRunningStatePtr 			fRunningState;	// Player state
		
	public :
	
		TRunningEventReceiver (TEventReceiverInterfacePtr receiver,TRunningStatePtr state) {
			fReceiver = receiver;
			fRunningState = state;
		}
		
		virtual ~TRunningEventReceiver(){}

		void ReceiveEvents(MidiEvPtr e) 
		{
		 	// Never filter synchro events 
		 	switch (EvType(e)) {
		 	
		 		case typeClock:
				case typeStart:
				case typeStop:
				case typeContinue:
				case typeSongPos:
				case typeScoreEnd:
					 fReceiver->ReceiveEvents(e);
					 break;
				
				// filter all other events when not running
				default:
					if (fRunningState->IsRunning()) {
						fReceiver->ReceiveEvents(e);
					}else {
						MidiFreeEv(e);
					}
					break;
			}
		}

};


typedef TRunningEventReceiver FAR * TRunningEventReceiverPtr;


#endif