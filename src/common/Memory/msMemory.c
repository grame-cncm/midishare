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

*/

#include "msEvents.h"
#include "msMemory.h"
#include "msExtern.h"
#include "msSync.h"


/*===========================================================================
  Internal functions prototypes
  =========================================================================== */
static ulong GrowSpace (MSMemoryPtr g, ulong nbev);
static ulong NewBlock  (MSMemoryPtr g, ulong nbev);
static void  AddBlock  (MSMemoryPtr g, MemBlockPtr mb, MidiEvPtr last);

/*===========================================================================
  External MidiShare functions implementation
  =========================================================================== */		
MSFunctionType(ulong) MSDesiredSpace (MSMemoryPtr g)
{
	return g->desiredSpace;
}

MSFunctionType(ulong) MSTotalSpace (MSMemoryPtr g)
{
	return (g->active > 0) ? g->allocatedSpace : g->desiredSpace;
}

MSFunctionType(ulong) MSGrowSpace (unsigned long nbev, MSMemoryPtr g)
{
	if (nbev > 0) {
		if (g->active <= 0) {
			g->desiredSpace += nbev;
			return nbev;
		}
		else return GrowSpace (g, nbev);
	}
	return 0;
}

MSFunctionType(ulong) MSFreeSpace (MSMemoryPtr g)
{
	ulong count = 0, blocksize = freeSizeBlock;
	MidiEvPtr start, last = 0, e;

	if (g->active <= 0) return g->desiredSpace;
	blocksize = 50;
	start = e = MSNewCell(g);		/* first allocates blocksize count events */
	while (e) {
		count++;					/* update allocated events count */
		last = e;					/* keep the last allocated event */
		if (!--blocksize) break;
		e = MSNewCell(g);
		Link(last) = e;
	}
	if (last) {
		Link(last) = 0;				/* close the allocated events list */
		do {
			e = g->freeList;		/* and replace the freelist with the allocated list */
		} while (!CompareAndSwap (&g->freeList, e, start));
									/* e is now the pointer to the remaining free events */
		while (e) {
			start = e;
			blocksize = freeSizeBlock;
			while (e && blocksize--) {	/* count blocksize events */
				count++;
				last = e;
				e = Link(e);
			}
			PushMidiList (&g->freeList, start, last); /* free the block */
		}
	}
	return count;
}

/*===========================================================================
  External initialization functions
  =========================================================================== */
void InitMemory (MSMemoryPtr g, ulong defaultSpace)
{
	g->freeList = 0;
	g->blockList = 0;
	g->allocatedSpace = 0;
	g->desiredSpace = defaultSpace;
	g->active = 0;
	InitEvents (g);
}

Boolean OpenMemory (MSMemoryPtr g) 
{
	Boolean retCode = true;
	
	g->active++;
	if (g->active == 1) {
		retCode = (GrowSpace(g, g->desiredSpace) != 0);
	}
	return retCode;
}

void CloseMemory (MSMemoryPtr g)
{
	if (OpenMutex (kMemoryMutex) == kSuccess) {
		g->active--;
		if (g->active == 0) {
			MemBlockPtr mem, next;
			mem = g->blockList;
			while (mem) {
				next = mem->nextblock;
				DisposeMemory (mem);
				mem = next;
			}
			g->freeList = 0;
			g->blockList = 0;
			g->allocatedSpace = 0;
		}
		CloseMutex (kMemoryMutex);
	}
}


/*===========================================================================
  Internal functions implementation
  =========================================================================== */

static void AddBlock (MSMemoryPtr g, MemBlockPtr mb, MidiEvPtr last)
{
	PushMemBlock (BlockList(g), mb);
	PushMidiList (FreeList(g), Event(mb), last);
}

static ulong GrowSpace (MSMemoryPtr g, ulong nbev)
{
	ulong count = 0;
	ulong maxEvs = kMaxEventsPerBlock;

	if (OpenMutex (kMemoryMutex) == kSuccess) {
		if ( nbev > maxEvs ) {
			while ( nbev > maxEvs ) {
				count += NewBlock(g, maxEvs);
				nbev -= maxEvs;
			}
		}
		if ( nbev > 0 )  count += NewBlock(g, nbev);
		g->allocatedSpace += count;
		CloseMutex (kMemoryMutex);
	}
	return count;
}

static ulong NewBlock (MSMemoryPtr g, ulong nbev)
{
	ulong allocated=0, size = (nbev * kLenEvent) + sizeof(MemBlockPtr);
	MemBlockPtr mb = (MemBlockPtr)AllocateMemory (kSharedMemory, size);

	if (mb) {
		MidiEvPtr last, next = &mb->events;
		nbev = allocated = size / kLenEvent;
		while (nbev--) {
			last = next;
			last->link = ++next;
		}
		AddBlock (g, mb, last);
	}
	return allocated;
}
