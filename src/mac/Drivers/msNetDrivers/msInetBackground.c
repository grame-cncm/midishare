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
#include "msInetDriverAppl.h"
#include "INetAlert.h"

/* Constants definition */
#define osEvent 				app4Evt
#define	resumeMask				1
#define	suspendResumeMessage	1
#define kStateResType			'Stat'
#define kBaseRsrcId				128

/* high level event structure */
typedef struct AppleEventRecord {
	short 	what;
	OSType 	evtClass;
	long 	when;
	OSType 	evtID;
	short 	modifiers;
} AppleEventRecord;

/* Application name	*/
#define ApplName 		"\pMidiShare Internet Driver"

/* global variables	*/
Boolean		doneFlag;
Boolean		autoQuit = true;
static SysEnvRec	gMac;
static Boolean		hasWNE;
static EventRecord	myEvent;
short 		gResFile;
State 		gState;


/********************************************************************************/
/* 								Macintosh	 utilities							*/
/* 																				*/
/********************************************************************************/
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
	return NGetTrapAddress(trapNum, tType) != NGetTrapAddress(_Unimplemented, tType);
}

//_________________________________________________________________________________
static void InitState (StatePtr g)
{
	Handle h;
	StatePtr state;

	g->driverMode= true;
	g->showWindow= false;
	g->autoQuit= true;
	g->net.port = kDefaultLANPort;
	g->net.maxlat = kDefaultLANLatency;
	g->net.groupTime = kDefaultLANGroupTime;
	h= GetResource( kStateResType, kBaseRsrcId);
	if( h) {
		state= (StatePtr)*h;
		*g = *state;
	}
}

/* -----------------------------------------------------------------------------*/
static OSErr AEQuitHandler (const AppleEvent *aevt, AppleEvent *reply, UInt32 refcon)
{
	doneFlag = true;
	return noErr;
}

/* -----------------------------------------------------------------------------*/
static void Initialize()
{
	OSErr err; unsigned char *msg; INetAlert alert;

	err = SysEnvirons(1, &gMac);
	if (gMac.machineType < 0) //AlertUser( "\pneed at least 128K ROMs" );
		alert.FatalError ("\pCannot launch ", ApplName, "\pneed at least 128K ROMs", 0); 
	hasWNE = TrapAvailable(_WaitNextEvent, ToolTrap);
	doneFlag = false;											
		
	InitGraf(&qd.thePort); 
	InitFonts();
	InitWindows();
	InitDialogs(nil);
	InitMenus();
	TEInit();
	InitCursor(); 

	gResFile = CurResFile();
	FlushEvents(everyEvent, 0);
	AEInstallEventHandler ( kCoreEventClass, kAEQuitApplication, 
							NewAEEventHandlerProc(AEQuitHandler), 0, false);
	if (!CheckOT()) { 
		alert.FatalError ("\pCannot launch ", ApplName, "\pOpen Transport TCP 1.1 is required", 0); 
		return;
	}
	if (!MidiShare()) {  
		alert.FatalError ("\pCannot launch ", ApplName, "\pMidiShare is required", 0); 
		return;
	}
	if (MidiGetVersion() < 182) { 
		alert.FatalError ("\pCannot launch ", ApplName, "\prequire MidiShare version 1.82 or later", 0); 
		return;
	}
	InitState (&gState);
	msg = SetUpDriver(&gState);
	if (msg)
		alert.FatalError ("\pCannot launch ", ApplName, msg, 0); 

}

/*======================================== main loop =======================================*/
void main()
{
	MaxApplZone();
	Initialize();
	
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
	CloseInetDriver ();
}






	

