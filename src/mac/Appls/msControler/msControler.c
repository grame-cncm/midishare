/*

  Copyright © Grame 2000

  This software is free software; you can redistribute it and modify it under 
  the terms of the GNU General Public License as published by the 
  Free Software Foundation version 2 of the License, or any later version.

  This software is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License 
  for more details.

  You should have received a copy of the GNU General Public License
  along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

  Grame Research Laboratory, 9, rue du Garet 69001 Lyon - France
  grame@rd.grame.fr

*/


#include <Controls.h>
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
#include <QuickDrawText.h>
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
enum { AppleMenu, FileMenu, mBarSize };
enum { AppleID = 128, FileID };

/* dialog & alerts */
enum { windowID = 128, AlertID = 500, AboutID = 129, AboutPictID = 128 };
/* controls */
enum { kFirstCtrl = 1, kTextOffset = 8,
	kPortCtrl = kFirstCtrl,  kChanCtrl, kProgCtrl, kVolCtrl, kModCtrl, 
	kPanCtrl, kPitchCtrl, kVelCtrl, kCtrlEnd, kCtrlCount = kCtrlEnd - kFirstCtrl };

/* Application name	*/
#define ApplName 		"\pmsControler"
enum { kWinPosRsrc = 'wipo', kWinposID = 128 };
enum { kStateRsrc  = 'stat', kStateID  = 128 };

/* global variables	*/
WindowPtr	myWindow;
Handle 		textItem[kCtrlCount];
short 		ctrlValue[kCtrlCount];
short 		ctrlMax[kCtrlCount];
short 		ctrlMin[kCtrlCount];
Boolean		doneFlag;
ControlHandle cTracked;
ControlActionUPP thumbActionPtr;

SysEnvRec	gMac;
Boolean		hasWNE;
Boolean		foreGround;

EventRecord	myEvent;
Rect		dragRect;
MenuHandle	myMenus[mBarSize];
Boolean 	chgAppl, chgCon;
short		myRefNum, resFile;

/* -----------------------------------------------------------------------------*/
/* macros */
#define CurrentPort		ctrlValue [kPortCtrl - kFirstCtrl]
#define CurrentChan		ctrlValue [kChanCtrl - kFirstCtrl]
#define CurrentPitch	ctrlValue [kPitchCtrl - kFirstCtrl]
#define CurrentVel		ctrlValue [kVelCtrl - kFirstCtrl]

#define CurrentProg		ctrlValue [kProgCtrl - kFirstCtrl]
#define CurrentMod		ctrlValue [kModCtrl - kFirstCtrl]
#define CurrentVol		ctrlValue [kVolCtrl - kFirstCtrl]
#define CurrentPan		ctrlValue [kPanCtrl - kFirstCtrl]


/* -----------------------------------------------------------------------------*/
/* prototypes */
static void ShowControlValue (short index, short value);
static pascal void ThumbAction (ControlHandle , short);


/* -----------------------------------------------------------------------------*/
static pascal void RcvAlarm (short refnum)
{
	MidiFlushEvs (refnum);
}

/* -----------------------------------------------------------------------------*/
static Boolean SetupMidi(void)
{
	myRefNum = MidiOpen (ApplName);
	if (myRefNum == MIDIerrSpace) return false;

	chgAppl = chgCon = false;
	MidiSetRcvAlarm (myRefNum, RcvAlarm);
	MidiConnect (myRefNum, 0, true);
	return true;
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

//_______________________________________________________________________________
static Boolean ReadState ()
{
	Handle h = GetResource (kStateRsrc, kStateID);
	if (h) {
		short * ptr = (short *)*h;
		short i;
		
		for (i=0; i< kCtrlCount; i++) {
			ctrlValue[i] = *ptr++;
		}
		return true;
	}
	return false;
}

//_______________________________________________________________________________
static void WriteState ()
{
	Handle h = NewHandle (kCtrlCount * sizeof(short));
	if (h) {
		short * ptr = (short *)*h;
		short i;
		
		for (i=0; i< kCtrlCount; i++) {
			*ptr++ = ctrlValue[i];
		}
		WriteRsrc (h, kStateRsrc, kStateID);
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
static void InitControlers (WindowPtr win)
{
	short type, i; Handle item = 0; Rect r;
	
	ReadState ();
	thumbActionPtr = NewControlActionProc (ThumbAction);
	for (i=kFirstCtrl; i < kCtrlEnd; i++) {
		GetDialogItem (win, i, &type, &item, &r);
		GetDialogItem (win, i+kTextOffset, &type, &textItem[i-1], &r);
		if (item) {
			ControlHandle ctrl = (ControlHandle)item;
			ctrlMax[i-1] = GetControlMaximum (ctrl);
			ctrlMin[i-1] = GetControlMinimum (ctrl);
			SetControlReference (ctrl, i);
			SetControlValue (ctrl, ctrlMax[i-1] - ctrlValue[i-1]);
			ShowControlValue (i-1, ctrlValue[i-1]);
		}
	}
}

/* -----------------------------------------------------------------------------*/
static void SetUpWindows()
{
	Point p;
	
	SetRect (&dragRect, qd.screenBits.bounds.left, qd.screenBits.bounds.top, 
						qd.screenBits.bounds.right,qd.screenBits.bounds.bottom);
	InsetRect (&dragRect, 4, 4);
  	myWindow = GetNewDialog (windowID, NULL, (WindowPtr)-1);
  	if (!myWindow) return;
  	
  	SetPort(myWindow);
  	TextFont (kFontIDGeneva);
  	TextSize (9);
  	InitControlers (myWindow);
	if (ReadPos (&p))
		MoveWindow (myWindow, p.h, p.v, false);
	AdjustLocation (myWindow);
	ShowWindow (myWindow);
}
		
/* -----------------------------------------------------------------------------*/
static void SetUpMenus()
{
	short	i;
		
	myMenus[AppleMenu] = GetMenu(AppleID);
	AppendResMenu(myMenus[AppleMenu],'DRVR');
	myMenus[FileMenu] = GetMenu(FileID);
	for (i = 0; i < mBarSize; i++)
		InsertMenu(myMenus[i], 0);
	DrawMenuBar();
}			

/* -----------------------------------------------------------------------------*/
static void DoIdle()
{
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
	}
	HiliteMenu(0);  
}

/* -----------------------------------------------------------------------------*/
static void SendNote ()
{
	MidiEvPtr e = MidiNewEv (typeNote);
	if (e) {
		Port(e) = CurrentPort;
		Chan(e) = CurrentChan;
		Pitch(e) = CurrentPitch;
		Vel(e) = CurrentVel;
		Dur(e) = 100;
		MidiSendIm (myRefNum, e);
	}
}

/* -----------------------------------------------------------------------------*/
static void SendProg ()
{
	MidiEvPtr e = MidiNewEv (typeProgChange);
	if (e) {
		Port(e) = CurrentPort;
		Chan(e) = CurrentChan;
		Data(e)[0] = CurrentProg;
		MidiSendIm (myRefNum, e);
		SendNote ();
	}
}

/* -----------------------------------------------------------------------------*/
static void SendCtrl (short num, short val)
{
	MidiEvPtr e = MidiNewEv (typeCtrlChange);
	if (e) {
		Port(e) = CurrentPort;
		Chan(e) = CurrentChan;
		Data(e)[0] = num;
		Data(e)[1] = val;
		MidiSendIm (myRefNum, e);
		SendNote ();
	}
}

/* -----------------------------------------------------------------------------*/
static void ShowControlValue (short index, short value)
{
	Str255 numStr; short i;
	NumToString (value, numStr);
	i = *numStr + 1;
	switch (i) {
		case 3:
			while (i--) numStr[i+1] = numStr[i];
			numStr[1] = ' ';
			numStr[0]++;
			break;
		case 2:
			while (i--) numStr[i+2] = numStr[i];
			numStr[1] = numStr[2] = ' ';
			numStr[0] += 2;
			break;
	}
	SetDialogItemText(textItem[index], numStr);
}

/* -----------------------------------------------------------------------------*/
static void SendMsg (short action)
{
	switch (action) {
		case kPortCtrl:
		case kChanCtrl:
		case kPitchCtrl:
		case kVelCtrl:		SendNote ();
			break;
		case kProgCtrl:		SendProg ();
			break;
		case kModCtrl:		SendCtrl (1, CurrentMod);
			break;
		case kVolCtrl:		SendCtrl (7, CurrentVol);
			break;
		case kPanCtrl:		SendCtrl (10, CurrentPan);
			break;
	}
}

/* -----------------------------------------------------------------------------*/
static void SetActionValue (ControlHandle control, short action, short value)
{
	short i = action - 1;
	if (value == ctrlValue[i]) return;

	ctrlValue[i] = value;
	SendMsg (action);
	SetControlValue (control, ctrlMax[i] - value);
	ShowControlValue (i, value);
}

/* -----------------------------------------------------------------------------*/
static void ValueOffset (ControlHandle control, short action, short chge)
{
	short i = action - 1;
	short newVal = ctrlValue[i] + chge;
	if (newVal > ctrlMax[i]) newVal = ctrlMax[i];
	else if (newVal < ctrlMin[i]) newVal = ctrlMin[i];
	SetActionValue (control, action, newVal);
}

/* -----------------------------------------------------------------------------*/
static pascal void ThumbAction (ControlHandle , short)
{
	Point p; ControlRecord * c = *cTracked;
	long v, h; short i, amp, val;

	GetMouse( &p);
	i = GetControlReference (cTracked) - 1;
	v = p.v - c->contrlRect.top - 16;
	h = c->contrlRect.bottom - c->contrlRect.top - 32;
	if (v < 0) val = ctrlMax[i];
	else if (v > h) val = ctrlMin[i];
	else {
		amp = ctrlMax[i] - ctrlMin[i];
		val =  ctrlMax[i] - (v * amp) / h;
	}
	if (val != ctrlValue[i]) {
		ctrlValue[i] = val;
		SendMsg (i+1);
		ShowControlValue (i, val);
	}
}

/* -----------------------------------------------------------------------------*/
static void TrackDialog (WindowPtr w)
{
	ControlHandle control; Point p;
	short part, action, chge = 0;

	GetMouse (&p);
	part = FindControl (p, w, &control);
	if (part) {
		action = GetControlReference (control);
		switch( part) {
			case kControlIndicatorPart:
				cTracked = control;
				TrackControl (control, p, thumbActionPtr);
				break;
			case kControlDownButtonPart:
				chge = -1;
				break;
			case kControlPageDownPart:
				chge = -10;
				break;
			case kControlUpButtonPart:
				chge = 1;
				break;
			case kControlPageUpPart:
				chge = 10;
				break;
		}
		if (chge) ValueOffset (control, action, chge);
		else if (cTracked) {
			short i = action - 1;
			SetControlValue (cTracked, ctrlMax[i] - ctrlValue[i]);
			cTracked = 0;
		}
	}
}

/* -----------------------------------------------------------------------------*/
static void Wait (long time) 
{
	long t = MidiGetTime() + time;
	while (MidiGetTime() < t) 
		;
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
			else if (whichWindow == myWindow) {
				TrackDialog (myWindow);
				Wait (250);
				while (StillDown()) {
					TrackDialog (myWindow);
					Wait (30);
				}
			}
			break;
	}
}

/* -----------------------------------------------------------------------------*/
static void DoRedraw (WindowPtr win) 
{
	SetPort (win);
	BeginUpdate (win);
	if (!EmptyRgn (win->visRgn)) {
		DrawDialog (win);
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
	if (!SetupMidi()) 	AlertUser ("\pMidiShare setup failed");
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
					if (myEvent.modifiers & cmdKey )
						DoCommand (MenuKey(c));
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
	MidiClose (myRefNum);
	WritePos ();
	WriteState ();
	CloseAllWinds( FrontWindow() );
}






	

