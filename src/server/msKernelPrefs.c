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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "msKernelPrefs.h"
#include "msServerInit.h"
#include "profport.h"

#ifdef WIN32
	#include <windows.h>

	#define pathSep           '\\'
	#define profileName       "midishare.ini"
	#define defaultTimeMode	  kTimeModeMMSystem
	#define defaultLog        "midishare.log"

#else
	#define pathSep           '/'
	#define profileName       "/etc/midishare.conf"
	#define defaultLog        "/var/log/midishare.log"
#	ifdef linux
		#define defaultTimeMode	  kTimeModeRTC
#	else
		#define defaultTimeMode	  kTimeModeAudio
#	endif
#endif

#define kArgs "[-c initfile -l logfile -m memSize -t timeMode -r timeRes]"

#define memorySectionName "memory"
#define driverSectionName "drivers"
#define timeSectionName   "time"
#define logSectionName    "log"

#define memSizeStr        "size"
#define timeModeStr       "mode"
#define timeResStr        "resolution"
#define audiodevStr       "audiodev"
#define activeDriversStr  "active"
#define logFileStr        "file"

#define DriversSep        ','

#define kDefaultSpace		40000
#define kDefaultTimeRes		1
#define kDefaultAudioDev	""

#define DriverMaxEntry	2048

static unsigned long GetMemSize (char * file);
static int  TimeModeStr2TimeMode (char *str);
static int  GetTimeMode (char * file);
static int  GetTimeRes (char * file);
static void GetLog (char * file, char *buffer, int len);
static void GetAudioDev (char * file, char *buffer, int len);
static char * GetDrivers (char * file, char *buffer, int len);
static void ScanDrivers (msKernelPrefs * prefs, char *buffer);
static int  readnum (char *str);
static void usage (char *name);
static int  checkPrefs  (msKernelPrefs * prefs);

static char * availableTimeModes[] = {
/* order of the strings must not be changed : it corresponds 
   to the enum declaration in msKernelPrefs.h */
	"rtc",
	"audio",
	"mmsys",
	0
};

static msKernelPrefs gPrefs = { 0 };

//________________________________________________________________________
// exported functions implementation
//________________________________________________________________________
msKernelPrefs * ReadPrefs (const char * conffile)
{
	static char buffer[DriverMaxEntry];
	
	char * confName = conffile ? conffile : profileName;

	gPrefs.memory   = GetMemSize(confName);
	gPrefs.timeMode = GetTimeMode(confName);
	gPrefs.timeRes  = GetTimeRes(confName);
	gPrefs.drvCount = 0;
	GetLog (confName, gPrefs.logfile, sizeof(gPrefs.logfile));
	GetAudioDev (confName, gPrefs.audioDev, sizeof(gPrefs.audioDev));
	if (GetDrivers (confName, buffer, DriverMaxEntry)) {
		ScanDrivers (&gPrefs, buffer);
	}
	return &gPrefs;
}

//________________________________________________________________________
void LogPrefs (msKernelPrefs * prefs)
{
	char buffer[1024], *ptr, *dev="";
	int i;
	
	sprintf (buffer, "Kernel memory size : %ld", prefs->memory);
	LogWrite (buffer);
	switch (prefs->timeMode) {
		case kTimeModeRTC: 		ptr = "real time clock (/dev/rtc)";
			break;
		case kTimeModeAudio:	ptr = "audio using "; dev = prefs->audioDev;
			break;
		case kTimeModeMMSystem: ptr = "Windows MultiMedia Timer";
			break;
		default: 				ptr = "unknow time mode";
	}
	sprintf (buffer, "Time management    : %s%s", ptr, dev);
	LogWrite (buffer);
	sprintf (buffer, "Time resolution    : %d", (int)prefs->timeRes);
	LogWrite (buffer);
	sprintf (buffer, "Drivers count      : %d", (int)prefs->drvCount);
	LogWrite (buffer);
	if (prefs->drvCount) {
		LogWrite ("  active:");
		for (i=0; i<prefs->drvCount; i++) {
			sprintf (buffer, "     %s", DrvName(prefs, i));
			LogWrite (buffer);		
		}
	}
}

//________________________________________________________________________
void AdjustPrefs (msKernelPrefs * prefs, msCmdLinePrefs * args)
{
	if (args->logfile) strncpy (prefs->logfile, args->logfile, MaxLogName);
	if (args->memory != -1) prefs->memory = args->memory;
	if (args->timeMode > 0) prefs->timeMode = args->timeMode;
	if (args->timeRes > 0) prefs->timeRes = args->timeRes;
}

//________________________________________________________________________
void ReadArgs (msCmdLinePrefs * prefs, int argc, char *argv[])
{
	int i; int val; char msg[256];
	
	prefs->memory = -1;
	prefs->timeMode = -1;
	prefs->timeRes = -1;
	prefs->conffile = 0;
	prefs->logfile = 0;
	
	for (i=1; i<argc; i++) {
		char * ptr = argv[i];

		if (*ptr++ == '-') {
			char c = *ptr;
			ptr = argv[++i];
			if (i == argc) usage (argv[0]);

			switch (c) {

				case 'c': 	prefs->conffile = ptr;
					break;

				case 'l': 	prefs->logfile = ptr;
					break;

				case 'm':
					val = readnum (ptr);
					if (val < 0) {
						sprintf (msg, "-m option must be followed by a number");
						goto err;
					}
					else 		 prefs->memory = (unsigned long)val;
					break;

				case 't':
					val = TimeModeStr2TimeMode (ptr);
					if (val < 0) {
						sprintf (msg, "invalid value '%s' for -t option", ptr);
						goto err;
					}
					break;

				case 'r':
					val = readnum (ptr);
					if (val < 0) {
						sprintf (msg, "-r option must be followed by a number");
						goto err;
					}
					else 		 prefs->timeRes = (short)val;
					break;

				default:
					sprintf (msg, "unknown option %c", c);
					goto err;
			}
		}
		else usage (argv[0]);
	}
	return;
err:
	LogWrite (msg);
	usage (argv[0]);
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
	sprintf (buffer, "usage: %s %s\n", name, kArgs);
	LogWrite (buffer);
	exit (1);
}

//________________________________________________________________________
static unsigned long GetMemSize (char *file)
{
	unsigned long n;
	n = get_private_profile_int (memorySectionName, memSizeStr, kDefaultSpace, file);
	return  n ? n : kDefaultSpace;
}

//________________________________________________________________________
static int TimeModeStr2TimeMode (char *str)
{
	char **ptr = availableTimeModes;
	int mode = 0;
	while (*ptr) {
		if (!strcmp(str, *ptr++))
			return mode;
		mode++;
	}
	return -1;
}

//________________________________________________________________________
static int checkPrefs (msKernelPrefs * prefs)
{
	int ret = 1;
	
	if (prefs->memory < 10000) {
		LogWrite ("minimum kernel memory size is 10000");
		prefs->memory = 10000;
	}
	
#ifndef WIN32
	if (prefs->timeMode == kTimeModeMMSystem) {
		prefs->timeMode = defaultTimeMode;
		LogWrite ("'mmsys' time mode only supported on Windows");
		ret = 0;
	}
#endif
	
#ifndef linux
	if (prefs->timeMode == kTimeModeRTC) {
		prefs->timeMode = defaultTimeMode;
		LogWrite ("'rtc' time mode only supported on Linux");
		ret = 0;
	}
#endif
	return ret;
}

//________________________________________________________________________
static int GetTimeMode (char *file)
{
	char buffer[256]; unsigned long n;
	n = get_private_profile_string (timeSectionName, timeModeStr, "", buffer, 256, file);
	return  n ? TimeModeStr2TimeMode (buffer) : defaultTimeMode;
}

//________________________________________________________________________
static int GetTimeRes (char *file)
{
	unsigned long n;
	n = get_private_profile_int (timeSectionName, timeResStr, kDefaultTimeRes, file);
	return  n ? n : kDefaultTimeRes;
}

//________________________________________________________________________
static void GetLog (char *file, char *buffer, int len)
{
	unsigned long n;
	n = get_private_profile_string (logSectionName, logFileStr, defaultLog, buffer, len, file);	
	if (!n) strcpy (buffer, defaultLog);
}

//________________________________________________________________________
static void GetAudioDev (char *file, char *buffer, int len)
{
	unsigned long n;
	n = get_private_profile_string (timeSectionName, audiodevStr, kDefaultAudioDev, buffer, len, file);	
	if (!n) strcpy (buffer, kDefaultAudioDev);
}

//________________________________________________________________________
static char * GetDrivers (char *file, char *buffer, int len)
{
	unsigned long n;
	n = get_private_profile_string (driverSectionName, activeDriversStr, "", buffer, len, file);	
	return  n ? buffer : 0;
}

//________________________________________________________________________
static void ScanDrivers (msKernelPrefs * prefs, char *buffer)
{
	char *ptr = buffer, *enddrv;
	
	do {
		enddrv = strchr (ptr, DriversSep);
		if (*enddrv) {		
			*enddrv = 0;
			if (strlen (ptr))
				prefs->drivers[prefs->drvCount++] = ptr;
			ptr = ++enddrv;
		}
		else if (strlen (ptr))
			prefs->drivers[prefs->drvCount++] = ptr;
	} while (enddrv);
}

//________________________________________________________________________
static int readnum (char *str)
{
	char *ptr = str;
	if (!*ptr) return -1;
	while (*ptr)
		if (!isdigit(*ptr++)) return -1;
	return atoi(str);
}
