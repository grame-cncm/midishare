/*

  Copyright © Grame 2000

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
  grame@rd.grame.fr

*/


#include <AppleEvents.h>
#include <Dialogs.h>
#include <Events.h>
#include <Fonts.h>
#include <Notification.h>
#include <Processes.h>
#include <TextEdit.h>
#include <Traps.h>
#include <Types.h>
#include <Windows.h>

#include "MidiShare.h"
#include "msSerialDriverAppl.h"

/* Constants definition */
#define osEvent 				app4Evt
#define	resumeMask				1
#define	suspendResumeMessage	1

/* high level event structure */
typedef struct AppleEventRecord {
	short 	what;
	OSType 	evtClass;
	long 	when;
	OSType 	evtID;
	short 	modifiers;
} AppleEventRecord;

/* Application name	*/
#define ApplName 		"\pMidiShare Serial Driver"

/* global variables	*/
Boolean		doneFlag;
static SysEnvRec	gMac;
static Boolean		hasWNE;
static EventRecord	myEvent;


/********************************************************************************/
/* 								Macintosh	 utilities							*/
/* 																				*/
/********************************************************************************/
static StringPtr PStrCat (Str255 s1, Str255 s2, Str255 dst)
{
	unsigned char * ptr = &dst[1];
	short i, n1 = *s1++, n2 = *s2++;
	if ((n1 + n2) > 255) n2 = 255 - n1;
	
	for (i=0; i<n1; i++)
		*ptr++ = *s1++;
	for (i=0; i<n2; i++)
		*ptr++ = *s2++;
	dst[0] = n1 + n2;
	return dst;
}

static void myNMProc (NMRecPtr nmReqPtr)
{
	Boolean * flag = (Boolean *)nmReqPtr->nmRefCon;
	*flag = true;
}

static void AlertUser( unsigned char *msg )
{
	static NMRec notify;
	static Str255 str;
	OSErr err;

	notify.qType = nmType;
	notify.nmMark = 0;
	notify.nmIcon = 0;
	notify.nmSound = (Handle)-1;
	PStrCat ("\pCannot launch ", ApplName, str);
	PStrCat (str, "\p: ", str);
	notify.nmStr = PStrCat (str, msg, str);
	notify.nmResp = NewNMProc(myNMProc);
	notify.nmRefCon = (long)&doneFlag;

	err = NMInstall(&notify);
	if (err != noErr) ExitToShell ();
}

static Boolean TrapAvailable(short	trapNum, short tType)
{
	if ( (tType == ToolTrap)  &&						/* Toolbox trap		*/
		 (gMac.machineType > envMachUnknown)  &&		/* É512KE, Plus, or	*/
		 (gMac.machineType < envMacII) ) {				/* ÉSE				*/
		 
		trapNum &= 0x3FF;				/* Traps numbers are 10 bits long	*/
		
		if (trapNum > 0x1FF) {			/* Traps only go up to 0x1FF on		*/
			return(false);				/*   these machines					*/
		}
	}
	return NGetTrapAddress(trapNum, tType) != GetTrapAddress(_Unimplemented);
}

/* -----------------------------------------------------------------------------*/
static OSErr AEQuitHandler (const AppleEvent *aevt, AppleEvent *reply, UInt32 refcon)
{
	CloseMidi ();
	doneFlag = true;
	return noErr;
}

/* -----------------------------------------------------------------------------*/
static void Initialize()
{
	OSErr	err;

	err = SysEnvirons(1, &gMac);
	if (gMac.machineType < 0) AlertUser( "\pneed at least 128K ROMs" );
	hasWNE = TrapAvailable(_WaitNextEvent, ToolTrap);
		
	InitGraf(&qd.thePort); 
	InitFonts();
	InitWindows();
	InitDialogs(nil);
	InitMenus();
	TEInit();
	InitCursor(); 

	FlushEvents(everyEvent, 0);
	AEInstallEventHandler ( kCoreEventClass, kAEQuitApplication, 
							NewAEEventHandlerProc(AEQuitHandler), 0, false);
	if (!CheckSCC()) 		{ AlertUser ("\pserial ports are required"); return; }
	if (!MidiShare()) 		{ AlertUser ("\pMidiShare is required"); return; }
	if (MidiGetVersion() < 180) { 
		AlertUser ("\prequire MidiShare version 1.80 or later");
		return;
	}
	if (!SetUpMidi()) AlertUser ("\pMidiShare initialization failed");
}

/*======================================== main loop =======================================*/
void main()
{
	MaxApplZone();
	Initialize();
	
	doneFlag = false;											
	while (!doneFlag) {
		if (hasWNE) 
			WaitNextEvent(everyEvent, &myEvent, 0, nil);
		else {
			SystemTask();
	 		GetNextEvent(everyEvent, &myEvent);
		}
		switch( myEvent.what ) {
			case kHighLevelEvent:
				AEProcessAppleEvent (&myEvent);
				break;
			default:
				DoIdle();
		}
	}
}






	

