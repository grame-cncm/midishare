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
  
*/


#include "msPrefs.h"
#include "profport.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 

#define MidiShareDirectory  "Library/Preferences/MidiShare"
#define ErrFile  "midishare.log"

#define profileName "midishare.ini"
#define memorySectionName "Events memory"
#define driverSectionName "Drivers"
#define audioSectionName  "Audio"
#define timeSectionName  "Time"
#define timeMode  "mode"
#define timeRes  "resolution"
#define bufferSize  "size"
#define driverName  "audiodev"
#define memDefault  "default"
#define active  "active"
#define disable "disable"


#define kDefaultSpace	40000
#define kDefaultSize	64
#define kDefaultName	"Built-in Audio"
#define kDefaultResolution	1
#define kDefaultTimeMode	"timer"

#define DriverMaxEntry	512

//________________________________________________________________________
static char * GetProfileFullName (char* filename)
{
	static char  buff [1024];
	const char* home = getenv("HOME");
	if (home) {
		sprintf (buff, "%s/%s/%s", home, MidiShareDirectory, filename);
		return buff;
	}
	return filename;
}

//________________________________________________________________________
unsigned long LoadTimeMode()
{
	char buff[128];
	get_private_profile_string (timeSectionName, timeMode, kDefaultTimeMode, buff, 128, GetProfileFullName(profileName));
	if (!strcmp(buff, "audio")) return kAudioTime;
	return kTimerTime;
}

//________________________________________________________________________
unsigned long LoadTimeRes()
{
	unsigned long n = get_private_profile_int (timeSectionName, timeRes, kDefaultResolution, GetProfileFullName(profileName));
	return  n ? n : kDefaultResolution;
}

//________________________________________________________________________
unsigned long LoadSpace()
{
	unsigned long n = get_private_profile_int (memorySectionName, memDefault, kDefaultSpace, GetProfileFullName(profileName));
	return  n ? n : kDefaultSpace;
}

//________________________________________________________________________
unsigned long LoadBufferSize()
{
	unsigned long n = get_private_profile_int (audioSectionName, bufferSize, kDefaultSize, GetProfileFullName(profileName));
	return  n ? n : kDefaultSize;
}

//________________________________________________________________________
void LoadDriverName(char* name, unsigned long size)
{
   	get_private_profile_string (audioSectionName, driverName, kDefaultName, name,
  												 size, GetProfileFullName(profileName));
}

static __inline Boolean DrvSeparator (c) { return ((c)==' ') || ((c)=='	'); }
//________________________________________________________________________
static char * NextDriver (char *ptr, Boolean first)
{
	Boolean skipped = first;
	while (*ptr) {
		if (DrvSeparator(*ptr))	skipped = true;
		else if (skipped)		return ptr;
		ptr++;
	}
	return 0;
}


//________________________________________________________________________
unsigned short CountDrivers()
{
	char * defaultEntry= "", buff[DriverMaxEntry];
	unsigned long n; unsigned short count = 0;
	n= get_private_profile_string (driverSectionName, active, defaultEntry, buff,
										DriverMaxEntry, GetProfileFullName(profileName));
	if (n) {
		char * ptr = NextDriver (buff, true);
		while (ptr) {
			count++;
			ptr = NextDriver (ptr, false);
		}
	}
	return count;
}

//________________________________________________________________________
Boolean GetDriver(short index, char *dst, short bufsize)
{
	char * defaultEntry= "", buff[DriverMaxEntry], * ptr;
	unsigned long n;

	n= get_private_profile_string (driverSectionName, active, defaultEntry, buff,
										DriverMaxEntry, GetProfileFullName(profileName));
	if (!n) return false;
	ptr = NextDriver (buff, true);
	while (index-- && ptr)
		ptr = NextDriver (ptr, false);
	if (!ptr) return false;
	
	while (*ptr && !DrvSeparator(*ptr) && --bufsize)
		*dst++ = *ptr++;
	if (!bufsize) return false;
	*dst = 0;
	return true;
}

//_______________________________________________________________________
char * dateString ()
{
	static char date[256];
	time_t t;
	time(&t);
	strftime (date, 256, "[%D %T]", localtime(&t));
	return date;
}

//_______________________________________________________________________
void logmsg (char *msg)
{
	FILE * fd = fopen (GetProfileFullName(ErrFile), "a");
	
	if (fd) {
		fprintf (fd, "%s\n", msg);
		fclose (fd);
	}
}

//_________________________________________________________________________________
void Report (const char* what, const char* obj, const char* reason)
{
	char msg[512];
	sprintf (msg, "%s: %s: %s %s", dateString(), what, obj, reason);
	logmsg (msg);
}

//_________________________________________________________________________________
void ReportN (const char* what, const char* obj, long num)
{
	char msg[512];
	sprintf (msg, "%s: %s: %s %ld", dateString(), what, obj, num);
	logmsg (msg);
}
