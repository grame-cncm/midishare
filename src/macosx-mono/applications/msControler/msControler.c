/*

  Copyright © Grame 2002

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
  grame@grame.fr

*/

#include <Carbon/Carbon.h>
#include <unistd.h>

#include "../lib/states.h"
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

/* Application name	and state */
#define ApplName 		"\pmsControler"
#define CApplName 		"msControler"
static char * profileName 		= "msControler.ini";
static char * StateSectionName	= "Application state";
static char * portEntryName		= "port";
static char * chanEntryName		= "chan";
static char * progChgeEntryName	= "progChge";
static char * volEntryName		= "vol";
static char * modulationEntryName  = "modulation";
static char * panEntryName		= "pan";
static char * pitchEntryName	= "pitch";
static char * velEntryName		= "vel";

/* global variables	*/
WindowRef	myWindow;
DialogRef	myDlog;
Handle 		textItem[kCtrlCount];
short 		ctrlValue[kCtrlCount];
short 		ctrlMax[kCtrlCount];
short 		ctrlMin[kCtrlCount];
Boolean		doneFlag;
ControlRef cTracked;
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
static pascal void ThumbAction (ControlRef , short);


/* -----------------------------------------------------------------------------*/
static pascal void RcvAlarm (short refnum)
{
	MidiFlushEvs (refnum);
}

/* -----------------------------------------------------------------------------*/
static Boolean SetupMidi(void)
{
	myRefNum = MidiOpen (CApplName);
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

//_______________________________________________________________________________
static void ReadState ()
{
	CurrentPort = ReadIntState (profileName, StateSectionName, portEntryName, 0);
	CurrentChan = ReadIntState (profileName, StateSectionName, chanEntryName, 0);
	CurrentProg = ReadIntState(profileName, StateSectionName, progChgeEntryName, 0);
	CurrentVol  = ReadIntState (profileName, StateSectionName, volEntryName, 127);
	CurrentMod  = ReadIntState (profileName, StateSectionName, modulationEntryName, 0);
	CurrentPan  = ReadIntState (profileName, StateSectionName, panEntryName, 64);
	CurrentPitch= ReadIntState (profileName, StateSectionName, pitchEntryName, 60);
	CurrentVel  = ReadIntState (profileName, StateSectionName, velEntryName, 100);
}

//_______________________________________________________________________________
static void WriteState ()
{
	WriteIntState (profileName, StateSectionName, portEntryName, CurrentPort);
	WriteIntState (profileName, StateSectionName, chanEntryName, CurrentChan);
	WriteIntState (profileName, StateSectionName, progChgeEntryName, CurrentProg);
	WriteIntState (profileName, StateSectionName, volEntryName, CurrentVol);
	WriteIntState (profileName, StateSectionName, modulationEntryName, CurrentMod);
	WriteIntState (profileName, StateSectionName, panEntryName, CurrentPan);
	WriteIntState (profileName, StateSectionName, pitchEntryName, CurrentPitch);
	WriteIntState (profileName, StateSectionName, velEntryName, CurrentVel);
}

/* -----------------------------------------------------------------------------*/
static void InitControlers (DialogRef dlg)
{
	short type, i; Handle item = 0; Rect r;
	
	ReadState ();
	thumbActionPtr = NewControlActionUPP (ThumbAction);
	for (i=kFirstCtrl; i < kCtrlEnd; i++) {
		GetDialogItem (dlg, i, &type, &item, &r);
		GetDialogItem (dlg, i+kTextOffset, &type, &textItem[i-1], &r);
		if (item) {
			ControlRef ctrl = (ControlRef)item;
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
	Rect bounds; Point p;

	GetRegionBounds (GetGrayRgn(), &bounds);
	dragRect = bounds;
	InsetRect (&dragRect, 4, 4);
  	myDlog = GetNewDialog (windowID, NULL, (WindowRef)-1);
  	if (!myDlog) return;
	
  	myWindow = GetDialogWindow(myDlog);
  	SetPort(GetWindowPort(myWindow));
  	TextFont (kFontIDGeneva);
  	TextSize (9);
	InitControlers (myDlog);
	if (ReadPos (profileName, &p))
		MoveWindow (myWindow, p.h, p.v, false);
	AdjustLocation (myWindow);
	ShowWindow (myWindow);
}

/* -----------------------------------------------------------------------------*/
static void SetUpMenus()
{
	short	i;
	myMenus[AppleMenu] = GetMenu(AppleID);
	myMenus[FileMenu] = GetMenu(FileID);
	for (i = 0; i < mBarSize; i++)
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
	} else {
		SysBeep(30);
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
			if (theItem == 1)
				ShowAbout();
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
static void SetActionValue (ControlRef control, short action, short value)
{
	short i = action - 1;
	if (value == ctrlValue[i]) return;

	ctrlValue[i] = value;
	SendMsg (action);
	SetControlValue (control, ctrlMax[i] - value);
	ShowControlValue (i, value);
}

/* -----------------------------------------------------------------------------*/
static void ValueOffset (ControlRef control, short action, short chge)
{
	short i = action - 1;
	short newVal = ctrlValue[i] + chge;
	if (newVal > ctrlMax[i]) newVal = ctrlMax[i];
	else if (newVal < ctrlMin[i]) newVal = ctrlMin[i];
	SetActionValue (control, action, newVal);
}

/* -----------------------------------------------------------------------------*/
static pascal void ThumbAction (ControlRef ref, short unused)
{
	Point p;
	long v, h; short i, amp, val; Rect bounds;

	GetMouse( &p);
	i = GetControlReference (cTracked) - 1;
	GetControlBounds (cTracked, &bounds);
	v = p.v - bounds.top - 16;
	h = bounds.bottom - bounds.top - 64;
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
static void TrackDialog (WindowRef w)
{
	ControlRef control; Point p;
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
	usleep (time * 1000);
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
					Wait (50);
				}
			}
			break;
	}
}

/* -----------------------------------------------------------------------------*/
static void DoRedraw (WindowRef win) 
{
	GrafPtr curPort;
	GetPort (&curPort);
	SetPort (GetWindowPort(win));
	BeginUpdate (win);
	DrawDialog (myDlog);
	EndUpdate(win);
	SetPort (curPort);
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
	if (!SetupMidi()) 	AlertUser ("\pMidiShare setup failed");
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
	while (!doneFlag) {										/* Main Loop				*/
		b = WaitNextEvent(everyEvent, &myEvent, 32000, nil);/* no sleep, no mouseRgn	*/
		AdjustCursor();											
		switch( myEvent.what ) {
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
					DoRedraw ((WindowRef)myEvent.message);
					break;
			case kHighLevelEvent:
				AEProcessAppleEvent (&myEvent);
				break;
		}
	}
	MidiClose (myRefNum);
	WritePos (profileName, myWindow);
	WriteState ();
	CloseAllWinds( FrontWindow() );
	return 0;
}
