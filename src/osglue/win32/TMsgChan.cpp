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
  
  modifications history:
 
*/

#include "TMsgChan.h"

#define kKeyID	0x0a0b0c0d	// key identification of the shared memory segment

typedef struct {
	DWORD	key;
	DWORD	servID;
} ContactInfos, *ContactInfosPtr;

//_____________________________________________________________________
TMsgChan::TMsgChan ()
{
	fInfos = 0;
}

//_____________________________________________________________________
int TMsgChan::Create (char *name) 
{
	fInfos = fHandle.Create (name, sizeof(ContactInfos));
	if (fInfos) {
		ContactInfosPtr inf = (ContactInfosPtr)fInfos;
		MSG msg;
		inf->key = kKeyID;
		inf->servID = GetCurrentThreadId();
		PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);
		return TRUE;
	}
	return FALSE;
}

//_____________________________________________________________________
int TMsgChan::Open (char *name)
{
	fInfos = fHandle.Open (name);
	if (fInfos) {
		ContactInfosPtr inf = (ContactInfosPtr)fInfos;
		if (inf->key == kKeyID) return TRUE;
		Close ();
	}
	return FALSE;
}

//_____________________________________________________________________
void TMsgChan::Close ()
{
	fHandle.Close();
	fInfos = 0;
}

//_____________________________________________________________________
DWORD TMsgChan::ServerAddr () 
{ 
	return fInfos ? ((ContactInfosPtr)fInfos)->servID : 0; 
}

//_____________________________________________________________________
int TMsgChan::Write (ContactMsgPtr msg, DWORD to)
{
	if (!to) to = ServerAddr();
	if (PostThreadMessage(to, WM_USER + msg->type, msg->id, GetCurrentThreadId())) {
		return TRUE;
	}
	return FALSE;
}

//_____________________________________________________________________
int TMsgChan::Read (ContactMsgPtr msg, DWORD * from)
{
	MSG read;

	if (GetMessage (&read,(HWND)NULL,WM_USER,WM_USER+0xffff) > 0) {
		msg->type = read.message - WM_USER;
		msg->id = read.wParam;
		*from = read.lParam;
		return TRUE;
	}
	return FALSE;
}
