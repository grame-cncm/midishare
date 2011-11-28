/*

  Copyright (c) Grame 1999, 2011

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

*/

#include <stdlib.h>
#include <stdio.h>

#include "MidiShareLight.h"
#include "msMemory.h"
#include "msMem.h"
#include "msEvents.h"


#define kMaxEventsPerBlock	8191

//------------------------------------------------------------------------------
MSMemory::MSMemory(unsigned long defaultSpace) 
{
	fFreeList = (lifo*) malloc(sizeof(lifo));
	fBlockList = (lifo*) malloc(sizeof(lifo));

	InitMemory (defaultSpace);
	InitEvents();
	OpenMemory ();
}

//------------------------------------------------------------------------------
MSMemory::~MSMemory()
{
	CloseMemory (); 
	delete fFreeList;
	delete fBlockList;
}


/*===========================================================================
  External initialization functions
  =========================================================================== */
unsigned long MSMemory::FreeSpace () const
{
	return fActive ? lfsize (fFreeList) : fDesiredSpace;
}

void MSMemory::InitMemory (unsigned long defaultSpace)
{
	lfinit (fBlockList);
	lfinit (fFreeList);	
	fTotalSpace = 0;
	fDesiredSpace = defaultSpace;
	fActive = 0;
}

bool MSMemory::OpenMemory () 
{
	fActive++;
	if (fActive == 1) {
		return (GrowSpace(fDesiredSpace) != 0);
	}
	return true;
}

void MSMemory::CloseMemory ()
{
	void* blk, *next;
	fActive--;
	if (fActive == 0) {
		blk = lfpop (fBlockList);
		while ( blk ) {
			next = lfpop (fBlockList);	// fetch the next block before the function call
									// otherwise lfpop inlining is splitted by
									// a stack correction
									// a bug in CodeWarrior 5  compiler for mac is suspected  
			free (blk);
			blk = next;
		}
		InitMemory (DesiredSpace());	
	}
}

/*===========================================================================
  Internal functions implementation
  =========================================================================== */
unsigned long MSMemory::GrowSpace (unsigned long nbev)
{
	unsigned long count = 0;

	while ( nbev > kMaxEventsPerBlock ) {
		long n = NewBlock(kMaxEventsPerBlock);
		if (n) {
			count += n;
			nbev -= kMaxEventsPerBlock;
		}
		else return count;
	}
	if ( nbev > 0 )  count += NewBlock(nbev);
	return count;
}

unsigned long MSMemory::NewBlock (unsigned long nbev)
{
	void** 		blk;
	MidiEvPtr 	cl;
	unsigned long i;
	
	if (nbev > 0) {
		unsigned long size = nbev * sizeof(TMidiEv) + sizeof(void*);
		blk = (void **)malloc (size+32);	// for manual alignment
		if (!blk) return 0;
		lfpush ( fBlockList, (lifocell*)blk);
		cl = (MidiEvPtr)(blk+1);
		for (i=0; i<nbev; i++) {
			lfpush(fFreeList, (lifocell*)(cl++));
		}
		fTotalSpace += nbev; /* A REVOIR POUR UN SYSTEME LOCK FREE */
	}
	return nbev;
}
