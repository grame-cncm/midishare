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

#ifndef __TClientInit__
#define __TClientInit__

#include "TInitComGlue.h"

class TLog;
class TPipesPair;
//___________________________________________________________________
// Abstraction layer for the client initialization:
// The class responsability is to initialize the pipes communication 
// channels. Transactions to create these pipes operate using the
// special server communication channel which current implementation
// is:
// 		socket based for unix
//		message based for windows
//___________________________________________________________________
class TClientInit
{
	public:
		 	 TClientInit (TLog * log = 0);
	virtual ~TClientInit ()	{ Close (); }
	
		TPipesPair * Start  ();
		void 		Close  ();
	
	private:
		int		HandleServerReply (int id);
		int		NewWPipe ();

		TPipesPair * PipesDetach ();
		
		TInitComGlue fCommChan;
		TPipesPair * fCommPipes;
		TLog *		fLog;		// optionnal log capabilities
};

#endif
