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
//	TClockEventReceiver.h			    
// ===========================================================================
//
//	Event receiver in kClockSync mode
// 

#include "TEventReceiver.h"

/*----------------------------------------------------------------------------*/

TClockEventReceiver::TClockEventReceiver( TPlayerInterfacePtr  player, 
										  TLoopManagerPtr loopmanager,
										  TClockConverterPtr converter,
										  TEventDispatcherPtr successor)
	:TEventReceiver(player,loopmanager,successor)
{
	fClockConverter = converter;
}

/*----------------------------------------------------------------------------*/

void TClockEventReceiver::ReceiveEvents(MidiEvPtr e)
{
	switch (EvType(e)) {
		case typeClock: ReceiveClock(e); break;
		case typeStart: ReceiveStart(e); break;
		case typeStop: ReceiveStop(e); break;
		case typeContinue: ReceiveContinue(e); break;
		case typeSongPos: ReceiveSongPos(e); break;
		case typeScoreEnd: ReceiveStop(e); break;
	}
	
	TEventReceiver::ReceiveEvents(e);
}

/*----------------------------------------------------------------------------*/

void TClockEventReceiver::ReceiveClock(MidiEvPtr e) {fPlayer->RcvClock(Date(e)); }

/*----------------------------------------------------------------------------*/

void TClockEventReceiver::ReceiveStart(MidiEvPtr e) {fPlayer->Start(); }

/*----------------------------------------------------------------------------*/

void TClockEventReceiver::ReceiveStop(MidiEvPtr e) {fPlayer->Stop();}

/*----------------------------------------------------------------------------*/

void TClockEventReceiver::ReceiveContinue(MidiEvPtr e) {fPlayer->Cont();}

/*----------------------------------------------------------------------------*/

void TClockEventReceiver::ReceiveSongPos(MidiEvPtr e) 
{
	ULONG songpos = (MidiGetField(e,0) + (MidiGetField(e,1)<<7));
	fPlayer->SetPosTicks((ULONG)fClockConverter->ConvertSongPosToTick((float)songpos));
}