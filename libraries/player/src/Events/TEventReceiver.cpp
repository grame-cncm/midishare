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
//	TEventReceiver.cpp			    
// ===========================================================================
//
//	A TEventReceiver object handles incoming events
// 

#include "TEventReceiver.h"

/*----------------------------------------------------------------------------*/

void TEventReceiver::ReceiveLoopEnd(MidiEvPtr e)
{
	if (fLoopManager->IsLoopOn()) {
		fPlayer->SetPosTicks(fLoopManager->GetLoopStartTicks());
	}
}

/*----------------------------------------------------------------------------*/

void TEventReceiver::ReceiveStop(MidiEvPtr e) {fPlayer->Stop();}

/*----------------------------------------------------------------------------*/

void TEventReceiver::ReceiveEvents(MidiEvPtr e)
{
	switch (EvType(e)) {
		case typeLoopEnd: ReceiveLoopEnd(e); break;
		case typeScoreEnd: ReceiveStop(e); break;
	}
	
	TEventDispatcher::ReceiveEvents(e); // Calls the next object ReceiveEvents method
}

