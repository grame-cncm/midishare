/*

  Copyright © Grame 2001

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
  
  modifications history:

*/

#include <windows.h>
#include <MidiShare.h>
#include "msctrl.h"


// ------------------------------------------------------------------
// CONSTANTS - DATA TYPES
// ------------------------------------------------------------------
typedef void (*ScrollActionProcPtr) (void);
typedef char State[8], *StatePtr;
enum {kPort, kChan, kProgChge, kVol, kModulation, kPan, kPitch, kVel};

#define kDefaultXPos 100
#define kDefaultYPos 100

#define kNoteLen		200
#define kPageStep		10

#define WriteState WritePrivateProfileString
#define GetState	 GetPrivateProfileInt

enum { false, true };

// ------------------------------------------------------------------
// FUNCTIONS PROTOTYPES
// ------------------------------------------------------------------
void AlertUser(LPCSTR);
Boolean SetUpMidi(void);
void SendNote(void);
void SendProgChge(void);
void SendVol(void);
void SendMod(void);
void SendPan(void);

void MSALARMAPI ReceiveEvents(register short);

void LoadApplState (StatePtr state);
void SaveApplState (StatePtr state);
BOOL CALLBACKAPI AboutDlgProc(HWND hDlg,UINT message,UINT wParam,LONG lParam);


// ------------------------------------------------------------------
// VARIABLES
// ------------------------------------------------------------------
#ifdef WIN32
#pragma data_seg(".global")
DWORD	gInstanceCount = 0;
#pragma data_seg()
UINT	gStartupMsg;
#endif

HANDLE	ghInst;
HWND		ghWnd;
HWND		abouthDlg;
DLGPROC  AboutDlgProcInst;

State gState;
ScrollActionProcPtr	gScrollAction[]= {
		  SendNote,
		  SendNote,
		  SendProgChge,
		  SendVol,
		  SendMod,
		  SendPan,
		  SendNote,
		  SendNote
};

// MidiShare variables
short myRefNum;		// numero de l'application
const char* AppliName = "msControler";

// for private profile file
char * profileName = "msControl32.ini";
char * sectionName = "Window position";
char * xEntryName  = "xpos";
char * yEntryName  = "ypos";
char * StateSectionName		= "Application state";
char * portEntryName		= "port";
char * chanEntryName		= "chan";
char * progChgeEntryName	= "progChge";
char * volEntryName			= "vol";
char * modulationEntryName  = "modulation";
char * panEntryName			= "pan";
char * pitchEntryName		= "pitch";
char * velEntryName			= "vel";
int	dlgXPos,dlgYPos;


/* ------------------------------------------------------------------

APPLICATION MAIN ENTRY POINT

------------------------------------------------------------------ */
int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
			LPSTR lpszCmdLine, int nCmdShow)
{
	if ( hPrevInstance ) return FALSE ;
#ifdef WIN32
	gStartupMsg = RegisterWindowMessage("msControlToFront");
	if (gInstanceCount) {
		PostMessage (HWND_BROADCAST, gStartupMsg, 0, 0);
		return TRUE;
	}
	InterlockedIncrement (&gInstanceCount);
#endif
	if ( !SetUpMidi() ) return 0;
	ghInst = hInstance;
	DialogBox(hInstance, "UCTRL", NULL, MakeProcInstance(UniversalCtrlDlgProc,hInstance));
	MidiClose(myRefNum);
	return TRUE;
}

/* ------------------------------------------------------------------
					Initializations
------------------------------------------------------------------ */
Boolean InitScroller(HWND hDlg, int ID, int txtID, int min, int max, int state)
{
	HWND		hCtrl;

	hCtrl = GetDlgItem (hDlg, ID);
	if (hCtrl) {
		SetScrollRange(hCtrl, SB_CTL, min, max, FALSE);
		SetScrollPos  (hCtrl, SB_CTL, max-state, FALSE);
		SetDlgItemInt (hDlg,  txtID,  txtID==IDD_TX_CHAN ? state+1 : state, FALSE);
		return true;
	}
	return false;
}

//______________________________________________________________________
Boolean Initialize(HWND hDlg)
{
	LoadApplState (gState);
	AboutDlgProcInst= (DLGPROC)MakeProcInstance(AboutDlgProc,ghInst);
	if (!AboutDlgProcInst)
		return false;
	if (!InitScroller (hDlg,IDD_SB_PORT, IDD_TX_PORT, 0, 127, gState[kPort]))
		return false;
	if (!InitScroller (hDlg,IDD_SB_CHAN, IDD_TX_CHAN, 0, 15, gState[kChan]))
		return false;
	if (!InitScroller (hDlg,IDD_SB_PROG, IDD_TX_PROG, 0, 127, gState[kProgChge]))
		return false;
	if (!InitScroller (hDlg,IDD_SB_VOL, IDD_TX_VOL, 0, 127, gState[kVol]))
		return false;
	if (!InitScroller (hDlg,IDD_SB_MOD, IDD_TX_MOD, 0, 127, gState[kModulation]))
		return false;
	if (!InitScroller (hDlg,IDD_SB_PAN, IDD_TX_PAN, 0, 127, gState[kPan]))
		return false;
	if (!InitScroller (hDlg,IDD_SB_PITCH, IDD_TX_PITCH, 0, 127, gState[kPitch]))
		return false;
	if (!InitScroller (hDlg,IDD_SB_VEL, IDD_TX_VEL, 0, 127, gState[kVel]))
		return false;
	SetWindowPos (hDlg, HWND_TOP, dlgXPos, dlgYPos,0,0,SWP_NOSIZE);
#ifdef WIN32
	 // Associate an icon with the dialog box.
	//SetClassLong (hDlg, GCL_HICON, (LONG)LoadIcon (ghInst, "ICON"));
	//SendDlgItemMessage(hDlg, AboutButton, BM_SETIMAGE, 0, (LONG)LoadBitmap (ghInst, (char *)1800));
#endif
	return TRUE ;
}


/* ------------------------------------------------------------------
					Scroll bar management
------------------------------------------------------------------ */
void VScroll (HWND hCtrl, WORD part, WORD lp )
{
	short ctrlID, max, i, val;

#ifdef WIN32
	ctrlID = (short)GetWindowLong (hCtrl,GWL_ID);
#else
	ctrlID = GetWindowWord (hCtrl,GWW_ID);
#endif
	i 		 = ctrlID - IDD_SB_PORT;
	max	 = ctrlID==IDD_SB_CHAN ? 15 : 127;
	switch (part) {
		case SB_PAGEUP :
			val = min(max, gState[i] + kPageStep);
			break;
		case SB_PAGEDOWN :
			val = max(0, gState[i] - kPageStep);
			break;
		case SB_LINEUP :
			val = min(max, gState[i] + 1);
			break;
		case SB_LINEDOWN :
			val = max(0, gState[i] - 1);
			break;
		case SB_TOP:
			val = max;
			break;
		case SB_BOTTOM :
			val = 0;
			break;
		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:
			val = max - lp;
			break;
		default:
			return;
	}
	SetScrollPos (hCtrl, SB_CTL, max - val, TRUE);
	SetDlgItemInt(ghWnd, IDD_TX_PORT+i, i==kChan ? val+1 : val, FALSE);
    gState[i] = (char)val;
	gScrollAction[i]();
}

/* ------------------------------------------------------------------
					main window management
------------------------------------------------------------------ */
BOOL CALLBACKAPI UniversalCtrlDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND	hCtrl;

#ifdef WIN32
	if (message == gStartupMsg) {
		if (IsIconic(hDlg))
			ShowWindow (hDlg, SW_RESTORE);
		else
			SetForegroundWindow (hDlg);
	}
	else
#endif
	switch (message) {
		case WM_INITDIALOG:
			ghWnd= hDlg;
			return Initialize (hDlg);

		case WM_VSCROLL :
#ifdef WIN32
			hCtrl = (HWND)lParam;
			VScroll (hCtrl, LOWORD(wParam), HIWORD(wParam));
#else
			hCtrl = (HWND)HIWORD(lParam);
			VScroll (hCtrl, wParam, LOWORD(lParam));
#endif
			break;

		case WM_MOVE:
			dlgXPos= LOWORD(lParam) - 5;
			dlgYPos= HIWORD(lParam) - 24;
			break;

		case WM_COMMAND:
			switch ( wParam ) {
				case AboutButton :
//					EnableWindow(GetDlgItem(ghWnd,AboutButton),false);
					DialogBox(ghInst, "AboutDlg", hDlg, AboutDlgProcInst);
					break;
				case IDD_QUITTER :
					if (abouthDlg) EndDialog (abouthDlg, TRUE);
					SaveApplState(gState);
					EndDialog(hDlg,TRUE);
					break;
			}
			break;
	}
	return FALSE ;
}


/* ------------------------------------------------------------------

 Messages corresponding to user actions

------------------------------------------------------------------ */
void SendNote(void)
{
	 MidiEvPtr e;

	 e = MidiNewEv(typeNote);
	 if (e) {
		Port(e)	= gState[kPort];
		Chan(e)	= gState[kChan];
		Pitch(e) = gState[kPitch];
		Vel(e)	= gState[kVel];
		Dur(e)	= kNoteLen;
		MidiSendIm (myRefNum,e);
	 }
}

void SendCtrl(char ctrlNum, char ctrlVal)
{
	 MidiEvPtr e;

	 e = MidiNewEv(typeCtrlChange);
	 if (e) {
		Port(e)	= gState[kPort];
		Chan(e)	= gState[kChan];
		Data(e)[0]	= ctrlNum;
		Data(e)[1]	= ctrlVal;
		MidiSendIm (myRefNum,e);
	 }
}

void SendVol(void)	{ SendCtrl (7,  gState[kVol]);}
void SendPan(void)	{ SendCtrl (10, gState[kPan]);}
void SendMod(void)	{ SendCtrl (1,  gState[kModulation]);}

void SendProgChge(void)
{
	 MidiEvPtr e;

	 e = MidiNewEv(typeProgChange);
	 if (e) {
		Port(e)	= gState[kPort];
		Chan(e)	= gState[kChan];
		Data(e)[0] = gState[kProgChge];
		MidiSendIm (myRefNum,e);
	 }
}
void   MidiShareSpecialInit(unsigned long defaultSpace);
/* ----------------------------------------------------------------------------

MidiShare client application setup

---------------------------------------------------------------------------- */
Boolean SetUpMidi(void) {
	if (!MidiShare()) {
		AlertUser("Can't access to 'MSHARE.DLL'");
		return false;
	}
	myRefNum = MidiOpen(AppliName);
	if ( myRefNum == MIDIerrSpace ) {
		AlertUser("Too much MidiShare client applications");
		return false;
	}
	MidiSetRcvAlarm(myRefNum,ReceiveEvents);
	MidiConnect(myRefNum,0,true);
	return true;
}


/* ----------------------------------------------------------------------------
			Alert messages
---------------------------------------------------------------------------- */
void AlertUser(LPCSTR mess)
{
	MessageBox (ghWnd, mess, AppliName,MB_OK|MB_ICONSTOP);
	SendMessage(ghWnd,WM_SYSCOMMAND,SC_CLOSE,0L);
}


/* ----------------------------------------------------------------------------

 Application receive alarm (called at interrupt level)
 msCtrl don't use incoming events

---------------------------------------------------------------------------- */
void MSALARMAPI ReceiveEvents(register short r) {
	MidiFlushEvs(r);
}


/* ----------------------------------------------------------------------------

 Private profile functions

---------------------------------------------------------------------------- */
void SaveApplState (StatePtr state)
{
	char buff[30];

	wsprintf (buff, "%d", dlgXPos);
	WriteState (sectionName, xEntryName, buff, profileName);
	wsprintf (buff, "%d", dlgYPos);
	WriteState (sectionName, yEntryName, buff, profileName);

	wsprintf (buff, "%d", (int)state[kPort]);
	WriteState (StateSectionName, portEntryName, buff, profileName);
	wsprintf (buff, "%d", (int)state[kChan]);
	WriteState (StateSectionName, chanEntryName, buff, profileName);
	wsprintf (buff, "%d", (int)state[kProgChge]);
	WriteState (StateSectionName, progChgeEntryName, buff, profileName);
	wsprintf (buff, "%d", (int)state[kVol]);
	WriteState (StateSectionName, volEntryName, buff, profileName);
	wsprintf (buff, "%d", (int)state[kModulation]);
	WriteState (StateSectionName, modulationEntryName, buff, profileName);
	wsprintf (buff, "%d", (int)state[kPan]);
	WriteState (StateSectionName, panEntryName, buff, profileName);
	wsprintf (buff, "%d", (int)state[kPitch]);
	WriteState (StateSectionName, pitchEntryName, buff, profileName);
	wsprintf (buff, "%d", (int)state[kVel]);
	WriteState (StateSectionName, velEntryName, buff, profileName);
}

//________________________________________________________________________
void LoadApplState (StatePtr state)
{
	int x, y;

	x = GetState (sectionName, xEntryName, kDefaultXPos, profileName);
	y = GetState (sectionName, yEntryName, kDefaultYPos, profileName);
	dlgXPos =  x ? x : kDefaultXPos;
	dlgYPos =  y ? y : kDefaultYPos;
	state[kPort]		= GetState (StateSectionName, portEntryName, 0, profileName);
	state[kChan]		= GetState (StateSectionName, chanEntryName, 0, profileName);
	state[kProgChge]	= GetState (StateSectionName, progChgeEntryName, 0, profileName);
	state[kVol]			= GetState (StateSectionName, volEntryName, 0, profileName);
	state[kModulation]= GetState (StateSectionName, modulationEntryName, 0, profileName);
	state[kPan]			= GetState (StateSectionName, panEntryName, 0, profileName);
	state[kPitch]		= GetState (StateSectionName, pitchEntryName, 0, profileName);
	state[kVel]			= GetState (StateSectionName, velEntryName, 0, profileName);
}

//________________________________________________________________________
BOOL CALLBACKAPI AboutDlgProc(HWND hDlg,UINT message,
													UINT wParam,LONG lParam)
{
	char buff[10];

	switch (message) {
		case WM_INITDIALOG :
			abouthDlg = hDlg ;
			wsprintf(buff,"%d.%02d",MidiGetVersion()/100,MidiGetVersion()%100);
			SetDlgItemText(hDlg, Version, buff);
			return TRUE ;

		case WM_COMMAND :
			switch(wParam) {
				case IDOK :
//					EnableWindow(GetDlgItem(ghWnd,AboutButton),true);
					EndDialog(hDlg,0);
					abouthDlg = 0 ;
					break;
			}
			break;
	}
	return FALSE ;
}



