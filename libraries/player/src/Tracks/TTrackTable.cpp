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
//	TTrackTable.cpp			    
// ===========================================================================
//
//	Tracks management
//


#include "TTrackTable.h"

/*--------------------------------------------------------------------------*/

void TTrackTable::SetParam(short tracknum, short param, short value)
{
	if (IsValid(tracknum, param) && IsNewValue(tracknum, param, value)) // If params are valid 
	{ 
		fTracktable[tracknum].fParam[param] = value;
		if (param == kSolo) {if (value == kSoloOn) fSolo++; else fSolo--;}
	}
}

/*--------------------------------------------------------------------------*/

short TTrackTable::GetParam(short tracknum, short param) 
{
    return (IsValid(tracknum, param)) ? fTracktable[tracknum].fParam[param] : -1;
}

/*--------------------------------------------------------------------------*/

MidiEvPtr TTrackTable::IsPlayable(MidiEvPtr e) {return (IsOnTrack(TrackNum(e))) ? MidiCopyEv(e): 0;}

/*--------------------------------------------------------------------------*/

void TTrackTable::Clear()
{
	fSolo = 0;
	for (short i = 0; i< kMaxTrack; i++) fTracktable[i].Init();
}
