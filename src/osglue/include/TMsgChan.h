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

#ifndef __TMsgChan__
#define __TMsgChan__

#include <Windows.h>
#include "TShMem.h"
#include "../msCommChans.h"
	
//___________________________________________________________________
// MidiShare oriented implementation of message based communication.
// Used only for Windows based implementations.
//___________________________________________________________________
class TMsgChan
{
	public:
		 	 TMsgChan ();
	virtual ~TMsgChan ()	{ Close (); }
	
		// typically called by the server:  
		// warning: this method MUST be called by the reader thread
		int 	Create 	(char *name);
		// typically called by the client 
		int 	Open 	(char *basename);
		void 	Close 	();

		// when destination address is 0, the message is sent
		// to the server 
		int		Write 	(ContactMsgPtr msg, DWORD to = 0);
		int		Read 	(ContactMsgPtr msg, DWORD * from);
	
	private:
		DWORD	ServerAddr ();

		void *	fInfos;
		TShMem 	fHandle;	// handler on shared memory dedicated
							// the server thread identification
};

#endif
