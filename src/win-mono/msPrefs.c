
#include "msPrefs.h"

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

static char * profileName 		= "midishare.ini";
static char * memorySectionName = "Events memory";
static char * driverSectionName = "Drivers";
static char * memDefault  		= "default";
static char * active  			= "active";
static char * disable  			= "disable";

#define kDefaultSpace	40000
//________________________________________________________________________
unsigned long LoadSpace()
{
	char * defaultEntry= "40000", buff[30];
	unsigned long desiredSpace=0, n;

	n= GetPrivateProfileString (memorySectionName, memDefault, defaultEntry, buff,
										30, profileName);
	desiredSpace = n ? atol (buff) : kDefaultSpace;
	return  (desiredSpace > 0) ? desiredSpace : kDefaultSpace;
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

#define DriverMaxEntry	512
//________________________________________________________________________
unsigned short CountDrivers()
{
	char * defaultEntry= "", buff[DriverMaxEntry];
	unsigned long n; unsigned short count = 0;

	n= GetPrivateProfileString (driverSectionName, active, defaultEntry, buff,
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

	n= GetPrivateProfileString (driverSectionName, active, defaultEntry, buff,
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
