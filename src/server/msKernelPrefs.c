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

#include <stdlib.h>
#include <string.h>

#include "msKernelPrefs.h"
#include "msServerInit.h"
#include "profport.h"

#ifdef WIN32
	#include <windows.h>

	#define pathSep           '\'
	#define profileName       "midishare.ini"
	#define defaultLog        "midishare.log"

#else
	#define pathSep           '/'
	#define profileName       "/etc/midishare.conf"
	#define defaultLog        "/var/log/midishare.log"
#endif

#define memorySectionName "memory"
#define driverSectionName "drivers"
#define timeSectionName   "time"
#define logSectionName    "log"

#define memSize           "size"
#define timeMode          "mode"
#define timeRes           "resolution"
#define activeDrivers  	  "active"
#define logFile           "file"

static msKernelPrefs gPrefs = 0;

#define kDefaultSpace		40000
#define kDefaultTimeMode	kTimeModeDefault
#define kDefaultTimeRes		1

#define DriverMaxEntry	2048

static unsigned long GetMemSize ();
static int TimeModeStr2TimeMode (char *str);
static int GetTimeMode ();
static int GetTimeRes ();
static void GetLog (char *buffer, int len);
static char * GetDrivers (char *buffer, int len);
static void usage (char *name);

//________________________________________________________________________
msKernelPrefs * ReadPrefs ()
{
	char buffer[DriverMaxEntry], *drv; 
	
	gPrefs.memory   = GetMemSize();
	gPrefs.timeMode = GetTimeMode();
	gPrefs.timeRes  = GetTimeRes();
	gPrefs.drvCount = 0;
	GetLog (gPrefs.logfile, sizeof(gPrefs.logfile));
	if (GetDrivers (buffer, DriverMaxEntry)) {
	}
	return &gPrefs;
}

//________________________________________________________________________
void AdjustPrefs (msKernelPrefs * prefs, int argc, char *argv[])
{
	int i;
	for (i=1; i<argc; i++) {
		char * ptr = argv[i];
		
		if (*ptr++ == '-') {
			ptr = argv[++i];
			switch (*ptr) {
				case 'l':
					strcpy (gPrefs.logfile, ptr);
					break;
				case 'm':
					break;
				case 't':
					break;
				case 'r':
					break;
				case default:
					usage (argv[0]);
			}
		}
		else usage (argv[0]);
	}
}

//________________________________________________________________________
char * DrvName (msKernelPrefs * prefs, short index)
{
	if (index < 0) return 0;
	if (index >= prefs->drvCount) return 0;
	return prefs->drivers[index];
}

//________________________________________________________________________
// internal functions implementation
//________________________________________________________________________
static void usage (char *str)
{
	char buffer[1024], * name = strrchr (str, pathSep);
	if (!name) name = str;
	sprintf (buffer, "usage: %s [-l logfile -m memSize -t timeMode -r timeRes]\n", name);
	LogWrite (buffer);
	exit (1);
}

//________________________________________________________________________
static unsigned long GetMemSize ()
{
	unsigned long n;
	n = get_private_profile_int (memorySectionName, memSize, kDefaultSpace, profileName);
	return  n ? n : kDefaultSpace;
}

//________________________________________________________________________
static int TimeModeStr2TimeMode (char *str)
{
}

//________________________________________________________________________
static int GetTimeMode ()
{
	char buffer[256]; unsigned long n;
	n = get_private_profile_string (timeSectionName, timeMode, "", buffer, 256, profileName);	
	return  n ? TimeModeStr2TimeMode (buffer) : kDefaultTimeMode;
}

//________________________________________________________________________
static int GetTimeRes ()
{
	unsigned long n;
	n = get_private_profile_int (timeSectionName, timeRes, kDefaultTimeRes, profileName);
	return  n ? n : kDefaultTimeRes;
}

//________________________________________________________________________
static void GetLog (char *buffer, int len)
{
	unsigned long n;
	n = get_private_profile_string (logSectionName, logFile, defaultLog, buffer, len, profileName);	
	if (!n) strcpy (buffer, defaultLog);
}

//________________________________________________________________________
static char * GetDrivers (char *buffer, int len)
{
	unsigned long n;
	n = get_private_profile_string (driverSectionName, activeDrivers, "", buffer, len, profileName);	
	return  n ? buffer : 0;
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
										DriverMaxEntry, profileName);
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
										DriverMaxEntry, profileName);
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
