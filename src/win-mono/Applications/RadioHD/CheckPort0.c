/*

  Copyright © Grame 2001

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


#include <Windows.h>
#include "MidiShare.h"
#include "RadioHD.h"

//________________________________________________________________
static Boolean Port0Connected ()
{
	short i, j, n = MidiCountDrivers ();
	for (i=1; i<=n; i++) {
		TDriverInfos infos;
		short ref = MidiGetIndDriver (i);
		if ((ref > 0) && MidiGetDriverInfos (ref, &infos)) {
			for (j=0; j < infos.slots; j++) {
				SlotRefNum sref = MidiGetIndSlot (ref, j);
				if ((Slot(sref) > 0) && MidiIsSlotConnected (0, sref))
					return TRUE;
			}
		}
	}
	return FALSE;
}

//________________________________________________________________
static Boolean IsInternalSynth (char *name)
{
	UINT i, n = midiOutGetNumDevs ();
	for (i=0; i<n; i++) {
		MIDIOUTCAPS caps; 
		MMRESULT res = midiOutGetDevCaps (i, &caps, sizeof(caps));
		if (res == MMSYSERR_NOERROR) {
			if (caps.wTechnology != MOD_MIDIPORT)
				return strcmp (name, caps.szPname) == 0;
		}
	}
	return FALSE;
}

//________________________________________________________________
static SlotRefNum ChooseDefaultSlot ()
{
	SlotRefNum sref = { 0 };
	short i, j, n = MidiCountDrivers ();
	for (i=1; i<=n; i++) {
		TDriverInfos infos;
		short ref = MidiGetIndDriver (i);
		if ((ref > 0) && MidiGetDriverInfos (ref, &infos)) {
			for (j=0; j < infos.slots; j++) {
				TSlotInfos sinfos;
				sref = MidiGetIndSlot (ref, j);
				if ((Slot(sref) > 0) && MidiGetSlotInfos (sref, &sinfos)) {
					if (sinfos.direction & MidiOutputSlot) {
						if (IsInternalSynth (sinfos.name))
							return sref;
					}
				}
			}
		}
	}
	sref.drvRef = sref.slotRef = 0;
	return sref;
}

//________________________________________________________________
void CheckPort0Connection ()
{
	if (!Port0Connected ()) {
		SlotRefNum ref = ChooseDefaultSlot ();
		if (Slot(ref))
			MidiConnectSlot (0, ref, TRUE);
	}
}

