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


#include <Memory.h>
#include <Resources.h>

#include "StatesManager.h"
#include "TInetAddress.h"

extern short gResFile;

//____________________________________________________________
static unsigned char * c2pstring (char * str)
{
	static unsigned char pstr[256];
	unsigned char *pPtr = &pstr[1];
	short count = 0;
	while (*str && (count < 255)) {
		*pPtr++ = *str++;
		count++;
	}
	pstr[0] = count;
	return pstr;
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
	if (!fState) return;
	Handle h = fState->GetState ();
	if (!h) return;
	
	unsigned char * rsrcName = c2pstring(TInetAddress::IP2String(ip));
	short current = CurResFile();
	Size len = GetHandleSize(h);
	UseResFile (gResFile);
	Handle rsrc = GetNamedResource (fState->ID(), rsrcName);
	if (rsrc) {
		Size len = GetHandleSize(h);
		SetHandleSize (rsrc, len);
		if (MemError() != noErr) return;
		BlockMove (*h, *rsrc, len);
		HLock (rsrc);
		ChangedResource ( rsrc);
		UpdateResFile (gResFile);
		HUnlock (rsrc);
		ReleaseResource (rsrc);
	}
	else {
		Handle copy = NewHandle (len);
		if (!copy) return;
		
		BlockMove (*h, *copy, len);
		short err = 0;
		ResType id = Unique1ID(fState->ID());
		HLock (copy);
		AddResource (copy, fState->ID(), id, rsrcName);
		err = ResError ();
		WriteResource (copy);
		err = ResError ();
		DetachResource (copy);
		HUnlock (copy);
		DisposeHandle (copy);
	}
	UseResFile (current);
}

//____________________________________________________________
void StatesManager::LoadState (IPNum ip)
{
	if (!fState) return;
	short current = CurResFile();
	UseResFile (gResFile);
	Handle h= GetNamedResource (fState->ID(), c2pstring(TInetAddress::IP2String(ip)));
	if (h) {
		fState->SetState (h);
		ReleaseResource (h);
	}
	UseResFile (current);
}
