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
		Boolean ok = MidiGetSlotInfos (fSlots->InRef (), infos++);
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

