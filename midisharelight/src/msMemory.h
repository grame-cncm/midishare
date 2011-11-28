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
