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

void TMidiPlayer::CopyAndUseEvent(TEventPtr event, ULONG date_ms) { CopyAndUseEvent(event->MidiEvent(),date_ms); }
	
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

void TMidiPlayer::UseEvent(TEventPtr e, ULONG date_ms) { UseEvent(e->MidiEvent(),date_ms);}
	
/*--------------------------------------------------------------------------*/

void TMidiPlayer::UseEvent(MidiEvPtr e, ULONG date_ms) 
{ 
	if (IsPrivate(e)) { // Private events (typeLoopEnd and typeScoreEnd) are always sent
		fMidiAppl->SendAt(e,date_ms); 
	}else if (fTrackTable->IsOnTrack(TrackNum(e))) {
		fMidiAppl->SendAt(e,date_ms);
	}
}
	
