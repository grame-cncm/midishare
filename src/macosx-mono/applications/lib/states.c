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

#define MidiShareDirectory ".MidiShare"

static char * sectionName = "Window position";
static char * xEntryName  = "xpos";
static char * yEntryName  = "ypos";

//________________________________________________________________________
static char * GetProfileFullName (char * name)
{
	static char  buff [1024];
	const char* home = getenv("HOME");
	if (home) {
		sprintf (buff, "%s/%s/%s", home, MidiShareDirectory, name);
		return buff;
	}
	return name;
}

//________________________________________________________________________
static void SaveWinState (int x, int y, char * fullName)
{
	char buff[30];
	sprintf (buff, "%d", x);
	write_private_profile_string (sectionName, xEntryName, buff, fullName);
	sprintf (buff, "%d", y);
	write_private_profile_string (sectionName, yEntryName, buff, fullName);
}

//________________________________________________________________________
static int LoadWinState (int * posx, int * posy, char * fullName)
{
	int x = get_private_profile_int (sectionName, xEntryName, 50, fullName);
	int y = get_private_profile_int (sectionName, yEntryName, 50, fullName);
	if ((x > 0) && (y > 0)) {
		*posx =  x;
		*posy =  y;
		return 1;
	}
	return 0;
}

//_______________________________________________________________________________
int ReadIntState (char *profile, char *section, char *entry, int defVal)
{
	int val = get_private_profile_int (section, entry, defVal, GetProfileFullName(profile));
	return (val > 0) ? val : defVal;
}

//_______________________________________________________________________________
void WriteIntState (char *profile, char *section, char *entry, int val)
{
	char buff[30];
	sprintf (buff, "%d", val);
	write_private_profile_string (section, entry, buff, GetProfileFullName(profile));
}

//_______________________________________________________________________________
int ReadPos (char *profileName, Point *p)
{
	int x, y;
	char * fullName = GetProfileFullName (profileName);
	if (LoadWinState (&x, &y, fullName)) {
		p->h = x;
		p->v = y;
		return 1;
	}
	return 0;
}

//_______________________________________________________________________________
void WritePos (char *profileName, WindowRef win)
{
	CGrafPtr port; Rect portRect; Point p;

	port = GetWindowPort(win);
	GetPortBounds (port, &portRect);
	SetPort(port);
	p.h = portRect.left;
	p.v = portRect.top;
	LocalToGlobal(&p);
	SaveWinState (p.h, p.v, GetProfileFullName (profileName));
}

//_______________________________________________________________________________
void AdjustLocation (WindowRef win)
{
	Point pt; Rect portRect, r;
	
	GetWindowPortBounds(win, &portRect);
	pt.h = portRect.left + 6;	// 6 is enough to grab
	pt.v = portRect.top + 6;	// 6 is enough to grab
	LocalToGlobal(&pt);
	
	GetRegionBounds (GetGrayRgn(), &r);
	if (!PtInRect( pt, &r)) {
		MoveWindow (win, r.left + 50, r.top  + 50, false);
	}
}

