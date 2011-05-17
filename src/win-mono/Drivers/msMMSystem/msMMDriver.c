/*

  Copyright © Grame, Sony CSL-Paris 2001

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
#include <windowsx.h>
#include <mmsystem.h>
#include "msMMDriver.h"
#include "msMMInOut.h"
#include "msMMError.h"

enum { false, true };

SlotPtr gInSlots = 0, gOutSlots = 0;
extern LinearizeMthTbl	gLinMethods;
extern ParseMethodTbl	gParseTbl;
extern Status2TypeTbl	gTypeTbl;

static void CloseSlot (SlotPtr slot, Boolean inputSlot);
//_________________________________________________________
static SlotPtr NewSlot ()
{
	return LocalAlloc (LMEM_FIXED, sizeof(Slot));
}

//_________________________________________________________
static void FreeSlot (SlotPtr slot)
{
	LocalFree ((HLOCAL)slot);
}

//_________________________________________________________
static SlotPtr CreateSlot(short refNum, char *name, SlotDirection dir)
{
	SlotPtr slot = NewSlot ();
	if (!slot) return 0;
	slot->refNum = MidiAddSlot (refNum, name, dir);
	if (slot->refNum.slotRef < 0) {
		FreeSlot (slot);
		return 0;
	}
	MidiStreamInit (&slot->out, gLinMethods);
	MidiParseInit (&slot->in, gParseTbl, gTypeTbl);
	slot->mmHandle = 0;
	slot->next = 0;
	return slot;
}

//_________________________________________________________
void AddSlots (short refNum)
{
	UINT i, n = midiInGetNumDevs();
	MMRESULT res;

	for (i=0; i<n; i++) {
		MIDIINCAPS caps;
		res = midiInGetDevCaps (i, &caps, sizeof(caps));
		if (res == MMSYSERR_NOERROR) {
			SlotPtr slot = CreateSlot (refNum, caps.szPname, MidiInputSlot);
			if (slot) {
				slot->mmIndex = i;
				slot->next = gInSlots;
				gInSlots = slot;
			}
		}
	}

	n = midiOutGetNumDevs ();
	for (i=0; i<n; i++) {
		MIDIOUTCAPS caps;
		res = midiOutGetDevCaps (i, &caps, sizeof(caps));
		if (res == MMSYSERR_NOERROR) {
			SlotPtr slot = CreateSlot (refNum, caps.szPname, MidiOutputSlot);
			if (slot) {
				slot->mmIndex = i;
				slot->next = gOutSlots;
				gOutSlots = slot;
			}
		}
	}
}

//_________________________________________________________
void RemoveSlotList (SlotPtr slot, Boolean input)
{
	SlotPtr next;
	while (slot) {
		next = slot->next;
		if (slot->mmHandle)
			CloseSlot (slot, input);
		FreeSlot (slot);
		slot = next;
	}
}

//_________________________________________________________
void RemoveSlots (short refNum)
{
	RemoveSlotList (gInSlots, true);
	gInSlots = 0;
	RemoveSlotList (gOutSlots, false);
	gOutSlots = 0;
}

//_________________________________________________________
static SlotPtr FindSlot (SlotPtr list, short port)
{
	while (list) {
		if (list->refNum.slotRef == port)
			return list;
		list = list->next;
	}
	return 0;
}

//_________________________________________________________
void MSALARMAPI KOffTask( long date, short ref, void* a1, void* a2, void* a3)
{
	MidiEvPtr e = (MidiEvPtr)a1;
	MS2MM ((SlotPtr)a2, e);
}

//_________________________________________________________
void MSALARMAPI RcvAlarm  (short refNum )
{
	SlotPtr slot = 0;
	MidiEvPtr e = MidiGetEv (refNum);
	while (e) {
		if (!slot || (slot->refNum.slotRef != Port(e)))
			slot = FindSlot(gOutSlots, Port(e));
		if (slot && slot->mmHandle) {
			e = MS2MM (slot, e);
			if (e)
				MidiTask (KOffTask, Date(e), refNum, e, slot, 0);
		}else{
			MidiFreeEv(e);
		}
		e = MidiGetEv (refNum);
	}
}

//_________________________________________________________
static Boolean IsSlotConnected (SlotRefNum sref)
{
	short i;
	for (i=0; i<256; i++) {
		if (MidiIsSlotConnected (i, sref))
			return true;
	}
	return false;
}

//_________________________________________________________
void Disconnect (SlotRefNum sref)
{
	short i;
	for (i=0; i<256; i++) {
		MidiConnectSlot (i, sref, false);
	}
}

//_________________________________________________________
static Boolean InitHeaders (SlotPtr slot)
{
    slot->header = (LPMIDIHDR)GlobalAllocPtr(GMEM_MOVEABLE|GMEM_SHARE|GMEM_ZEROINIT,
                                             sizeof(MIDIHDR) + kBuffSize);
	if (!slot->header) return false;
	slot->header->lpData = (LPBYTE)slot->header + sizeof(MIDIHDR);
	slot->header->dwBufferLength = kBuffSize;
	slot->header->dwFlags = 0;
	slot->header->dwUser = 0;
	slot->header->lpNext = 0;
	slot->header->dwBytesRecorded = 0;
	return true;
}

//_________________________________________________________
static Boolean OpenInputSlot (SlotPtr slot)
{
	HMIDIIN h;
	MMRESULT ret = midiInOpen (&h, slot->mmIndex, (DWORD)MidiInProc, (DWORD)slot, CALLBACK_FUNCTION);
	if (ret == MMSYSERR_NOERROR) {
		slot->mmHandle = h;
		if (!InitHeaders (slot)) {
			MMError ("memory allocation failed", slot->refNum, 0, true);
			midiInClose (h);
			return false;
		}
		ret= midiInPrepareHeader (h, slot->header, sizeof(MIDIHDR));
		if (ret == MMSYSERR_NOERROR) {
			slot->header->dwUser = 1;
			ret= midiInAddBuffer (h, slot->header, sizeof(MIDIHDR));
			if (ret == MMSYSERR_NOERROR) {
				ret= midiInStart (h);
				if (ret == MMSYSERR_NOERROR) return true;
				else MMError ("midiInStart", slot->refNum, ret, true);
			}
			else MMError ("midiInAddBuffer", slot->refNum, ret, true);
			CloseSlot (slot, true);
		}
		else {
			MMError ("midiInPrepareHeader", slot->refNum, ret, true);
			midiInClose (h);
		}
	}
	else MMError ("midiInOpen", slot->refNum, ret, true);
	return false;
}

//_________________________________________________________
static Boolean OpenOutputSlot (SlotPtr slot)
{
	HMIDIOUT h; 
	UINT ret = midiOutOpen(&h, slot->mmIndex, 0L, 0L, CALLBACK_NULL);
	if (ret == MMSYSERR_NOERROR) {
		MMRESULT res;
		slot->mmHandle = h;
		if (!InitHeaders (slot)) {
			MMError ("memory allocation failed", slot->refNum, 0, false);
			midiOutClose (h);
			return false;
		}
		res = midiOutPrepareHeader(h, slot->header, sizeof(MIDIHDR)); 
		if (res != MMSYSERR_NOERROR) {
			MMError ("midiOutPrepareHeader", slot->refNum, res, false);
			return false;
		}
		else slot->header->dwUser = 1;
	}
	else {
		MMError ("midiOutOpen", slot->refNum, ret, false);
		return false;
	}
	return true;
}

//_________________________________________________________
Boolean OpenSlot (SlotPtr slot, Boolean inputSlot)
{
	Boolean ret;
	if (inputSlot) ret = OpenInputSlot (slot);
	else ret = OpenOutputSlot (slot);
	return ret;
}

#define CheckErr(res,ref,function,in) if (res != MMSYSERR_NOERROR) MMError (function, ref, res, in)
//_________________________________________________________
static void CloseSlot (SlotPtr slot, Boolean inputSlot)
{
	MMRESULT res; int retry = 0;
	if (!slot->mmHandle) return;
	if (inputSlot) {
		HMIDIIN h = (HMIDIIN)slot->mmHandle;
		slot->header->dwUser = 0;
		res = midiInStop (h);
		CheckErr(res, slot->refNum, "midiInStop", true);
		res = midiInReset(h);
		CheckErr(res, slot->refNum, "midiInReset", true);
		res = midiInUnprepareHeader(h, slot->header, sizeof(MIDIHDR)); 
		CheckErr(res, slot->refNum, "midiInUnprepareHeader", true);
		do {
			res= midiInClose (h);
			CheckErr(res, slot->refNum, "midiInClose", true);
			if (res == MIDIERR_STILLPLAYING)
				midiInReset(h);
			Sleep (10);
			retry++;
		} while ((res == MIDIERR_STILLPLAYING) && (retry < 10));
	}
	else {
		HMIDIOUT h = (HMIDIOUT)slot->mmHandle;
		res = midiOutReset (h); 
		if (res != MMSYSERR_NOERROR)
			MMError ("midiOutReset", slot->refNum, res, false);
		midiOutUnprepareHeader (h, slot->header, sizeof(MIDIHDR)); 
		do {
			res= midiOutClose (h);
			if (res != MMSYSERR_NOERROR)
				MMError ("midiOutClose", slot->refNum, res, false);
			Sleep (10);
			retry++;
		} while ((res == MIDIERR_STILLPLAYING) && (retry < 10));
	}
	slot->mmHandle = 0;
	if (slot->header) {
		GlobalFreePtr (slot->header);
		slot->header = 0;
	}
}

//_________________________________________________________
static void ScanSlotChanges (SlotPtr slot, Boolean input)
{
	while (slot) {
		if (IsSlotConnected (slot->refNum)) {
			if (!slot->mmHandle) {
				if (!OpenSlot (slot, input))
					Disconnect (slot->refNum);
			}
		}
		else if (slot->mmHandle)
			CloseSlot (slot, input);
		slot = slot->next;
	}
}

//_________________________________________________________
void MSALARMAPI ApplAlarm (short refNum, long code )
{
	short alarmCode = (short)code;
	short ref = (short)(code >> 16);
	switch (alarmCode) {        
		case MIDIChgSlotConnect:
			if (ref == refNum) {
				ScanSlotChanges (gInSlots, true);
				ScanSlotChanges (gOutSlots, false);
			}
			break;
	}
}
