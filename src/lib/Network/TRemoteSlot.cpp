/*

  Copyright © Grame 2001
  Copyright © Mil Productions 2001

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
  grame@grame.fr

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
Boolean TRemoteSlot::Open (MidiName name, Boolean wantFilter)
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
