/*

  Copyright � Grame 2001

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
WindowRef	myWindow = 0;
Boolean		doneFlag;
State 		gState;
short 		gResFile;

SysEnvRec	gMac;
Boolean		hasWNE;
Boolean		foreGround;

EventRecord	myEvent;
Rect		dragRect;
MenuHandle	myMenus[mBarSize];

void AlertUser (unsigned char * s, unsigned char *msg );
static void DoMouseDown (EventRecord *event, WindowRef win);


/********************************************************************************/
/* 								Macintosh	 utilities							*/
/* 																				*/
/********************************************************************************/
void AlertUser (unsigned char * s, unsigned char *msg )
{
	ResetAlertStage ();
	ParamText(s, strApplName, msg, "\p");
	Alert (AlertID,nil);
}

static void FatalError( unsigned char *msg )
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

//_______________________________________________________________________________
static void AdjustLocation (WindowRef win)
{
	Point pt; Rect portRect, r;
	
	GetWindowPortBounds(win, &portRect);
	pt.h = portRect.left + 6;	// 6 is enough to grab
	pt.v = portRect.top + 6;	// 6 is enough to grab
	LocalToGlobal(&pt);
	
	GetRegionBounds (GetGrayRgn(), &r);
	if (!PtInRect( pt, &r)) {
		MoveWindow (win, r.left + 50, r.top  + 50, false);
	}
}

/* -----------------------------------------------------------------------------*/
static void SetUpWindows()
{
	Rect bounds; Point p;

	GetRegionBounds (GetGrayRgn(), &bounds);
	dragRect = bounds;
	InsetRect (&dragRect, 4, 4);
  	myDlog = GetNewDialog (windowID, NULL, (WindowRef)-1);
  	if (!myDlog) return;

 	myWindow = GetDialogWindow(myDlog);
  	SetPort(GetWindowPort(myWindow));
 	TextFont(kFontIDGeneva); TextSize(9);
	AdjustLocation (myWindow);
	ShowWindow(myWindow);
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

//_________________________________________________________________________________
static void SaveLocation (DialogPtr dlog)
{
/*	Handle	h1;
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
*/
}
		
/* -----------------------------------------------------------------------------*/
static long GetItemValue (DialogPtr dialog, short item)
{
	short iType; Handle iHandle; Rect r; Str255 text;
	long value;
	
	GetDialogItem (dialog, item, &iType, &iHandle, &r);
	GetDialogItemText (iHandle, text);
	StringToNum (text, &value);
	return value;
}
		
/* -----------------------------------------------------------------------------*/
static void SetItemValue (DialogPtr dialog, short item, long value)
{
	short iType; Handle iHandle; Rect r; Str255 text;
	
	GetDialogItem (dialog, item, &iType, &iHandle, &r);
	NumToString (value, text);
	SetDialogItemText (iHandle, text);
}
		
/* -----------------------------------------------------------------------------*/
static void DoCommand(long mResult)
{
	short	theItem, theMenu;
	Str255	name;

	theItem = LoWord(mResult);
	theMenu = HiWord(mResult);
	
	switch ( theMenu ) {
	
		case AppleID:
			if (theItem != 1) {
				GetMenuItemText(myMenus[AppleMenu], theItem, name);
				theItem = OpenDeskAcc(name);
			} else {
				ShowAbout();
			}
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

//_________________________________________________________________________________
static void SaveState ()
{
	if( myWindow)
//		SaveLocation( myWindow);
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
	gResFile = CurResFile();
	InitState (&gState);
//	if (!CheckOT()) 			FatalError ("\pOpen Transport TCP 1.1 is required");
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
	if (msg) FatalError (msg);
	foreGround = true;
}

/*======================================== main loop =======================================*/
void main()
{
	Boolean b; char c;
	
	Initialize();
		doneFlag = false;											
	while (!doneFlag) {											/* Main Loop				*/
		b = WaitNextEvent(everyEvent, &myEvent, 0, nil);	/* no sleep, no mouseRgn	*/
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
					if ( IsAppWindow( (WindowRef)myEvent.message ) ) {
						BeginUpdate( (WindowRef)myEvent.message );
						if ( !EmptyRgn( ((WindowRef)myEvent.message)->visRgn ) ) {
							SetPort    ( (WindowRef)myEvent.message );
							DrawDialog ( (WindowRef)myEvent.message );
							gFeedback->DoRedraw();
						}
						EndUpdate( (WindowRef)myEvent.message );
					}
					break;
			case kHighLevelEvent:
				AEProcessAppleEvent (&myEvent);
				break;
		}
	}
	delete gFeedback;
	SaveState ();
	CloseInetDriver ();
	CloseAllWinds( FrontWindow() );
}






	

