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
//	TTrackTable.cpp			    
// ===========================================================================
//
//	Tracks management
//


#include "TTrackTable.h"

/*--------------------------------------------------------------------------*/

TTrackTable::TTrackTable ()
{
	fSolo = kSoloOff;
	for (short i = 0; i< kMaxTrack; i++) {fTracktable[i] = new TTrack();}
}

/*--------------------------------------------------------------------------*/

TTrackTable::~TTrackTable () { for (short i = 0; i< kMaxTrack; i++) { delete(fTracktable[i]);} }

/*--------------------------------------------------------------------------*/

void TTrackTable::SetParam (short tr, short p, short v)
{
	if (IsValid (tr,p) && IsNewValue(tr,p,v)){ // If params are valid 
		
		fTracktable[tr]->fParam[p] = v;
		if (p == kSolo) { if (v == kSoloOn) fSolo++; else fSolo--;}
	}
}

/*--------------------------------------------------------------------------*/

short  TTrackTable::GetParam (short tr, short p) { return (IsValid (tr,p)) ? fTracktable[tr]->fParam[p] : -1;}

/*--------------------------------------------------------------------------*/

MidiEvPtr TTrackTable::IsPlayable (MidiEvPtr e) { return (IsOnTrack(TrackNum(e))) ? MidiCopyEv(e): 0; }

/*--------------------------------------------------------------------------*/

void TTrackTable::Clear()
{
	fSolo = kSoloOff;
	for (short i = 0; i< kMaxTrack; i++) { fTracktable[i]->Init();}
}