/*

  Copyright © Grame 2001
  Copyright © Mil Productions 2001

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

*/


#ifndef __INetAlert__
#define __INetAlert__

#include "InetTypes.h"

//_________________________________________________________________________________
//
//                class INetAlert
//_________________________________________________________________________________
class INetAlert
{
	private:
		enum { kAlertID = 501 };
		short fAlertID;

	public:
		//------------------------------------------ constructeur / destructeur
				 INetAlert (short alertID = kAlertID) { fAlertID = alertID; }
		virtual ~INetAlert (void) {}	

		//------------------------------------------ methodes
		void 	Report (const ErrString what, const ErrString obj, 
						const ErrString reason, long err);
		void 	FatalError (const ErrString what, const ErrString obj, 
						const ErrString reason, long err);
						
		void 	Report (const ErrString what, const ErrString obj, 
						const ErrString reason, const ErrString infos, void *proc=0);
		void 	FatalError (const ErrString what, const ErrString obj, 
						const ErrString reason, const ErrString infos);
#if macintosh
		unsigned char *	GetErrorText (long err);
		unsigned char * MakeOSErrString (long err);
#endif
};


#endif
