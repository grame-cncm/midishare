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

#ifndef __TShmem__
#define __TShmem__

#ifdef WIN32
#	include <windows.h>

	typedef char * 	MemID;
	typedef HANDLE 	ShMemHandler;

#else
#	include <sys/shm.h>
#	include <sys/ipc.h>

	typedef key_t 	MemID;
	typedef int 	ShMemHandler;
#endif

//___________________________________________________________________
// MidiShare oriented shared memory segments management:
//   segments are always created in exclusive mode
//   owner has read/write permission (granted by the Create method)
//   others have read only permission (granted by the Open method)
//___________________________________________________________________
class TShMem
{
	public:
		 	 TShMem ();
	virtual ~TShMem ();
			
		void *	Create 	(MemID id, unsigned long size);
		void *	Open 	(MemID id);
		void	Close 	();
				
	protected:
		int 	Get 	(MemID id, unsigned long size, int flags);
		void *	Attach 	();
		void	Detach 	();

	private:
		void * 	fMemAddr;	// the memory map address
		int		fCreated;	// a flag to check wether the segment has been created

		ShMemHandler fHandler;	// handler to the shared segment
};

#endif
