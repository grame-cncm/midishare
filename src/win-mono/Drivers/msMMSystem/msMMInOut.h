
#ifndef __msMMInOut__
#define __msMMInOut__

#include "MidiShare.h"
#include "msMMDriver.h"


MidiEvPtr MS2MM (SlotPtr slot, MidiEvPtr e);
void CALLBACK MidiInProc( HMIDIIN hMidiIn, UINT wMsg, 
			DWORD dwInstance, DWORD dwParam1, DWORD dwParam2);

#endif
