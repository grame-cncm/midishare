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

#include "msMidiDriver.h"
#include "msMidiInOut.h"

#define SlotLength 64

SlotPtr gInSlots = 0, gOutSlots = 0;
extern LinearizeMthTbl	gLinMethods;
extern ParseMethodTbl	gParseTbl;
extern Status2TypeTbl	gTypeTbl;

/* MacOSX Midi client */
extern MIDIPortRef	gInPort;
extern MIDIPortRef	gOutPort;
extern MIDIClientRef 	gClient;


//_________________________________________________________
static SlotPtr NewSlot (){ return malloc (sizeof(Slot)); }

//_________________________________________________________
static void FreeSlot (SlotPtr slot){free (slot); }

//_________________________________________________________
static SlotPtr CreateSlot(short refNum, char *name, SlotDirection dir, MIDIEndpointRef src)
{
	SlotPtr slot = NewSlot ();
	if (!slot) return 0;
	slot->refNum = MidiAddSlot (refNum, name, dir);
	if (Slot(slot->refNum) < 0) {
		FreeSlot (slot);
		return 0;
	}
	slot->src = src;
	slot->sending = FALSE;
	MidiStreamInit (&slot->state1.outsmall, gLinMethods);
        MidiStreamInit (&slot->state2.outsmall, gLinMethods);
	MidiStreamInit (&slot->outsysex, gLinMethods);
	MidiParseInit (&slot->in, gParseTbl, gTypeTbl);
	fifoinit(&slot->pending);
	slot->next = 0;
	return slot;
}


//_________________________________________________________
// Find the device model associated to a MIDIEndpointRef

Boolean GetModel (MIDIEndpointRef device, char* gmodel)
{
	int i,j,k, n,m,o,p;
	MIDIDeviceRef dev;
	MIDIEntityRef ref;
	CFStringRef pmodel;
	
	n = MIDIGetNumberOfDevices();
	
	for (i = 0; i < n; i++) {
	
		dev = MIDIGetDevice(i);
		
		MIDIObjectGetStringProperty(dev, kMIDIPropertyModel, &pmodel);
		CFStringGetCString(pmodel, gmodel, SlotLength, 0);
		CFRelease(pmodel);
		
		m = MIDIDeviceGetNumberOfEntities(dev);

		for (j = 0; j < m; j++) {
		
			ref = MIDIDeviceGetEntity(dev,j);
			o = MIDIEntityGetNumberOfSources(ref);
			p = MIDIEntityGetNumberOfDestinations(ref);
			
			for (k = 0; k < o; k++) {
				if (MIDIEntityGetSource(ref,k) == device) return true;
			}
			
			for (k = 0; k < p; k++) {
				if (MIDIEntityGetDestination(ref,k) == device) return true;
			}
		}
	}
	return false;
}	


//_________________________________________________________

void AddSlots (short refNum)
{
	int i, n;
	MIDIEndpointRef src,dest;
	CFStringRef pname;
	char name[SlotLength],slotname[SlotLength], model[SlotLength];
	SlotPtr slot;
	
	n = MIDIGetNumberOfSources();
	for (i=0; i<n; i++) {
	
		src = MIDIGetSource(i);
		MIDIObjectGetStringProperty(src, kMIDIPropertyName, &pname);
		CFStringGetCString(pname, name, sizeof(name), 0);
		CFRelease(pname);
		
		// If found, add the model name before the device name
		if (GetModel(src,model)) {
                        sprintf(slotname, "%s:%s", model,name);
           	}else {
			strcpy(slotname,name);
		}
		
		slot = CreateSlot (refNum, slotname, MidiInputSlot, src);
		if (slot) {
			slot->next = gInSlots;
			gInSlots = slot;
			// No connection with the source : done by the application alarme
		}
	}
	
	n = MIDIGetNumberOfDestinations();
	for (i=0; i<n; i++) {
	
		dest = MIDIGetDestination(i);
		MIDIObjectGetStringProperty(dest, kMIDIPropertyName, &pname);
		CFStringGetCString(pname, name, sizeof(name), 0);
		CFRelease(pname);
		
		// If found, add the model name before the device name
		if (GetModel(dest,model)) {
                        sprintf (slotname, "%s:%s", model,name);
                 }else {
			strcpy(slotname,name);
		}
		
		slot = CreateSlot (refNum, slotname, MidiOutputSlot, 0);
		if (slot) {
			slot->port = gOutPort;
			slot->dest = dest;
			slot->next = gOutSlots;
			gOutSlots = slot;
		}
	}
}	


//_________________________________________________________
void RemoveSlotList (SlotPtr slot, Boolean input)
{
	OSStatus err;
	SlotPtr next;
	while (slot) {
		next = slot->next;
		if (input) err = MIDIPortDisconnectSource(gInPort, slot->src);  // disconnect source
		MidiRemoveSlot(slot->refNum);
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
		if (Slot(list->refNum) == port)
			return list;
		list = list->next;
	}
	return 0;
}

//_________________________________________________________
void MSALARMAPI KOffTask( long date, short ref, long a1,long a2,long a3)
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
		if (!slot || (Slot(slot->refNum) != Port(e)))
			slot = FindSlot(gOutSlots, Port(e));
		if (slot) {
			e = MS2MM (slot, e);
			if (e)
				MidiTask (KOffTask, Date(e), refNum, (long)e, (long)slot, 0);
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
		if (MidiIsSlotConnected (i, sref)) return true;
	}
	return false;
}


//_________________________________________________________
static void OpenInputSlot (SlotPtr slot)
{
	MIDIPortConnectSource(gInPort, slot->src,slot);  // connect source
}

//_________________________________________________________
static void CloseInputSlot (SlotPtr slot)
{
	MIDIPortDisconnectSource(gInPort, slot->src);    // disconnect source
}

//_________________________________________________________
void OpenSlot (SlotPtr slot, Boolean input)
{
	if (input) OpenInputSlot(slot);
}


//_________________________________________________________
static void ScanSlotChanges (SlotPtr slot)
{
	while (slot) {
    		if (IsSlotConnected (slot->refNum)) {
   			OpenInputSlot (slot);
		}else{
   			CloseInputSlot (slot);
		}
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
   			if (ref == refNum) ScanSlotChanges (gInSlots);
			break;
	}
}


