/*

  Copyright © Grame 2001

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

#include <Windows.h>

#include "MidiShare.h"
#include "NetTools.h"
//________________________________________________________________________
static BOOL GlobalInitExist (char *fileName)
{
	char dir[512], buff[600];
	if (!GetWindowsDirectory(dir, 512))
		return FALSE;
	wsprintf (buff, "%s\\%s", dir, fileName);
	return GetFileAttributes(buff) != -1;
}

//________________________________________________________________________
char * GetProfileFullName (char * file)
{
	static char buff [1024];
	char dir[512];
	if (!GetCurrentDirectory(512, dir))
		return file;

	wsprintf (buff, "%s\\%s", dir, file);
	// uses local init file when it exists
	if (GetFileAttributes(buff) != -1)
		return buff;
	// uses local init file when global init file don't exist
	if (!GlobalInitExist (file))
		return buff;
	// no local init file and global init is present: use it
	return file;
}

/* ----------------------------------------------------------------------*/
char * TCPOpen ()
{
	WORD version; WSADATA data;
	version = MAKEWORD (2,2);
	if (WSAStartup (version, &data))
		return "cannot initialize Internet services";
	return 0;
}

/* ----------------------------------------------------------------------*/
MidiName InetDriverName (MidiName baseName, short port, short defaultPort)
{
	static char name [256];
	if (port == defaultPort) {
		wsprintf (name, "%s", baseName);
	}
	else {
		wsprintf (name, "%s:%d", baseName, (int)port);
	}
	return name;
}

/* ----------------------------------------------------------------------*/
char * CheckName (MidiName name)
{
	static char msg[256];
	if (MidiGetNamedAppl (name) > 0) {
		wsprintf (msg, "%s is already running", name);
		return msg;
	}
	return 0;
}
