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

#ifndef __TInitComGlue__
#define __TInitComGlue__

#ifdef WIN32
	#include "TMsgChan.h"
	typedef TMsgChan	 CommChan;
	typedef DWORD   DestAddr;
	typedef DWORD * FromHandler;
#else
	#include "TLocalSocket.h"	
	typedef TLocalSocket CommChan;
	typedef char * DestAddr;
	typedef char * FromHandler;
#endif

#include "msCommDefs.h"

class TLog;

//___________________________________________________________________
// This class provides only an abstraction layer built over the
// different communication channels used for unix and windows
//___________________________________________________________________
class TInitComGlue
{
	public:
		 	 TInitComGlue (TLog * log = 0);
	virtual ~TInitComGlue ()	{ Close (); }
	
		int		Create 	(char *name);	// creates a new communication channel
		int		Open 	(char *name);	// opens an existing communication channel
		int		Read 	(FromHandler from);
		int		Write 	(int id, DestAddr to);
		void 	Close 	();
	
	private:		
		CommChan	fCommChan;
		TLog *		fLog;		// optionnal log capabilities
};

#endif
