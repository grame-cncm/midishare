/*

  Copyright © Grame, Sony CSL-Paris 2001

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

#include <windows.h>

#include "MidiShare.h"
#include "FilterUtils.h"
#include "msMMState.h"

static char * profileName 		 = "msMMSystem.ini";
static char * outSlotSectionName = "Output Slots";
static char * inSlotSectionName  = "Input Slots";
static char * fullProfileName = 0;

#define kMaxEntryLen	1024
#define PortMaxEntry	10
#define CnxError		-1

static unsigned short CountCnx (char * cnxString);
static short GetCnx (char * cnxString, short index);

//________________________________________________________________________
static BOOL GlobalInitExist (char *fileName)
{
	char dir[512], buff[600];
	if (!GetWindowsDirectory(dir, 512))
		return FALSE;
	wsprintf (buff, "%s\\%s", dir, fileName);
	return GetFileAttributes(buff) != -1;
}

//________________________________________________________________________
char * GetProfileFullName ()
{
	static char buff [1024];
	char dir[512];
	if (!GetCurrentDirectory(512, dir))
		return profileName;

	wsprintf (buff, "%s\\%s", dir, profileName);
	// uses local init file when it exists
	if (GetFileAttributes(buff) != -1)
		return buff;
	// uses local init file when global init file don't exist
	if (!GlobalInitExist (profileName))
		return buff;
	// no local init file and global init is present: use it
	return profileName;
}

//________________________________________________________________________
static void SaveSlot (SlotPtr slot, char * section)
{
	TSlotInfos infos;
	if (MidiGetSlotInfos (slot->refNum, &infos)) {
		char buff[kMaxEntryLen]; int i;
		buff[0] = 0;
		for (i=0; i<256; i++) {
			if (IsAcceptedBit (infos.cnx, i)) {
				char numStr[10];
				wsprintf (numStr, "%d ", i);
				lstrcat (buff, numStr);
			}
		}
		WritePrivateProfileString (section, infos.name, buff, fullProfileName);
	}
}

//________________________________________________________________________
void SaveState (SlotPtr in, SlotPtr out)
{
	while (in) {
		SaveSlot (in, inSlotSectionName);
		in = in->next;
	}
	while (out) {
		SaveSlot (out, outSlotSectionName);
		out = out->next;
	}
}

//________________________________________________________________________
static void LoadSlot (SlotPtr slot, char * section)
{
	TSlotInfos infos;
	if (MidiGetSlotInfos (slot->refNum, &infos)) {
		char buff[kMaxEntryLen];
		unsigned long desiredSpace=0, n;
		n= GetPrivateProfileString (section, infos.name, "", buff, kMaxEntryLen, fullProfileName);
		if (n) {
			unsigned short i, c = CountCnx (buff);
			Boolean opened = false, success;
			for (i=0; i<c; i++) {
				short port = GetCnx (buff, i);
				if (port != CnxError) {
					Boolean input = infos.direction & MidiInputSlot;
					if (!opened) {
						success = OpenSlot (slot, input);
						opened = true;
					}
					if (success) MidiConnectSlot (port, slot->refNum, true);
				}
			}
		}
	}
}

//________________________________________________________________________
void LoadState (SlotPtr in, SlotPtr out)
{
	fullProfileName = GetProfileFullName ();
	while (out) {
		LoadSlot (out, outSlotSectionName);
		out = out->next;
	}
	while (in) {
		LoadSlot (in, inSlotSectionName);
		in = in->next;
	}
}

static __inline Boolean CnxSeparator(c) { return ((c)==' ') || ((c)=='	'); }

//________________________________________________________________________
static char * NextCnx (char *ptr, Boolean first)
{
	Boolean skipped = first;
	while (*ptr) {
		if (CnxSeparator(*ptr))	skipped = true;
		else if (skipped)		return ptr;
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

