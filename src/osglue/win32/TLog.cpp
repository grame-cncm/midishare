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
#include <time.h>

#include "TLog.h"

#define kMaxTimeString	30

//__________________________________________________________________________
TLog::TLog (const char * logpath)
{
	fLogFile = INVALID_HANDLE_VALUE;
	if (logpath) Open (logpath);
}

//__________________________________________________________________________
char * TLog::DateString (char * buff, short len)
{
	time_t t;
	time(&t);
	strftime (buff, len, "[%D %T]", localtime(&t));
	return buff;
}

//__________________________________________________________________________
void TLog::Open (const char *logpath)
{
	Close ();
	if (logpath) {
		fLogFile = CreateFile (logpath, GENERIC_WRITE, FILE_SHARE_READ,
			NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	}
}

//__________________________________________________________________________
void TLog::Close ()
{
	if (fLogFile  != INVALID_HANDLE_VALUE) CloseHandle (fLogFile);
	fLogFile = INVALID_HANDLE_VALUE;
}

//__________________________________________________________________________
void TLog::Write (const char *msg)
{
	if (fLogFile != INVALID_HANDLE_VALUE) {
		char buff[kMaxTimeString], *cr = "\n";
		DWORD written;

		DateString (buff, kMaxTimeString);
		SetFilePointer(fLogFile, 0, 0, FILE_END);
		WriteFile (fLogFile, buff, strlen(buff), &written, NULL);
		WriteFile (fLogFile, msg, strlen(msg), &written, NULL);
		WriteFile (fLogFile, cr, strlen(cr), &written, NULL);
	}
	else fprintf (stderr, "%s\n", msg);
}

//__________________________________________________________________________
void TLog::WriteErr (const char *msg)
{
	char * errstring = ErrorString();
	if (fLogFile != INVALID_HANDLE_VALUE) {
		char buff[kMaxTimeString], *cr = "\n";
		DWORD written;

		DateString (buff, kMaxTimeString);
		SetFilePointer(fLogFile, 0, 0, FILE_END);
		WriteFile (fLogFile, buff, strlen(buff), &written, NULL);
		WriteFile (fLogFile, msg, strlen(msg), &written, NULL);
		WriteFile (fLogFile, errstring, strlen(errstring), &written, NULL);
		WriteFile (fLogFile, cr, strlen(cr), &written, NULL);
	}
	else fprintf (stderr, "%s %s\n", msg, errstring);
	LocalFree (errstring);
}

//__________________________________________________________________________
char * TLog::ErrorString () {
	LPVOID lpMsgBuf;
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
	);
	return (char *)lpMsgBuf;
}

//__________________________________________________________________________
char * TLog::DefaultDir (char *buff, int size)
{
	GetWindowsDirectory (buff, size);
	return buff;
}
