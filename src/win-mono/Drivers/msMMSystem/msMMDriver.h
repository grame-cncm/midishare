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

#ifndef __msMMDriver__
#define __msMMDriver__

#include "MidiShare.h"
#include "EventToMidiStream.h"
#include "MidiStreamToEvent.h"

enum { false, true };

#define kMMDriverVersion	100
#define MidiShareDrvRef     127
#define kBuffSize			512

//_________________________________________________________
typedef struct slot Slot, *SlotPtr;
struct slot {
	SlotPtr		next;
	SlotRefNum	refNum;		// the MidiShare slot refnum
	LPVOID		mmHandle;	// MMSystem handler
	short		mmIndex;	// MMSystem dev index
	Ev2StreamRec out;
	StreamFifo	in;
	LPMIDIHDR	header;		// for long msg output
//	char		buff[kBuffSize];
};

extern SlotPtr gInSlots, gOutSlots;

//_________________________________________________________
// functions definitions
//_________________________________________________________

void MSALARMAPI RcvAlarm  (short refNum );
void MSALARMAPI ApplAlarm (short refNum, long code );

void AddSlots (short refnum);
void RemoveSlots (short refnum);
void OpenSlot (SlotPtr slot, Boolean inputSlot);

#endif
