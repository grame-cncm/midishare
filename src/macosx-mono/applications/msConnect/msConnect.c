/*

  Copyright © Grame 2003

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


#include <Carbon/Carbon.h>
//#include <unistd.h>

#include "MidiShare.h"
#include "../lib/states.h"

/* Constants definition */
#define osEvent 				app4Evt
#define	resumeMask				1
#define	suspendResumeMessage	1

/* menu bar constants */
enum { AppleMenu, FileMenu, mBarSize };
enum { AppleID = 128, FileID, EditID, PortsID };
enum { UndoI = 1, CutI = 3, CopyI, PasteI, ClearI };

/* dialog & alerts */
enum { windowID = 128, AlertID = 128, AboutID = 128, AboutPictID = 130 };

/* liste des src, applications, destinations*/
enum { SrcL, AppL, DstL, listSize };   


/* Application name	*/
#define PApplName 		"\pmsConnect"
#define ApplName 		"msConnect"
enum { kWinPosRsrc = 'wipo', kWinposID = 128 };

/* global variables	*/
static char * profileName 		= "msConnect.ini";

WindowRef	myWindow;
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

static RgnHandle 	gRgn;

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
static void RcvAlarm (short refnum)
{
	MidiFlushEvs (refnum);
}

/* -----------------------------------------------------------------------------*/
static void ApplAlarm (short refnum, long code)
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
	GetWindowRegion (myWindow, kWindowContentRgn, gRgn);
	LDelRow(0, 0, theList[SrcL]);   			/* détruit la colonne source*/
	LDelRow(0, 0, theList[DstL]); 				/* détruit la colonne destination*/
	LUpdate(gRgn, theList[SrcL]);	/* et les met à jour*/
	LUpdate(gRgn, theList[DstL]);
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
		LSetCell(name, strlen(name), aCell, theList[aList]);
	}
	LUpdate(gRgn, theList[aList]);
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
	ParamText( PApplName, msg, "\p", "\p");
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
	Rect view, bounds; Cell theCell = { 0, 0};
	short i; Rect *r; Point p;
	
	GetRegionBounds (GetGrayRgn(), &bounds);
	dragRect = bounds;
	InsetRect (&dragRect, 4, 4);
  	myWindow = GetNewWindow(windowID, NULL, (WindowPtr)-1);
  	SetPort(GetWindowPort(myWindow));

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
	GetWindowPortBounds (myWindow, &view);
	SetRect (&handleRect, 4, view.bottom - 10, 421, view.bottom);

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
	AppendResMenu(myMenus[AppleMenu],'DRVR');
	myMenus[FileMenu] = GetMenu(FileID);
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

	theItem = LoWord(mResult);
	theMenu = HiWord(mResult);
	
	switch ( theMenu )
	{
		case AppleID:
			if (theItem == 1)
				ShowAbout();
			break;

		case FileID: 
			doneFlag = true;
			break;

		case PortsID:
#ifndef __POWERPC__
			MidiSetPortState(theItem - 1, ~MidiGetPortState(theItem - 1));
#endif
			break;
	}
	HiliteMenu(0);  
}
/* -----------------------------------------------------------------------------*/
static void ResizeWind (Point where)
{
	Point aPt;
	short i, f, v, bottom;
	Rect aRect, r;

	SetPort(GetWindowPort(myWindow));
	aPt = where;
	GlobalToLocal(&aPt);
    GetWindowPortBounds (myWindow, &r);
	if (aPt.v < r.bottom - 10)
		return;	/* point pas dans poignée -> exit*/

	/* hauteur totale du font*/
	f = fInf.ascent + fInf.descent + fInf.leading;
	SetRect (&aRect, 425, f * 4 + 41, 425, dragRect.bottom);
	/* rect d'agrandissement autorisé*/
	bottom = r.bottom;
	i = bottom - r.top;
	v = ((unsigned long)GrowWindow (myWindow, where, &aRect)) >> 16;
	/* track déplacements*/
	v = f * ((v - 41) / f) + 41;   /* la hauteur est fonction du font*/
	if (v == i)  /* si un changement est intervenu,*/
		return;

	SizeWindow (myWindow, 425, v, true);   /*redimensionne la fenêtre*/
    GetWindowPortBounds (myWindow, &r);
	bottom = r.bottom;
	SetRect(&aRect, 0, 30, 425, bottom);   /* efface les listes et la poignée*/
	EraseRect (&aRect);
	SetRect(&handleRect, 4, bottom - 10, 421, bottom);

	for (i = 0; i < listSize; i++) {
		ListRect[i].bottom = bottom - 10;
		LSetDrawingMode (false, theList[i]);
		LSize (110, bottom - 41, theList[i]);
		LSetDrawingMode (true, theList[i]);
	}
	InvalWindowRect (myWindow, &aRect);   	/*invalide la zone effacée*/
	LAutoScroll (theList[AppL]);   	/*pour toujours voir la sélection*/
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
static void DoRedraw (WindowRef win) 
{
	short i; GrafPtr curPort; Rect r;
    GrafPtr wport = GetWindowPort(win);
    
	GetPort (&curPort);
	
    SetPort (wport);
	BeginUpdate (win);
    DrawPicture (titlePict, &titleRect);
	GetWindowRegion (win, kWindowContentRgn, gRgn);
// trouver pourquoi la region gRgn n'est pas valide ?
    SetRectRgn (gRgn, 0, 0, 5000, 5000);
    for (i = 0; i < listSize; i++) {
        LUpdate (gRgn, theList[i]);
        FrameRect (&ListRect[i]);
    }
    DrawPicture (handlePict, &handleRect);
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
    gRgn = NewRgn();
	foreGround = true;
	resFile = CurResFile();
	SetUpMenus();
	SetUpWindows();
}

/*======================================== main loop =======================================*/
int main()
{
	char c; int i;
	
	Initialize();
    doneFlag = false;											
	while (!doneFlag) {	
		WaitNextEvent(everyEvent, &myEvent, 10, nil);
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






	

