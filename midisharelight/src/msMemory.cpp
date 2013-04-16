/*
  MidiShare Project
  Copyright (C) Grame 1999-2011

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
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
