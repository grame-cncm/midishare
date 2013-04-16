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


#include "TRemoteAppl.h"
#include "TMidiRemote.h"

//____________________________________________________________
// TRemoteAppl
//____________________________________________________________
TRemoteAppl::TRemoteAppl (TMidiRemote * owner) 
{ 
	fOwner = owner;
	fApplState = 0; 
}

//____________________________________________________________
bool TRemoteAppl::Open (MidiName name, bool wantFilter)
{
	if (MidiShareAppl::Open (name, wantFilter)) {
		SetApplAlarm (true);
		fApplState = new ApplState (GetRefNum());
		if (fApplState) {
			fStateManager.SetState (fApplState);
			fStateManager.LoadState (fOwner->GetID());
		}
		return true;
	}
	return false;
}

//____________________________________________________________
void TRemoteAppl::Close	()
{
	if (fApplState) {
		fStateManager.SaveState (fOwner->GetID());
		fStateManager.SetState (0);
		delete fApplState;
		fApplState = 0;
	}
	MidiShareAppl::Close ();
}

//____________________________________________________________
void TRemoteAppl::ApplAlarm (short refnum, short srcRefNum, short changeCode)
{
	if (fApplState) fApplState->Alarm (refnum, srcRefNum, changeCode);
}

//____________________________________________________________
void TRemoteAppl::RcvAlarm (short refnum)
{
	SetRcvAlarm (false);		// switch to grouping mode
	fOwner->RcvAlarm (refnum);
}
