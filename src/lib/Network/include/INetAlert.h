/*
  MidiShare Project
  Copyright (C) Grame 2001
  Copyright (C) Mil Productions 2001

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

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
