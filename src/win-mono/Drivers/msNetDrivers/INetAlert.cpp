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

#include <time.h>
#include <windows.h>

#include "INetAlert.h"

extern Boolean doneFlag;

#ifdef WANDriver
static char * ErrFile = "\\msWANDriver.log";
#else
static char * ErrFile = "\\msLANDriver.log";
#endif

//_________________________________________________________
char *dateString ()
{
	char *ptr;
	time_t t;
	time (&t);
	ptr = asctime (localtime (&t));
	if (ptr) {
		short n = strlen (ptr);
		if (n) ptr[n-1] = 0;
	}
	return ptr;
}

//_________________________________________________________
static char * ErrFilePath ()
{
	static char buff[MAX_PATH ];
	UINT len = GetCurrentDirectory (MAX_PATH, buff);
	if (len && (len + strlen(ErrFile) < MAX_PATH)) {
		lstrcat (buff, ErrFile);
		return buff;
	}
	return 0;
}

//_________________________________________________________
void log (char * msg)
{
	char * errFile = ErrFilePath ();
	char buff[1024];
	wsprintf (buff, "%s\n", msg);
	if (errFile) {
		HANDLE h = CreateFile (errFile, GENERIC_WRITE, FILE_SHARE_READ,
			NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (h != INVALID_HANDLE_VALUE) {
			DWORD written;
			SetFilePointer(h, 0, 0, FILE_END);
			WriteFile(h, buff, strlen(buff), &written, NULL);
			CloseHandle (h);
		}
	}
}

//_________________________________________________________________________________
void INetAlert::Report (const ErrString what, const ErrString obj, 
	const ErrString reason, long err)
{
	Report (what, obj, reason, err ? strerror (err) : "");
}

//_________________________________________________________________________________
void INetAlert::Report (const ErrString what, const ErrString obj, 
	const ErrString reason, const ErrString infos, void *proc)
{
	char msg[512];
	wsprintf (msg, "%s: %s: %s, %s %s\n", dateString(), 
			what, obj, reason, infos);
	log (msg);
}

//_________________________________________________________________________________
void INetAlert::FatalError (const ErrString what, const ErrString obj, 
						const ErrString reason, long err)
{
	Report (what, obj, reason, err);
	doneFlag = true;
}

//_________________________________________________________________________________
void INetAlert::FatalError (const ErrString what, const ErrString obj, 
						const ErrString reason, const ErrString infos)
{
	Report (what, obj, reason, infos);
	doneFlag = true;
}
