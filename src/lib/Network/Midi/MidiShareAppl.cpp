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
