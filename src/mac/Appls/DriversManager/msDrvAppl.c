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
#include <Lists.h>
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
#include "msDrivers.h"
#include "msRsrc.h"
#include "msDrvAppl.h"

/* Constants definition */
#define osEvent 				app4Evt
#define	resumeMask				1
#define	suspendResumeMessage	1

/* menu bar constants */
enum { AppleMenu, FileMenu, EditMenu, mBarSize };
enum { AppleID = 128, FileID, EditID };
enum { UndoI = 1, CutI = 3, CopyI, PasteI, ClearI };

/* dialog & alerts */
enum { windowID = 128, AlertID = 128, AboutID = 128, AboutPictID = 130 };

/* global variables	*/
WindowPtr	myWindow;

/* local variables	*/
static Boolean		doneFlag;
static SysEnvRec	gMac;
static Boolean		hasWNE;
static Boolean		foreGround;

static EventRecord	myEvent;
static Rect			dragRect;
static MenuHandle	myMenus[mBarSize];
static short		resFile;
static Rect 		ListRect[listSize];

static short 		lastDrv = -1, lastPort = -1;

static PicHandle 	titlePict, handlePict;
static Rect 		titleRect, handleRect;
static FontInfo 	fInf;

static RGBColor		busyPortColor 		= { 0, 0, 0xffff };
static RGBColor		freePortColor 		= { 0, 0, 0 };
static RGBColor		black 				= { 0, 0, 0 };
static RGBColor		white 				= { 0xffff, 0xffff, 0xffff };
static RGBColor		selectedPortColor 	= { 0xffff, 0, 0 };
static RGBColor		selectedSlotColor 	= { 0, 0xffff, 0 };

#define FontHeight(f) 	(f.ascent + f.descent + f.leading)

/* -----------------------------------------------------------------------------*/
static void DoRedraw 	(WindowPtr win);

/* -----------------------------------------------------------------------------*/
static Boolean TrackMouse (Point where, short modifiers)
{
	Boolean result = false;
	short i, port;

	GlobalToLocal(&where);
	port = PointToPort (myWindow, where);
	if (port >= 0) {
		SelectPort (port, port != portSelected);
		result = true;
	}	
	else for (i = 0; i < listSize; i++) {
		if (PtInRect(where, &ListRect[i])) {
			result = true;
			if (portSelected >= 0) {
				modifiers |= cmdKey;
		  		LClick (where, modifiers, theList[i]);
		  		if (!ChangeConnect (theList[i]))
					SelectPort (portSelected, false);
		  	}
		  	else {
				modifiers = 0;
		  		LClick (where, modifiers, theList[i]);
		  		ListPorts (theList[i], true);
		  	}
		  	break;
		}
	}
	return result;
}


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
static void SetUpWindows()
{
	Rect view, bounds; Cell cell = { 0, 0};
	short w, i; Rect *r; Point p;
	
	SetRect (&dragRect, qd.screenBits.bounds.left, qd.screenBits.bounds.top, 
						qd.screenBits.bounds.right,qd.screenBits.bounds.bottom);
	InsetRect (&dragRect, 4, 4);
  	myWindow = GetNewWindow(windowID, NULL, (WindowPtr)-1);
  	if (myWindow) {
		SetPort(myWindow);

		titlePict 	= GetPicture(128);
		handlePict 	= GetPicture(129);
		TextSize(10);
		GetFontInfo(&fInf);
		SetRect(&bounds, 0, 0, 1, 0);

		for (i = 0; i < listSize; i++) {
			r = &ListRect[i];
			r->top = 30;
			r->left = i ? kPortLeft + kPortArea + kEdgeWidth : 5;
			r->bottom = r->top + FontHeight(fInf) * 14;
			r->right = r->left + kListWidth;
			view = ListRect[i];
			view.right -= 15;
			InsetRect(&ListRect[i], -1, -1);
			theList[i] = LNew(&view, &bounds, cell, 0, myWindow, true, false, false, true);
		}

		SizeWindow (myWindow, ListRect[DstL].right + 5, ListRect[DstL].bottom + 10, false);
		w = myWindow->portRect.right - myWindow->portRect.left;
		titleRect = (*titlePict)->picFrame;
		handleRect = (*handlePict)->picFrame;
		OffsetRect (&titleRect, 4, 4);
		OffsetRect (&handleRect, 4, myWindow->portRect.bottom - 10);
		if (ReadPos (&p)) MoveWindow (myWindow, p.h, p.v, false);
		AdjustLocation (myWindow);
		ShowWindow (myWindow);
		chgDriver 	= true;
	}
}

/* -----------------------------------------------------------------------------*/
static void AdjustMenus() 
{
	DisableItem( myMenus[EditMenu], UndoI );
	DisableItem( myMenus[EditMenu], CutI );
	DisableItem( myMenus[EditMenu], CopyI );
	DisableItem( myMenus[EditMenu], PasteI );
	DisableItem( myMenus[EditMenu], ClearI );
}			
		
/* -----------------------------------------------------------------------------*/
static void SetUpMenus()
{
	short	i;
		
	myMenus[AppleMenu] = GetMenu(AppleID);
	AppendResMenu(myMenus[AppleMenu],'DRVR');
	myMenus[FileMenu] = GetMenu(FileID);
	myMenus[EditMenu] = GetMenu(EditID);
	for (i = 0; i < mBarSize; i++)
		InsertMenu(myMenus[i], 0);
	DrawMenuBar();
}			

/* -----------------------------------------------------------------------------*/
static void ClearInfoString (WindowPtr win)
{
	short w = win->portRect.right - win->portRect.left;
	short rw = fInf.widMax * 12;
	short top = win->portRect.bottom - FontHeight(fInf) - 2;
	short left = (w - rw) / 2;
	Rect r;
	SetRect (&r, left, top, left + rw, win->portRect.bottom - 2);
	EraseRect (&r);
}

/* -----------------------------------------------------------------------------*/
static void DrawInfoString (WindowPtr win, Str255 str)
{
	short w = win->portRect.right - win->portRect.left;
	short tw = TextWidth(str, 1, str[0]);	
	MoveTo ((w - tw) / 2, win->portRect.bottom - 4);
	ClearInfoString (win);
	DrawString (str);
}

/* -----------------------------------------------------------------------------*/
static unsigned char * pStrCat (Str255 s1, Str255 s2, Str255 dst)
{
	short i, l1=*s1++, l2=*s2++;
	unsigned char * ret = dst;
	*dst++ = l1 + l2;
	for (i = 0; i < l1; i++)
		*dst++ = *s1++;
	for (i = 0; i < l2; i++)
		*dst++ = *s2++;
	return ret;
}

/* -----------------------------------------------------------------------------*/
static void ShowPort (WindowPtr win, short portNum)
{
	Str255 clear = "\p         ";
	if (portNum == lastPort) return;

	if (portNum >= 0) { 
		Str255 dst, num;
		NumToString (portNum, num);
		DrawInfoString (win, pStrCat ("\pport ", num, dst));
	}
	else ClearInfoString (win);
	lastPort = portNum;
}

/* -----------------------------------------------------------------------------*/
static void ShowDriver (WindowPtr win, Point p, ListHandle list)
{
	short index = PointToDrvIndex (win, p, list);
	if (index == lastDrv) return;
	
	if (index > 0) {
		MidiName name; short ref;
		ref = MidiGetIndDriver (index);
		if (ref > 0) {
			name = MidiGetName (ref);
			DrawInfoString (win, name);
		}
	}
	else ClearInfoString (win);
	lastDrv = index;
}

/* -----------------------------------------------------------------------------*/
static void TrackMouseMove (WindowPtr win)
{
	if (foreGround) {
		Point p; GrafPtr graphPort;
		short port;

		GetPort(&graphPort);
		SetPort (win);
		GetMouse (&p);
		port = PointToPort (win, p);
		if (port >= 0) {
			ShowPort (win, port);
		}
		else if (PtInRect(p, &ListRect[SrcL])) {
			ShowDriver (win, p, theList[SrcL]);
		}
		else if (PtInRect(p, &ListRect[DstL])) {
			ShowDriver (win, p, theList[DstL]);
		}
		else {
			ClearInfoString (win);
			lastDrv = lastPort = -1;
		}
		SetPort (graphPort);
	}
}

/* -----------------------------------------------------------------------------*/
static void DoIdle()
{
	if (chgDriver) {  	
		chgDriver = false;
 		FillLists();
 		InvalRect (&myWindow->portRect);
 		DoRedraw (myWindow);					
	}
	if (chgCon) {
		chgCon = false;
		RefreshPortMap ();
		if (portSelected >= 0)
			ShowSrcDest (portSelected);
	}
	TrackMouseMove (myWindow);
}

/* -----------------------------------------------------------------------------*/
static void ShowAbout()
{										
	DialogPtr myDialog;
	short Hit;

	myDialog = GetNewDialog(AboutID, NULL, (void *)(-1));
	if (myDialog) {
		ModalDialog(NULL, &Hit);
		DisposeDialog(myDialog);
	} else {
		SysBeep(30);
	}
}
		
/* -----------------------------------------------------------------------------*/
static void DoCommand(long mResult)
{
	short	theItem, theMenu;
	Str255	name;

	theItem = LoWord(mResult);
	theMenu = HiWord(mResult);
	
	switch ( theMenu )
	{
		case AppleID:
			if (theItem != 1) {
				GetMenuItemText(myMenus[AppleMenu], theItem, name);
				theItem = OpenDeskAcc(name);
			} else {
				ShowAbout();
			}
			break;

		case FileID: 
			doneFlag = true;
			break;

		case EditID:
			SystemEdit (theItem - 1);
			break;
	}
	HiliteMenu(0);  
}

/* -----------------------------------------------------------------------------*/
static void ResizeWind (Point where)
{
	Point aPt;
	short i, h, f, v, bottom, width;
	Rect aRect;

	SetPort(myWindow);
	aPt = where;
	GlobalToLocal(&aPt);
	if (aPt.v < myWindow->portRect.bottom - 10)
		return;

	f = FontHeight(fInf);
	width = myWindow->portRect.right - myWindow->portRect.left;
	SetRect (&aRect, width, f * 4 + 41, width, dragRect.bottom);
	bottom = myWindow->portRect.bottom;
	h = bottom - myWindow->portRect.top;
	v = ((unsigned long)GrowWindow (myWindow, where, &aRect)) >> 16;
	if (v < kMinHeight) v = kMinHeight;
	v = f * ((v - 41) / f) + 41;
	if (v == h) return;

	SizeWindow (myWindow, width, v, true);
	bottom = myWindow->portRect.bottom;
	SetRect(&aRect, 0, 30, width, bottom);
	EraseRect (&aRect);
	SetRect(&handleRect, 4, bottom - 10, 421, bottom);

	for (i = 0; i < listSize; i++) {
		ListRect[i].bottom = bottom - 10;
		LSetDrawingMode (false, theList[i]);
		LSize (110, bottom - 41, theList[i]);
		LSetDrawingMode (true, theList[i]);
	}
	InvalRect (&aRect);
}

/* -----------------------------------------------------------------------------*/
static void DoMouseDown() 
{
	WindowPtr whichWindow;
	
	switch ( FindWindow( myEvent.where, &whichWindow ) )
	{ 
		case inMenuBar:
			AdjustMenus ();
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
			else if (whichWindow == myWindow) {
				if (!TrackMouse (myEvent.where, myEvent.modifiers))
					ResizeWind (myEvent.where);
			}
			break;
	}
}

/* -----------------------------------------------------------------------------*/
static void DrawPorts (WindowPtr win) 
{
	short i, j, top, port = 0; Rect r;
	
	top = topPortArea(win);
	SetRect (&r, kPortLeft, top, kPortLeft + kPortWidth, top + kPortWidth);
	for (i=0; i<16; i++) {
		for (j=0; j<16; j++) {
			EraseRect (&r);
			if (port == portSelected) {
				RGBForeColor (&selectedPortColor);
				FillRect (&r, &qd.black);
				RGBForeColor (&black);
				FrameRect (&r);
			}
			else if (IsAcceptedBit(SelectedSlotMap, port)) {
				RGBForeColor (&selectedSlotColor);
				FillRect (&r, &qd.black);
				RGBForeColor (&black);
				FrameRect (&r);
			}
			else if (Busy(port)) {
				RGBForeColor (&busyPortColor);
				FrameRect (&r);
				InsetRect (&r, 1, 1);
				FrameRect (&r);
				InsetRect (&r, -1, -1);
			}
			else {
				RGBForeColor (&freePortColor);
				FrameRect (&r);
			}
			r.left += kPortWidth + kPortSep;
			r.right = r.left + kPortWidth;
			port++;
		}
		r.left = kPortLeft;
		r.right = r.left + kPortWidth;
		r.top += kPortWidth + kPortSep;
		r.bottom = r.top + kPortWidth;
	}
	RGBForeColor (&black);
}

/* -----------------------------------------------------------------------------*/
void ShowEditMode (WindowPtr win, Boolean on) 
{
	Rect r; RGBColor *frameColor = on ? &selectedPortColor : &white;
	r = win->portRect;
	RGBForeColor (frameColor);
	FrameRect (&r);
	InsetRect (&r, 1, 1);
	FrameRect (&r);
	RGBForeColor (&black);
}

/* -----------------------------------------------------------------------------*/
static void DoRedraw (WindowPtr win) 
{
	short i;
	BeginUpdate (win);
	if (!EmptyRgn (win->visRgn)) {
		SetPort (win);
		DrawPicture (titlePict, &titleRect);
		for (i = 0; i < listSize; i++) {
			LUpdate (win->visRgn, theList[i]);
			FrameRect (&ListRect[i]);
		}
		DrawPorts (win);
		DrawPicture (handlePict, &handleRect);
		ShowEditMode (win, (portSelected >= 0));
	}
	EndUpdate(win);
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

	if (!MidiShare()) 	AlertUser ("\pMidiShare is required");
	if (MidiGetVersion() < 180) AlertUser ("\prequire MidiShare version 1.80 or later");
	if (!SetupMidi()) 	AlertUser ("\pMidiShare setup failed");
	foreGround = true;
	resFile = CurResFile();
	SetUpMenus();
	SetUpWindows();
}

/*======================================== main loop =======================================*/
void main()
{
	Boolean b; char c, i;
	
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
							if (myWindow == FrontWindow()) {
	  							for (i = 0; i < listSize; i++)
	   								LActivate(foreGround, theList[i]);
	  						}
							break;
					}
					break;
			case keyDown:
			case autoKey:
					c = myEvent.message & charCodeMask;
					if (myEvent.modifiers & cmdKey )
						DoCommand (MenuKey(c));
					else if ((c >= kLeftArrowCharCode) || (c <= kDownArrowCharCode))
	    				MoveSelection (c);
					break;
			case mouseDown:
					DoMouseDown();
					break;
			case updateEvt:
					if (IsAppWindow ((WindowPtr)myEvent.message))
						DoRedraw ((WindowPtr)myEvent.message);
					break;
			case kHighLevelEvent:
				AEProcessAppleEvent (&myEvent);
				break;
		}
	}
	CloseMSConnect ();
	WritePos (myWindow, resFile);
	CloseAllWinds( FrontWindow() );
}






	

