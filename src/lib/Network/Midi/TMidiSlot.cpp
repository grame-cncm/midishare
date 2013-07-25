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


#include "TMidiSlot.h"

//_______________________________________________________________________________
// classe TMidiSlot
//_______________________________________________________________________________
TMidiSlot::TMidiSlot (short drvref)
{
	fDrvRef = drvref;
	fRefNum.slotRef = undefinedRefNum;
}

//_______________________________________________________________________________
TMidiSlot::~TMidiSlot()
{
	Close ();
}

//_______________________________________________________________________________
bool TMidiSlot::Open (MidiName name, SlotDirection dir)
{
	if (!Opened()) {
		fRefNum = MidiAddSlot (fDrvRef, name, dir);
		if (fRefNum.slotRef < 0) {
			fRefNum.slotRef = undefinedRefNum;
			return false;
		}
	}
	return true;
}

//_______________________________________________________________________________
void TMidiSlot::Close()
{
	if (Opened ()) {
		MidiRemoveSlot (fRefNum);
		fRefNum.slotRef = undefinedRefNum;
	}
}

//_______________________________________________________________________________
MidiName TMidiSlot::GetName()
{
	if (!MidiGetSlotInfos(fRefNum, &fInfos)) {
		fInfos.name[0] = 0;
	}
	return fInfos.name;
}
