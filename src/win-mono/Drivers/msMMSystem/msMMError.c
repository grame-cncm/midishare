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
#include <stdio.h>
#include <windows.h>
#include <mmsystem.h>

#include "msMMError.h"

#define kGetErrorTextBuf	256

static char * ErrFile = "\\msMMSystemErrors.txt";

//_________________________________________________________
static char *DateString ()
{
	time_t t;
	time (&t);
	return asctime (localtime (&t));
}

//_________________________________________________________
static char *makeErrString (char *s, int errCode, short in)
{
	char buff[kGetErrorTextBuf];
	static char out[512];

	if(in)
		midiInGetErrorText(errCode, buff, kGetErrorTextBuf);
	else
		midiOutGetErrorText(errCode, buff, kGetErrorTextBuf);
	wsprintf (out, "%s %s : %s\n", DateString(), s, buff);
	return out;
}

//_________________________________________________________
static char * ErrFilePath ()
{
	static char buff[MAX_PATH ];
	UINT len = GetWindowsDirectory (buff, MAX_PATH);
	if (len && (len + strlen(ErrFile) < MAX_PATH)) {
		lstrcat (buff, ErrFile);
		return buff;
	}
	return 0;
}

//_________________________________________________________
void MMError (char *s, int errCode, short in)
{
	char * string = makeErrString(s, errCode, in);
	char * errFile = ErrFilePath ();
	if (errFile) {
		HANDLE h = CreateFile (errFile, GENERIC_WRITE, FILE_SHARE_READ,
			NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (h != INVALID_HANDLE_VALUE) {
			DWORD written;
			SetFilePointer(h, 0, 0, FILE_END);
			WriteFile(h, string, strlen(string), &written, NULL);
			CloseHandle (h);
		}
	}
}
