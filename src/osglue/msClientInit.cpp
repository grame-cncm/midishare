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

#ifndef WIN32
#include <stdlib.h>
#endif

#include "msClientInit.h"
#include "msCommInit.h"
#include "include/TShMem.h"

typedef struct {
	TShMem * shmem;
	void * shmemPtr;
} ClientEnv, *ClientEnvPtr;

static ClientEnv 	gEnv = { 0 };

//___________________________________________________________________
// global initialization
//___________________________________________________________________
static void msExit ()
{
	if (gEnv.shmem) delete gEnv.shmem;
	gEnv.shmem = 0;
	gEnv.shmemPtr = 0;
}

//___________________________________________________________________
// shared memory
//___________________________________________________________________
void *	GetPubMemory ()
{
    if (!gEnv.shmem) gEnv.shmem = new TShMem ();
    if (!gEnv.shmem) return 0;
#ifndef WIN32
	atexit (msExit);
#endif
	gEnv.shmemPtr = gEnv.shmem->Open (kShMemId);
	return gEnv.shmemPtr;
}
