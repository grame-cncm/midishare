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
Boolean TMidiSlot::Open (MidiName name, SlotDirection dir)
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
