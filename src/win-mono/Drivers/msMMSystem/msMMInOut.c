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
#include <mmsystem.h>

#include "msMMInOut.h"
#include "msMMError.h"

extern short gRefNum;
//_________________________________________________________
static void MM2MS (SlotPtr slot, char * buff)
{
	MidiEvPtr e; short n = 4;
	do {
		e = MidiParseByte (&slot->in, *buff++);
		n--;
	} while (!e && n);
	if (e) {
		Port(e) = (Byte)Slot(slot->refNum);
		MidiSendIm (gRefNum, e);
	}
	else MidiParseReset(&slot->in);
}

//_________________________________________________________
static void LMM2MS (SlotPtr slot, LPMIDIHDR h)
{
	MidiEvPtr e; long n = h->dwBytesRecorded;
	char *ptr = h->lpData;
	while (n--) {
		unsigned long v = *(unsigned char *)ptr;
		e = MidiParseByte (&slot->in, *ptr++);
		if (e) {
			Port(e) = (Byte)Slot(slot->refNum);
			MidiSendIm (gRefNum, e);
		}	
	}
	h->dwBytesRecorded = 0;
}

//_________________________________________________________
void CALLBACK MidiInProc( HMIDIIN hMidiIn, UINT wMsg, 
			DWORD userData, DWORD dwParam1, DWORD dwParam2)
{
	SlotPtr slot = (SlotPtr)userData;
	switch (wMsg) {
		case MIM_OPEN: break;

		case MIM_ERROR:
		case MIM_DATA:
			MM2MS (slot, (char *)&dwParam1);
			break;

		case MIM_LONGERROR:
		case MIM_LONGDATA:
			LMM2MS (slot, (LPMIDIHDR)dwParam1);
			midiInAddBuffer (slot->mmHandle, &slot->header, sizeof(slot->header));
			break;
	}
}

//_________________________________________________________
static MidiEvPtr LMS2MM (SlotPtr slot, MidiEvPtr e)
{
	char * ptr = slot->buff; short free = kBuffSize;
	MMRESULT res;
	if (!slot->header.dwUser) {	// Midi Header not prepared
		MidiFreeEv(e);			// free the event 
		return 0;				// and cancel output
	}
	e = MidiStreamPutEvent (&slot->out, e);
	while (MidiStreamGetByte (&slot->out, ptr++)) {
		if (!--free) {
			slot->header.dwBufferLength = kBuffSize;
			res = midiOutLongMsg (slot->mmHandle, &slot->header, sizeof(slot->header));
			if (res != MMSYSERR_NOERROR)
				goto err;
			ptr = slot->buff;
			free = kBuffSize;
		}
	}
	slot->header.dwBufferLength = kBuffSize - free;
	if (slot->header.dwBufferLength) {
		res = midiOutLongMsg (slot->mmHandle, &slot->header, sizeof(slot->header));
		if (res != MMSYSERR_NOERROR)
			goto err;
	}
	return 0;
err:
	MMError ("midiOutLongMsg", res, false);
	MidiStreamReset (&slot->out);
	return 0;
}

//_________________________________________________________
MidiEvPtr MS2MM (SlotPtr slot, MidiEvPtr e)
{
	if ((EvType(e) >= typeSysEx) && (EvType(e) != typeQuarterFrame))
		return LMS2MM (slot, e);
	else {
		DWORD data; MMRESULT res;
		char * ptr = (char *)&data;
		e = MidiStreamPutEvent (&slot->out, e);
		while (MidiStreamGetByte (&slot->out, ptr++))
			;
		res = midiOutShortMsg (slot->mmHandle, data);
		if (res != MMSYSERR_NOERROR)
			MMError ("midiOutShortMsg", res, false);
	}
	return e;
}
