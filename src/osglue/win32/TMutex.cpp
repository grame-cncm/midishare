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


#include "TMutex.h"
#include "TLog.h"


int TMutex::fMIndex = 0;
//_____________________________________________________________________
TMutex::TMutex (TLog * log)
{
	fLog = log;
	Init ();
}

//_____________________________________________________________________
void TMutex::Init ()
{
	char mname[256]; int n=0;
	do {
		wsprintf (mname, "msMutex%02d", fMIndex++);
		fMutex = CreateMutex (0, FALSE, mname);
		if (!fMutex && (GetLastError() != ERROR_ALREADY_EXISTS)) {
			if (fLog) fLog->WriteErr ("CreateMutex failed:");
				break;
		}
	} while (!fMutex && (++n < 10));
}

//_____________________________________________________________________
void TMutex::Destroy ()
{
	if (fMutex) CloseHandle(fMutex);
	fMutex = 0;
}

//_____________________________________________________________________
int TMutex::Lock ()
{
	if (fMutex) {
		DWORD ret = WaitForSingleObject (fMutex, INFINITE);
		switch (ret) {
			case WAIT_FAILED:
				if (fLog) fLog->WriteErr ("WaitForSingleObject failed:");
				return FALSE;
			case WAIT_OBJECT_0:
				return TRUE;
		}
	}
	return FALSE;
}

//_____________________________________________________________________
int TMutex::Unlock ()
{
	if (fMutex) {
		if (!ReleaseMutex (fMutex))
			if (fLog) fLog->WriteErr ("ReleaseMutex failed:");
		else return TRUE;
	}
	return FALSE;
}
