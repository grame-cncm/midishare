
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
