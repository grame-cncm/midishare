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
  grame@grame.fr

*/


#include <AppleEvents.h>
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
#include "msDisplay.h"

/* Constants definition */
#define osEvent 				app4Evt
#define	resumeMask				1
#define	suspendResumeMessage	1
enum { kWinPosRsrc = 'wipo', kWinposID = 128 };

/* menu bar constants */
#define mBarSize	2
enum { AppleMenu, FileMenu };
enum { AppleID = 128, FileID };

/* dialog & alerts */
enum { dialogID = 128, AboutID, AlertID = 500 };

/* high level event structure */
typedef struct AppleEventRecord {
	short 	what;
	OSType 	evtClass;
	long 	when;
	OSType 	evtID;
	short 	modifiers;
} AppleEventRecord;

/* Application name	*/
#define ApplName 		"\pmsDisplay"

/* global variables	*/
WindowPtr	myWindow;
Boolean		doneFlag;

SysEnvRec	gMac;
Boolean		hasWNE;
Boolean		foreGround;

EventRecord	myEvent;
Rect		dragRect;
MenuHandle	myMenus[mBarSize];
short		resFile;

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


//_______________________________________________________________________________
static OSErr WriteRsrc	(Handle rsrc, ResType type, short ID)
{
	OSErr err; Handle h;
	short saved;
	
	saved = CurResFile();
	UseResFile( resFile);
	if ((err= ResError())!=noErr)	return err;
	
	h= GetResource (type, ID);
	if (h) RemoveResource (h);
	HLock (rsrc);
	AddResource( rsrc, type, ID, "\p");
	err= ResError();
	if( err== noErr) {
		UpdateResFile( resFile);
		err= ResError();
	}
	HUnlock( rsrc);
	UseResFile( saved);
	return err;
}

//_______________________________________________________________________________
static Boolean ReadPos (Point *p)
{
	Handle h = GetResource (kWinPosRsrc, kWinposID);
	if (h) {
		*p = *((Point *)*h);
		ReleaseResource (h);
		return true;
	}
	return false;
}

//_______________________________________________________________________________
static void WritePos ()
{
	Handle h = NewHandle (sizeof(Point));
	if (h) {
		Point *p = (Point *)*h;
  		SetPort(myWindow);
		p->h = myWindow->portRect.left;
		p->v = myWindow->portRect.top;
		LocalToGlobal(p);
		WriteRsrc (h, kWinPosRsrc, kWinposID);
	}
}

/* -----------------------------------------------------------------------------*/
static void AdjustLocation (WindowPtr win)
{
	Point pt;
	
	pt.h = win->portRect.left + 6;	// 6 is enough to grab
	pt.v = win->portRect.top + 6;	// 6 is enough to grab
	LocalToGlobal(&pt);
	
	if (!PtInRgn( pt, GetGrayRgn())) {
		MoveWindow (win, qd.screenBits.bounds.left + 50, qd.screenBits.bounds.top  + 50, false);
	}
}

/* -----------------------------------------------------------------------------*/
static void SetUpWindows()
{
	Point p;
	SetRect (&dragRect, qd.screenBits.bounds.left, qd.screenBits.bounds.top, 
						qd.screenBits.bounds.right,qd.screenBits.bounds.bottom);
	InsetRect (&dragRect, 4, 4);
 	myWindow = GetNewDialog (dialogID, nil, (WindowPtr) -1);
 	SetPort(myWindow);
	if (ReadPos (&p))
		MoveWindow (myWindow, p.h, p.v, false);
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
	for (i = AppleMenu; i <= FileMenu; i++)
		InsertMenu(myMenus[i], 0);
	DrawMenuBar();
}			

/* -----------------------------------------------------------------------------*/
static void ShowAbout()
{										
	DialogPtr myDialog;
	short Hit;

	myDialog = GetNewDialog(AboutID, NULL, (WindowPtr)(-1));
	if (myDialog) {
		ModalDialog(NULL, &Hit);
		DisposeDialog(myDialog);
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
					while (StillDown())
						;
				break;
	}
}


/* -----------------------------------------------------------------------------*/
static void AdjustCursor() 
{
	if (foreGround && IsAppWindow(FrontWindow())) 
		SetCursor(&qd.arrow);
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

	if (!MidiShare()) 			AlertUser ("\pMidiShare is required");
	if (!SetupMidi(ApplName)) 	AlertUser ("\pMidiShare initialization failed");
	foreGround = true;
	resFile = CurResFile();
	SetUpMenus();
	SetUpWindows();
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
	CloseMidi ();
	WritePos ();
	CloseAllWinds( FrontWindow() );
}






	

