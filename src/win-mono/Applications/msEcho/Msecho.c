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

#include <MidiShare.h>
#include <windows.h>
#include "msecho.h"

// ------------------------------------------------------------------
// CONSTANTS
// ------------------------------------------------------------------
enum { kDelay, kVel, kPitch, kChan, kParamCount };
enum { false, true };

// ------------------------------------------------------------------
// PROTOTYPES
// ------------------------------------------------------------------
void 		NEAR 	AlertUser(LPCSTR);
Boolean 	SetUpMidi(void);

void MSALARMAPI ReceiveEvents(register short);
void MSALARMAPI MyEchoTask(long,short,long,long,long);
BOOL CALLBACKAPI AboutDlgProc(HWND,UINT ,UINT ,LONG );


// ------------------------------------------------------------------
// VARIABLES
// ------------------------------------------------------------------
#ifdef WIN32
#pragma data_seg(".global")
DWORD	gInstanceCount = 0;
#pragma data_seg()
#endif

HANDLE		ghInst;
HWND		ghWnd;
HWND		abouthDlg;
DLGPROC 	AboutDlgProcInst;
UINT gNewMsg, gQuitMsg, gAppIndex;

short	FAR MSParam[kParamCount];
int dlgXPos;
int dlgYPos;
const char * kAllChan= "All";

// MidiShare variables
short		myRefNum; // numero de l'application
MidiName	AppliName = "msEcho";

//_____________________________________________________________________
// for private profile file
#define kDefaultXPos	10
#define kDefaultYPos	30

char * profileName = "msEcho32.ini";
char * sectionName = "Window position";
char * xEntryName  = "xpos";
char * yEntryName  = "ypos";
char * stateSection = "Echo state";
char * paramEntry[] = { "delay", "vel", "pitch", "chan" };

//________________________________________________________________________
// private profile functions
//________________________________________________________________________
void SaveState (void)
{
	char buff[30];
	short i;

	wsprintf (buff, "%d", dlgXPos);
	WritePrivateProfileString (sectionName, xEntryName, buff, profileName);
	wsprintf (buff, "%d", dlgYPos);
	WritePrivateProfileString (sectionName, yEntryName, buff, profileName);
	for (i=0; i<kParamCount; i++) {
		wsprintf (buff, "%d", MSParam[i]);
		WritePrivateProfileString (stateSection, paramEntry[i], buff, profileName);
	}
}

//________________________________________________________________________
void LoadState (void)
{
	int x, y, val;

	x = GetPrivateProfileInt (sectionName, xEntryName, kDefaultXPos, profileName);
	y = GetPrivateProfileInt (sectionName, yEntryName, kDefaultYPos, profileName);
	dlgXPos =  x ? x : kDefaultXPos;
	dlgYPos =  y ? y : kDefaultYPos;
	val = GetPrivateProfileInt (stateSection, paramEntry[kDelay], kDefaultDelay, profileName);
	MSParam[kDelay]=  ((val<=kMaxDelay) && (val>=kMinDelay)) ? val : kDefaultDelay;
	val = GetPrivateProfileInt (stateSection, paramEntry[kVel], kDefaultVel, profileName);
	MSParam[kVel]=  ((val<=kMaxVel) && (val>=kMinVel)) ? val : kDefaultVel;
	val = GetPrivateProfileInt (stateSection, paramEntry[kPitch], kDefaultPitch, profileName);
	MSParam[kPitch]=  ((val<=kMaxPitch) && (val>=kMinPitch)) ? val : kDefaultPitch;
	val = GetPrivateProfileInt (stateSection, paramEntry[kChan], kDefaultChan, profileName);
	MSParam[kChan]=  ((val<=kMaxChan) && (val>=kMinChan)) ? val : kDefaultChan;
}

/* ------------------------------------------------------------------

APPLICATION MAIN ENTRY POINT

------------------------------------------------------------------ */
int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
			LPSTR lpszCmdLine, int nCmdShow)
{
	if ( hPrevInstance ) return FALSE ;
	ghInst = hInstance;
	gNewMsg = RegisterWindowMessage("msDisplayNew");
	gQuitMsg = RegisterWindowMessage("msDisplayQuit");
#ifdef WIN32
	if (gInstanceCount)
		PostMessage (HWND_BROADCAST, gNewMsg, 0, 0);
	InterlockedIncrement (&gInstanceCount);
	gAppIndex = gInstanceCount;
#endif
	if ( !SetUpMidi() ) return 0;
	DialogBox (hInstance, "msEcho", NULL,
				(DLGPROC)MakeProcInstance(msEchoDlgProc,hInstance));
	MidiClose(myRefNum);
	InterlockedDecrement (&gInstanceCount);
	PostMessage (HWND_BROADCAST, gQuitMsg, gAppIndex, 0);
	return 1;
}

//___________________________________________________________________
char * MakeInstName (char *dst, char *base, UINT index)
{
	wsprintf (dst, "%s %d", base, index);
	return dst;
}

//___________________________________________________________________
Boolean InitScroller ( HWND hDlg, int ID, int txtID, short min, short max, short val)
{
	HWND		hCtrl;

	hCtrl = GetDlgItem (hDlg, ID);
	if (hCtrl){
		SetScrollRange (hCtrl, SB_CTL, min, max, FALSE);
		SetScrollPos   (hCtrl, SB_CTL, val, FALSE);
		if ((txtID==IDD_TX_CHAN) && !val)
			SetDlgItemText (hDlg , txtID, kAllChan);
		else
			SetDlgItemInt  (hDlg , txtID, val, TRUE);
	}
	return false;
}

Boolean Initialize ( HWND hDlg)
{
	Boolean  retCode= true;

    ghWnd = hDlg;
#ifdef WIN32
//	SetClassLong (hDlg, GCL_HICON, (LONG)LoadIcon (ghInst, "ICON"));
	SendDlgItemMessage(hDlg, AboutButton, BM_SETIMAGE, 0, (LONG)LoadBitmap (ghInst, (char *)1800));	
	SetWindowText(hDlg, MidiGetName(myRefNum));
#endif
	AboutDlgProcInst= (DLGPROC)MakeProcInstance(AboutDlgProc,ghInst);
	LoadState ();
	retCode|= InitScroller (hDlg, IDD_SB_DELAY, IDD_TX_DELAY,
									kMinDelay,kMaxDelay,MSParam[kDelay]);
	retCode|= InitScroller (hDlg, IDD_SB_VEL, IDD_TX_VEL,
									kMinVel,kMaxVel,MSParam[kVel]);
	retCode|= InitScroller (hDlg, IDD_SB_PITCH, IDD_TX_PITCH,
									kMinPitch,kMaxPitch,MSParam[kPitch]);
	retCode|= InitScroller (hDlg, IDD_SB_CHAN, IDD_TX_CHAN,
									kMinChan,kMaxChan,MSParam[kChan]);
	SetWindowPos(hDlg, HWND_TOP, dlgXPos, dlgYPos,0,0,SWP_NOSIZE);
	return retCode ;
}

/* ------------------------------------------------------------------
				  Horizontal scrolling management
				  it uses the control IDs to get the parameter index
              and assume that IDs offset is 200
	------------------------------------------------------------------ */
void HScroll (short nCtrlID, UINT message, short tp)
{
	short	nIndex;

	nIndex  = nCtrlID - 200;
	switch (message) {
		case SB_PAGEUP :
			switch ( nCtrlID ) {
				case IDD_SB_DELAY:
					MSParam[nIndex] = max(kMinDelay, MSParam[nIndex] - 20);
					break;
				case IDD_SB_VEL:
					MSParam[nIndex] = max(kMinVel, MSParam[nIndex] - 10);
					break;
				case IDD_SB_PITCH:
					MSParam[nIndex] = max(kMinPitch, MSParam[nIndex] - 10);
					break;
			}
			break;

		case SB_PAGEDOWN :
			switch ( nCtrlID ) {
				case IDD_SB_DELAY:
					MSParam[nIndex] = min(kMaxDelay,MSParam[nIndex] + 20);
					break;
				case IDD_SB_VEL:
					MSParam[nIndex] = min(kMaxVel,MSParam[nIndex] + 10);
					break;
				case IDD_SB_PITCH:
					MSParam[nIndex] = min(kMaxPitch,MSParam[nIndex] + 10);
					break;
			}
			break;

		case SB_LINEUP :
			switch ( nCtrlID ) {
				case IDD_SB_DELAY:
					MSParam[nIndex] = max(kMinDelay,MSParam[nIndex] - 1);
					break;
				case IDD_SB_VEL:
					MSParam[nIndex] = max(kMinVel,MSParam[nIndex] - 1);
					break;
				case IDD_SB_PITCH:
					MSParam[nIndex] = max(kMinPitch,MSParam[nIndex] - 1);
					break;
			}
			break;

		case SB_LINEDOWN :
			switch ( nCtrlID ) {
				case IDD_SB_DELAY:
					MSParam[nIndex] = min(kMaxDelay,MSParam[nIndex] + 1);
					break;
				case IDD_SB_VEL:
					MSParam[nIndex] = min(kMaxVel,MSParam[nIndex] + 1);
					break;
				case IDD_SB_PITCH:
					MSParam[nIndex] = min(kMaxPitch,MSParam[nIndex] + 1);
					break;
			}
			break ;

		case SB_TOP:
			switch ( nCtrlID ) {
				case IDD_SB_DELAY:
					MSParam[nIndex] = kMaxDelay;
					break;
				case IDD_SB_VEL:
					MSParam[nIndex] = kMaxVel;
					break;
				case IDD_SB_PITCH:
					MSParam[nIndex] = kMaxPitch;
					break;
			}
			break;

		case SB_BOTTOM :
			switch ( nCtrlID ) {
				case IDD_SB_DELAY:
					MSParam[nIndex] = kMinDelay;
					break;
				case IDD_SB_VEL:
					MSParam[nIndex] = kMinVel;
					break;
				case IDD_SB_PITCH:
					MSParam[nIndex] = kMinPitch;
					break;
			}
			break;

		case SB_THUMBPOSITION :
		case SB_THUMBTRACK :
				MSParam[nIndex] = tp;
				break ;
	}
}

/* ------------------------------------------------------------------
				  Vertical scrolling management
	------------------------------------------------------------------ */
void VScroll (short nCtrlID, UINT message, short tp)
{
	short	nIndex;

	if ( nCtrlID == IDD_SB_CHAN) {
		nIndex  = nCtrlID - 200;
		switch (message) {
			case SB_PAGEUP :
			case SB_LINEUP :
				MSParam[nIndex] = min(kMaxChan,MSParam[nIndex] + 1);
				break;
			case SB_PAGEDOWN :
			case SB_LINEDOWN :
				MSParam[nIndex] = max(kMinChan,MSParam[nIndex] - 1);
				break ;
			case SB_TOP:
				MSParam[nIndex] = kMaxChan;
				break;
			case SB_BOTTOM :
				MSParam[nIndex] = kMinChan;
				break;
			case SB_THUMBPOSITION :
			case SB_THUMBTRACK :
				MSParam[nIndex] = kMaxChan - tp;
				break ;
		}
	}
}


//________________________________________________________________________________
BOOL ProcessPrivateMsg (HWND hDlg, UINT message, UINT wParam, LONG lParam)
{
	char buff [32], *newName=0;

	if (message==gNewMsg) {
		if (gAppIndex==1) {
			MidiSetName (myRefNum, MakeInstName(buff,AppliName,1)); 
			SetWindowText(hDlg, MidiGetName(myRefNum));
		}
	}
	else if (message==gQuitMsg) {
		if (gInstanceCount==1) {
			gAppIndex = 1;
			newName = AppliName;
		}
		else if ((wParam < gAppIndex) || (gInstanceCount==1)) {
			gAppIndex--;
			newName = MakeInstName(buff,AppliName,gAppIndex);
		}
		if (newName) {
			MidiSetName (myRefNum, newName); 
			SetWindowText(hDlg, MidiGetName(myRefNum));
		}
	}
	else return FALSE;
	return TRUE;
}

//________________________________________________________________________________
BOOL MSALARMAPI msEchoDlgProc(HWND hDlg, UINT message, UINT wParam, LONG lParam)
{
	HWND		hCtrl;
	short		nCtrlID, nIndex;

#ifdef WIN32
	hCtrl= (HWND)lParam;
	nCtrlID = (short)GetWindowLong (hCtrl,GWL_ID);
	if (ProcessPrivateMsg (hDlg, message, wParam, lParam))
		return TRUE;
#else
	hCtrl= HIWORD(lParam);
	nCtrlID = GetWindowWord (hCtrl,GWW_ID);
#endif
	nIndex  = nCtrlID - 200;
	switch (message) {

		case WM_INITDIALOG:
			return Initialize (hDlg);

		case WM_HSCROLL :
#ifdef WIN32
			HScroll (nCtrlID, LOWORD(wParam), HIWORD(wParam));
#else
			HScroll (nCtrlID, wParam, LOWORD(lParam));
#endif
			SetScrollPos	(hCtrl, SB_CTL, MSParam[nIndex], TRUE);
			SetDlgItemInt	(hDlg, nCtrlID-100, MSParam[nIndex], TRUE);
			break;

		case WM_VSCROLL :
#ifdef WIN32
			VScroll (nCtrlID, LOWORD(wParam), HIWORD(wParam));
#else
			VScroll (nCtrlID, wParam, LOWORD(lParam));
#endif
			SetScrollPos	(hCtrl, SB_CTL, MSParam[kChan],TRUE);
			if (MSParam[nIndex]==0)
				SetDlgItemText	(hDlg, nCtrlID-100, kAllChan);
			else
				SetDlgItemInt	(hDlg, nCtrlID-100, MSParam[nIndex],TRUE);
			break;

		case WM_COMMAND:
			switch (wParam) {
				case AboutButton :
					DialogBox(ghInst, "AboutDlg", hDlg, AboutDlgProcInst);
					break;
				case IDD_QUITTER :
            	SaveState();
					if (abouthDlg) EndDialog(abouthDlg,TRUE);
					EndDialog(hDlg,TRUE);
					break;
         }
			break;

		case WM_MOVE:
			dlgXPos= LOWORD(lParam) - 5;
			dlgYPos= HIWORD(lParam) - 24;
			break;
	}
	return FALSE ;
}

/* ----------------------------------------------------------------------------

MidiShare setup

---------------------------------------------------------------------------- */
Boolean SetUpMidi(void)
{
  char buff[32], *name;

  if ( !MidiShare() ) {
		AlertUser("MidiShare not available");
		return false;
  }
  name = gAppIndex > 1 ? MakeInstName (buff,AppliName,gAppIndex) : AppliName;
  myRefNum = MidiOpen(name);
  if ( myRefNum == MIDIerrSpace ) {
		AlertUser("Too much MidiShare client applications");
		return false;
  }
  MidiSetRcvAlarm(myRefNum,ReceiveEvents);
  MidiConnect (myRefNum, 0, true);
  MidiConnect (0, myRefNum, true);
  return true;
}


/* --------------------------------------------------------

Alert Messages

-------------------------------------------------------- */
void NEAR AlertUser(LPCSTR mess)
{
	MessageBox (ghWnd, mess, AppliName,MB_OK|MB_ICONSTOP);
	SendMessage(ghWnd, WM_SYSCOMMAND,SC_CLOSE,0L);
}

/* -----------------------------------------------------
Application receive alarm (called at interrupt level)
-------------------------------------------------------- */
void MSALARMAPI ReceiveEvents(register short r)
{
  register MidiEvPtr e;
  register unsigned long d;

  while (e = MidiGetEv(r)) {
	 d = Date(e) + MSParam[kDelay];
	 if ( (MSParam[kChan]!=0) && (MSParam[kChan]!=Chan(e)+1) )
		MidiFreeEv(e);
	 else if (Vel(e) == 0)
		MidiFreeEv(e);
	 else switch ( EvType(e) ) {
		case typeKeyOn:
			EvType(e) = typeNote;
			Dur(e) = 100;
		case typeNote:
			if( !MidiTask(MyEchoTask,d,r,(long)e,0,0))
				MidiFreeEv(e);
			break;
		default:
			MidiFreeEv(e);
	 }
  }
}

/* --------------------------------------------------------
Real time echo management task
-------------------------------------------------------- */
void MSALARMAPI MyEchoTask(long d,short ref,long el, long unused3,long unused4)
{
  MidiEvPtr e;
  unsigned char v,p;

  e= (MidiEvPtr)el;
  v = Vel(e)+MSParam[kVel];
  p = Pitch(e)+MSParam[kPitch];
  if ( ((v>0)&&(v<128)) && ((p>=0)&&(p<128)) ) {
	 Vel(e) = v;
	 Pitch(e) = p;
	 MidiSendAt(ref, MidiCopyEv(e), d);
	 if( !MidiTask(MyEchoTask, d+MSParam[kDelay], ref, el, 0, 0))
		MidiFreeEv(e);
  }
  else MidiFreeEv(e);
}

//_____________________________________________________________________________
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
					EndDialog(hDlg,0);
					abouthDlg = 0 ;
					break;
			}
			break;
	}
	return FALSE ;
}
