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
//	TExtEventReceiver.h			    
// ===========================================================================
//
//	Event receiver in kExternalSync mode
// 

#include "TEventReceiver.h"

/*----------------------------------------------------------------------------*/

void TExtEventReceiver::ReceiveEvents(MidiEvPtr e)
{
	switch (EvType(e)) {
		case typeTempo: ReceiveTempo(e); break;
	}
	
	TEventReceiver::ReceiveEvents(e);
}

/*----------------------------------------------------------------------------*/

void TExtEventReceiver::ReceiveTempo(MidiEvPtr e) {fPlayer->SetTempo(MidiGetField(e,0));}

