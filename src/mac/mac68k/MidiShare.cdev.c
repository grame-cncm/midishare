/*

  Copyright © Grame 1999-2000

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

/*
   MidiShare cdev
   --------------------------------------------------------------------------------
   Display the MidiShare version number. 
   Allows to specify the MidiShare memory size (allocated at the first MidiOpen).
   The cdev stores its preferences in the LVAL ressource.
 */

#include <Devices.h>
#include <Dialogs.h>
#include <Events.h>
#include <Gestalt.h>
#include <Memory.h>
#include <Packages.h>
#include <Resources.h>
#include <Sound.h>
#include <TextUtils.h>
#include <ToolUtils.h>
#include <Types.h>

#include "LVAL.h"
#include "MidiShareMac.h"
#include "msKernel.h"
#include "SignatureToApp.h"

pascal unsigned long main (short message, short item, short numItems, short CPanelID,
					EventRecord *theEvent, unsigned long cdevValue, DialogPtr CPDialog);
void MidiPrintf (char *string);
void MidiPrintn (long n);

/*_______________________________________________________________________*/
/* constants definition */
enum {
	kLVal		= -4048,			/* LVal resource			*/
	kStrings		= -4064,		/* str#						*/
	kIcon			= -4064,		/* cicon					*/
	kNoMidiShare	= 1
};

enum {	
	iIcon		= 1,			/* MidiShare icon			*/
	iMidiShare,					/* MidiShare title			*/
	iVersion,					/* Version number			*/
	iMemory1,					/* static text				*/
	iMemory2,					/* static text				*/
	iEvents,					/* Events count				*/
	iSize,						/* Allocated memory size	*/
	iMsg,						/* bottom message			*/
	iSetup						/* setup button				*/
};

#define DriverManagerSign	'SlCt'

#define kDelete		0x08		/* character codes for various keys */
#define leftArrow	0x1c
#define rightArrow	0x1d
#define upArrow	0x1e
#define downArrow	0x1f

/*_______________________________________________________________________*/
/* data structure and type definitions */
typedef struct {
	DialogPtr	dlgPtr;	
	short		dlgItems;
	long		LVal;				/* desired allocated events		*/
	long		oldLVal;			/* previous value (for Undo)	*/
	Boolean		hasIcn;				/* icn, colorQD and sys < 7.0	*/
	Handle		icn;				/* cicn							*/
	short 		state;
} Storage, * StoragePtr, ** StorageHdle;

enum { kNotInstalled=1, kUnsupportedVersion, kClear, kChangeDisplay }; 	
#define Supported(version)	((version) >= 180)

static void SetItemText (StorageHdle ourHandle, short item, Str255 text);
Boolean InitCdev (DialogPtr CPdialog, short numItems, StorageHdle * sto);
void 	UpdateCdev (StorageHdle ourHandle);
void 	UpdateSize (StorageHdle ourHandle);
void 	ValidateChanges (StorageHdle ourHandle);
void 	DoEditCommand (StorageHdle ourHandle, short message);
void 	UndoCdev (StorageHdle ourHandle);
void 	DrawCdevItem (StorageHdle ourHandle, short item);
void 	LoadDriverManager (StorageHdle ourHandle);

/*_______________________________________________________________________*/
/*	Main Cdev Function													 */
pascal unsigned long main (short message, short item, short numItems, short CPanelID,
					EventRecord *theEvent, unsigned long cdevValue, DialogPtr CPDialog)
{
	char 			theChar;
	StorageHdle	ourStorage;
	short		itemHit;

	if (message == macDev) {
		return 1;
	}
	else if (cdevValue) {
		ourStorage = (StorageHdle)cdevValue;
		switch (message) {
			case initDev:	
				if (! InitCdev(CPDialog, numItems, &ourStorage)) return 0;
				break;
			case closeDev:
				ValidateChanges (ourStorage);
				if ((*ourStorage)->hasIcn)
					DisposeHandle ((*ourStorage)->icn);
				DisposeHandle ((Handle)ourStorage);
				break;
			case updateDev:	
				UpdateCdev(ourStorage);
				break;
			case hitDev:
				itemHit = item-numItems;
				if (itemHit == iSetup)
					LoadDriverManager (ourStorage);
				break;
			case keyEvtDev:
				theChar = theEvent->message & charCodeMask;
				if (theEvent->modifiers & cmdKey) {
					message = nulDev;
					switch (theChar) {
						case 'x': case 'X': message = cutDev;
							break;
						case 'c': case 'C': message = copyDev;
							break;
						case 'v': case 'V': message = pasteDev;
							break;
						case 'z': case 'Z': message = undoDev;
							break;
					}
					DoEditCommand (ourStorage,message);
				}
				else if ((theChar==kDelete) || ((theChar>=leftArrow) && (theChar<=downArrow)) ||
						((theChar>='0') && (theChar<='9'))) {
					UpdateSize(ourStorage);
					if ((*ourStorage)->state == kClear) {
						SetItemText (ourStorage, iMsg, 
							"\pChanges won't take effect until first MidiOpen.");
						(*ourStorage)->state = kChangeDisplay;
					}
				}
				else {
					message = nulDev;
					theEvent->what = nullEvent;
				}
				break;
			case undoDev:	
				UndoCdev (ourStorage);
				break;
			case cutDev:
			case copyDev:
			case pasteDev:
			case clearDev:	
				DoEditCommand (ourStorage, message);
				break;
		}
		return (unsigned long)ourStorage;
	}
	else return 0;
}

/*_______________________________________________________________________*/
void MidiPrintf (char *string)
{
	MidiEvPtr e = MidiNewEv (typeTextual);
	if( e) {
		 while( *string)
			MidiAddField ( e, *string++);
		MidiSendIm (0, e);
	}
}

/*_______________________________________________________________________*/
void MidiPrintn (long n)
{
	MidiEvPtr e = MidiNewEv (typeTempo);
	if( e) {
		Tempo(e) = n;
		MidiSendIm (0, e);
	}
}

/*_______________________________________________________________________*/
void MidiPrintp (unsigned char * str)
{
	MidiEvPtr e = MidiNewEv (typeTextual);
	if( e) {
		short i, n = str[0];
		for (i=1; i<=n; i++)
			MidiAddField ( e, str[i]);
		MidiSendIm (0, e);
	}
}

/*_______________________________________________________________________*/
/*	items management														  */
static ControlHandle IGetCtlHand (StorageHdle ourHandle, short item, Rect * itemRect)
{
	Handle itemHand;
	short  itemType;
	StoragePtr storage = *ourHandle;
	GetDialogItem (storage->dlgPtr, item+storage->dlgItems, &itemType, &itemHand, itemRect);
	return (ControlHandle)itemHand;
}

static void SetItemText (StorageHdle ourHandle, short item, Str255 text)
{
	Handle h; Rect r; 
	h = (Handle)IGetCtlHand (ourHandle,item, &r);
	if (h) SetDialogItemText (h, text);
}

static void DisableButton (StorageHdle ourHandle, short item)
{
	Handle h; short itemType; Rect r;
	StoragePtr storage = *ourHandle;
	GetDialogItem (storage->dlgPtr, item, &itemType, &h, &r);
	if (h) {
		itemType += 128;
		SetDialogItem(storage->dlgPtr, item, itemType, h, &r);
	}
}

static void SetItemValue (StorageHdle ourHandle, short item, long val)
{
	Handle h; Rect r; Str255 str;
	StoragePtr storage = *ourHandle;

	NumToString (val,str);
	h = (Handle)IGetCtlHand (ourHandle,item, &r);
	if (h) {
		SetDialogItemText (h,str);
		SelectDialogItemText (storage->dlgPtr, item + storage->dlgItems, 0, 999);
		TEUpdate (&r, ((DialogPeek)(storage->dlgPtr))->textH);
	}
}

static long GetItemValue (StorageHdle ourHandle, short item)
{
	Handle h; Rect r; Str255 str; long val;
	StoragePtr storage = *ourHandle;

	h = (Handle)IGetCtlHand (ourHandle,item, &r);
	GetDialogItemText (h,str);
	StringToNum (str, &val);
	return val;
}

/*_______________________________________________________________________*/
void LoadDriverManager (StorageHdle ourStorage)
{
	if ((*ourStorage)->state >= kClear) {
		Boolean launched; OSErr err;
		err = SignatureToApp (DriverManagerSign, 0, 0, &launched, 
							Sig2App_LaunchApplication, launchContinue);
		if ((err != noErr) || !launched) {
			SetItemText (ourStorage, iMsg, "\pMidiShare Drivers Manager not found");
			SysBeep (30);
		}
	}
}

/*_______________________________________________________________________*/
/*	changes validation										 			 */
void ValidateChanges (StorageHdle ourHandle)
{
	LVALResHdl  LValRes; LVALResPtr LValPtr;
	TMSGlobalPtr * g;
	StoragePtr storage = *ourHandle;

	LValRes = (LVALResHdl)GetResource('LVAL',kLVal);	/* LVAL la resource handle	*/
	if (LValRes) {
		LValPtr = *LValRes;
		LValPtr->memory = storage->LVal;			/* the new memory size 		*/
		ChangedResource ((Handle)LValRes);
		WriteResource((Handle)LValRes);
		if (MidiShare() && (Supported(MidiGetVersion ()))) { /* if MidiShare loaded 		*/
			g = (TMSGlobalPtr *)*kTrap14;
			g--;
			(*g)->memory.desiredSpace = storage->LVal;	/* stores new memory size 	*/
		}
	}
}

#define FailResError(h)	if ((!h) || ResError()) { *sto = (StorageHdle)cdevResErr; return false; }
#define FailMemError(h)	if ((!h) || MemError()) { *sto = (StorageHdle)cdevMemErr; return false; }

/*_______________________________________________________________________*/
/*	cdev initialization 													 	  */
Boolean InitCdev (DialogPtr CPdialog, short numItems, StorageHdle * sto)
{
	Handle  h;  Rect itemRect; 
	LVALResHdl	LValRes; StoragePtr storagePtr;
	SysEnvRec theWorld; TEHandle textH;
	OSErr err; 

	LValRes = (LVALResHdl)GetResource('LVAL', kLVal);		/* LVAL resource handle		*/
	FailResError(LValRes);
	*sto = (StorageHdle)NewHandle (sizeof(Storage));
	FailMemError(sto);
	storagePtr = **sto;

	storagePtr->dlgPtr 	 = CPdialog;
	storagePtr->dlgItems = numItems;
	storagePtr->LVal	 = (*LValRes)->memory;
		
	storagePtr->oldLVal = storagePtr->LVal;				/* saving for Undo or Esc		*/

	textH = ((DialogPeek)storagePtr->dlgPtr)->textH;
	InsetRect (&(*textH)->viewRect,-1,-1);
	InsetRect (&(*textH)->destRect,-1,-1);
	(*textH)->lineHeight = 11;
	(*textH)->fontAscent = 9;
	(*textH)->crOnly	 = -1;

	SetItemValue (*sto, iEvents, storagePtr->LVal);

	storagePtr->hasIcn = false;
	err = SysEnvirons (1, &theWorld);
	if ((HiWord(theWorld.systemVersion) < 7) && theWorld.hasColorQD) {
		storagePtr->icn = (Handle) GetCIcon(kIcon);
		if (storagePtr->icn) {
			h = (Handle)IGetCtlHand (*sto, iIcon, &itemRect);
			SetDialogItem (storagePtr->dlgPtr, storagePtr->dlgItems + iIcon, userItem, nil, &itemRect);
			storagePtr->hasIcn = true;
			MoveHHi(storagePtr->icn);
			HLock(storagePtr->icn);
		}
	}
	return true;
}

/*_______________________________________________________________________*/
/*	cdev management : Update events											  */
void UpdateCdev (StorageHdle ourHandle)
{
	if ((*ourHandle)->hasIcn)
		DrawCdevItem (ourHandle,iIcon);
	DrawCdevItem (ourHandle,iVersion);
	DrawCdevItem (ourHandle,iSize);
}

/*_______________________________________________________________________*/
/*	cdev management : Key down events											  */
void UpdateSize (StorageHdle ourHandle)
{
	Handle h; Rect r;
	GrafPtr savePort;

	GetPort (&savePort);
	h = (Handle)IGetCtlHand (ourHandle, iSize, &r);
	SetPort ((*ourHandle)->dlgPtr);
	InvalRect (&r);
	SetPort (savePort);
}

/*_______________________________________________________________________*/
void DoEditCommand (StorageHdle ourHandle, short message)
{
	DialogPtr dlog = (*ourHandle)->dlgPtr;
	switch (message) {
		case cutDev :	DialogCut (dlog);
			break;
		case copyDev :	DialogCopy (dlog);
			break;
		case pasteDev :	DialogPaste(dlog);
			break;
		case clearDev :	DialogDelete(dlog);
			break;
		case undoDev :	UndoCdev(ourHandle);
			break;
		default:	return;
	}
	if ((message != copyDev) && (message != undoDev))
		UpdateSize(ourHandle);
}			
	
/*_______________________________________________________________________*/
/*	cdev management : Undo events												  */
void UndoCdev (StorageHdle ourHandle)
{
	(*ourHandle)->LVal = (*ourHandle)->oldLVal;
	SetItemValue(ourHandle, iEvents, (*ourHandle)->LVal);
	UpdateSize (ourHandle);
}

/*_______________________________________________________________________*/
/*	drawing user items														  */
void DrawCdevItem (StorageHdle ourHandle, short item)
{
	ControlHandle itemHand;
	Rect 	itemRect; Str255 	str;
	GrafPtr savePort; short version;
	StoragePtr storage = *ourHandle;

	GetPort(&savePort);
	itemHand = IGetCtlHand (ourHandle,item,&itemRect);
	SetPort(storage->dlgPtr);
	switch (item) {
		case iVersion:	
			if (MidiShare()) {
				version = MidiGetVersion();
				NumToString (version, str);
				str[str[0]+1] = str[str[0]];
				str[str[0]] = str[str[0]-1];
				str[str[0]-1] = '.';
				str[0] += 1;
				if (!Supported(version)) {
					SetItemText (ourHandle, iMsg, "\pUnsupported MidiShare version, changes will be ignored.");
					storage->state = kUnsupportedVersion;
				}
				else storage->state = kClear;
			}
			else {
				GetIndString (str, kStrings, kNoMidiShare);
				SetItemText (ourHandle, iMsg, "\pPlease drop MidiShare on your system folder and restart.");
				storage->state = kNotInstalled;
			}
			if (storage->state != kClear) {
				DisableButton (ourHandle, iSetup);
			}
			break;
		case iSize:
			storage->LVal = GetItemValue (ourHandle, iEvents);
			if (storage->LVal < 100) storage->LVal = 100;
			else if (storage->LVal > 999999) storage->LVal = 999999;
			NumToString (((storage->LVal * sizeof(TMidiEv)) >> 10) + 1, str);
			BlockMove (" Kbytes.", &str[str[0]+1], 8);
			str[0] += 8;
			break;
		case iIcon:
			PlotCIcon (&itemRect, (CIconHandle)storage->icn);
			break;
	}
	if (item != iIcon) {
		EraseRect (&itemRect);
		MoveTo (itemRect.left+1,itemRect.top+10);
		DrawString (str);
	}
	SetPort(savePort);
}