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


#include "MidiShareAppl.h"

//_______________________________________________________________________________
static msCallback __ReceiveEvents (short refnum)
{
	MidiShareApplPtr appl = (MidiShareApplPtr)MidiGetInfo (refnum);
	appl->RcvAlarm (refnum);
}

//_______________________________________________________________________________
static msCallback __ApplAlarm ( short refNum, long code )
{
	MidiShareApplPtr appl = (MidiShareApplPtr)MidiGetInfo (refNum);
	appl->ApplAlarm (refNum, (short)(code >> 16), (short)(code & 0xffff));
}



//_______________________________________________________________________________
// classe MidiShareAppl
//_______________________________________________________________________________
MidiShareAppl::MidiShareAppl()
{
	fRefNum	= undefinedRefNum;
#if defined(macintosh) && defined(MidiSharePPC_68k)
	fUPPRcvAlarm = NewRcvAlarmPtr(__ReceiveEvents);
	fUPPApplAlarm = NewApplAlarmPtr(__ApplAlarm);
#endif
}


//_______________________________________________________________________________
MidiShareAppl::~MidiShareAppl()
{
	Close ();
#if defined(macintosh) && defined(MidiSharePPC_68k)
	if (fUPPRcvAlarm) DisposeRoutineDescriptor(fUPPRcvAlarm);
	if (fUPPApplAlarm) DisposeRoutineDescriptor(fUPPApplAlarm);
#endif
}

//_______________________________________________________________________________
bool MidiShareAppl::Open (MidiName name, bool wantFilter)
{
	if( fRefNum == undefinedRefNum) {
		fRefNum= MidiOpen (name);		
		if (fRefNum == MIDIerrSpace) {
			fRefNum= undefinedRefNum;
			return false;
		}
		MidiSetInfo (fRefNum, this);
		SetRcvAlarm (true);
		if (wantFilter) MidiSetFilter (fRefNum, fFilter.GetMidiFilter());
	}
	return true;
}

//_______________________________________________________________________________
void MidiShareAppl::Close()
{
	if( Opened () ) {
		SetRcvAlarm (false);
		MidiFlushEvs (fRefNum);
		MidiClose( fRefNum);
		fRefNum= undefinedRefNum;
	}
}

//_______________________________________________________________________________
void MidiShareAppl::SetApplAlarm (bool on)
{
#if defined(macintosh) && defined(MidiSharePPC_68k)
	MidiSetApplAlarm (fRefNum, on ? fUPPApplAlarm : 0);
#else
	MidiSetApplAlarm (fRefNum, on ? __ApplAlarm : 0);
#endif	
}	

//_______________________________________________________________________________
void MidiShareAppl::SetRcvAlarm (bool on)
{
#if defined(macintosh) && defined(MidiSharePPC_68k)
	MidiSetRcvAlarm (fRefNum, on ? fUPPRcvAlarm : 0);	
#else
	MidiSetRcvAlarm (fRefNum, on ? __ReceiveEvents : 0);	
#endif	
}	
