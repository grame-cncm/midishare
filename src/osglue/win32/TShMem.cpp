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

#include "TShMem.h"
#include "TLog.h"

#define SHMErr			0

#define OwnerPerm			PAGE_READWRITE
#define OwnerAttachPerm		FILE_MAP_WRITE
#define CreateFlags			OwnerPerm

#define OtherPerm			FILE_MAP_READ
#define OtherAttachPerm		FILE_MAP_READ

//__________________________________________________________________________
TShMem::TShMem (TLog * log) 
{
	fMemAddr = 0;
	fHandler = SHMErr;
	fLog = log;
	fCreated = false;
}

//__________________________________________________________________________
TShMem::~TShMem () 
{
	Close ();
}

//__________________________________________________________________________
void * TShMem::Create (MemID id, unsigned long size) 
{
	void * ptr = 0;
	LPSECURITY_ATTRIBUTES sec = 0;

	fHandler = CreateFileMapping(INVALID_HANDLE_VALUE, sec, CreateFlags, 0, size, id);
	if (!fHandler) {
		if (fLog) fLog->WriteErr ("CreateFileMapping error:");
	}
	else if (GetLastError() == ERROR_ALREADY_EXISTS) {
		if (fLog) fLog->WriteErr ("CreateFileMapping error:");
		Close ();
	}
	else {
		fCreated = TRUE;
		ptr = Attach ();
		if (!ptr) Close();
	}
	return ptr;
}

//__________________________________________________________________________
void * TShMem::Open (MemID id) 
{
	void *ptr = 0;
	if (Get (id, 0, OtherPerm)) {
		ptr = Attach ();
		if (!ptr) Close();
	}
	return ptr;
}

//__________________________________________________________________________
int TShMem::Get (MemID id, unsigned long size, int flags) 
{
	BOOL inheritflag = FALSE;
	fHandler = OpenFileMapping (flags, inheritflag, id);
	if (!fHandler) {
		if (fLog) fLog->WriteErr ("OpenFileMapping failed:");
		return  FALSE;
	}
	return TRUE;
}

//__________________________________________________________________________
void * TShMem::Attach () {
	if (fMemAddr) return fMemAddr;	
	if (fHandler) {
		DWORD accessRights = fCreated ? OwnerAttachPerm : OtherAttachPerm;
		fMemAddr = (void *)MapViewOfFile (fHandler, accessRights, 0, 0, 0);
		if (!fMemAddr)
			if (fLog) fLog->WriteErr ("MapViewOfFile error:");
	}
	return fMemAddr;
}

//__________________________________________________________________________
void TShMem::Detach () {
	if (!fMemAddr) return;	
	if (!UnmapViewOfFile (fMemAddr))
		if (fLog) fLog->WriteErr ("UnmapViewOfFile failed:");
	fMemAddr = 0;
}

//__________________________________________________________________________
void TShMem::Close () {
	Detach ();
	if (fHandler) {
		if (!CloseHandle (fHandler))
			if (fLog) fLog->WriteErr ("TShMem::Close CloseHandle failed:");
		fHandler = 0;
	}
	fCreated = FALSE;
}
