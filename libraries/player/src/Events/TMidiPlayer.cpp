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
//	TMidiPlayer.cpp			    
// ===========================================================================
//
//	Midi events output management
// 


#include "TMidiPlayer.h"
#include "UTools.h"


/*--------------------------------------------------------------------------*/

void TMidiPlayer::SendEvent(MidiEvPtr event, ULONG date_ms) {fMidiAppl->SendAt(event,date_ms);}

/*--------------------------------------------------------------------------*/

void TMidiPlayer::CopyAndUseEvent(TEventPtr event, ULONG date_ms) {CopyAndUseEvent(event->MidiEvent(),date_ms);}
	
/*--------------------------------------------------------------------------*/

void TMidiPlayer::CopyAndUseEvent(MidiEvPtr e, ULONG date_ms)
{ 
	if (IsPrivate(e)) { // Private events (typeLoopEnd and typeScoreEnd) are always sent
		fMidiAppl->SendAt(MidiCopyEv(e),date_ms); 
	}else{
		fMidiAppl->SendAt(fTrackTable->IsPlayable(e),date_ms); 
	}
}

/*--------------------------------------------------------------------------*/

void TMidiPlayer::UseEvent(TEventPtr e, ULONG date_ms) {UseEvent(e->MidiEvent(),date_ms);}
	
/*--------------------------------------------------------------------------*/

void TMidiPlayer::UseEvent(MidiEvPtr e, ULONG date_ms) 
{ 
	if (IsPrivate(e)) { // Private events (typeLoopEnd and typeScoreEnd) are always sent
		fMidiAppl->SendAt(e,date_ms); 
	}else if (fTrackTable->IsOnTrack(TrackNum(e))) {
		fMidiAppl->SendAt(e,date_ms);
	}
}
	
