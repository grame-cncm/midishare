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
//	TEventReceiver.cpp			    
// ===========================================================================
//
//	A TEventReceiver object handles incoming events
// 


#include "TEventReceiver.h"
#include "TDestructor.h"

/*----------------------------------------------------------------------------*/

TEventReceiver::TEventReceiver( TPlayerInterfacePtr  player, TLoopManagerPtr loopmanager,TEventDispatcherPtr successor)
	:TEventDispatcher(successor)
{
	fLoopManager = loopmanager;
	fPlayer = player;
}

/*----------------------------------------------------------------------------*/

void TEventReceiver::ReceiveLoopEnd(MidiEvPtr e)
{
	if (fLoopManager->IsLoopOn()) {
		fPlayer->SetPosTicks(fLoopManager->GetLoopStartTicks());
	}
}

/*----------------------------------------------------------------------------*/

void TEventReceiver::ReceiveStop(MidiEvPtr e){ fPlayer->Stop(); }

/*----------------------------------------------------------------------------*/

void TEventReceiver::ReceiveEvents(MidiEvPtr e)
{
	switch (EvType(e)) {
		case typeLoopEnd: ReceiveLoopEnd(e); break;
		case typeScoreEnd: ReceiveStop(e); break;
	}
	
	TEventDispatcher::ReceiveEvents(e); // Calls the next object ReceiveEvents method
}

