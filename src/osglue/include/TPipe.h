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

#ifndef __msPipe__
#define __msPipe__

#ifdef WIN32
	#include <Windows.h>

	typedef HANDLE	PipeHandler;
#else
	typedef int		PipeHandler;
#endif

//___________________________________________________________________
// MidiShare oriented pipes management:
//   pipes are always created in rw-r--r-- mode
//     owner has read/write permission
//     others have read only permission
//   pipes are unlinked by the Close method when caller is the owner
//___________________________________________________________________
class TPipe
{
	public:
		 	 TPipe ();
	virtual ~TPipe ()	{ Close (); }
			
		enum { kReadPerm, kWritePerm, kReadWritePerm, kMaxPipeName=512 };
		
		int		Create 	(const char * name);
		int		Open 	(const char *name, int perm=kReadPerm);
		int		Open 	(int perm=kReadPerm) { return Open (fName, perm); }
		void	Close 	();

		long	Write 	(void *buff, long len);
		long	Read 	(void *buff, long len);
		
		const char *	GetName () { return fName; }

		// BuffSize represents the internal pipe buffer size for unix
		// and a value fixed by the implementation for windows
		long	BuffSize ()		{ return fBuffSize; }

	private:
		void	SetName (const char * name) { snprintf (fName, kMaxPipeName-1, "%s", name); }

		PipeHandler fPipe;		// the pipe handler
		long		fBuffSize;	// pipe buffer size
		int			fOwner;		// a flag to check wether the pipe has been created
		char 		fName[kMaxPipeName];
/*
#ifndef WIN32
		char *		fPath;		// access path
#endif
*/
};

#endif
