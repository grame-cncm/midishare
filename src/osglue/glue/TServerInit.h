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

#ifndef __TServerInit__
#define __TServerInit__

#include "TInitComGlue.h"

class TLog;
class TPipesPair;
//___________________________________________________________________
// Abstraction layer for the server initialization:
// The class responsability is to initialize a communication channel.
// Clients should contact the server using this communication channel
// in order to negociate and initialize the client/server communication
// scheme.
//
// Current implementation is:
// 		socket based for unix
//		message based for windows
// it is designed separately from the main client/server communication
// scheme because for the first contact, all clients need to share 
// the same communication channel to address the server.
//___________________________________________________________________
class TServerInit
{
	public:
		 	 TServerInit (TLog * log = 0);
	virtual ~TServerInit ()	{ Close (); }
	
		// warning: 
		// on windows, the Start method MUST be called by the reader thread
		int 	Start 	();
		void 	Close 	();
		TPipesPair * HandleClientRequest (int id);

		int		Read 	(FromHandler from) 		{ return fCommChan.Read(from); }
		int		Write 	(int id, DestAddr to)	{ return fCommChan.Write(id, to); }
	
	private:
		TPipesPair * PipesInit (int clientID);
		void 		 Cleanup ();
		
		TInitComGlue fCommChan;
		TLog *		fLog;		// optionnal log capabilities
};

#endif
