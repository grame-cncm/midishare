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

#include "msSharedMem.h"
#include "TShMem.h"

#ifdef WIN32
#include <stdio.h>
typedef struct ShMemInfo {
    TShMem * memh;
    char	id[keyMaxSize];
} ShMemInfo, * ShMemInfoPtr;

#else
typedef struct ShMemInfo {
    TShMem * memh;
    ShMemID id;
} ShMemInfo, * ShMemInfoPtr;
#endif

void * msSharedMemCreate(ShMemID id, unsigned long size)
{
    TShMem * shm = new TShMem();
    if (shm) {
        ShMemInfoPtr ptr = (ShMemInfoPtr)shm->Create(id, size + sizeof(ShMemInfo));
        if (ptr) {
            ptr->memh = shm;
#ifdef WIN32
            strcpy (ptr->id, id);
#else
            ptr->id = id;
#endif
            return ++ptr;
        }
        delete shm;
    }
    return 0;
}

static ShMemID CreateShmID (ShMemID baseid, int index)
{
#ifdef WIN32
    static char	id[keyMaxSize];
    _snprintf (id, keyMaxSize, "%s%d", baseid, index);
    return id;
#else
    return baseid + index;
#endif
}

void * msSharedMemCreateIndexed (ShMemID baseid, unsigned long size, int indexlimit)
{
    for (int i=1; i<indexlimit; i++) {
        void * ptr = msSharedMemCreate (CreateShmID(baseid, i), size);
        if (ptr) return ptr;
    }
    return 0;
}

void msSharedMemDelete (void *memPtr)
{
    ShMemInfoPtr ptr = (ShMemInfoPtr)memPtr;
    if (ptr) {
        TShMem * shm = (--ptr)->memh;
        delete shm;
    }
}

void * msSharedMemGetID (void * memPtr)
{
    void * ret = 0;
    ShMemInfoPtr ptr = (ShMemInfoPtr)memPtr;
    if (ptr) {
        ptr--;
#ifdef WIN32
        ret = ptr->id;
#else
        ret = &ptr->id;
#endif
    }
    return ret;
}

SharedMemHandler msSharedMemOpen  (ShMemID id, void ** memPtr)
{
    TShMem * shm = new TShMem();
    if (shm) {
        ShMemInfoPtr ptr = (ShMemInfoPtr)shm->Open (id);
        if (ptr) *memPtr = ++ptr;
        else {
            delete shm;
            shm = 0;
        }
    }
    return shm;
}

void msSharedMemClose (SharedMemHandler shmh)
{
    if (shmh) {
        TShMem * shm = (TShMem *)shmh;
        shm->Close();
        delete shm;
    }
}

