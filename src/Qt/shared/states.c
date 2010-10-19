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
#include <stdio.h>

#include "states.h"
#include "profport.h"

#define MidiShareDirectory "Library/Preferences/MidiShare"

static const char * sectionName = "Window position";
static const char * xEntryName  = "xpos";
static const char * yEntryName  = "ypos";

//________________________________________________________________________
static const char * GetProfileFullName (const char * name)
{
	static char  buff [1024];
	const char* home = getenv("HOME");
	if (home) {
		sprintf (buff, "%s/%s/%s", home, MidiShareDirectory, name);
		return buff;
	}
	return name;
}

//_______________________________________________________________________________
int ReadIntState (const char *profile, const char *section, const char *entry, int defVal)
{
	int val = get_private_profile_int (section, entry, defVal, GetProfileFullName(profile));
	return (val > 0) ? val : defVal;
}

//_______________________________________________________________________________
void WriteIntState (const char *profile, const char *section, const char *entry, int val)
{
	char buff[30];
	sprintf (buff, "%d", val);
	write_private_profile_string (section, entry, buff, GetProfileFullName(profile));
}

//_______________________________________________________________________________
int ReadPos (const char *profileName, int* outx, int* outy)
{
	int x, y;
	const char * fullName = GetProfileFullName (profileName);
	x = get_private_profile_int (sectionName, xEntryName, 50, fullName);
	y = get_private_profile_int (sectionName, yEntryName, 50, fullName);
	if ((x > 0) && (y > 0)) {
		*outx =  x;
		*outy =  y;
		return 1;
	}
	return 0;
}

//_______________________________________________________________________________
void WritePos (const char *profileName, int x, int y)
{
	char buff[30];
	const char * fullName = GetProfileFullName (profileName);
	sprintf (buff, "%d", x);
	write_private_profile_string (sectionName, xEntryName, buff, fullName);
	sprintf (buff, "%d", y);
	write_private_profile_string (sectionName, yEntryName, buff, fullName);
}
