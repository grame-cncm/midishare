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
#include "msDrivers.h"

// ------------------------------------------------------------------
// Constants  - types
// ------------------------------------------------------------------

#define kApplAlarm	WM_USER

#define kMaxAppls		64
#define kMaxSlots		256
#define kMaxNameLen		32
#define noRef			-1

#define kDefaultXPos	10
#define kDefaultYPos	30
enum { false, true };

typedef short	RefList	[kMaxAppls];
HBRUSH  gWhite, gRed, gBlue, gGreen;
short gSelected=0; SlotRefNum gInfos; 
BOOL gTextOn;

enum {
	kxBox=145, kyBox=22, kwBox=160, khBox=160,
	kwPort=10, khPort=10, kIn=2
};

// ------------------------------------------------------------------
// PROTOTYPES
// ------------------------------------------------------------------
void 	 	AlertUser(LPCSTR);
Boolean  	SetUpMidi(void);
void		ShowConnections (HWND hDlg, short port);
void		ShowSlotInfos (HWND hDlg, short index, BOOL in, BOOL selected);

void MSALARMAPI ApplAlarm(short,long);
BOOL CALLBACKAPI msCnctDlg(HWND hDlg, UINT message, UINT wParam, LONG lParam);
BOOL CALLBACKAPI AboutDlgProc(HWND hDlg,UINT message,
													UINT wParam,LONG lParam);


// ------------------------------------------------------------------
// VARIABLES
// ------------------------------------------------------------------
DWORD	gInstanceCount = 0;
HINSTANCE	ghInst;
HWND		abouthDlg;
HWND	FAR ghDlg;
DLGPROC		AboutDlgProcInst;

RefList	gRefNums;
SlotRefNum gInSlots [kMaxSlots], gOutSlots [kMaxSlots];
short		myRefNum, selectedRef= noRef;
MidiFilterPtr	myFilter;

UINT		gStartupMsg;
MidiName	AppliName = "msDrivers";
char * profileName = "msDrivers.ini";
char * sectionName = "Window position";
char * xEntryName  = "xpos";
char * yEntryName  = "ypos";
WORD 	dlgXPos, dlgYPos;

//________________________________________________________________________
void SaveState (void)
{
	char buff[30];

	wsprintf (buff, "%d", dlgXPos);
	WritePrivateProfileString (sectionName, xEntryName, buff, profileName);
	wsprintf (buff, "%d", dlgYPos);
	WritePrivateProfileString (sectionName, yEntryName, buff, profileName);
}

//________________________________________________________________________
void LoadState (void)
{
	dlgXPos = GetPrivateProfileInt (sectionName, xEntryName, kDefaultXPos, profileName);
	dlgYPos = GetPrivateProfileInt (sectionName, yEntryName, kDefaultYPos, profileName);
	if (!dlgXPos || (dlgXPos < 0)) dlgXPos =  kDefaultXPos;
	if (!dlgYPos || (dlgYPos < 0)) dlgYPos =  kDefaultYPos;
}

/* ------------------------------------------------------------------
			application main
------------------------------------------------------------------ */
int PASCAL WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
						  LPSTR lpszCmdLine, int nCmdShow)
{
  int ret; DWORD err;
  if ( hPrevInstance ) return FALSE ;
  gStartupMsg = RegisterWindowMessage("msDriverToFront");
  if (gInstanceCount) {
	PostMessage (HWND_BROADCAST, gStartupMsg, 0, 0);
	return TRUE;
  }
  InterlockedIncrement (&gInstanceCount);
  ghInst = hInstance;
  if ( !SetUpMidi() ) return FALSE;
  ret = DialogBox(hInstance, "CNCTDLG", NULL, MakeProcInstance(msCnctDlg,hInstance));
  if (ret == -1) err = GetLastError ();
  FreeProcInstance((FARPROC)msCnctDlg);
  SaveState();
  MidiClose(myRefNum);
  MidiFreeFilter (myFilter);
  return TRUE;
}


/* ------------------------------------------------------------------
				initialization
------------------------------------------------------------------ */
void ChangeName (HWND hDlg, short index, char *name)
{
	SendDlgItemMessage(hDlg, IDD_LB_SRC, LB_INSERTSTRING,index,(LPARAM)name);
	SendDlgItemMessage(hDlg, IDD_LB_SRC, LB_DELETESTRING,index+1,0);

	SendDlgItemMessage(hDlg, IDD_LB_DST, LB_INSERTSTRING,index,(LPARAM)name);
	SendDlgItemMessage(hDlg, IDD_LB_DST, LB_DELETESTRING,index+1,0);
}

//________________________________________________________________________
void AddSlots (HWND hDlg, short ref)
{
	TDriverInfos infos; LRESULT n;
	if (MidiGetDriverInfos (ref, &infos)) {
		short i; SlotRefNum sref; TSlotInfos sinf;
		for (i=1; i<=infos.slots; i++) {
			sref = MidiGetIndSlot (ref, i);
			if (MidiGetSlotInfos (sref, &sinf)) {
				if (sinf.direction & MidiInputSlot) {
					n = SendDlgItemMessage(hDlg, IDD_LB_SRC, LB_GETCOUNT,0,0);
					SendDlgItemMessage(hDlg, IDD_LB_SRC, LB_ADDSTRING,0,(LPARAM)sinf.name);
					if (n < kMaxSlots) gInSlots[n] = sref;
				}
				if (sinf.direction & MidiOutputSlot) {
					n = SendDlgItemMessage(hDlg, IDD_LB_DST, LB_GETCOUNT,0,0);
					SendDlgItemMessage(hDlg, IDD_LB_DST, LB_ADDSTRING,0,(LPARAM)sinf.name);
					if (n < kMaxSlots) gOutSlots[n] = sref;
				}
			}
		}
	}
}

//________________________________________________________________________
BOOL Initialize (HWND hDlg)
{
	short i, r;
	RECT rect = {kxBox, kyBox, kxBox+kwBox, kyBox+khBox};
	gSelected= 0;
	gInfos.slotRef = gInfos.drvRef = 0;
	gTextOn = false;
	SendDlgItemMessage(hDlg, IDD_LB_SRC, LB_RESETCONTENT,0,0L);
	SendDlgItemMessage(hDlg, IDD_LB_DST, LB_RESETCONTENT,0,0L);
	for (i=0; i<kMaxAppls; i++)
		gRefNums[i] = noRef;
	for (i=0; i<MidiCountDrivers(); i++) {
		gRefNums[i] = r = MidiGetIndDriver((short)(i+1));
		AddSlots (hDlg, r);
	}
	gWhite = CreateSolidBrush(RGB (0xff,0xff,0xff));
	gRed = CreateSolidBrush(RGB (0xff,0,0));
	gBlue = CreateSolidBrush(RGB (0,0,0xff));
	gGreen = CreateSolidBrush(RGB (0,0xff,0));
	SetDlgItemText (hDlg, IDC_INFOS, "");
	InvalidateRect (hDlg, &rect, TRUE);
	return TRUE ;
}

/* ------------------------------------------------------------------
	Convert an index to a slot reference number and so on...
	------------------------------------------------------------------ */
SlotRefNum Index2MSRef (short index, BOOL in)
{
	SlotRefNum bad = { 0, -1 };
	if (index >= kMaxSlots) return bad;
	return  in ? gInSlots[index] : gOutSlots[index];
}

short MSRef2Index (HWND hDlg, SlotRefNum ref, BOOL in)
{
	SlotRefNum * tbl = in ? gInSlots : gOutSlots;
	short i=0;
	LRESULT n = SendDlgItemMessage(hDlg, in ? IDD_LB_SRC : IDD_LB_DST, LB_GETCOUNT,0,0);

	while (i < n) {
		if ((tbl->drvRef == ref.drvRef) && (tbl->slotRef == ref.slotRef))
			return i;
		tbl++;
		i++;
	}
	return -1;
}

/* ------------------------------------------------------------------
	User connections  management
	------------------------------------------------------------------ */
void CnxManagement (HWND hDlg, WORD ctrl)
{
	SlotRefNum ref; short sel;

	switch (ctrl) {
		case IDD_LB_SRC:
			sel = (short)SendDlgItemMessage (hDlg, IDD_LB_SRC, LB_GETCARETINDEX,0,0L);
			if (gSelected) {
				short port = gSelected-1;
				ref = Index2MSRef (sel, true);
				if (ref.slotRef >= 0)
					MidiConnectSlot (port, ref, (Boolean)!MidiIsSlotConnected (port, ref));
			}
			else ShowSlotInfos (hDlg, sel, true, 
				(BOOL)SendDlgItemMessage (hDlg, IDD_LB_SRC, LB_GETSEL,sel,0L));
			break;
		case IDD_LB_DST :
			sel = (short)SendDlgItemMessage (hDlg, IDD_LB_DST, LB_GETCARETINDEX,0,0L);
			if (gSelected) {
				short port = gSelected-1;
				ref = Index2MSRef (sel, false);
				if (ref.slotRef >= 0)
					MidiConnectSlot (port, ref, (Boolean)!MidiIsSlotConnected (port, ref));
			}
			else ShowSlotInfos (hDlg, sel, false,
				(BOOL)SendDlgItemMessage (hDlg, IDD_LB_DST, LB_GETSEL,sel,0L));
			break;
	}
}

//_____________________________________________________________________________
BOOL ProcessPrivateMessage (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == kApplAlarm) {
		short ref = LOWORD(lParam);
		switch ( wParam ) {
			case MIDIChgSlotName:
			case MIDIOpenDriver:
			case MIDICloseDriver:
			case MIDIAddSlot:
			case MIDIRemoveSlot:
				Initialize (hDlg);
				break;
			case MIDIChgSlotConnect:
				ShowConnections (hDlg, gSelected);
				break;
		}
	}
	else if (message == gStartupMsg) {
		if (IsIconic(hDlg))
			ShowWindow (hDlg, SW_RESTORE);
		else
			SetForegroundWindow (hDlg);
	}
	else return FALSE;
	return TRUE;
}

//_____________________________________________________________________________
void DrawPorts (HWND hDlg)
{
	RECT rect = {kxBox, kyBox, kxBox+kwBox, kyBox+khBox};
	HDC dc = GetDC (hDlg);
	short i, j, offset=0, n=0;
	InflateRect (&rect, 1, 1);
	FrameRect (dc, &rect, gSelected ? gRed : gWhite);
	InflateRect (&rect, -1, -1);
	for (i=0; i<16; i++) {
		SetRect(&rect, kxBox, kyBox+offset, kxBox+kwPort, kyBox+khPort+offset);
		InflateRect (&rect, -kIn, -kIn);
		for(j=0; j<16; j++){
			n++;
			FillRect (dc, &rect, (n==gSelected) ? gRed :
			(gInfos.drvRef && MidiIsSlotConnected ((short)(n-1), gInfos)) ? gGreen : gWhite);
			OffsetRect (&rect, kwPort, 0);
		}
		offset += kwPort;
	}
}

//_____________________________________________________________________________
BOOL FindPort (short x, short y, short *port)
{
	POINT p = { x, y };
	RECT rect = {kxBox, kyBox, kxBox+kwBox, kyBox+khBox};
	short xoffset, yoffset;
	if (PtInRect (&rect, p)) {
		x -= kxBox + kIn;
		y -= kyBox + kIn;
		xoffset = x / kwPort;
		yoffset = y / khPort;
		if ((x % kwPort) > (kwPort - kIn*2)) return false;
		if ((y % khPort) > (khPort - kIn*2)) return false;
		*port = (16 * yoffset) + xoffset;
		return true;
	}
	return false;
}

//_____________________________________________________________________________
void TrackMouse (HWND hDlg, short x, short y)
{
	short port;
	if (FindPort (x, y, &port)) {
		SetDlgItemInt (hDlg, IDC_INFOS, port, false);
		gTextOn = true;
	}
	else if (gTextOn) {
		SetDlgItemText (hDlg, IDC_INFOS, "");
		gTextOn = false;
	}
}

//_____________________________________________________________________________
void HandleKeyDown (HWND hDlg, WPARAM key)
{
	if (key == 0) DrawPorts (hDlg);
}

//_____________________________________________________________________________
void ShowSlotInfos (HWND hDlg, short i, BOOL in, BOOL selected)
{
	static short selItem = 0;
	static BOOL inItem;
	RECT rect = {kxBox, kyBox, kxBox+kwBox, kyBox+khBox};
	SlotRefNum ref = Index2MSRef (i, in);
	if (ref.drvRef > 0) {
		if (gInfos.drvRef) {
			SendDlgItemMessage(hDlg, inItem ? IDD_LB_SRC : IDD_LB_DST, LB_SETSEL, false, MAKELPARAM(selItem,0));
			gInfos.drvRef = gInfos.slotRef = 0;
		}
		if (selected) {
			gInfos.drvRef = ref.drvRef;
			gInfos.slotRef = ref.slotRef;
			selItem = i;
			inItem = in;
		}
		gSelected = 0;
		DrawPorts (hDlg);
	}
}

//_____________________________________________________________________________
void ShowConnections (HWND hDlg, short port)
{
	LRESULT in = SendDlgItemMessage(hDlg, IDD_LB_SRC, LB_GETCOUNT,0,0);
	LRESULT out = SendDlgItemMessage(hDlg, IDD_LB_DST, LB_GETCOUNT,0,0);
	short i;
	if (port) {
		WPARAM param;
		port--;
		for (i=0; i<in; i++) {
			param = MidiIsSlotConnected(port, Index2MSRef(i, true));
			SendDlgItemMessage(hDlg, IDD_LB_SRC, LB_SETSEL, param, MAKELPARAM(i,0));
		}
		for (i=0; i<out; i++) {
			param = MidiIsSlotConnected(port, Index2MSRef(i, false));
			SendDlgItemMessage(hDlg, IDD_LB_DST, LB_SETSEL, param, MAKELPARAM(i,0));
		}
	}
	else {
		for (i=0; i<in; i++)
			SendDlgItemMessage(hDlg, IDD_LB_SRC, LB_SETSEL, FALSE,MAKELPARAM(i,0));
		for (i=0; i<out; i++)
			SendDlgItemMessage(hDlg, IDD_LB_DST, LB_SETSEL, FALSE,MAKELPARAM(i,0));
	}
}

//_____________________________________________________________________________
void TrackMouseDown (HWND hDlg, short x, short y)
{
	short port; 
	if (FindPort (x, y, &port)) {
		HDC dc = GetDC (hDlg);
		gSelected = (port == gSelected-1) ? 0 : port + 1;
		gInfos.drvRef = 0;

		DrawPorts (hDlg);
	}
	ShowConnections (hDlg, gSelected);
}

/* ------------------------------------------------------------------
				main window management
------------------------------------------------------------------ */
BOOL CALLBACKAPI msCnctDlg (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ProcessPrivateMessage (hDlg, message, wParam, lParam))
		return TRUE;

	switch (message) {
		case WM_INITDIALOG:
			ghDlg = hDlg;
			SetClassLong (hDlg, GCL_HICON, (LONG)LoadIcon (ghInst, "ICON"));
			SendDlgItemMessage(hDlg, AboutButton, BM_SETIMAGE, 0, (LONG)LoadBitmap (ghInst, (char *)1800));	
			AboutDlgProcInst= (DLGPROC)MakeProcInstance((FARPROC)AboutDlgProc,ghInst);
			LoadState();
			SetWindowPos(hDlg, HWND_TOP, dlgXPos, dlgYPos,0,0,SWP_NOSIZE);
			return Initialize (hDlg);

		case WM_PAINT:
			DrawPorts (hDlg);
			return FALSE;
		case WM_MOUSEMOVE:
			TrackMouse (hDlg, LOWORD(lParam), HIWORD(lParam)); 
			return FALSE;
		case WM_LBUTTONDOWN:
			TrackMouseDown (hDlg, LOWORD(lParam), HIWORD(lParam)); 
			return FALSE;

		case WM_COMMAND:
			switch (LOWORD(wParam)){

				case IDD_QUITTER:
					if (abouthDlg)
						SendMessage (abouthDlg, WM_COMMAND, IDOK, 0);
					EndDialog(hDlg,TRUE);
					break;
				case AboutButton :
					DialogBox(ghInst, "AboutDlg", hDlg, AboutDlgProcInst);
					break;

				default:
					if (HIWORD(wParam)==LBN_SELCHANGE)
						CnxManagement (hDlg, LOWORD(wParam));
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
			Alert messages
---------------------------------------------------------------------------- */
void NEAR AlertUser(LPCSTR mess)
{
	MessageBox(ghDlg, mess, AppliName,MB_OK|MB_ICONSTOP);
	SendMessage(ghDlg,WM_SYSCOMMAND,SC_CLOSE,0L);
}

/*---------------------------------------------------------------------------
		MidiShare part
  --------------------------------------------------------------------------- */
void NEAR InstallFilter(short refNum)
{
	short i;
	myFilter = MidiNewFilter ();
	if (myFilter) {
		for (i=0;i<256;i++) {
			MidiAcceptPort(myFilter, i, false);
			MidiAcceptType(myFilter, i, false);
		}
		for (i=0;i<16;i++)
			MidiAcceptChan(myFilter, i, false);
		MidiSetFilter(refNum,myFilter);
	}
}

/*-------------------------------------------------------------------------- */
Boolean NEAR SetUpMidi()
{
	if ( !MidiShare() ) {
		AlertUser("Can't access to 'mshare32.dll'");
		return false;
	}
	if ( MidiGetVersion() < 184 ) {
		AlertUser("MidiShare version 1.84 or later required");
		return false;
	}
	myRefNum = MidiOpen(AppliName); 		// open a MidiShare application
	if ( myRefNum == MIDIerrSpace ) {
		AlertUser("Too much MidiShare client applications");
		return false;
	}
	InstallFilter(myRefNum);
	MidiSetApplAlarm(myRefNum, ApplAlarm);
	return true;
}

/* ----------------------------------------------------------------------------
		MidiShare application alarm
---------------------------------------------------------------------------- */
void MSALARMAPI ApplAlarm(short refNum,long code) {
  PostMessage(ghDlg, kApplAlarm, LOWORD(code), HIWORD(code));
}

BOOL CALLBACKAPI AboutDlgProc(HWND hDlg,UINT message, UINT wParam,LONG lParam)
{
	char buff[10]; BOOL ret;

	switch (message) {
		case WM_INITDIALOG : {
			abouthDlg = hDlg;
			wsprintf(buff,"%d.%02d",MidiGetVersion()/100,MidiGetVersion()%100);
			ret = SetDlgItemText(hDlg, Version, buff);
			return TRUE ;
		}
		case WM_COMMAND : {
			switch(wParam) {
				case IDOK :
					EnableWindow( GetDlgItem(ghDlg,AboutButton),true);
					EndDialog(hDlg,0);
					abouthDlg = 0 ;
					return TRUE;
			}
		}
		break;
	}
	return FALSE ;
}


