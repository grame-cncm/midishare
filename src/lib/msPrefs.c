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


#include "msPrefs.h"
#include "msPrefs.h"
#include "profport.h"

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

/*
static char * GetProfileFullName ()
{
	static char buff [1024];
	char dir[512];
	if (GetCurrentDirectory(512, dir)) {
		sprintf (buff, "%s\\%s", dir, profileName);
		// test if the file exist
		if (GetFileAttributes(buff) != -1)
			return buff;
	}
	return profileName;
}
*/

static char * GetProfileFullName ()
{
	return profileName;
}

//________________________________________________________________________
unsigned long LoadSpace()
{
	unsigned long n;
	n= get_private_profile_int (memorySectionName, memDefault, kDefaultSpace, 
		GetProfileFullName());

	return  n ? n : kDefaultSpace;
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

	n= get_private_profile_string (driverSectionName, active, defaultEntry, buff,
										DriverMaxEntry, GetProfileFullName());
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
										DriverMaxEntry, GetProfileFullName());
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
