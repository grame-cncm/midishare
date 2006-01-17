/*

  Copyright © Grame 1996-2004

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
//	TClockEventReceiver.h			    
// ===========================================================================
//
//	Event receiver in kClockSync mode
// 

#include "TEventReceiver.h"

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

void TClockEventReceiver::ReceiveClock(MidiEvPtr e) {fPlayer->RcvClock(Date(e));}

/*----------------------------------------------------------------------------*/

void TClockEventReceiver::ReceiveStart(MidiEvPtr e) {fPlayer->Start();}

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

