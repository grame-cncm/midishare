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

*/

#include <windows.h>
#include <MidiShare.h>
#include "RadioHD.h"
#include "RadioHDFeedback.h"
#include "msInetDriverAppl.h"

// ------------------------------------------------------------------
// CONSTANTS - DATA TYPES
// ------------------------------------------------------------------
typedef char ApplState[8], *ApplStatePtr;

#define kDefaultXPos 100
#define kDefaultYPos 100

#define WriteState WritePrivateProfileString
#define GetState	 GetPrivateProfileInt

#define IDB_BITMAP1	103

// ------------------------------------------------------------------
// FUNCTIONS PROTOTYPES
// ------------------------------------------------------------------
void AlertUser(LPCSTR);
void MSALARMAPI ReceiveEvents(register short);
BOOL CALLBACKAPI RDHDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

void LoadApplState (ApplStatePtr state);
void SaveApplState (ApplStatePtr state);

// ------------------------------------------------------------------
// VARIABLES
// ------------------------------------------------------------------
long	gInstanceCount = 0;
UINT	gStartupMsg;
HINSTANCE	ghInst;
HBITMAP		gBitmap;
HWND	ghWnd; HFONT gFont;
RadioHDFeedback * gFeedback;

State gState;
ApplState gApplState;
Boolean doneFlag = false;

// MidiShare variables
MidiName AppliName = "RDH-monitor";

// for private profile file
char * profileName = "RadioHardDisk.ini";
char * sectionName = "Window position";
char * xEntryName  = "xpos";
char * yEntryName  = "ypos";
int	dlgXPos,dlgYPos;


/* ------------------------------------------------------------------
APPLICATION MAIN ENTRY POINT
------------------------------------------------------------------ */
int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
			LPSTR lpszCmdLine, int nCmdShow)
{
	int ret;

	if ( hPrevInstance ) return FALSE ;
	gStartupMsg = RegisterWindowMessage("RDHToFront");
	if (gInstanceCount) {
		PostMessage (HWND_BROADCAST, gStartupMsg, 0, 0);
		return TRUE;
	}
	InterlockedIncrement (&gInstanceCount);

	ghInst = hInstance;
	gBitmap = LoadBitmap (hInstance, MAKEINTRESOURCE(IDB_BITMAP1));
	ret=DialogBox(hInstance, "RDH", NULL, MakeProcInstance(RDHDlgProc,hInstance));
	CloseInetDriver ();
	delete gFeedback;
	return TRUE;
}

//_______________________________________________________________________
static DWORD WINAPI idle_loop (LPVOID ptr)
{
	RadioHDFeedback * fdb = (RadioHDFeedback *)ptr;
	while (!doneFlag) {
		DoIdle ();
		Sleep (100);
	}
	return 0;
}

//____________________________________________________________
static void CreateIdleThread ()
{
	DWORD id;
	CreateThread(NULL, 0, idle_loop, gFeedback, 0, &id);
}

/* ------------------------------------------------------------------
					Initializations
------------------------------------------------------------------ */
static void InitState (StatePtr g)
{
	g->driverMode= false;
	g->showWindow= true;
	g->autoQuit= false;
	g->net.port = kDefaultWANPort;
	g->net.maxlat = kDefaultWANLatency;
	g->net.groupTime = kDefaultWANGroupTime;
}

Boolean Initialize(HWND hDlg)
{
	char buff [255], *msg;
	// Associate an icon with the dialog box.
	HICON icon = LoadIcon (ghInst, "ICON");
	DWORD ret = SetClassLong (hDlg, GCL_HICON, (LONG)icon);
	LoadApplState (gApplState);
	SetWindowPos (hDlg, HWND_TOP, dlgXPos, dlgYPos,0,0,SWP_NOSIZE);
	gFeedback = new RadioHDFeedback (hDlg);
	if (!gFeedback) return false;
	InitState (&gState);
	GetTextFace (GetDC (hDlg), 255, buff);
	gFont = CreateFont( 12,5, 0, 0,
			FW_NORMAL, FALSE, FALSE, FALSE,
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, FF_ROMAN, "Times");
	msg = SetUpDriver(&gState, gFeedback);
	if (msg) {
		AlertUser (msg);
		return false;
	}
	CheckPort0Connection ();
	CreateIdleThread ();
	return true;
}

void DrawBitmap (HDC dst)
{
	HDC hdcMem = CreateCompatibleDC(NULL);
	HBITMAP hbmT = (HBITMAP)SelectObject(hdcMem, gBitmap);  
	BITMAP bm;
	GetObject(gBitmap,sizeof(bm),&bm);  
	BitBlt(dst,0,0,bm.bmWidth,bm.bmHeight,hdcMem,0,0,SRCCOPY);  
	SelectObject(hdcMem, hbmT);  
	DeleteDC(hdcMem);
}

/* ------------------------------------------------------------------
					main window management
------------------------------------------------------------------ */
BOOL CALLBACKAPI RDHDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT p; HDC hdc;
	if (message == gStartupMsg) {
		if (IsIconic(hDlg))
			ShowWindow (hDlg, SW_RESTORE);
		else
			SetForegroundWindow (hDlg);
	}
	else switch (message) {
		case WM_INITDIALOG:
			ghWnd= hDlg;
			return Initialize (hDlg);

		case WM_MOVE:
			if (!IsIconic (hDlg)) {
				dlgXPos= LOWORD(lParam) - 5;
				dlgYPos= HIWORD(lParam) - 24;
			}
			break;

		case WM_PAINT:
			hdc = BeginPaint (hDlg, &p);
			DrawBitmap (hdc);
			gFeedback->DoRedraw ();
			EndPaint (hDlg, &p);
			return FALSE;

		case WM_COMMAND:
			switch ( wParam ) {
				case IDD_QUITTER :
					doneFlag = true;
					SaveApplState(gApplState);
					EndDialog(hDlg,TRUE);
					break;
			}
			break;
	}
	return FALSE ;
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
 Private profile functions
---------------------------------------------------------------------------- */
void SaveApplState (ApplStatePtr state)
{
	char buff[30];

	wsprintf (buff, "%d", dlgXPos);
	WriteState (sectionName, xEntryName, buff, profileName);
	wsprintf (buff, "%d", dlgYPos);
	WriteState (sectionName, yEntryName, buff, profileName);
}

//________________________________________________________________________
void LoadApplState (ApplStatePtr state)
{
	int x, y;

	x = GetState (sectionName, xEntryName, kDefaultXPos, profileName);
	y = GetState (sectionName, yEntryName, kDefaultYPos, profileName);
	dlgXPos =  x ? x : kDefaultXPos;
	dlgYPos =  y ? y : kDefaultYPos;
}
