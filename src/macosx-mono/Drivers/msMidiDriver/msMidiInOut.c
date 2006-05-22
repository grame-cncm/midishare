/*

  Copyright © Grame 2002

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


#include "msMidiInOut.h"
#include <mach/mach_time.h>

#define PRINT(x) { printf x; fflush(stdout); }
#define DBUG(x)    /* PRINT(x) */

extern short gRefNum;

static int min(a,b) {return (a<b)?a:b;}
static void CompletionProc( MIDISysexSendRequest *request );

//____________________________________________________________________________________________________________
// Timing functions found in /Developer/Examples/CoreAudio/MIDI/SampleUSBDriver/Shared/MIDIDriverUtils.cpp

static bool		sIsInited = false;
static Float64	sNanoRatio;

//_________________________________________________________
static void InitHostTime()
{
	struct mach_timebase_info info;

	mach_timebase_info(&info);
	sNanoRatio = (double)info.numer / (double)info.denom;
	sIsInited = true;
}

//_________________________________________________________
MIDITimeStamp MIDIGetCurrentHostTime()
{
	return mach_absolute_time();
}

//_________________________________________________________
static void LMM2MS (SlotPtr slot, MIDIPacket *packet)
{
	MidiEvPtr e; 
	long n = packet->length;
	unsigned char *ptr = packet->data;
	
	while (n--) {
		e = MidiParseByte (&slot->in, *ptr++);
		if (e) {
			Port(e) = (Byte)Slot(slot->refNum);
			MidiSendIm (gRefNum, e);
		}	
	}
}

//_________________________________________________________
static void SendSysExAux(SlotPtr slot)
{
	int i,bytestosend;
	unsigned char* ptr;
	OSErr err;

	// Prepare bytes
	bytestosend = min(MAX_SYSEX_BYTES,slot->remaining);
	slot->remaining-= bytestosend;
	ptr = slot->data;
	for (i = 0 ; i < bytestosend ; i++) MidiStreamGetByte (&slot->outsysex, ptr++);
 	
	// Init completion structure
	slot->request.destination = slot->dest;
	slot->request.data = slot->data;
	slot->request.bytesToSend = bytestosend;
	slot->request.complete = FALSE;
	slot->request.completionProc = CompletionProc;
	slot->request.completionRefCon = slot;
	
	err = MIDISendSysex(&slot->request);
	if (err != noErr) fprintf(stderr, "MIDISendSysex : error %ld\n", err);
 }
 
//_________________________________________________________
static void SendSysEx(SlotPtr slot, MidiEvPtr e)
{
	slot->remaining = (EvType(e) == typeSysEx) ? (MidiCountFields(e)+2) : MidiCountFields(e);
	
	// Write event to be sent
	MidiStreamPutEvent (&slot->outsysex, e);
	SendSysExAux(slot);
}

//_________________________________________________________
void KOffTask (long date, short ref, long a1,long a2,long a3)
{
	MidiEvPtr e = (MidiEvPtr)a1;
	MS2MM (ref, (SlotPtr)a2, e);
}

//_________________________________________________________
static void SendSmallEv(SlotPtr slot, MidiEvPtr e, sendState* state)
{ 
	MIDIPacketList* pktlist = (MIDIPacketList*)state->packetBuffer;
	MIDIPacket* packet = MIDIPacketListInit(pktlist);
	unsigned char * ptr = state->evBuffer;
	OSErr err;
	int n = 0;
  	 
	e = MidiStreamPutEvent (&state->outsmall, e);
	if (e) MidiTask (KOffTask, Date(e), gRefNum, (long)e, (long)slot, 0); // typeNote
	
	while (MidiStreamGetByte (&state->outsmall, ptr++)) {n++;}
	
	MIDIPacketListAdd(pktlist,sizeof(state->packetBuffer),packet,MIDIGetCurrentHostTime(),n,state->evBuffer);
	err = MIDISend(slot->port,slot->dest,pktlist);
	if (err != noErr) fprintf(stderr, "MIDISend : error %ld\n", err);
}


//________________________________________________________________________________________
static void CompletionProc(MIDISysexSendRequest *request)
{
	/* check client */
	if (gClient == NULL) {
		fprintf(stderr, "CompletionProc : dead driver\n");
		return;
	}
		
    SlotPtr slot = (SlotPtr)request->completionRefCon;
	
	if (slot->remaining > 0){
		SendSysExAux(slot);
	}else{
		SendEvents2(gRefNum);
	}
 }

//________________________________________________________________________________________
bool MS2MM(short refNum, SlotPtr slot, MidiEvPtr e)
{
	int type = EvType(e);
	
	if ((type == typeSysEx) || (type == typeStream)) {
		MidiSetRcvAlarm(refNum,0);  
		SendSysEx(slot,e); 
		return false;
	}else{
		SendSmallEv(slot,e,&slot->state);
		return true;
	}
}

//________________________________________________________________________________________
void ReadProc(const MIDIPacketList *pktlist, void *refCon, void *connRefCon)
{
	SlotPtr slot = (SlotPtr)connRefCon;
	MIDIPacket *packet = (MIDIPacket *)pktlist->packet;	
	int i;
	DBUG(("ReadProc \n"));

	for (i = 0; i < pktlist->numPackets; ++i) {
		LMM2MS( slot, packet);
		packet = MIDIPacketNext(packet);
	}
	
	DBUG(("ReadProc OK\n"));
}

