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

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "TShMem.h"
#include "TLog.h"

#define SHMErr			-1
#define OwnerPerm		0x1A4
#define OwnerAttachPerm	0
#define CreateFlags		IPC_CREAT|IPC_EXCL|OwnerPerm

#define OtherPerm		0x124
#define OtherAttachPerm	SHM_RDONLY


//__________________________________________________________________________
ShMem::ShMem (TLog * log) 
{
	fMemAddr = 0;
	fHandler = SHMErr;
	fLog = log;
	fCreated = false;
}

//__________________________________________________________________________
ShMem::~ShMem () 
{
	Close ();
}

//__________________________________________________________________________
void * ShMem::Create (MemID id, int size) 
{
	void * ptr = 0;
	
	if (Get (id, size, CreateFlags)) {
#ifdef linux
		struct shmid_ds desc;
		if ((shmctl (fHandler, SHM_LOCK, &desc) == SHMErr) && fLog)
			fLog->WriteErr ("shmctl IPC_LOCK failed:");
#endif
		fCreated = true;
		ptr = Attach ();
		if (!ptr) Close ();
	}
	return ptr;
}

//__________________________________________________________________________
void * ShMem::Open (MemID id) 
{
	if (Get (id, 0, OtherPerm)) {
		return Attach ();
	}
	return 0;
}

//__________________________________________________________________________
int ShMem::Get (key_t id, int size, int flags) {
	fHandler = shmget (id, size, flags);
	if (fHandler == SHMErr) {
		if (fLog) fLog->WriteErr ("shmget failed:");
		return false;
	}
	return true;
}

//__________________________________________________________________________
void * ShMem::Attach () {
	if (fMemAddr) return fMemAddr;
	
	void * memPtr = (void *)shmat (fHandler, 0, fCreated ? OwnerAttachPerm : OtherAttachPerm);
	if (memPtr == (void *)SHMErr)  {
		if (fLog) fLog->WriteErr ("shmat failed:");
		return 0;
	}
	fMemAddr = memPtr;
	return memPtr;
}

//__________________________________________________________________________
void ShMem::Detach () {
	if (!fMemAddr) return;
	if (shmdt ((char *)fMemAddr) == SHMErr)
		if (fLog) fLog->WriteErr ("shmdt failed:");
	fMemAddr = 0;
}

//__________________________________________________________________________
void ShMem::Close () {
	Detach ();
	if (fCreated && (fHandler != SHMErr)) {
		struct shmid_ds desc;
		
	#ifdef linux
		shmctl (fHandler, SHM_UNLOCK, &desc);
	#endif
		if (shmctl (fHandler, IPC_RMID, &desc) == SHMErr)
			if (fLog) fLog->WriteErr ("shmctl IPC_RMID failed:");
		fHandler = SHMErr;
		fCreated = false;
	}
}
