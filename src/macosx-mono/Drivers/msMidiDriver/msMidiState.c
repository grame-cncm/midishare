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
#include "profport.h"

static char * outSlotSectionName = "Output Slots";
static char * inSlotSectionName  = "Input Slots";
static char * MidiShareDirectory = "MidiShare";

#define kMaxEntryLen	1024
#define PortMaxEntry	10
#define CnxError	-1

static unsigned short CountCnx (char * cnxString);
static short GetCnx (char * cnxString, short index);
static __inline Boolean CnxSeparator(c) { return ((c)==' ') || ((c)=='	'); }

//________________________________________________________________________
char * GetProfileFullName (char * name)
{
	static char  buff [1024];
	const char* home = getenv("HOME");
	if (home) {
		sprintf (buff, "%s/%s/%s", home, MidiShareDirectory, name);
		return buff;
	}
	return name;
}

//________________________________________________________________________
static void SaveSlot (SlotPtr slot, char * section, char* fullname)
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
 		SaveSlot (in, inSlotSectionName,fullname);
		in = in->next;
	}
        
	while (out) {
 		SaveSlot (out, outSlotSectionName,fullname);
		out = out->next;
	}
}

//________________________________________________________________________
static void LoadSlot (SlotPtr slot, char * section, char* fullname)
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
		LoadSlot (out, outSlotSectionName,fullname);
		out = out->next;
	}
	while (in) {
		LoadSlot (in, inSlotSectionName,fullname);
		in = in->next;
	}
}

//________________________________________________________________________
static char * NextCnx (char *ptr, Boolean first)
{
	Boolean skipped = first;
	while (*ptr) {
		if (CnxSeparator(*ptr))	skipped = true;
		else if (skipped) return ptr;
		ptr++;
	}
	return 0;
}

//________________________________________________________________________
static unsigned short CountCnx (char * cnxString)
{
	unsigned short count = 0;
	char * ptr = NextCnx (cnxString, true);
	while (ptr) {
		count++;
		ptr = NextCnx (ptr, false);
	}
	return count;
}

//________________________________________________________________________
static Boolean NullString (char * ptr)
{
	while (*ptr)
		if (*ptr++ != '0') return false;
	return true;
}

//________________________________________________________________________
static short GetCnx (char * cnxString, short index)
{
	short cnx, bufsize=PortMaxEntry; char buff[PortMaxEntry];
	char * dst = buff;
	char * ptr = NextCnx (cnxString, true);
	while (index-- && ptr)
		ptr = NextCnx (ptr, false);
	if (!ptr) return CnxError;
	
	while (*ptr && !CnxSeparator(*ptr) && --bufsize)
		*dst++ = *ptr++;
	if (!bufsize) return CnxError;
	*dst = 0;
	cnx = (short)atol (buff);
	if (!cnx && !NullString (buff)) return CnxError;
	return cnx;
}

