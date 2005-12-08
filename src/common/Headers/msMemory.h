/*******************************************************************************
 * C H A M E L E O N    S. D. K.                                               *
 *******************************************************************************
 *  $Archive:: /Chameleon.sdk/SYSTEM/midishare/common/Headers/msMemory.h       $
 *     $Date: 2005/12/08 13:38:28 $
 * $Revision: 1.2.6.1 $
 *-----------------------------------------------------------------------------*
 * This file is part of the Chameleon Software Development Kit                 *
 *                                                                             *
 * Copyright (C) 2001 soundart                                                 *
 * www.soundart-hot.com                                                        *
 * codemaster@soundart-hot.com                                                 *
 ******************************************************************************/

/*

  Copyright � Grame 1999

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
    DWORD	     desiredSpace;         	/* count of free+used events      */
    DWORD	     totalSpace;         	/* count of free+used events      */
    long         active;                /* active > 0 if memory is active */
} TMSMemory, * MSMemoryPtr;

/*------------------------------------------------------------------------------*/
/*                             fields access macros                             */
/*------------------------------------------------------------------------------*/
#define FreeList(g)         &g->freeList
#define BlockList(g)        &g->blockList

/* Memory management functions         */
DWORD MSGrowSpace    (DWORD nbev, MSMemoryPtr g);
DWORD MSDesiredSpace (MSMemoryPtr g);
DWORD MSFreeSpace    (MSMemoryPtr g);
DWORD MSTotalSpace   (MSMemoryPtr g);

/* private memory management functions */
void  InitMemory  (MSMemoryPtr g, DWORD defaultSpace);
BOOL  OpenMemory  (MSMemoryPtr g);
void  CloseMemory (MSMemoryPtr g);

#endif
