/*

  Copyright © Grame 2002

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

#include <Carbon/Carbon.h>

#include "MidiShare.h"
#include "msDisplay.h"
#include "../lib/states.h"

static char * profileName = "msDisplay.ini";

/* Constants definition */
#define osEvent 				app4Evt
#define	resumeMask				1
#define	suspendResumeMessage	1

/* menu bar constants */
#define mBarSize	2
enum { AppleMenu, FileMenu };
enum { AppleID = 128, FileID };

/* dialog & alerts */
enum { dialogID = 128, AboutID, AlertID = 500 };

/* Application name	*/
#define ApplName 		"msDisplay"

/* global variables	*/
WindowRef	myWindow;
DialogRef	myDlog;
Boolean		doneFlag;

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

static void CloseWind (WindowRef wind)
{
	DisposeWindow(wind);
}

static void CloseAllWinds (WindowRef wind)
{
	if (wind) {
		CloseAllWinds (GetNextWindow(wind));
		CloseWind(wind);
	}
}

/* -----------------------------------------------------------------------------*/
static void SetUpWindows()
{
	Rect bounds; Point p;

	GetRegionBounds (GetGrayRgn(), &bounds);
	dragRect = bounds;
	InsetRect (&dragRect, 4, 4);
  	myDlog = GetNewDialog (dialogID, NULL, (WindowRef)-1);
  	if (!myDlog) return;

  	myWindow = GetDialogWindow(myDlog);
  	SetPort(GetWindowPort(myWindow));
	if (ReadPos (profileName, &p))
		MoveWindow (myWindow, p.h, p.v, false);
	AdjustLocation (myWindow);
	ShowWindow(myWindow);
}
		
/* -----------------------------------------------------------------------------*/
static void SetUpMenus()
{
	short	i;
	myMenus[AppleMenu] = GetMenu(AppleID);
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

	myDialog = GetNewDialog(AboutID, NULL, (WindowRef)(-1));
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

	theItem = LoWord(mResult);
	theMenu = HiWord(mResult);
	
	switch ( theMenu )
	{
		case AppleID:
			if (theItem == 1) ShowAbout();
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
	WindowRef whichWindow;
	
	switch ( FindWindow( myEvent.where, &whichWindow ) )
	{ 
		case inMenuBar:	
				DoCommand(MenuSelect(myEvent.where));
				break;
		case inSysWindow: 
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
	if (foreGround) {
		Cursor c;
		GetQDGlobalsArrow(&c);
		SetCursor(&c);
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
	InitCursor(); 
	FlushEvents(everyEvent, 0);
	AEInstallEventHandler ( kCoreEventClass, kAEQuitApplication, 
							NewAEEventHandlerUPP((AEEventHandlerProcPtr)AEQuitHandler), 0, false);
	if (!MidiShare()) 			AlertUser ("\pMidiShare is required");
	if (!SetupMidi(ApplName)) 	AlertUser ("\pMidiShare initialization failed");
	foreGround = true;
	resFile = CurResFile();
	SetUpMenus();
	SetUpWindows();
}

/*======================================== main loop =======================================*/
int main(int argc, char *argv[])
{
	Boolean b; char c;
	
	Initialize();
		doneFlag = false;											
	while (!doneFlag) {											/* Main Loop				*/
		b = WaitNextEvent(everyEvent, &myEvent, 10, nil);	/* no sleep, no mouseRgn	*/
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
				BeginUpdate(myWindow);
				SetPort( GetWindowPort(myWindow));
				DrawDialog(myDlog );
				DrawMyContent();
				EndUpdate(myWindow);
				break;
			case kHighLevelEvent:
				AEProcessAppleEvent (&myEvent);
				break;
		}
	}
	CloseMidi ();
	WritePos (profileName, myWindow);
	CloseAllWinds( FrontWindow() );
	return 0;
}
