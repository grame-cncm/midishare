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
//	TSMPTEEventReceiver.h			    
// ===========================================================================
//
//	Event receiver in kSMPTESync mode
// 


#include "TEventReceiver.h"

/*----------------------------------------------------------------------------*/

TSMPTEEventReceiver::TSMPTEEventReceiver( TPlayerInterfacePtr  player, TLoopManagerPtr loopmanager,TEventDispatcherPtr successor)
	:TEventReceiver(player,loopmanager,successor)
{
}

/*----------------------------------------------------------------------------*/

void TSMPTEEventReceiver::ReceiveEvents(MidiEvPtr e)
{
	switch (EvType(e)) {
		case typeStart: ReceiveStart(e); break;
		case typeStop: ReceiveStop(e); break;
	}
	
	TEventReceiver::ReceiveEvents(e);
}

/*----------------------------------------------------------------------------*/

void TSMPTEEventReceiver::ReceiveStart(MidiEvPtr e) { fPlayer->Start(); }

/*----------------------------------------------------------------------------*/

void TSMPTEEventReceiver::ReceiveStop(MidiEvPtr e) { fPlayer->Stop(); }

