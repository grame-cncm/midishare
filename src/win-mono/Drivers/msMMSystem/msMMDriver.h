

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
	MIDIHDR		header;		// for long msg output
	char		buff[kBuffSize];
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
