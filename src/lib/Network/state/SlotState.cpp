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


#include "StatesManager.h"
#include "TRemoteSlot.h"

#if macintosh
#define kSlotCnxID 'cnSl'
#else
#include "FilterUtils.h"
#define kSlotCnxID	2
#endif

//____________________________________________________________
// Application connections state
//____________________________________________________________
SlotState::SlotState (TRemoteSlot * slots)
{
	fSlots = slots;
	fMemory = INetNewHandle (sizeof (TSlotInfos) * 2);
}

//____________________________________________________________
SlotState::~SlotState ()
{
	if (fMemory)  INetDisposeHandle (fMemory);
}

//____________________________________________________________
long SlotState::ID ()
{
	return kSlotCnxID;
}

//____________________________________________________________
INetHandle SlotState::GetState ()
{
	if (fMemory) {
		TSlotInfos * infos = (TSlotInfos *)HandlePtr(fMemory);
		bool ok = MidiGetSlotInfos (fSlots->InRef (), infos++);
		ok = MidiGetSlotInfos (fSlots->OutRef (), infos);
	}
	return fMemory;
}

//____________________________________________________________
void SlotState::SetState (INetHandle h)
{
	TSlotInfos * infos = (TSlotInfos *)HandlePtr(h);
	RestoreSlot (fSlots->InRef (), infos->cnx);
	infos++;
	RestoreSlot (fSlots->OutRef (), infos->cnx);
}

//____________________________________________________________
void SlotState::RestoreSlot (SlotRefNum slot, char *cnx)
{
	for (short i=0; i<256; i++) {
		if (IsAcceptedBit (cnx, i))
			MidiConnectSlot (i, slot, true);
	}
}

//____________________________________________________________
void SlotState::Alarm (short refnum, short srcRef, short changeCode)
{
}

