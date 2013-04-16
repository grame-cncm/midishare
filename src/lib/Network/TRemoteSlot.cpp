/*
  MidiShare Project
  Copyright (C) Grame 2001
  Copyright (C) Mil Productions 2001

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/


#include "TRemoteSlot.h"

//____________________________________________________________
// TRemoteSlot
//____________________________________________________________
TRemoteSlot::TRemoteSlot (IPNum id, short drvRef)
	: fInSlot(drvRef), fOutSlot(drvRef)
{
	fSeq = 0;
	fID = id;
	fSlotsState = 0;
}

//____________________________________________________________
bool TRemoteSlot::Open (MidiName name, bool wantFilter)
{
	if (!fInSlot.Open (name, MidiInputSlot))
		return false;
	if (!fOutSlot.Open (name, MidiOutputSlot))
		return false;

	fSeq = MidiNewSeq ();
	if (!fSeq) return false;
	
	fRefNum = fInSlot.DrvRef ();
	fSlotsState = new SlotState (this);
	if (fSlotsState) {
		fStateManager.SetState (fSlotsState);
		fStateManager.LoadState (fID);
	}
	return true;
}

//____________________________________________________________
void TRemoteSlot::Close ()
{
	if (fSlotsState) {
		fStateManager.SaveState (fID);
		fStateManager.SetState (0);
		delete fSlotsState;
		fSlotsState = 0;
	}
	if (fSeq) MidiFreeSeq (fSeq);
	fSeq = 0;
	fInSlot.Close ();
	fOutSlot.Close ();
	fRefNum = undefinedRefNum;
}

//____________________________________________________________
MidiName TRemoteSlot::GetName ()
{ 
	return fInSlot.GetName();
}

//____________________________________________________________
void TRemoteSlot::Send (MidiEvPtr e)
{
	Port(e) = fInSlot.SlotRef();
	MidiSend (GetRefNum(), e);
}

//____________________________________________________________
MidiEvPtr TRemoteSlot::GetEv ()
{
	MidiEvPtr e = fSeq->first;
	if (e) {
		fSeq->first = Link(e);
		if (!Link(e)) fSeq->last = 0;
	}
	return e;
}
