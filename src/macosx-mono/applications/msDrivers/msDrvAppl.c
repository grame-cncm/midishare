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
#include "msDrivers.h"
#include "msDrvAppl.h"
#include "../lib/states.h"

static char * profileName = "msDrivers.ini";

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
WindowRef	myWindow;

/* local variables	*/
static Boolean		doneFlag;
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
static void DoRedraw 	(WindowRef win);

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
	Rect view, bounds; Cell cell = { 0, 0};
	short /*w,*/ i; Rect *r; Point p; CGrafPtr port;

	GetRegionBounds (GetGrayRgn(), &bounds);
	dragRect = bounds;
	InsetRect (&dragRect, 4, 4);
  	myWindow = GetNewWindow(windowID, NULL, (WindowRef)-1);
  	if (myWindow) {
		port = GetWindowPort(myWindow);
		SetPort(port);

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
		titleRect = (*titlePict)->picFrame;
		handleRect = (*handlePict)->picFrame;
		OffsetRect (&titleRect, 4, 4);
		OffsetRect (&handleRect, 4, GetPortBounds(port, &bounds)->bottom - 10);
		if (ReadPos (profileName, &p)) MoveWindow (myWindow, p.h, p.v, false);
		AdjustLocation (myWindow);
		ShowWindow (myWindow);
		chgDriver = true;
	}
}

/* -----------------------------------------------------------------------------*/
static void AdjustMenus() 
{
	DisableMenuItem( myMenus[EditMenu], UndoI );
	DisableMenuItem( myMenus[EditMenu], CutI );
	DisableMenuItem( myMenus[EditMenu], CopyI );
	DisableMenuItem( myMenus[EditMenu], PasteI );
	DisableMenuItem( myMenus[EditMenu], ClearI );
}			
		
/* -----------------------------------------------------------------------------*/
static void SetUpMenus()
{
	short	i;
		
	myMenus[AppleMenu] = GetMenu(AppleID);
	myMenus[FileMenu] = GetMenu(FileID);
	myMenus[EditMenu] = GetMenu(EditID);
	for (i = 0; i < mBarSize; i++)
		InsertMenu(myMenus[i], 0);
	DrawMenuBar();
}			

/* -----------------------------------------------------------------------------*/
static void ClearInfoString (WindowRef win)
{
	Rect portRect, r; short w, rw, top, left;
	GetWindowPortBounds(win, &portRect);
	w = portRect.right - portRect.left;
	rw = fInf.widMax * 12;
	top = portRect.bottom - FontHeight(fInf) - 2;
	left = (w - rw) / 2;
	SetRect (&r, left, top, left + rw, portRect.bottom - 2);
	EraseRect (&r);
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
static void DrawInfoString (WindowRef win, Str255 str)
{
	Rect portRect; short w, tw;
	
	CGrafPtr port = GetWindowPort(win);
	GetPortBounds(port, &portRect);
	w = portRect.right - portRect.left;
	tw = TextWidth(str, 1, str[0]);	
	MoveTo ((w - tw) / 2, portRect.bottom - 4);
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
static void ShowPort (WindowRef win, short portNum)
{
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
static void ShowDriver (WindowRef win, Point p, ListHandle list)
{
	short index = PointToDrvIndex (win, p, list);
	if (index == lastDrv) return;
	
	if (index > 0) {
		MidiName name; short ref;
		ref = MidiGetIndDriver (index);
		if (ref > 0) {
			Str255 buf;
			name = MidiGetName (ref);
			DrawInfoString (win, C2PStr(name, buf));
		}
	}
	else ClearInfoString (win);
	lastDrv = index;
}

/* -----------------------------------------------------------------------------*/
static void TrackMouseMove (WindowRef win)
{
	if (foreGround) {
		Point p; GrafPtr graphPort;
		short port;

		GetPort(&graphPort);
		SetPort (GetWindowPort(win));
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
		Rect portRect;
		chgDriver = false;
 		FillLists();
		GetPortBounds(GetWindowPort(myWindow), &portRect);
 		InvalWindowRect (myWindow, &portRect);
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
//				theItem = OpenDeskAcc(name);
			} else {
				ShowAbout();
			}
			break;

		case FileID: 
			doneFlag = true;
			break;

		case EditID:
//			SystemEdit (theItem - 1);
			break;
	}
	HiliteMenu(0);  
}

/* -----------------------------------------------------------------------------*/
static void ResizeWind (Point where)
{
	Rect portRect, aRect;
	Point aPt;
	short i, h, f, v, bottom, width;
	CGrafPtr port = GetWindowPort(myWindow);

	GetPortBounds(port, &portRect);
	SetPort(port);
	aPt = where;
	GlobalToLocal(&aPt);
	if (aPt.v < portRect.bottom - 10)
		return;

	f = FontHeight(fInf);
	width = portRect.right - portRect.left;
	SetRect (&aRect, width, f * 4 + 41, width, dragRect.bottom);
	bottom = portRect.bottom;
	h = bottom - portRect.top;
	v = ((unsigned long)GrowWindow (myWindow, where, &aRect)) >> 16;
	if (v < kMinHeight) v = kMinHeight;
	v = f * ((v - 41) / f) + 41;
	if (v == h) return;

	SizeWindow (myWindow, width, v, true);
	GetPortBounds(port, &portRect);
	bottom = portRect.bottom;
	SetRect(&aRect, 0, 30, width, bottom);
	EraseRect (&aRect);
	SetRect(&handleRect, 4, bottom - 10, 421, bottom);

	for (i = 0; i < listSize; i++) {
		ListRect[i].bottom = bottom - 10;
		LSetDrawingMode (false, theList[i]);
		LSize (110, bottom - 41, theList[i]);
		LSetDrawingMode (true, theList[i]);
	}
        InvalWindowRect (myWindow, &portRect);
        chgDriver = true;
}

/* -----------------------------------------------------------------------------*/
static void DoMouseDown() 
{
	WindowRef whichWindow;
	
	switch ( FindWindow( myEvent.where, &whichWindow ) )
	{ 
		case inMenuBar:
			AdjustMenus ();
			DoCommand(MenuSelect(myEvent.where));
			break;
		case inSysWindow: 
			break;
		case inGoAway: 	 
			doneFlag = TrackGoAway(whichWindow, myEvent.where);
			break;
		case inDrag:
			DragWindow(myWindow, myEvent.where, &dragRect);
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
static void DrawPorts (WindowRef win) 
{
	short i, j, top, port = 0; Rect r;
	Rect portRect; Pattern pblack;
	GetPortBounds(GetWindowPort(win), &portRect);
	
	top = topPortArea(portRect);
	GetQDGlobalsBlack (&pblack);
	SetRect (&r, kPortLeft, top, kPortLeft + kPortWidth, top + kPortWidth);
	for (i=0; i<16; i++) {
		for (j=0; j<16; j++) {
			EraseRect (&r);
			if (port == portSelected) {
				RGBForeColor (&selectedPortColor);
				FillRect (&r, &pblack);
				RGBForeColor (&black);
				FrameRect (&r);
			}
			else if (IsAcceptedBit(SelectedSlotMap, port)) {
				RGBForeColor (&selectedSlotColor);
				FillRect (&r, &pblack);
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
void ShowEditMode (WindowRef win, Boolean on) 
{
	Rect r; RGBColor *frameColor = on ? &selectedPortColor : &white;
	GetPortBounds(GetWindowPort(win), &r);
	RGBForeColor (frameColor);
	FrameRect (&r);
	InsetRect (&r, 1, 1);
	FrameRect (&r);
	RGBForeColor (&black);
}

/* -----------------------------------------------------------------------------*/
static void DoRedraw (WindowRef win) 
{
	short i; RgnHandle rgn = 0;
	GrafPtr curPort;

	rgn = NewRgn();
	if (rgn) GetWindowRegion (win, kWindowContentRgn, rgn);
	GetPort (&curPort);
	SetPort (GetWindowPort(win));
	BeginUpdate (win);
	DrawPicture (titlePict, &titleRect);
	for (i = 0; i < listSize; i++) {
		LUpdate (rgn, theList[i]);
		FrameRect (&ListRect[i]);
	}
	DrawPorts (win);
	DrawPicture (handlePict, &handleRect);
	ShowEditMode (win, (portSelected >= 0));
	EndUpdate(win);
	SetPort (curPort);
	if (rgn) DisposeRgn (rgn);
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

	if (!MidiShare()) 	AlertUser ("\pMidiShare is required");
	if (MidiGetVersion() < 180) AlertUser ("\prequire MidiShare version 1.80 or later");
	if (!SetupMidi()) 	AlertUser ("\pMidiShare setup failed");
	foreGround = true;
	resFile = CurResFile();
	SetUpMenus();
	SetUpWindows();
}

/*======================================== main loop =======================================*/
int main(int argc, char *argv[])
{
	Boolean b; char c, i;
	
	Initialize();
	doneFlag = false;											
	while (!doneFlag) {										/* Main Loop				*/
		b = WaitNextEvent(everyEvent, &myEvent, 20, nil);	/* no sleep, no mouseRgn	*/
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
					DoRedraw ((WindowRef)myEvent.message);
					break;
			case kHighLevelEvent:
				AEProcessAppleEvent (&myEvent);
				break;
		}
	}
	CloseMSConnect ();
	WritePos (profileName, myWindow);
	CloseAllWinds( FrontWindow() );
	return 0;
}






	

