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
#include "FilterUtils.h"
#include "StatesManager.h"
#include "TInetAddress.h"

extern char * fullProfileName;
char * HostsIn = "Hosts input connections";
char * HostsOut = "Hosts output connections";
#define kMaxEntryLen	1024

//____________________________________________________________
static void GetPorts (char *dst, TSlotInfos *infos)
{
	char * cnx = infos->cnx, num[10];
	short i;
	for (i=0; i<256; i++) {
		if (IsAcceptedBit (cnx, i)) {
			wsprintf (num, "%d ", i);
			lstrcat (dst, num);
		}
	}
}

//____________________________________________________________
// StatesManager
//____________________________________________________________
StatesManager::~StatesManager ()
{
}

//____________________________________________________________
void StatesManager::SaveState (IPNum ip)
{
	TInetAddress addr (ip); char buff [kMaxEntryLen];
	if (!fState) return;
	INetHandle h = fState->GetState ();
	if (h) {
		TSlotInfos * infos = (TSlotInfos *)HandlePtr(h);
		buff[0] = 0;
		GetPorts (buff, infos++);
		WritePrivateProfileString (HostsIn, addr.IP2String(), buff, fullProfileName);
		buff[0] = 0;
		GetPorts (buff, infos);
		WritePrivateProfileString (HostsOut, addr.IP2String(), buff, fullProfileName);
	}
}

static __inline Boolean CnxSeparator(char c) { return ((c)==' ') || ((c)=='	'); }
//________________________________________________________________________
char * NextCnx (char *ptr, Boolean first)
{
	Boolean skipped = first;
	while (*ptr) {
		if (CnxSeparator(*ptr))	skipped = true;
		else if (skipped)		return ptr;
		ptr++;
	}
	return 0;
}

//____________________________________________________________
static void SetPorts (char *buff, TSlotInfos *infos)
{
	char * cnx = infos->cnx, *ptr; int i;
	ptr = NextCnx (buff, true);
	while (ptr) {
		i = atoi (ptr);
		if (i || (*ptr == '0'))
			AcceptBit (cnx, i);
		ptr = NextCnx (ptr, false);
	}
}

//____________________________________________________________
void StatesManager::LoadState (IPNum ip)
{
	TInetAddress addr (ip); char buff [kMaxEntryLen];
	INetHandle h = INetNewHandle (sizeof(TSlotInfos) * 2);
	if (h) {
		DWORD n, m;
		TSlotInfos * infos = (TSlotInfos *)HandlePtr(h);
		memset (infos, 0, GetHandleSize(h));
		n = GetPrivateProfileString (HostsIn, addr.IP2String(), "", buff, 
								kMaxEntryLen, fullProfileName);	
		if (n) SetPorts (buff, infos);
		m = GetPrivateProfileString (HostsOut, addr.IP2String(), "", buff, 
								kMaxEntryLen, fullProfileName);	
		if (m) SetPorts (buff, ++infos);
		if ((n + m) && fState) {
			fState->SetState (h);
		}
		else INetDisposeHandle (h);
	}
}
