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

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "TPipe.h"

#define PipeErrCode		-1
#define OwnerPerm		S_IRUSR+S_IWUSR+S_IRGRP+S_IROTH
#define kInternalPipeBuffSize	2048		// to be checked

//_____________________________________________________________________
TPipe::TPipe ()
{
	fPipe = PipeErrCode;
	fBuffSize = 0;
	fOwner = false;
}

//_____________________________________________________________________
int TPipe::Create (const char * name)
{
	if (mkfifo (name, OwnerPerm) == PipeErrCode) {
		return false;
	}
	fOwner = true;
	SetName (name);
	return true;
}

//_____________________________________________________________________
int TPipe::Open (const char *name, int perm)
{
	if (fPipe != PipeErrCode) return true;
	
	switch (perm) {
		case kReadWritePerm:
			perm = O_RDWR;
			break;
		case kWritePerm:
			perm = O_WRONLY;
			break;
		default:
			perm = O_RDONLY;
	}
	fPipe = open (name, perm);
	if (fPipe == PipeErrCode) {
		return false;
	}
	fBuffSize = kInternalPipeBuffSize;
	SetName (name);
	return true;
}

//_____________________________________________________________________
void TPipe::Close ()
{
	if (fPipe != PipeErrCode)
		close (fPipe);
	fPipe = PipeErrCode;
	if (fOwner) unlink (fName);
	fBuffSize = 0;
	fOwner = false;
}

//_____________________________________________________________________
long TPipe::Write (void *buff, long len)
{
	return write (fPipe, buff, len);
}

//_____________________________________________________________________
long TPipe::Read (void *buff, long len)
{
	return read (fPipe, buff, len);
}
