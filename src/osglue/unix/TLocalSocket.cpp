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
  
  modifications history:
 
*/

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#include "TLocalSocket.h"
#include "TLog.h"

#define SOKERRCODE		-1
#define kIndexLimit		99
#define kLastIndex		kIndexLimit-1
#define	kSokPerms		S_IRUSR+S_IWUSR+S_IRGRP+S_IWGRP+S_IROTH+S_IWOTH

#ifndef linux
	typedef int socklen_t;
#endif


//_____________________________________________________________________
TLocalSocket::TLocalSocket ()
{
	fSocket = SOKERRCODE;
	fAddr.sun_family = AF_UNIX;
	fBinded = false;
}

//_____________________________________________________________________
int TLocalSocket::Create (char *name) 
{
	if (SockOpen()) {
		sprintf (fAddr.sun_path, name);
		return Bind ();
	}
	return false;
}

//_____________________________________________________________________
void TLocalSocket::Delete (char *name) 
{
	if (access (name, F_OK) == 0)
		unlink( name);
}

//_____________________________________________________________________
int TLocalSocket::Open (char *path)
{
	if (SockOpen()) {
		for (int i=0; i < kIndexLimit; i++) {
			sprintf (fAddr.sun_path, "%s%02d", path, i);
			if (Bind(i != kLastIndex)) return true;
		}
		Close();
	}
	return false;
}

//_____________________________________________________________________
int TLocalSocket::SockOpen ()
{
	fSocket = socket (AF_UNIX, SOCK_DGRAM, 0);
	if (fSocket == SOKERRCODE) {
		return false;
	}
	return true;
}

//_____________________________________________________________________
int TLocalSocket::Bind (int silent)
{
	int ret = bind (fSocket, (struct sockaddr *)&fAddr, sizeof(fAddr));
	if (ret == SOKERRCODE) {
		return false;
	}
	fBinded = true;
    chmod (fAddr.sun_path, kSokPerms);
	return true;
}

//_____________________________________________________________________
void TLocalSocket::Close ()
{
	if (fSocket != SOKERRCODE) {
		close (fSocket);
		fSocket = SOKERRCODE;
	}
	if (fBinded) {
        unlink (fAddr.sun_path);
		fBinded = false;
	}
}

//_____________________________________________________________________
int TLocalSocket::Write (char * to, void * buff, int len)
{
	struct sockaddr_un addr;
	int ret;
	
	addr.sun_family = AF_UNIX;
	sprintf (addr.sun_path, to);
	ret = sendto (fSocket, buff, len, 0, 
			(struct sockaddr *)&addr, sizeof(struct sockaddr_un));
	if (ret == SOKERRCODE) {
		return false;
	}
	return true;
}

//_____________________________________________________________________
int TLocalSocket::Read (void * buff, int len, char * from)
{
	struct sockaddr_un addr;
	socklen_t addr_len = sizeof(addr); 
	int ret = recvfrom (fSocket, buff, len, 0, (struct sockaddr *)&addr, &addr_len);
	if (ret == SOKERRCODE) {
		return 0;
	}
	sprintf (from, addr.sun_path);
	return ret;
}
