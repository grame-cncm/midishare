/*
  MidiShare Project
  Copyright (C) Grame 1999

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

  modifications history:
   [08-09-99] DF - new memory management scheme

*/

#ifndef __Memory__
#define __Memory__

#include "lflifo.h"
#include "msDefs.h"
#include "msTypes.h"

/*------------------------------------------------------------------------------*/
/*     data structures                                                          */
/*------------------------------------------------------------------------------*/
typedef struct TMSMemory
{
    lifo	     freeList;              /* available events list          */
    lifo         blockList;             /* allocated block list           */
    ulong	     desiredSpace;         	/* count of free+used events      */
    ulong	     totalSpace;         	/* count of free+used events      */
    long         active;                /* active > 0 if memory is active */
	char		 padding[4];			/* alignment */
} TMSMemory, * MSMemoryPtr;

/*------------------------------------------------------------------------------*/
/*                             fields access macros                             */
/*------------------------------------------------------------------------------*/
#define FreeList(g)         &g->freeList
#define BlockList(g)        &g->blockList

/* Memory management functions         */
MSFunctionType(ulong) MSGrowSpace    (unsigned long nbev, MSMemoryPtr g);
MSFunctionType(ulong) MSDesiredSpace (MSMemoryPtr g);
MSFunctionType(atomic_long) MSFreeSpace (MSMemoryPtr g);
MSFunctionType(ulong) MSTotalSpace   (MSMemoryPtr g);

/* private memory management functions */
void     InitMemory  (MSMemoryPtr g, ulong defaultSpace);
Boolean  OpenMemory  (MSMemoryPtr g);
void     CloseMemory (MSMemoryPtr g);

#endif
