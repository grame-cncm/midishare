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

#ifdef WIN32
#	include <windows.h>
#else
#	include <stdlib.h>
#endif

#include "msDefs.h"
#include "msMem.h"
#include "msSharedMem.h"

#ifdef WIN32
#define allocate(size) 	LocalAlloc(LMEM_FIXED, size)
#define free(ptr) 		LocalFree((HANDLE)ptr)
#define kBaseFKey			"msFlt"
#else
#define allocate(size) (void*)malloc(size)
#define free(ptr) 		free(ptr)
#define kBaseFKey			0x0e1e10000
#endif

/*_________________________________________________________________________*/
/* memory allocation implementation                                        */
/*_________________________________________________________________________*/
FarPtr(void) AllocateMemory (unsigned long size)
{ 
	return allocate(size);
}

void DisposeMemory (FarPtr(void) memPtr) 
{
	if (!memPtr) return;
	free(memPtr);
}

FarPtr(void) AllocateFilter (unsigned long size)
{
	void * ptr = msSharedMemCreateIndexed (kBaseFKey, size, MaxFilters);
	return ptr;
}

void * GetShMemID (MidiFilterPtr filter)
{
	return filter ? msSharedMemGetID (filter) : 0;
}

void FreeFilter (FarPtr(void) filter)
{
    if (filter)
        msSharedMemDelete (filter);
}
