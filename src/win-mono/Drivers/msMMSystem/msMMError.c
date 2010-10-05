/*

  Copyright © Grame, Sony CSL-Paris 2001

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

#pragma warning(disable: 4996)

#include <time.h>
#include <stdio.h>
#include <windows.h>
#include <mmsystem.h>

#include "msMMError.h"

#define kGetErrorTextBuf	256

static char * ErrFile = "\\msMMSystem.log";
void MMTrace (char *s, long value);
char * GetProfileFullName ();

//_________________________________________________________
static void CheckMessageBox (char * msg)
{
	char buff[20];
	int n = GetPrivateProfileString("error", "messagebox", "no", buff, 20, GetProfileFullName());
	if (n) {
		MMTrace("GetPrivateProfileString result", n);
		MMTrace(buff, n);
		if (!strcmp(buff, "yes")) {
			MessageBox(0, msg, "msMMSystem driver error", MB_OK);
		}
	}
	else MMTrace("GetPrivateProfileString null", n);
}

//_________________________________________________________
static char *DateString ()
{
	time_t t;
	time (&t);
	return asctime (localtime (&t));
}

//_________________________________________________________
static char *makeShortString (char *s, SlotRefNum ref, int errCode, short in)
{
	static char out[1024];
	char buff[kGetErrorTextBuf];
	TSlotInfos infos;

	if(in)
		midiInGetErrorText(errCode, buff, kGetErrorTextBuf);
	else
		midiOutGetErrorText(errCode, buff, kGetErrorTextBuf);
	infos.name[0] = 0;
	MidiGetSlotInfos (ref, &infos);
	wsprintf (out, "%s : %s error\n%s", infos.name, s, buff);
	return out;
}
//_________________________________________________________
static char *makeErrString (char *s, SlotRefNum ref, int errCode, short in)
{
	static char out[1024];
	char buff[kGetErrorTextBuf];
	TSlotInfos infos;

	if(in)
		midiInGetErrorText(errCode, buff, kGetErrorTextBuf);
	else
		midiOutGetErrorText(errCode, buff, kGetErrorTextBuf);
	infos.name[0] = 0;
	MidiGetSlotInfos (ref, &infos);
	wsprintf (out, "%s %s : %s error %d\n\t%s\n", 
				DateString(), infos.name, s, errCode, buff);
	return out;
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
void MMError (char *s, SlotRefNum ref, int errCode, short in)
{
	char buff[20];
	char * string = makeErrString(s, ref, errCode, in);
	char * errFile = ErrFilePath ();

	GetPrivateProfileString("log", "file", "yes", buff, 20, GetProfileFullName());

	if (strcmp(buff, "yes") == 0) {
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
		CheckMessageBox(makeShortString(s, ref, errCode, in));
	}
}

//_________________________________________________________
void MMTrace (char *s, long value)
{
	char * errFile = ErrFilePath ();
	if (errFile) {
		HANDLE h = CreateFile (errFile, GENERIC_WRITE, FILE_SHARE_READ,
			NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (h != INVALID_HANDLE_VALUE) {
			DWORD written; char string[256];
			wsprintf (string, "%s : %ld\n", s, value);
			SetFilePointer(h, 0, 0, FILE_END);
			WriteFile(h, string, strlen(string), &written, NULL);
			CloseHandle (h);
		}
	}
}
