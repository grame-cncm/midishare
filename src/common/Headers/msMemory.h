/*

  Copyright © Grame 1999

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
  grame@rd.grame.fr

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
