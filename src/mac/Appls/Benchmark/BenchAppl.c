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
#include "MidiBench.h"

/* Constants definition */
#define osEvent 				app4Evt
#define	resumeMask				1
#define	suspendResumeMessage	1

/* menu bar constants */
#define mBarSize	4
enum { AppleMenu, FileMenu, BenchMenu, SpeedMenu };
enum { AppleID = 128, FileID, BenchID, SpeedID };

/* dialog & alerts */
enum { dialogID = 129, AlertID = 500 };
enum { startItem = 1, messageItem = 3, elapsedItem = 4 };

/* Application name	*/
#define ApplName 		"\pMidiShare Drivers Loader"

/* global variables	*/
static WindowPtr	myWindow;
static Boolean		doneFlag;

static SysEnvRec	gMac;
static Boolean		hasWNE;
static Boolean		foreGround;

static EventRecord	myEvent;
static Rect		dragRect;
static MenuHandle	myMenus[mBarSize];

static short currentBench, currentGen;


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
static void timeToString(long t, Str255 s)
{
  char z = '0';

  s[0] = 12;
  s[3] = s[6] = s[9] = ':';
  s[12] = (unsigned char)(z + t % 10);
  t /= 10;
  s[11] = (unsigned char)(z + t % 10);
  t /= 10;
  s[10] = (unsigned char)(z + t % 10);
  t /= 10;
  s[8] = (unsigned char)(z + t % 10);
  t /= 10;
  s[7] = (unsigned char)(z + t % 6);
  t /= 6;

  s[5] = (unsigned char)(z + t % 10);
  t /= 10;
  s[4] = (unsigned char)(z + t % 6);
  t /= 6;

  s[2] = (unsigned char)(z + t % 10);
  t /= 10;
  s[1] = (unsigned char)(z + t % 10);
  t /= 10;
}

/* -----------------------------------------------------------------------------*/
static void HiliteDiaIogItem(DialogPtr dialog, short i, short state)
{
  short t; Handle h; Rect r;
  GetDialogItem(dialog, i, &t, &h, &r);
  if (h) HiliteControl((ControlHandle)h, state);
}

/* -----------------------------------------------------------------------------*/
static void SetText(DialogPtr dialog, short item, Str255 s)
{
  short t; Handle h; Rect r;

  GetDialogItem(dialog, item, &t, &h, &r);
  if (t < statText) {
    SetControlTitle ((ControlHandle)h, s);
  } else {
    SetDialogItemText (h, s);
  }
}

/* -----------------------------------------------------------------------------*/
static void DoBenchCommand (DialogPtr dialog, short item)
{
  long elapsed;
  Str255 stime;

  switch (item) {

	  case startItem:
	    HiliteDiaIogItem (dialog, item, 255);
	    elapsed = 0;
	    SetText(dialog, elapsedItem, "\p00:00:00.000");
	    elapsed = DoBench (currentBench);
	    timeToString(elapsed, stime);
	    SetText (dialog, elapsedItem, stime);
	    HiliteDiaIogItem(dialog, item, 0);
	    break;
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
	TextFont(kFontIDMonaco); TextSize(9); TextMode(srcCopy); PenNormal();
	ShowWindow(myWindow);
	HiliteDiaIogItem (myWindow, startItem, 255);
	SetText (myWindow, messageItem, "\pPlease wait, initialization in progress");
	DrawDialog(myWindow);
}
		
/* -----------------------------------------------------------------------------*/
static void SetUpMenus()
{
	short i;	
	for (i = AppleMenu; i <= SpeedID; i++) {
		myMenus[i] = GetMenu(AppleID+i);
		InsertMenu(myMenus[i], 0);
	}
	AppendResMenu(myMenus[AppleMenu],'DRVR');
	DrawMenuBar();
}			

/* -----------------------------------------------------------------------------*/
static void DoIdle()
{
}

/* -----------------------------------------------------------------------------*/
static void ShowAbout()
{										
}

/* -----------------------------------------------------------------------------*/
static short StringLen (unsigned char * s)
{
	short len = 0;
	while (*s++) len++;
	return len;
}

/* -----------------------------------------------------------------------------*/
static void SelectBench (short b)
{
	unsigned char * info;
  
	if (b == currentBench) return;
	if (!currentBench)
		HiliteDiaIogItem (myWindow, startItem, 0);
	info = (unsigned char *)GetBenchInfos (b);
	info[0] = StringLen (info);
	if (currentBench > 0) {
		CheckItem(myMenus[BenchMenu], currentBench, false);
	}
	SetText (myWindow, messageItem, info);
	currentBench = b;
	CheckItem (myMenus[BenchMenu], currentBench, true);
}

/* -----------------------------------------------------------------------------*/
static void SelectGen (short b)
{
  if (currentGen > 0) {
    StopGen();
    CheckItem (myMenus[SpeedMenu], currentGen, false);
  }
  if (currentGen == b) {
    currentGen = 0;
    return;
  }
  currentGen = b;
  StartGen(b);
  CheckItem (myMenus[SpeedMenu], currentGen, true);
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
		case BenchID: 
    		SelectBench(theItem);
			break;
		case SpeedID: 
    		SelectGen(theItem);
			break;
	}
	HiliteMenu(0);  
}

/* -----------------------------------------------------------------------------*/
static void DoMouseDown() 
{
	WindowPtr whichWindow; short item;
	
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
				else if (IsDialogEvent(&myEvent)) {
	    			if (DialogSelect(&myEvent, &whichWindow, &item))
	      				DoBenchCommand (whichWindow, item);
				}
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

	if (!MidiShare()) 			AlertUser ("\pMidiShare is required");
	foreGround = true;
	SetUpMenus();
	SetUpWindows();
	InitBenchLoop ();
	SetText (myWindow, messageItem, "\pChoose a benchmark");
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
		}
	}
	CloseBenchs ();
	CloseAllWinds( FrontWindow() );
}






	

