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


#include "MidiShareDriver.h"

#ifndef WIN32
#define MSALARMAPI
#endif

static MidiShareDriver * gThis = 0;	// used for wakeup callback
/* -----------------------------------------------------------------------------*/
/* Driver required callbacks                                                    */
/* -----------------------------------------------------------------------------*/
static void MSALARMAPI __WakeUp (short r)
{
	if (gThis) gThis->WakeUp (r);
	MidiConnect (MidiShareDrvRef, r, true);
	MidiConnect (r, MidiShareDrvRef, true);
}

/* -----------------------------------------------------------------------------*/
static void MSALARMAPI __Sleep (short r)
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
bool MidiShareDriver::Open (MidiName name, bool wantFilter)
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
