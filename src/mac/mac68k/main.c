/*

  Copyright © Grame 1999

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

#include <Devices.h>
#include <Dialogs.h>
#include <Events.h>
#include <Fonts.h>
#include <Memory.h>
#include <Menus.h>
#include <OSUtils.h>
#include <Quickdraw.h>
#include <Resources.h>
#include <SegLoad.h>
#include <ToolUtils.h>
#include <Types.h>
#include <Windows.h>


#include "MidiShareMac.h"
#include "MSLoader.h"
#include "Loader.rsrc.h"


/* Some Macintosh constants */

#if __MWERKS__
#	define osEvent				osEvt
#else
#	define osEvent 				app4Evt
#endif
#define	resumeMask				1
#define	suspendResumeMessage	1
#define mouseMovedMessage		0xFA


/* Global variables */
WindowPtr	myWindow;							/* window							*/
Boolean		doneFlag;							/* stop flag 						*/
SysEnvRec	gMac;								/* machineÉ							*/
Boolean		foreGround;							/* true in foreGround				*/

EventRecord	myEvent;							/* for the main event loop			*/
char		theChar;							/* for keyboard commands			*/
Rect		dragRect;							/* window move rect					*/
MenuHandle	myMenus[mBarSize];					/* menus							*/

unsigned char *ApplName= "\pMidiShare";			/* the application name				*/

//_________________________________________________________________________________
Boolean 	AlertUser (unsigned char *what, unsigned char *msg, Boolean exit );

//_________________________________________________________________________________
static void DoIdle()
{
}

//_________________________________________________________________________________
Boolean AlertUser( unsigned char *what, unsigned char *msg, Boolean exit )
{
	short ret;
	
	ResetAlertStage();
	ParamText( what, msg, "\p", "\p");
	ret= Alert( AlertID, nil);
	if( exit) ExitToShell();
	return false;							
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

//_________________________________________________________________________________
static void AdjustLocation( DialogPtr TheDialogPtr)
{
	Point	TestPoint;
	
	TestPoint.h = TheDialogPtr->portRect.left + 6;	// 6 is enough to grab
	TestPoint.v = TheDialogPtr->portRect.top + 6;	// 6 is enough to grab
	LocalToGlobal(&TestPoint);
	
	if ( !PtInRgn( TestPoint, GetGrayRgn() ) ) 	/* Reposition */
	{
		MoveWindow( (WindowPtr) TheDialogPtr, 
			qd.screenBits.bounds.left + 50,			
			qd.screenBits.bounds.top  + 50,
			false
		);
	}
}

//_________________________________________________________________________________
static void SetUpWindows()
{
	GDHandle dev;
	short dlogID;
	
	dev= GetMainDevice();
	dlogID= dialogID;
	SetRect (&dragRect, qd.screenBits.bounds.left, qd.screenBits.bounds.top, 
						qd.screenBits.bounds.right,qd.screenBits.bounds.bottom);
	InsetRect (&dragRect, 4, 4);

 	myWindow = GetNewDialog (dlogID, nil, (WindowPtr) -1);
 	SetPort(myWindow);
	AdjustLocation( myWindow);
	ShowWindow(myWindow);
}

//_________________________________________________________________________________
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

//_________________________________________________________________________________
static void ShowAbout()
{											
	DialogPtr	myDialog;					
	short		Hit;

	myDialog = GetNewDialog(AboutID,nil,(WindowPtr) -1);
	if (myDialog) {
		ModalDialog(nil,&Hit);
		DisposeDialog(myDialog);
	}
}
			
//_________________________________________________________________________________
static Boolean Confirm( short id, short ok, Boolean defaut)
{
	short hit;			

	hit= Alert( id, nil);
	if( hit >0)
		return hit== ok;
	return defaut;
}
	
//_________________________________________________________________________________
static Boolean ReallyExit( EGlobalPtr g)
{
	return MidiCountAppls() ? Confirm( QuitID, kQuit, true) : true;
}

//_________________________________________________________________________________
static void DoCommand(long mResult)
{
	short	theItem, theMenu;
	Str255	name;

	theItem = LoWord(mResult);
	theMenu = HiWord(mResult);
	
	switch ( theMenu )
	{
		case AppleID:
			if (theItem != 1) 
			{
				GetMenuItemText (myMenus[AppleMenu], theItem, name);
				theItem = OpenDeskAcc(name);
			} else {
				ShowAbout();
			}
			break;
		case FileID: 
			doneFlag = ReallyExit( &gLoader);
			break;
	}
	HiliteMenu(0);  
}

//_________________________________________________________________________________
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
				break;
		case inDrag: 	 
				DragWindow(whichWindow, myEvent.where, &dragRect);
				break;
		case inContent: 	 
				if (whichWindow != FrontWindow())
					SelectWindow(whichWindow);
				else if( whichWindow== myWindow)
					DragWindow(whichWindow, myEvent.where, &dragRect);
				break;
	}
}

//_________________________________________________________________________________
static void AdjustCursor() 
{
	if (foreGround && IsAppWindow(FrontWindow())) 
		SetCursor(&qd.arrow);
}

//_________________________________________________________________________________
static pascal void DoRedraw()
{
	PicHandle pic;
	Rect dst;
	
	pic= (PicHandle)GetResource( 'PICT', PictID);
	if( pic) {
		dst= myWindow->portRect;
		SetPort( myWindow);
		HLock( (Handle)pic);
		DrawPicture( pic, &dst);
		HUnlock( (Handle)pic);
	}
}

//_________________________________________________________________________________
static void Initialize( EGlobalPtr g)
{
	OSErr err; short i;
	
	err = SysEnvirons(1, &gMac);
	if (gMac.machineType < 0) AlertUser( ApplName, "\pneed at least 128K ROMs", true);
	for (i=0; i<10; i++) {
		MoreMasters();
	}	
	foreGround = true;
		
	InitGraf(&qd.thePort); 			   				/* initialise QuickDraw					*/
	InitFonts();					   				/* initialise le Font Manager			*/
	InitWindows();					   				/* initialise le Window Manager			*/
	InitDialogs( 0);				   				/* initialise le Dialog Manager			*/
	InitMenus();				   					/* initialise le Menu Manager			*/
	TEInit();									   	/* initialise le Text Edit Manager		*/
	InitCursor(); 					   				/* initialise le curseur de la souris	*/

	FlushEvents(everyEvent, 0);						/* efface ŽvŽnements Mac en attente		*/

	SetUpMenus();									/* mise en place menus					*/
	SetUpWindows();
	
	if( MidiShare() && MidiCountAppls())
		FatalErreur( "\pMidiShare applications are running !");
	InitLoader( g);
	LoadMidiShare();
}

//_________________________________________________________________________________
void main()
{
	Boolean b;
	
	MaxApplZone();
	Initialize( &gLoader);
	
	doneFlag = false;											
	while (!doneFlag) {										/* Main Loop				*/
		b = WaitNextEvent(everyEvent, &myEvent, 0, nil);
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
						case mouseMovedMessage:
							DoIdle();
							break;
					}
					break;
			case keyDown:
			case autoKey:
					theChar = myEvent.message & charCodeMask;
					if (myEvent.modifiers & cmdKey ) {
						DoCommand(MenuKey(theChar));
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
							if( (WindowPtr)myEvent.message== myWindow)
								DoRedraw();
						}
						EndUpdate( (WindowPtr)myEvent.message );
					}
					break;
		}
	}
	RestoreMidiShare();
	CloseAllWinds( FrontWindow() );
}



