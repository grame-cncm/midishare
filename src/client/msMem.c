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
#define baseKey			"msFlt"
#else
#define allocate(size) (void*)malloc(size)
#define free(ptr) 		free(ptr)
#define baseKey			0x0e1e10000
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


static ShMemID CreateShMKey (int index)
{
#ifdef WIN32
    static char	id[keySize];
    wsprintf (id, "%s%d", baseKey, index);
    return id;
#else
    return baseKey + index;
#endif
}

FarPtr(void) AllocateFilter (unsigned long size)
{
    int i; FilterInfo * ptr;
    unsigned long fsize = sizeof(FilterInfo) + size;

    for (i = 1; i < MaxFilters; i++) {
        ShMemID id = CreateShMKey (i);
        SharedMemHandler mh = msSharedMemCreate (id, fsize, (void **)&ptr);
        if (mh) {
            ptr->memh = mh;
#ifdef WIN32
            strcpy (ptr->id, id);
#else
            ptr->id = id;
#endif
            return ++ptr;
        }
    }
	return 0;
}

void * GetShMemID (MidiFilterPtr filter)
{
    void * ret = 0;

    if (filter) {
        FilterInfo * fptr = (FilterInfo *)filter;
        fptr--;
#ifdef WIN32
        ret = fptr->id;
#else
        ret = &fptr->id;
#endif
    }
    return ret;
}

void FreeFilter (FarPtr(void) filter)
{
    if (filter) {
        FilterInfo * fptr = (FilterInfo *)filter;
        fptr--;
        msSharedMemClose (fptr->memh);
    }
}
