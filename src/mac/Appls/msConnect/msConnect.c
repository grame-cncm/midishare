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

/* Constants definition */
#define osEvent 				app4Evt
#define	resumeMask				1
#define	suspendResumeMessage	1

/* menu bar constants */
enum { AppleMenu, FileMenu, EditMenu, PortsMenu, mBarSize };
enum { AppleID = 128, FileID, EditID, PortsID };
enum { UndoI = 1, CutI = 3, CopyI, PasteI, ClearI };

/* dialog & alerts */
enum { windowID = 128, AlertID = 128, AboutID = 128, AboutPictID = 130 };

/* liste des src, applications, destinations*/
enum { SrcL, AppL, DstL, listSize };   


/* Application name	*/
#define ApplName 		"\pmsConnect"
enum { kWinPosRsrc = 'wipo', kWinposID = 128 };

/* global variables	*/
WindowPtr	myWindow;
Handle 		msgItem;
Boolean		doneFlag;

SysEnvRec	gMac;
Boolean		hasWNE;
Boolean		foreGround;

EventRecord	myEvent;
Rect		dragRect;
MenuHandle	myMenus[mBarSize];
Boolean 	chgAppl, chgCon;
short		myRefNum, resFile;

static Cell 		lastCell;   				/* dernière cellule sélectionnée*/
static ListHandle 	theList[listSize]; 			/* les trois listes: srce,appl,dest*/
static Rect 		ListRect[listSize];			/* et leurs rectangles*/
static Boolean 		applSelect;   				/* indique si les Src et Dst doivent*/
												/* être affichées*/
static PicHandle 	titlePict, handlePict;		/* les images de la fenêtre*/
static Rect 		titleRect, handleRect;   	/* et leurs rectangles de contour*/
static FontInfo 	fInf;   					/* information police utilisée*/


/* -----------------------------------------------------------------------------*/
static void ChangeConnect (short dir)
{
	short i, app, anApp;
	Cell aCell; Boolean state;

	aCell.h = 0;
	app = MidiGetIndAppl (lastCell.v + 1);

	for (i = 1; i <= MidiCountAppls(); i++) {  /* pour toutes les appl. Midi*/
		aCell.v = i - 1;
		state = LGetSelect(false, &aCell, theList[dir]);
		anApp = MidiGetIndAppl(i);

		switch (dir) {
			case SrcL:
				if (state != MidiIsConnected (anApp, app))
			  		MidiConnect(anApp, app, state);
				break;
			case DstL:
				if (state != MidiIsConnected (app, anApp))
					MidiConnect(app, anApp, state);
				break;
		}
	}
}

/* -----------------------------------------------------------------------------*/
static pascal void RcvAlarm (short refnum)
{
	MidiFlushEvs (refnum);
}

/* -----------------------------------------------------------------------------*/
static pascal void ApplAlarm (short refnum, long code)
{
	switch (code & 0xffff) {

		case MIDIOpenAppl:
		case MIDICloseAppl:
		case MIDIChgName:
			chgAppl = true;
			break;

		case MIDIChgConnect:
			chgCon = true;
			break;
	}
}

/* -----------------------------------------------------------------------------*/
static Boolean SetupMidi(void)
{
	myRefNum = MidiOpen (ApplName);
	if (myRefNum == MIDIerrSpace) return false;

	chgAppl = chgCon = false;
	MidiSetApplAlarm (myRefNum, ApplAlarm);
	MidiSetRcvAlarm (myRefNum, RcvAlarm);
	return true;
}

/* -----------------------------------------------------------------------------*/
static void EraseLists(void)
{
	LDelRow(0, 0, theList[SrcL]);   			/* détruit la colonne source*/
	LDelRow(0, 0, theList[DstL]); 				/* détruit la colonne destination*/
	LUpdate(myWindow->visRgn, theList[SrcL]);	/* et les met à jour*/
	LUpdate(myWindow->visRgn, theList[DstL]);
	applSelect = false;  						/* plus d'appl sélectionnée*/
}

/* -----------------------------------------------------------------------------*/
static void ShowSrcDest (Cell theSel)
{
	short i, app; Cell aCell;
	Boolean connect;

	if (!applSelect) return;
	
	app = MidiGetIndAppl(theSel.v + 1); 	/* refnum Midi de cette application*/
	aCell.h = 0;   							/* colonne 0*/
	for (i = 1; i <= MidiCountAppls(); i++) {	/* pour toutes les appl. Midi*/
		aCell.v = i - 1;   					/* ligne i-1 */

		connect = MidiIsConnected (MidiGetIndAppl(i), app);
		LSetSelect (connect, aCell, theList[SrcL]);

		connect = MidiIsConnected(app, MidiGetIndAppl(i));
		LSetSelect(connect, aCell, theList[DstL]);
	}
}

/* -----------------------------------------------------------------------------*/
static void FillList (short aList)
{
	short i;
	Cell aCell; MidiName name;
	
	aCell.h = 0;
	LDelRow(0, 0, theList[aList]);
	LAddRow(MidiCountAppls(), 0, theList[aList]);
	for (i = 1; i <= MidiCountAppls(); i++) {
		aCell.v = i - 1;   /*n° de ligne*/
		name = MidiGetName (MidiGetIndAppl(i));
		LSetCell(&name[1], name[0], aCell, theList[aList]);
	}
	LUpdate(myWindow->visRgn, theList[aList]);
}


/* -----------------------------------------------------------------------------*/
static void CloseMSConnect ()
{
	short i;
	MidiClose (myRefNum);
	for (i = 0; i < listSize; i++) {
		LDispose(theList[i]);
	}
}

/* -----------------------------------------------------------------------------*/
static void ScrollList (short move)
{
	short dep;

	if (!applSelect)   /* pas d'appl sélectionnée -> exit*/
		return;
	if (move == kUpArrowCharCode) {   	/* flêche haute: */
		if (lastCell.v == 0)   			/*si première déjà sélectionnée*/
	  		return;						/*alors exit*/
		dep = -1;   					/* sinon déplacement négatif*/
	} else {							/* flêche basse:*/
		if (lastCell.v == MidiCountAppls() - 1)  /*si dernière déjà sélectionnée*/
	 		return;						/*alors exit*/
		dep = 1;						/*sinon déplacement positif*/
	}
	/* déselectionne l'ancienne cell*/
	LSetSelect (false, lastCell, theList[AppL]);
	/* nouvelle cellule*/
	lastCell.v += dep;
	LSetSelect  (true, lastCell, theList[AppL]);   /* la sélectionne */
	LAutoScroll (theList[AppL]);   /* affiche ses src et dest*/
	ShowSrcDest (lastCell);
}

/* -----------------------------------------------------------------------------*/
static Boolean SelectList (Point where, short modifiers)
{
	Boolean result = false;
	short i;
	Cell click = { 0, 0 };

	GlobalToLocal(&where);   /* conversion en coordonnées fenêtre*/
	for (i = 0; i < listSize; i++) {
		if (PtInRect(where, &ListRect[i])) {  /* si le point est dans une liste*/
			result = true;

		  	if ((i == SrcL) || (i == DstL)) {
				if (!applSelect)   		/*pas d'appl sélect. -> exit*/
		  			return true;
				modifiers |= cmdKey;   	/*simule la touche commande*/
		  	}  							/*pour sélections multiples*/
		  	LClick (where, modifiers, theList[i]);
		  	switch (i) {
		  		case SrcL:
		  		case DstL: 
					ChangeConnect (i);
					break;
					
		  		case AppL:
					if (LGetSelect (true, &click, theList[AppL])) {
		  				if (!applSelect) {  	/*si pas encore fait,*/
		    				FillList (SrcL);	/*affiche les src*/
		    				FillList (DstL);	/*affiche les dest*/
		    				applSelect = true;	/*et positionne flag*/
		  				}
		  				lastCell = click;		/*montre ses connections*/
		  				ShowSrcDest(lastCell);
					} 
					else EraseLists ();
					break;
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
		 (gMac.machineType > envMachUnknown)  &&		/* …512KE, Plus, or	*/
		 (gMac.machineType < envMacII) ) {				/* …SE				*/
		 
		trapNum &= 0x3FF;				/* Traps numbers are 10 bits long	*/
		
		if (trapNum > 0x1FF) {			/* Traps only go up to 0x1FF on		*/
			return(false);				/*   these machines					*/
		}
	}
	return NGetTrapAddress(trapNum, tType) != GetTrapAddress(_Unimplemented);
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

/* -----------------------------------------------------------------------------*/
static void AdjustLocation (WindowPtr win)
{
	Point pt;
	
	pt.h = win->portRect.left + 6;	// 6 is enough to grab
	pt.v = win->portRect.top + 6;	// 6 is enough to grab
	LocalToGlobal(&pt);
	
	if (!PtInRgn( pt, GetGrayRgn())) {
		MoveWindow (win, qd.screenBits.bounds.left + 50,	qd.screenBits.bounds.top  + 50, false);
	}
}

/* -----------------------------------------------------------------------------*/
static void SetUpWindows()
{
	Rect view, bounds; Cell theCell = { 0, 0};
	short i; Rect *r; Point p;
	
	SetRect (&dragRect, qd.screenBits.bounds.left, qd.screenBits.bounds.top, 
						qd.screenBits.bounds.right,qd.screenBits.bounds.bottom);
	InsetRect (&dragRect, 4, 4);
  	myWindow = GetNewWindow(windowID, NULL, (WindowPtr)-1);
  	SetPort(myWindow);

	SetRect(&titleRect, 4, 4, 421, 25);	/* rect de la pict de titre*/
	titlePict 	= GetPicture(128); 		/* pict de titre*/
	handlePict 	= GetPicture(129);  	/* pict de la poignée inférieure*/
	TextSize(10);
	GetFontInfo(&fInf);  				/* infos du font*/
	SetRect(&bounds, 0, 0, 1, 0);		/* taille des listes*/

	for (i = 0; i < listSize; i++) {
		r = &ListRect[i];
		r->top = 30;   					/*calculé en fonction du font*/
		r->left = i * 145 + 5; 	/* pour présenter 8 applications*/
		r->bottom = r->top + (fInf.ascent + fInf.descent + fInf.leading) * 8;
		r->right = r->left + 125;
		view = ListRect[i]; 			/*rect de la liste,*/
		view.right -= 15;
		InsetRect(&ListRect[i], -1, -1);   /*rect intérieur de la liste*/
		theList[i] = LNew(&view, &bounds, theCell, 0, myWindow, true, false, false, true);
	}

	chgAppl 	= true; 		/* pour remplir la liste centrale*/
	applSelect	= false;		/* pas d'appl sélectionnée*/
	lastCell.h	= 0;
	(*theList[AppL])->selFlags |= lOnlyOne;

	SizeWindow (myWindow, 425, ListRect[AppL].bottom + 10, false);
	SetRect (&handleRect, 4, myWindow->portRect.bottom - 10, 421, myWindow->portRect.bottom);
	if (ReadPos (&p))
		MoveWindow (myWindow, p.h, p.v, false);
	AdjustLocation (myWindow);
	ShowWindow (myWindow);
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
	myMenus[PortsMenu] = GetMenu(PortsID);
	for (i = 0; i < mBarSize; i++)
		InsertMenu(myMenus[i], 0);
	DrawMenuBar();
}			

/* -----------------------------------------------------------------------------*/
static void DoIdle()
{
	if (chgAppl) {  		/* chgmt dans les applications:*/
		chgAppl = false;
		FillList(AppL);   	/*  reconstruit la liste centrale*/					
		EraseLists();		/*  et efface les deux autres*/
	}
	if (chgCon) {  			/* chgmt de connection:*/
		chgCon = false;   	/*  réaffiche les src et dest*/
		ShowSrcDest (lastCell);
	}
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

		case PortsID:
			MidiSetPortState(theItem - 1, ~MidiGetPortState(theItem - 1));
			break;
	}
	HiliteMenu(0);  
}

/* -----------------------------------------------------------------------------*/
static void SetPortMark(void)
{
	short port, chk;

	for (port = ModemPort; port <= PrinterPort; port++) {
		chk =  MidiGetPortState(port) ? checkMark : noMark;
		SetItemMark (myMenus[PortsMenu], port + 1, chk);
	}
}

/* -----------------------------------------------------------------------------*/
static void ResizeWind (Point where)
{
	Point aPt;
	short i, f, v, bottom;
	Rect aRect;

	SetPort(myWindow);
	aPt = where;
	GlobalToLocal(&aPt);
	if (aPt.v < myWindow->portRect.bottom - 10)
		return;	/* point pas dans poignée -> exit*/

	/* hauteur totale du font*/
	f = fInf.ascent + fInf.descent + fInf.leading;
	SetRect (&aRect, 425, f * 4 + 41, 425, dragRect.bottom);
	/* rect d'agrandissement autorisé*/
	bottom = myWindow->portRect.bottom;
	i = bottom - myWindow->portRect.top;
	v = ((unsigned long)GrowWindow (myWindow, where, &aRect)) >> 16;
	/* track déplacements*/
	v = f * ((v - 41) / f) + 41;   /* la hauteur est fonction du font*/
	if (v == i)  /* si un changement est intervenu,*/
		return;

	SizeWindow (myWindow, 425, v, true);   /*redimensionne la fenêtre*/
	bottom = myWindow->portRect.bottom;
	SetRect(&aRect, 0, 30, 425, bottom);   /* efface les listes et la poignée*/
	EraseRect (&aRect);
	SetRect(&handleRect, 4, bottom - 10, 421, bottom);

	for (i = 0; i < listSize; i++) {
		ListRect[i].bottom = bottom - 10;
		LSetDrawingMode (false, theList[i]);
		LSize (110, bottom - 41, theList[i]);
		LSetDrawingMode (true, theList[i]);
	}
	InvalRect (&aRect);   			/*invalide la zone effacée*/
	LAutoScroll (theList[AppL]);   	/*pour toujours voir la sélection*/
}

/* -----------------------------------------------------------------------------*/
static void DoMouseDown() 
{
	WindowPtr whichWindow;
	
	switch ( FindWindow( myEvent.where, &whichWindow ) )
	{ 
		case inMenuBar:
			AdjustMenus ();
			SetPortMark ();
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
				if (!SelectList (myEvent.where, myEvent.modifiers))
					ResizeWind (myEvent.where);
			}
			break;
	}
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
		DrawPicture (handlePict, &handleRect);
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
					else if ((c == kUpArrowCharCode) || (c == kDownArrowCharCode))
	    				ScrollList (c);
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
	WritePos ();
	CloseAllWinds( FrontWindow() );
}






	

