/*

  Copyright © Grame 2002

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

#include "MidiShare.h"
#include "FilterUtils.h"
#include "msMidiState.h"
#include "msDriverState.h"
#include "profport.h"

#define outSlotSectionName  "Output Slots"
#define inSlotSectionName   "Input Slots"

//________________________________________________________________________
static void SaveSlots (SlotPtr slot, char * section, char* fullname)
{
	TSlotInfos infos;
        
        if (MidiGetSlotInfos (slot->refNum, &infos)) {
		char buff[kMaxEntryLen]; int i;
		buff[0] = 0;
                
		for (i=0; i<256; i++) {
			if (IsAcceptedBit (infos.cnx, i)) {
				char numStr[10];
				sprintf (numStr, "%d ", i);
				strcat (buff, numStr);
			}
		}
		write_private_profile_string (section, infos.name, buff, fullname);
	}
}

//________________________________________________________________________
void SaveState (SlotPtr in, SlotPtr out, char* fullname)
{
	while (in) {
 		SaveSlots (in, inSlotSectionName,fullname);
		in = in->next;
	}
        
	while (out) {
 		SaveSlots (out, outSlotSectionName,fullname);
		out = out->next;
	}
}

//________________________________________________________________________
static void LoadSlots (SlotPtr slot, char * section, char* fullname)
{
	TSlotInfos infos;
	if (MidiGetSlotInfos (slot->refNum, &infos)) {
		char buff[kMaxEntryLen];
		unsigned long n;
		n= get_private_profile_string (section, infos.name, "", buff, kMaxEntryLen, fullname);
          	if (n) {
			unsigned short i, c = CountCnx (buff);
			for (i=0; i<c; i++) {
				short port = GetCnx (buff, i);
				if (port != CnxError) {
					Boolean input = infos.direction & MidiInputSlot;
					OpenSlot (slot, input);
					MidiConnectSlot (port, slot->refNum, true);
				}
			}
		}
	}
}

//________________________________________________________________________
void LoadState (SlotPtr in, SlotPtr out, char* fullname)
{
	while (out) {
		LoadSlots (out, outSlotSectionName,fullname);
		out = out->next;
	}
	while (in) {
		LoadSlots (in, inSlotSectionName,fullname);
		in = in->next;
	}
}

