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
