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
  grame@rd.grame.fr

*/

#include <Carbon/Carbon.h>
#include "MidiShare.h"

#include "profport.h"
#include "FilterUtils.h"
#include "msDriverState.h"

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
unsigned short CountCnx (char * cnxString)
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
short GetCnx (char * cnxString, short index)
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
char* LoadConfig (char * section, char* key, char* fullname, char* def)
{
	static char buff[kMaxEntryLen];
        int n = get_private_profile_string (section, key, "", buff, kMaxEntryLen, fullname);
        return n ? buff : def;
}

//________________________________________________________________________
unsigned long LoadConfigNum (char * section, char* key, char* fullname, int def)
{
       return get_private_profile_int (section, key, def, fullname);
}

//________________________________________________________________________
void SaveConfig (char * section, char* key, char* name, char* fullname)
{
        write_private_profile_string (section, key, name, fullname);
}

//________________________________________________________________________
void LoadSlot (char * section, char* fullname, char* drivername)
{
        SlotRefNum refNum = MidiGetIndSlot(MidiGetNamedAppl(drivername),1);
        TSlotInfos infos;
        
	  	if (MidiGetSlotInfos (refNum, &infos)) {
			char buff[kMaxEntryLen];
			unsigned long n;
			n= get_private_profile_string (section, infos.name, "", buff, kMaxEntryLen, fullname);
	          	if (n) {
				unsigned short i, c = CountCnx (buff);
				for (i=0; i<c; i++) {
					short port = GetCnx (buff, i);
					if (port != CnxError) {
						MidiConnectSlot (port, refNum, true);
					}
				}
			}
		}
}
//________________________________________________________________________
void SaveSlot (char * section, char* fullname, char* drivername)
{
        SlotRefNum refNum = MidiGetIndSlot(MidiGetNamedAppl(drivername),1);
        TSlotInfos infos;
        
        if (MidiGetSlotInfos (refNum, &infos)) {
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

