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
#include "msMidiError.h"

static int min(a,b) {return (a<b)?a:b;}

extern short gRefNum;
static void MyCompletionProc( MIDISysexSendRequest *request );

// Buffer for sending
static Byte gPacketBuffer [1024];
static Byte gEvBuffer [16];
	
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
void SendSysExAux(SlotPtr slot)
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
	slot->request.completionProc = MyCompletionProc;
	slot->request.completionRefCon = slot;
	
	err = MIDISendSysex( &slot->request);
    slot->sending = (err == noErr);
 }
 
//_________________________________________________________
 MidiEvPtr SendSysEx(SlotPtr slot,MidiEvPtr e)
{
	slot->remaining = (EvType(e) == typeSysEx) ? (MidiCountFields(e)+2) : MidiCountFields(e);
	
	// Write event to be sent
	e = MidiStreamPutEvent (&slot->outsysex, e);
	SendSysExAux(slot);
	return e;
}
 
//_________________________________________________________
 MidiEvPtr SendSmallEv(SlotPtr slot,MidiEvPtr e)
{
	MIDIPacketList* pktlist = (MIDIPacketList*)gPacketBuffer;
	MIDIPacket* packet = MIDIPacketListInit(pktlist);
	unsigned char * ptr ;
	int  n = 0;
	 
	ptr = gEvBuffer;
	e = MidiStreamPutEvent (&slot->out, e);
	while (MidiStreamGetByte (&slot->out, ptr++)) {n++;}
	
	MIDIPacketListAdd(pktlist,sizeof(gPacketBuffer),packet,0,n,gEvBuffer);
	MIDISend(slot->port,slot->dest,pktlist);
	
	return e;
 }

//_________________________________________________________
static void MyCompletionProc( MIDISysexSendRequest *request )
{
    SlotPtr slot =  (SlotPtr)request->completionRefCon;
    MidiEvPtr ev;
    
    if (slot->remaining == 0){
        slot->sending = FALSE;
        /*
        while ((ev = (MidiEvPtr)fifoget(&slot->pending))) {
        	// If typeSysEx or typeStream : send one, pending events will be sent by the CompletionProc
            if ((EvType(ev) == typeSysEx) || (EvType(ev) == typeStream)) {
            	MS2MM(slot,ev);
	        	break;
	        // Send all pending events
	        }else{ 
	        	MS2MM(slot,ev);
	        }
        }
        */
         
    } else {
        SendSysExAux(slot);
    }
}

//________________________________________________________________________________________
MidiEvPtr MS2MM (SlotPtr slot, MidiEvPtr e)
{
	if ((EvType(e) >= typeClock) && (EvType(e) <= typeReset)){
		return SendSmallEv(slot,e);
	}else if (slot->sending) {
		//fifoput(&slot->pending,(cell*)e);
		MidiFreeEv(e);
		return 0;
	}else  if ((EvType(e) == typeSysEx) || (EvType(e) == typeStream)) {
		return SendSysEx(slot,e);
	}else {
		return SendSmallEv(slot,e);
	}
}


//________________________________________________________________________________________
void MyReadProc(const MIDIPacketList *pktlist, void *refCon, void *connRefCon)
{
	SlotPtr slot = (SlotPtr)connRefCon;
	MIDIPacket *packet = (MIDIPacket *)pktlist->packet;	
	int i;
	
	for (i = 0; i < pktlist->numPackets; ++i) {
		LMM2MS( slot, packet);
		packet = MIDIPacketNext(packet);
	}
}

