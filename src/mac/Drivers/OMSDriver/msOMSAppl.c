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


#include <Dialogs.h>
#include <DiskInit.h>
#include <Events.h>
#include <Files.h>
#include <Fonts.h>
#include <Memory.h>
#include <Menus.h>
#include <OSUtils.h>
#include <Packages.h>
#include <QuickDraw.h>
#include <Resources.h>
#include <SegLoad.h>
#include <TextEdit.h>
#include <ToolUtils.h>
#include <Traps.h>
#include <Types.h>
#include <Windows.h>

#include "MidiShare.h"
#include "msOMSDriver.h"

/* Constants definition */
#define osEvent 				app4Evt
#define	resumeMask				1
#define	suspendResumeMessage	1
#define kStateResType	'Stat'
#define kBaseRsrcId		128

/* menu bar constants */
#define mBarSize	3
enum { AppleMenu, FileMenu, SetupMenu };
enum { AppleID = 128, FileID, SetupID };
enum { ShowHideI = 1, QuitSetupI };

/* dialog & alerts */
enum { dialogID = 128, AlertID = 500 };

/* Application name	*/
#define ApplName 		"\pMidiShare OMS Driver"

typedef struct state {
	Boolean		showWindow;
	Boolean		autoQuit;
} State, *StatePtr;

/* global variables	*/
WindowPtr	myWindow = 0;
Boolean		doneFlag;
State 		gState;
short 		resFile;

SysEnvRec	gMac;
Boolean		hasWNE;
Boolean		foreGround;

EventRecord	myEvent;
Rect		dragRect;
MenuHandle	myMenus[mBarSize];

unsigned char *Hide= "\pHide window";
unsigned char *Show= "\pShow window";


/********************************************************************************/
/* 								Macintosh	 utilities							*/
/* 																				*/
/********************************************************************************/
static void AlertUser( unsigned char *msg )
{
	ParamText( ApplName, msg, "\p", "\p");
	Alert(AlertID,nil);
	ExitToShell();								
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
	return NGetTrapAddress(trapNum, tType) != NGetTrapAddress(_Unimplemented, tType);
}

static Boolean IsAppWindow (WindowPtr wind)
{
	return  wind ? ((WindowPeek)wind)->windowKind >= 0 : false ;
}

static Boolean IsDAWindow (WindowPtr wind)
{
	return  wind ? ((WindowPeek)wind)->windowKind < 0 : false ;
}

static void CloseWind (WindowPtr wind)
{
	if ( IsDAWindow(wind) )
		CloseDeskAcc( ((WindowPeek)wind)->windowKind );
	else if ( IsAppWindow(wind) )
		DisposeWindow(wind);
}

static void CloseAllWinds (WindowPtr wind)
{
	if (wind) {
		CloseAllWinds( (WindowPtr) (((WindowPeek)wind)->nextWindow) );
		CloseWind(wind);
	}
}

//_________________________________________________________________________________
static void AdjustLocation (DialogPtr dlog)
{
	Point p;
	
	p.h = dlog->portRect.left + 6;	// 6 is enough to grab
	p.v = dlog->portRect.top + 6;	// 6 is enough to grab
	LocalToGlobal(&p);
	
	if (!PtInRgn (p, GetGrayRgn())) {
		MoveWindow ((WindowPtr) dlog, qd.screenBits.bounds.left + 50,			
									  qd.screenBits.bounds.top  + 50, false);
	}
}

/* -----------------------------------------------------------------------------*/
static void SetUpWindows()
{
	SetRect (&dragRect, qd.screenBits.bounds.left, qd.screenBits.bounds.top, 
						qd.screenBits.bounds.right,qd.screenBits.bounds.bottom);
	InsetRect (&dragRect, 4, 4);
 	myWindow = GetNewDialog (dialogID, nil, (WindowPtr) -1);
 	SetPort(myWindow);
	AdjustLocation (myWindow);
	ShowWindow(myWindow);
}
	
/* -----------------------------------------------------------------------------*/
static void SetUpMenus()
{
	short	i;
		
	myMenus[AppleMenu] = GetMenu(AppleID);
	AppendResMenu(myMenus[AppleMenu],'DRVR');
	myMenus[FileMenu] = GetMenu(FileID);
	myMenus[SetupMenu] = GetMenu(SetupID);
	for (i = AppleMenu; i <= SetupMenu; i++)
		InsertMenu(myMenus[i], 0);
	DrawMenuBar();
}			

/* -----------------------------------------------------------------------------*/
static void ShowAbout()
{										
}

//_________________________________________________________________________________
static void SaveLocation (DialogPtr dlog)
{
	Handle	h1;
	Rect	WhereItIs;
	
	h1 = GetResource( 'DLOG', dialogID);
	if( dlog) {
		WhereItIs = dlog->portRect;
		LocalToGlobal( (Point *) &(WhereItIs.top) );
		LocalToGlobal( (Point *) &(WhereItIs.bottom) );
		SetPort( dlog);

		if( h1) {
			HNoPurge( h1);
			( *((DialogTHndl)h1))->boundsRect = WhereItIs;
			ChangedResource( h1);
			WriteResource( h1);
			HPurge( h1);
		}
	}
}
		
/* -----------------------------------------------------------------------------*/
static void WShowHide ()
{
	if( myWindow) {
		SaveLocation (myWindow);
		CloseWind( myWindow);
		myWindow= 0;
		gState.showWindow= false;
	}
	else {
		SetUpWindows();
		gState.showWindow= true;;
	}
}
		
/* -----------------------------------------------------------------------------*/
static void DoCommand(long mResult)
{
	short	theItem;
	short	theMenu;
	Str255	name;

	theItem = LoWord(mResult);
	theMenu = HiWord(mResult);
	
	switch ( theMenu )
	{
		case AppleID:
			if (theItem != 1) 
			{
				GetMenuItemText(myMenus[AppleMenu], theItem, name);
				theItem = OpenDeskAcc(name);
			} else {
				ShowAbout();
			}
			break;

		case FileID: 
			doneFlag = true;
			break;

		case SetupID:
			switch (theItem) {
				case ShowHideI:
					WShowHide ();
					break;
				case QuitSetupI:
					gState.autoQuit = !gState.autoQuit;
					SetItemMark(myMenus[SetupMenu], QuitSetupI, 
									gState.autoQuit ? checkMark : noMark);
					break;
			} 
			break;
	}
	HiliteMenu(0);  
}

/* -----------------------------------------------------------------------------*/
static void DoMouseDown() 
{
	WindowPtr whichWindow;
	
	switch ( FindWindow( myEvent.where, &whichWindow ) )
	{ 
		case inMenuBar:	
				SetMenuItemText( myMenus[SetupMenu], ShowHideI, myWindow ? Hide : Show);
				DoCommand(MenuSelect(myEvent.where));
				break;
		case inSysWindow: 
				SystemClick(&myEvent, whichWindow);
				break;
		case inGoAway: 	 
				doneFlag = TrackGoAway(whichWindow, myEvent.where);
				break;
		case inDrag: 	 
				DragWindow(whichWindow, myEvent.where, &dragRect);
				break;
		case inContent: 	 
				if (whichWindow != FrontWindow())
					SelectWindow(whichWindow);
				else if (whichWindow == myWindow)
					DragWindow(whichWindow, myEvent.where, &dragRect);
				break;
	}
}


/* -----------------------------------------------------------------------------*/
static void AdjustCursor() 
{
	if (foreGround && IsAppWindow(FrontWindow())) 
		SetCursor(&qd.arrow);
}

//_________________________________________________________________________________
static void SetState (StatePtr state)
{
	if (state->showWindow) {
		SetUpWindows();
	}
	SetItemMark (myMenus[SetupMenu], QuitSetupI, 
									state->autoQuit ? checkMark : noMark);
}

//_________________________________________________________________________________
static void InitState (StatePtr g)
{
	Handle h;
	StatePtr state;

	g->showWindow= true;
	g->autoQuit= true;
	h= GetResource( kStateResType, kBaseRsrcId);
	if( h) {
		state= (StatePtr)*h;
		*g = *state;
	}
}

//_________________________________________________________________________________
static void SaveState ()
{
	Handle h;
	StatePtr *state;

	h= GetResource( kStateResType, kBaseRsrcId);
	if( !h) {
		state= (StatePtr *)NewHandle( sizeof( State));
		if( state){
			HLock( (Handle)state);
			**state= gState;
			AddResource( (Handle)state, kStateResType, kBaseRsrcId, "\pState");
			UpdateResFile( resFile);
			HUnlock( (Handle)state);
			DisposeHandle( (Handle)state);
		}
	}
	else {
		state= (StatePtr *)h;
		**state= gState;
		HLock( h);
		ChangedResource( h);
		UpdateResFile( resFile);
		HUnlock( h);
	}
	if( myWindow)
		SaveLocation( myWindow);
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

	resFile = CurResFile();
	if (!CheckOMS()) 			AlertUser ("\pOMS 2.00 or later is required");
	if (!MidiShare()) 			AlertUser ("\pMidiShare is required");
	if (MidiGetVersion() < 182) AlertUser ("\prequire MidiShare version 1.82 or later");
	if (MidiGetNamedAppl (OMSDriverName) > 0) AlertUser ("\pOMS driver is still running");
	if (!SetUpMidi(&gState.autoQuit)) 		AlertUser ("\pMidiShare initialization failed");
	foreGround = true;
	SetUpMenus();
	InitState (&gState);
	SetState (&gState);
}

/*======================================== main loop =======================================*/
void main()
{
	Boolean b; char c;
	
	MaxApplZone();
	Initialize();
	
	doneFlag = false;											
	while (!doneFlag) {											/* Main Loop				*/
		if (hasWNE) 
			b = WaitNextEvent(everyEvent, &myEvent, 0, nil);	/* no sleep, no mouseRgn	*/
		else {
			SystemTask();
	 		b = GetNextEvent(everyEvent, &myEvent);
		}
		AdjustCursor();											
		switch( myEvent.what ) {
			case nullEvent:
					DoIdle();
					break;
			case osEvent:
					switch (myEvent.message >> 24) {
						case suspendResumeMessage:
							foreGround = myEvent.message & resumeMask;
							break;
					}
					break;
			case keyDown:
			case autoKey:
					c = myEvent.message & charCodeMask;
					if (myEvent.modifiers & cmdKey ) {
						DoCommand(MenuKey(c));
					}
					break;
			case mouseDown:
					DoMouseDown();
					break;
			case updateEvt:
					if ( IsAppWindow( (WindowPtr)myEvent.message ) ) {
						BeginUpdate( (WindowPtr)myEvent.message );
						if ( !EmptyRgn( ((WindowPtr)myEvent.message)->visRgn ) ) {
							SetPort( (WindowPtr)myEvent.message );
							DrawDialog( (WindowPtr)myEvent.message );
						}
						EndUpdate( (WindowPtr)myEvent.message );
					}
					break;
			case kHighLevelEvent:
				AEProcessAppleEvent (&myEvent);
				break;
		}
	}
	SaveState ();
	CloseMidi ();
	CloseAllWinds( FrontWindow() );
}






	

