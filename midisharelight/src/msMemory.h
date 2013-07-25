/*
  MidiShare Project
  Copyright (C) Grame 1999-2011

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#ifndef __Memory__
#define __Memory__

#include "lflifo.h"

/*------------------------------------------------------------------------------*/
/*     data structures                                                          */
/*------------------------------------------------------------------------------*/
class MSMemory {
    lifo*	     fFreeList;             /* available events list          */
    lifo*        fBlockList;            /* allocated block list           */
    unsigned long fDesiredSpace;        /* count of free+used events      */
    unsigned long fTotalSpace;         	/* count of free+used events      */
    long         fActive;               /* active > 0 if memory is active */

	/* private memory management functions */
	void     InitMemory  (unsigned long defaultSpace);
	bool     OpenMemory  ();
	void     CloseMemory ();
	unsigned long NewBlock (unsigned long nbev);

	public:
				 MSMemory (unsigned long defaultSpace = 5000);
		virtual ~MSMemory ();
		
		unsigned long	GrowSpace    (unsigned long nbev);
		unsigned long	DesiredSpace () const		{ return fDesiredSpace; }
		void			DesiredSpace (unsigned long n) { fDesiredSpace = n; }
		unsigned long	FreeSpace	 () const;
		unsigned long	TotalSpace   () const		{ return fActive ? fTotalSpace : fDesiredSpace; }
		long			Active		 () const		{ return fActive; }
		
		lifo*			FreeList()	{ return fFreeList; }
};


#endif
