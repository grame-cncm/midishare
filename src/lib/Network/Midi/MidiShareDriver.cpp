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


#include "MidiShareDriver.h"

static MidiShareDriver * gThis = 0;	// used for wakeup callback
/* -----------------------------------------------------------------------------*/
/* Driver required callbacks                                                    */
/* -----------------------------------------------------------------------------*/
static void __WakeUp (short r)
{
	if (gThis) gThis->WakeUp (r);
}

/* -----------------------------------------------------------------------------*/
static void __Sleep (short r)
{
	MidiShareDriver * appl = (MidiShareDriver *)MidiGetInfo (r);
	if (appl) appl->Sleep (r);
}

//_______________________________________________________________________________
// classe MidiShareDriver
//_______________________________________________________________________________
MidiShareDriver::MidiShareDriver (short version) 
{ 
	fVersion = version;
#if defined(macintosh) && defined(MidiSharePPC_68k)
	fUPPWakeup = NewWakeupProcPtr(__WakeUp);
	fUPPSleep  = NewSleepProcPtr(__Sleep);
#endif
}

//_______________________________________________________________________________
MidiShareDriver::~MidiShareDriver()
{
	Close ();
#if defined(macintosh) && defined(MidiSharePPC_68k)
	if (fUPPWakeup) DisposeRoutineDescriptor(fUPPWakeup);
	if (fUPPSleep) DisposeRoutineDescriptor(fUPPSleep);
#endif
}

//_______________________________________________________________________________
Boolean MidiShareDriver::Open (MidiName name, Boolean wantFilter)
{
	if (!Opened()) {
#if defined(macintosh) && defined(MidiSharePPC_68k)
		TDriverOperation op = { (DriverAlarmPtr)fUPPWakeup, (DriverAlarmPtr)fUPPSleep };
#else
		TDriverOperation op = { __WakeUp, __Sleep };
#endif				
		TDriverInfos infos;
		
		MidiName src = name, dst = infos.name;
#if macintosh
		short n = *name;
		if (n > DrvNameLen-1) n = DrvNameLen-1;
		*dst++ = n;
		src++;
		while (n--) *dst++ = *src++;
#else
		short n = DrvNameLen;
		while (--n && *src) *dst++ = *src++;
		*dst = 0;
#endif				
		gThis = this;
		fRefNum= MidiRegisterDriver (&infos, &op);		
		if (fRefNum == MIDIerrSpace) {
			fRefNum= undefinedRefNum;
			return false;
		}
		MidiSetInfo (fRefNum, this);
		SetRcvAlarm (true);
		MidiConnect (MidiShareDrvRef, fRefNum, true);
		MidiConnect (fRefNum, MidiShareDrvRef, true);
	}
	return true;
}

//_______________________________________________________________________________
void MidiShareDriver::Close ()
{
	MidiSetRcvAlarm (fRefNum, 0);
	if (Opened ()) {
		MidiFlushEvs (fRefNum);
		MidiUnregisterDriver (fRefNum);
		fRefNum= undefinedRefNum;
	}
}
