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

#define SlotLength DrvNameLen // defined in "MidiShare.h"
#define StringLen 64

SlotPtr gInSlots = 0, gOutSlots = 0;
extern LinearizeMthTbl	gLinMethods;
extern ParseMethodTbl	gParseTbl;
extern Status2TypeTbl	gTypeTbl;

/* MacOSX Midi client */
extern MIDIPortRef gInPort;
extern MIDIPortRef gOutPort;
extern MIDIClientRef gClient;

//_________________________________________________________
static SlotPtr NewSlot () {return malloc (sizeof(Slot));}

//_________________________________________________________
static void FreeSlot (SlotPtr slot) {free (slot);}

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
	MidiStreamInit (&slot->state.outsmall, gLinMethods);
  	MidiStreamInit (&slot->outsysex, gLinMethods);
	MidiParseInit (&slot->in, gParseTbl, gTypeTbl);
	slot->next = 0;
	return slot;
}

//_________________________________________________________
// Find the device model associated to a MIDIEndpointRef
Boolean GetModel (MIDIEndpointRef device, char* gmodel, int strlen)
{
	int i,j,k, n,m,o,p;
	MIDIDeviceRef dev;
	MIDIEntityRef ref;
	CFStringRef pmodel;
    OSStatus err;
  	
	n = MIDIGetNumberOfDevices();
	
	for (i = 0; i < n; i++) {
	
        dev = MIDIGetDevice(i);
        err = MIDIObjectGetStringProperty(dev, kMIDIPropertyModel, &pmodel);
            
        if (err == noErr) {
            
            if (!CFStringGetCString(pmodel, gmodel, strlen, 0)) {
                fprintf(stderr, "GetModel error : string too long\n");
                return false;
            }
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
    }
	return false;
}	

//_________________________________________________________
void AddSlots (short refNum)
{
	int i, n;
	MIDIEndpointRef src,dest;
	CFStringRef pname;
    char slotname[SlotLength], name[StringLen], model[StringLen]; // 'name' + 'model' length must be < slotname 'length'
    OSStatus err;
	SlotPtr slot;
	
	n = MIDIGetNumberOfSources();
   	for (i=0; i<n; i++) {
	
		src = MIDIGetSource(i);
		err = MIDIObjectGetStringProperty(src, kMIDIPropertyName, &pname);
                
        if (err == noErr) {
            
            CFStringGetCString(pname, name, sizeof(name), 0);
            CFRelease(pname);
                
            // If found, add the model name before the device name, copy no more than SlotLength
            if (GetModel(src,model,StringLen)) {
                snprintf(slotname, SlotLength-1, "%s:%s", name,model); // 'name' and 'model' are concatenated to build 'slotname'
            }else {
                strncpy(slotname,name,SlotLength-1);
            }
            
            slot = CreateSlot (refNum, slotname, MidiInputSlot, src);
            if (slot) {
				slot->next = gInSlots;
				gInSlots = slot;
				// No connection with the source : done by the application alarm
            }
        }
	}
        
   	n = MIDIGetNumberOfDestinations();
	for (i=0; i<n; i++) {
	
		dest = MIDIGetDestination(i);
		err = MIDIObjectGetStringProperty(dest, kMIDIPropertyName, &pname);
                
        if (err == noErr) {
            
            CFStringGetCString(pname, name, sizeof(name), 0);
            CFRelease(pname);
            
            // If found, add the model name before the device name, copy no more than SlotLength
            if (GetModel(dest,model,StringLen)) {
                snprintf(slotname, SlotLength-1, "%s:%s",name, model); // 'name' and 'model' are concatenated to build 'slotname'
            }else {
                strncpy(slotname,name,SlotLength-1);
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
void SendEvents1 (short refNum)
{
	SlotPtr slot = 0;
	MidiEvPtr e;
	
	while ((e = MidiGetEv (refNum))) {
		if (!slot || (Slot(slot->refNum) != Port(e)))
			slot = FindSlot(gOutSlots, Port(e));
		if (slot) {
			if (!MS2MM (refNum, slot, e)) return; 
		}else{
			MidiFreeEv(e);
		}
	}
}

//_________________________________________________________
void SendEvents2 (short refNum)
{
	SlotPtr slot = 0;
	MidiEvPtr e;
	
	while ((e = MidiGetEv (refNum))) {
		if (!slot || (Slot(slot->refNum) != Port(e)))
			slot = FindSlot(gOutSlots, Port(e));
		if (slot) {
			if (!MS2MM (refNum, slot, e)) return;  // A SysEx is sent 
		}else{
			MidiFreeEv(e);
		}
	}
	
	MidiSetRcvAlarm(refNum,RcvAlarm);
	MidiSendIm(refNum, MidiNewEv(typeRestart));
}

//_________________________________________________________
void RcvAlarm (short refNum) {SendEvents1(refNum);}

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
void ApplAlarm (short refNum, long code )
{
	short alarmCode = (short)code;
	short ref = (short)(code >> 16);
	switch (alarmCode) {        
		case MIDIChgSlotConnect:
   			if (ref == refNum) ScanSlotChanges (gInSlots);
			break;
	}
}


