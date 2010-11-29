/*

  Copyright © Grame 2001-2002

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
#include "msInetDriverAppl.h"
#include "StrConstants.h"
#include "RadioHDFeedback.h"
#include "states.h"

/* Constants definition */
#define osEvent 				app4Evt
#define	resumeMask				1
#define	suspendResumeMessage	1
#define kStateResType	'Stat'
#define kBaseRsrcId		128

/* rsrc constants */
enum { AppleMenu, FileMenu };
enum { AppleID = 128, FileID };

/* dialog & alerts */
enum { dialogID = 128, aboutID, UDPPrefsID, AlertID = 500 };

extern RadioHDFeedback * gFeedback;

#if CustomRadio
#include "msWANAppl.h"
	enum { kOK = 1, kCancel, OpenI=1, QuitI=3 };
	ModalFilterUPP dlogFilterProcPtr;
	void OpenRemote (ModalFilterUPP filterProc);
#else
	enum { QuitI=1 };
#endif

#define mBarSize	2
enum { kDisconnectItem = 1 };
#define strApplName "\pRadio Hard Disk"


/* global variables	*/
static char * profileName = "RadioHardDisk.ini";

WindowRef	myWindow = 0;
DialogRef	myDlog;
Boolean		doneFlag;
State 		gState;
short 		gResFile;

//SysEnvRec	gMac;
Boolean		hasWNE;
Boolean		foreGround;

EventRecord	myEvent;
Rect		dragRect;
MenuHandle	myMenus[mBarSize];

void AlertUser (const unsigned char * s, const unsigned char *msg );
static void DoMouseDown (EventRecord *event, WindowRef win);


/********************************************************************************/
/* 								Macintosh	 utilities							*/
/* 																				*/
/********************************************************************************/
void AlertUser (const unsigned char * s, const unsigned char *msg )
{
	ResetAlertStage ();
	ParamText(s, strApplName, msg, "\p");
	Alert (AlertID,nil);
}

static void FatalError(const unsigned char *msg )
{
	AlertUser ("\pCannot launch", msg);
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

//_________________________________________________________________________________
#if CustomRadio
static pascal Boolean DlogFilterProc( DialogPtr dlog, EventRecord *event, short *item)
{
	char c;

	switch( event->what) {
		case nullEvent:
			DoIdle();
			break;
		case keyDown:
		case autoKey:
			c= (event->message & charCodeMask);
			switch(c) {
				case kEnterCharCode:
				case kReturnCharCode: 	*item= kOK;
					return true;
				case kEscapeCharCode: 	*item= kCancel;
					return true;
				default: 				DialogSelect( event, &dlog, item);
			} 
			event->what= nullEvent;
			break;
		case mouseDown:
			if( IsDialogEvent( event)) {
				if( DialogSelect( event, &dlog, item)) {
					switch( *item) {
						case kOK:
						case kCancel:
							return true;
						default:
							event->what= nullEvent;
					}
				}
				else DragWindow(dlog, event->where, &dragRect);
			}
			break;
	}	
	return false;
}
#endif

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
 	TextFont(kFontIDGeneva); TextSize(9);
	if (ReadPos (profileName, &p))
		MoveWindow (myWindow, p.h, p.v, false);
	AdjustLocation (myWindow);
}
		
/* -----------------------------------------------------------------------------*/
static void SetUpMenus()
{
	short	i;	
	myMenus[AppleMenu] = GetMenu(AppleID);
	myMenus[FileMenu] = GetMenu(FileID);
	for (i = AppleMenu; i < mBarSize; i++)
		InsertMenu(myMenus[i], 0);
	DrawMenuBar();
}			

/* -----------------------------------------------------------------------------*/
static void ShowAbout()
{
	DialogPtr myDialog;	short Hit;				

	myDialog = GetNewDialog (aboutID, nil, (WindowRef)-1);
	if (myDialog) {
		ModalDialog(nil, &Hit);
		DisposeDialog(myDialog);
	} 
}
		
/* -----------------------------------------------------------------------------*/
static void DoCommand(long mResult)
{
	short	theItem, theMenu;

	theItem = LoWord(mResult);
	theMenu = HiWord(mResult);
	switch ( theMenu ) {
	
		case AppleID:
			if (theItem == 1) ShowAbout();
			break;

		case FileID: 
			switch (theItem) {
#if CustomRadio
				case OpenI: 	OpenRemote (dlogFilterProcPtr);
					break;
#endif
				case QuitI: 	doneFlag = true;
					break;
			} 
			break;
	}
	HiliteMenu(0);  
}
		
/* -----------------------------------------------------------------------------*/
static void AdjustMenus() 
{
}			

/* -----------------------------------------------------------------------------*/
static void DoMouseDown (EventRecord *event, WindowRef win) 
{
	WindowRef whichWindow; 
	
	switch ( FindWindow( event->where, &whichWindow ) )
	{ 
		case inMenuBar:
			AdjustMenus();
			DoCommand(MenuSelect(event->where));
			break;
		case inSysWindow: 
			break;
		case inGoAway: 	 
			break;
		case inDrag: 	 
			DragWindow(whichWindow, event->where, &dragRect);
			break;
		case inContent: 	 
			if (whichWindow != FrontWindow())
				SelectWindow(whichWindow);
			else if (whichWindow == win)
				DragWindow(whichWindow, event->where, &dragRect);
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

//_________________________________________________________________________________
static void InitState (StatePtr g)
{
	g->driverMode= false;
	g->showWindow= true;
	g->autoQuit= false;
	g->net.port = kDefaultWANPort;
	g->net.maxlat = kDefaultWANLatency;
	g->net.groupTime = kDefaultWANGroupTime;
}

/* -----------------------------------------------------------------------------*/
static OSErr AEQuitHandler (const AppleEvent *aevt, AppleEvent *reply, UInt32 refcon)
{
	doneFlag = true;
	return noErr;
}

/* -----------------------------------------------------------------------------*/
static unsigned char* C2PStr (char * str, Str255 buf)
{
	unsigned char n = 0;
	while (*str) {
		buf[++n] = *str++;
	}
	buf[0] = n;
	return buf;
}

/* -----------------------------------------------------------------------------*/
static void Initialize()
{
	char * msg; Str255 buf;
	InitCursor(); 
	FlushEvents(everyEvent, 0);
	AEInstallEventHandler ( kCoreEventClass, kAEQuitApplication, 
							NewAEEventHandlerUPP((AEEventHandlerProcPtr)AEQuitHandler), 0, false);
	gResFile = CurResFile();
	InitState (&gState);
	if (!MidiShare()) 			FatalError ("\pMidiShare is required");
	if (MidiGetVersion() < 182) FatalError ("\prequire MidiShare version 1.82 or later");
#if CustomRadio
	dlogFilterProcPtr = NewModalFilterProc (DlogFilterProc);
#endif
	SetUpMenus();
	SetUpWindows();
	gFeedback = new RadioHDFeedback (myWindow);
	if (!gFeedback) FatalError ("\pMemory allocation failed");
	msg = SetUpDriver(&gState, gFeedback);
	if (msg) FatalError (C2PStr(msg, buf));
	else ShowWindow(myWindow);
	foreGround = true;
}

/*======================================== main loop =======================================*/
int main(int argc, char *argv[])
{
	Boolean b; char c;
	
	doneFlag = false;											
	Initialize();
	while (!doneFlag) {										/* Main Loop			*/
		b = WaitNextEvent(everyEvent, &myEvent, 5, nil);	/* no sleep, no mouseRg */
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
					DoMouseDown(&myEvent, myWindow);
					break;
			case updateEvt:
					BeginUpdate( (WindowRef)myEvent.message );
					SetPort    ( GetWindowPort((WindowRef)myEvent.message) );
					DrawDialog ( myDlog );
					gFeedback->DoRedraw();
					EndUpdate( (WindowRef)myEvent.message );
					break;
			case kHighLevelEvent:
				AEProcessAppleEvent (&myEvent);
				break;
		}
	}
	WritePos (profileName, myWindow);
	CloseInetDriver ();
	CloseAllWinds( FrontWindow() );
	delete gFeedback;
	return 0;
}






	

