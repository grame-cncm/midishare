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
#include "NetTools.h"

#define MidiShareDirectory "MidiShare"

//________________________________________________________________________
char * MakeMSFullName (char * name)
{
	static char  buff [1024];
	const char* home = getenv("HOME");
	if (home) {
		sprintf (buff, "%s/%s/%s", home, MidiShareDirectory, name);
		return buff;
	}
	return name;
}

/* ----------------------------------------------------------------------*/
MidiName InetDriverName (MidiName baseName, short port, short defaultPort)
{
	static char name [256];
	if (port == defaultPort) {
		sprintf (name, "%s", baseName);
	}
	else {
		sprintf (name, "%s:%d", baseName, (int)port);
	}
	return name;
}

/* ----------------------------------------------------------------------*/
char * CheckName (MidiName name)
{
	static char msg[256];
	if (MidiGetNamedAppl (name) > 0) {
		sprintf (msg, "%s is already running", name);
		return msg;
	}
	return 0;
}
