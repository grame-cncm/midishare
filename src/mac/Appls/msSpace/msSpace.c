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
#include <Devices.h>
#include <Events.h>
#include <Files.h>
#include <Fonts.h>
#include <Lists.h>
#include <Memory.h>
#include <Menus.h>
#include <OSUtils.h>
#include <Packages.h>
#include <Patches.h>
#include <QuickDraw.h>
#include <Resources.h>
#include <SegLoad.h>
#include <Sound.h>
#include <TextEdit.h>
#include <TextUtils.h>
#include <ToolUtils.h>
#include <Traps.h>
#include <Types.h>
#include <Windows.h>

#include "MidiShare.h"

/* Constants definition */
#define osEvent 				app4Evt
#define	resumeMask				1
#define	suspendResumeMessage	1

/* menu bar constants */
#define mBarSize	2
enum { AppleMenu, FileMenu };
enum { AppleID = 128, FileID };

/* dialog & alerts */
enum { windowID = 128, AboutID, AlertID = 128 };

/* high level event structure */
typedef struct AppleEventRecord {
	short 	what;
	OSType 	evtClass;
	long 	when;
	OSType 	evtID;
	short 	modifiers;
} AppleEventRecord;

/* Application name	*/
#define ApplName 		"\pmsSpace"

/* global variables	*/
WindowPtr	myWindow;
Boolean		doneFlag;

SysEnvRec	gMac;
Boolean		hasWNE;
Boolean		foreGround;

EventRecord	myEvent;
MenuHandle	myMenus[mBarSize];

static Rect dragRect;
static Rect closeRect;
static Rect inCloseRect, eventsRect;
static Rect freeRect;
static Rect allocRect;
static Rect heapRect; 

static long MSheap;   	/* desired events */
static long MSnbr;   	/* allocated events */
static long MSfree;   	/* free events  */

#define MSanchor        0xb8


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

/* -----------------------------------------------------------------------------*/
static long MidiHeapSpace(void)
{
#ifndef __POWERPC__
	if (MidiGetVersion() < 170) {
		return (*(long *)(*(long *)MSanchor - 8));
	}
#endif
	return MidiDesiredSpace();
}

/* -----------------------------------------------------------------------------*/
static void DrawCloseBox (Boolean draw)
{
	SetPort(myWindow);
	if (draw) {
		FrameRect (&closeRect);
		FrameRect (&inCloseRect);
	} else {
		EraseRect (&closeRect);
	}
}

/* -----------------------------------------------------------------------------*/
static void DrawHeap(void)
{
	Str255 str;

	EraseRect (&heapRect);
	NumToString (MSheap, str);
	MoveTo (heapRect.right - StringWidth(str), heapRect.bottom);
	DrawString (str);
}

static void DrawAllocated(void)
{
	Str255 str;

	EraseRect (&allocRect);
	NumToString (MSnbr, str);
	MoveTo (allocRect.right - StringWidth(str), allocRect.bottom);
	DrawString (str);
}

static void DrawFree (void)
{
	Str255 str;
	Rect aRect;
	short notfree, right = eventsRect.right, left = eventsRect.left;
	short top = eventsRect.top, bottom = eventsRect.bottom;

	EraseRect (&freeRect);
	NumToString (MSfree, str);
	MoveTo (freeRect.right - StringWidth(str), freeRect.bottom);
	DrawString (str);
	EraseRect (&eventsRect);
	FrameRect (&eventsRect);
	if (MSfree != MSnbr) {
		notfree = (long)((right - left - 1) * ((double)MSfree / MSheap) + 0.5);
		SetRect(&aRect, left + 1, top + 1, right - notfree - 1, bottom - 1);
		FillRect (&aRect, &qd.gray);
	}
	if (MSheap == MSnbr)
		return;
	notfree = (long)((right - left - 1) * ((double)MSnbr / MSheap) + 0.5);
	SetRect (&aRect, left + 1, top + 1, right - notfree - 1, bottom - 1);
	PaintRect (&aRect);
}

/* -----------------------------------------------------------------------------*/
static void DoIdle (void)
{
	long temp;

	SetPort(myWindow);
	temp = MidiHeapSpace();
	if (temp != MSheap) {
		MSheap = temp;
		DrawHeap();
	}
	temp = MidiTotalSpace();
	if (temp != MSnbr) {
		MSnbr = temp;
		DrawAllocated();
	}
	temp = MSfree;
	MSfree = MidiFreeSpace();
	if (temp != MSfree)
		DrawFree ();
}

/* -----------------------------------------------------------------------------*/
static void DoRedraw(void)
{
	SetPort(myWindow);
	UpdateDialog (myWindow, myWindow->visRgn);
	DrawCloseBox(foreGround);
	DrawHeap();
	DrawAllocated();
	DrawFree();
}

/* -----------------------------------------------------------------------------*/
static Boolean DragClose (void)
{
	Point apt;
	Boolean inRect = true, hilite = false;
	Boolean stop;

	do {
		hilite = inRect;
		InvertRect (&inCloseRect);
		do {
			stop = !StillDown ();
			GetMouse (&apt);
			inRect = PtInRect (apt, &closeRect);
		} while (!(inRect != hilite || stop));
	} while (!stop);
	if (hilite) InvertRect(&inCloseRect);
	return hilite;
}

/* -----------------------------------------------------------------------------*/
static void SetUpWindows()
{
	SetRect (&dragRect, qd.screenBits.bounds.left, qd.screenBits.bounds.top, 
						qd.screenBits.bounds.right,qd.screenBits.bounds.bottom);
	InsetRect (&dragRect, 4, 4);
 	myWindow = GetNewDialog (windowID, nil, (WindowPtr) -1);
	SetRect (&closeRect, 4, 4, 14, 14);
	SetRect (&inCloseRect, 6, 6, 12, 12);
	SetRect (&eventsRect, 18, 4, myWindow->portRect.right - 12, 14);
	SetRect (&freeRect, 4, 18, 47, 26);
	SetRect (&allocRect, 75, 18, 120, 26);
	SetRect (&heapRect, 152, 18, myWindow->portRect.right - 12, 26);
 	SetPort (myWindow);
	TextFont (kFontIDGeneva);
	TextSize (9);
	PenPat (&qd.black);
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
	short hit;

	myDialog = GetNewDialog (AboutID, NULL, (WindowPtr)(-1));
	if (myDialog != NULL) {
		ModalDialog (NULL, &hit);
		DisposeDialog (myDialog);
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
static void ClickContent (EventRecord * event)
{
	Point thePt;

	SetPort (myWindow);
	thePt = event->where;
	GlobalToLocal (&thePt);
	if (PtInRect (thePt, &closeRect)) {
		if (DragClose ())
			doneFlag = true;
	} else DragWindow (myWindow, event->where, &dragRect);
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
					ClickContent (&myEvent);
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
	foreGround = true;
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
							DoRedraw ();
						}
						EndUpdate( (WindowPtr)myEvent.message );
					}
					break;
			case kHighLevelEvent:
				AEProcessAppleEvent (&myEvent);
				break;
		}
	}
	CloseAllWinds( FrontWindow() );
}






	

