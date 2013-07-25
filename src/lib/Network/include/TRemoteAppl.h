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


#ifndef __TMidiRemoteAppl__
#define __TMidiRemoteAppl__

#include "MidiShareAppl.h"
#include "StatesManager.h"

class TMidiRemote;
//____________________________________________________________
class TRemoteAppl : public MidiShareAppl
{
	public:
			 	 TRemoteAppl (TMidiRemote * owner);
		virtual ~TRemoteAppl () { if (fApplState) delete fApplState; }
	
		bool	Open 	(MidiName name, bool wantFilter = false);
		void 	Close	();
		void RcvAlarm  (short refnum);
		void ApplAlarm (short refnum, short srcRefNum, short changeCode);	

	private:
		TMidiRemote * 	fOwner;
		StatesManager  	fStateManager;
		ApplState *		fApplState;	
};

#endif
