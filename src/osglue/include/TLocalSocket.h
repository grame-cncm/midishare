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

#ifndef __TLocalSockets__
#define __TLocalSockets__

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

//___________________________________________________________________
// MidiShare oriented implementation of local sockets.
// Used only for Unix based implementations.
//___________________________________________________________________
class TLocalSocket
{
	public:
		 	 TLocalSocket ();
	virtual ~TLocalSocket ()	{ Close (); }
	
		// typically called by the server: creates the socket and 
		// bind it to its name
		int 	Create 	(char *name);
		void 	Delete 	(char *name); // force deletion of a existing socket
		// typically called by the client: creates the socket and 
		// bind it to the first indexed free name based on basename
		int 	Open 	(char *basename);
		void 	Close 	();

		int		Write 	(char * to, void * msg, int len);
		int		Read 	(void * msg, int len, char * from);
	
	private:
		int 	SockOpen ();
		int 	Bind (int silent=false);

		int		fBinded;
		int		fSocket;
		struct sockaddr_un 	fAddr;
};

#endif
