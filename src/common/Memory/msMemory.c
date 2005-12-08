/*******************************************************************************
 * C H A M E L E O N    S. D. K.                                               *
 *******************************************************************************
 *  $Archive:: /Chameleon.sdk/SYSTEM/midishare/common/Memory/msMemory.c        $
 *     $Date: 2005/12/08 13:38:30 $
 * $Revision: 1.6.6.1 $
 *-----------------------------------------------------------------------------*
 * This file is part of the Chameleon Software Development Kit                 *
 *                                                                             *
 * Copyright (C) 2001 soundart                                                 *
 * www.soundart-hot.com                                                        *
 * codemaster@soundart-hot.com                                                 *
 ******************************************************************************/

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
  research@grame.fr

  modifications history:
   [08-09-99] DF - new memory management scheme

*/

#include "msDefs.h"
#include "msMemory.h"
#include "msMem.h"


#define kMaxEventsPerBlock	8191

/*===========================================================================
  Internal functions prototypes
  =========================================================================== */
static DWORD GrowSpace (MSMemoryPtr g, DWORD nbev);
static DWORD NewBlock  (MSMemoryPtr g, DWORD nbev);

/*===========================================================================
  External MidiShare functions implementation
  =========================================================================== */		
DWORD MSDesiredSpace(MSMemoryPtr g)
{
	return g->desiredSpace;
}

DWORD MSTotalSpace(MSMemoryPtr g)
{
	return g->active ? g->totalSpace : g->desiredSpace;
}

DWORD MSGrowSpace(DWORD nbev, MSMemoryPtr g)
{
	if (g->active <= 0) {
		g->desiredSpace += nbev;
		return nbev;
	}
	else return GrowSpace (g, nbev);
}

DWORD MSFreeSpace(MSMemoryPtr g)
{
	return g->active ? lfsize (FreeList(g)) : g->desiredSpace;
}

/*===========================================================================
  External initialization functions
  =========================================================================== */
void InitMemory(MSMemoryPtr g, DWORD defaultSpace)
{
	lfinit (BlockList(g));
	lfinit (FreeList(g));	
	g->totalSpace = 0;
	g->desiredSpace = defaultSpace;
	g->active = 0;
}

BOOL OpenMemory(MSMemoryPtr g) 
{
	g->active++;
	if (g->active == 1) {
		return (GrowSpace(g, g->desiredSpace) != 0);
	}
	return TRUE;
}

void CloseMemory(MSMemoryPtr g)
{
	void* blk, *next;
	g->active--;
	if (g->active == 0) {
		blk = lfpop (BlockList(g));
		while ( blk ) {
			next = lfpop (BlockList(g));	// fetch the next block before the function call
									// otherwise lfpop inlining is splitted by
									// a stack correction
									// a bug in CodeWarrior 5  compiler for mac is suspected  
			DisposeMemory (blk);
			blk = next;
		}
		InitMemory (g, g->desiredSpace);	
	}
}

/*===========================================================================
  Internal functions implementation
  =========================================================================== */
static DWORD GrowSpace(MSMemoryPtr g, DWORD nbev)
{
	DWORD count = 0;

	while ( nbev > kMaxEventsPerBlock ) {
		long n = NewBlock(g, kMaxEventsPerBlock);
		if (n) {
			count += n;
			nbev -= kMaxEventsPerBlock;
		}
		else return count;
	}
	if ( nbev > 0 )  count += NewBlock(g, nbev);
	return count;
}

static DWORD NewBlock(MSMemoryPtr g, DWORD nbev)
{
	void** 		blk;
	MidiEvPtr 	cl;
	DWORD 		i;
	
	if (nbev > 0) {
		DWORD size = nbev * sizeof(TMidiEv) + sizeof(void*);
		blk = (void **)AllocateMemory (kSharedMemory, size);
		if (!blk) return 0;
		lfpush ( BlockList(g), (cell*)blk);
		cl = (MidiEvPtr)(blk+1);
		for (i=0; i<nbev; i++) {
			lfpush(FreeList(g), (cell*)(cl++));
		}
		g->totalSpace += nbev; /* A REVOIR POUR UN SYSTEME LOCK FREE */
	}
	return nbev;
}
