/*

  Copyright © Grame 2000

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
  grame@rd.grame.fr

*/


#include <Gestalt.h>

#include "MidiShare.h"
#include "msSerialDriverAppl.h"
#include "SavingState.h"
#include "SCC.h"

/* ----------------------------------*/
/* constants definitions             */
#define MidiShareDrvRef		127

#define StateFile	"\pmsSerial Driver State"	
enum { StateCreator = 'MsSr', StateType = 'stSr' };	

SerialDrv gData;
static inline SerialDrvPtr GetData ()	{ return &gData; }

/* -----------------------------------------------------------------------------*/
/* Driver required callbacks                                                    */
/* -----------------------------------------------------------------------------*/
static void CloseLines (SerialDrvPtr data)
{
	if (State(data, kModem)) {
		CloseLineA (data);
	}
	if (State(data, kPrinter)) {
		CloseLineB (data);
	}
}

/* -----------------------------------------------------------------------------*/
static void WakeUp (short r)
{
	SerialDrvPtr data = GetData ();
	MidiConnect (MidiShareDrvRef, r, true);

	MidiConnect (r, MidiShareDrvRef, true);
	data->sleeping = false;
	data->cnxChange = true;
}

/* -----------------------------------------------------------------------------*/
static void Sleep (short r)
{
	SerialDrvPtr data = GetData ();
	CloseLines (data);
	data->sleeping = true;
#ifdef __BackgroundOnly__
	CloseMidi ();
	doneFlag = true;
#endif
}

/* -----------------------------------------------------------------------------*/
static void PStrCpy (unsigned char *src, unsigned char * dst)
{
	short i = src[0] + 1;
	while (i--)
		*dst++ = *src++;
}

/* -----------------------------------------------------------------------------*/
static Boolean SlotInfo (SlotRefNum slot, TSlotInfos * infos)
{
	Str32 modem = "\pModem";
	Str32 printer = "\pPrinter";
	unsigned char * name = 0;
	SerialDrvPtr data = GetData ();
	if (infos) {
		if (slot == data->slot[kModem])
			name = modem;
		else if (slot == data->slot[kPrinter])
			name = printer;
	 	if (name) {
			PStrCpy (name, infos->name);
			infos->direction = MidiInputOutputSlot;
			return true;
		}
	}
	return false;
}

/* -----------------------------------------------------------------------------*/
/* MidiShare part                                                               */
/* -----------------------------------------------------------------------------*/
static pascal void ReceiveEvents (short r)
{
	SerialDrvPtr data = GetData ();
	SCCPtr scc; MidiEvPtr e = MidiGetEv(r);
	Boolean rcv[2];

	rcv[0] = rcv[1] = false;
	while (e) {
		short i = data->slotIndex[Port(e)];
		scc = ((i == 0) || (i == 1)) ? &data->scc[i] : 0;
		if (scc && scc->state) {
			Link(e) = 0;
			INT_OFF();
			Link(scc->seq.last) = e;
			scc->seq.last = e;
			INT_ON();
			rcv[i] = true;
		}
		else MidiFreeEv (e);
		e = MidiGetEv(r);
	}
	scc = &data->scc[kModem];
	if (rcv[kModem] && !MidiStreamCountByte(&scc->xmt))
		StartXmt (scc);	
	scc = &data->scc[kPrinter];
	if (rcv[kPrinter] && !MidiStreamCountByte(&scc->xmt))
		StartXmt (scc);
}

/* -----------------------------------------------------------------------------*/
static pascal void ApplAlarm (short refnum, long code)
{
	switch (code & 0xffff) {    	
        case MIDIChgSlotConnect:
			GetData ()->cnxChange = true;
			break;
	}
}

/* -----------------------------------------------------------------------------*/
static void SetupFilter (MidiFilterPtr filter)
{
	short i;

	for (i = 0; i<256; i++) { 										
		RejectBit(filter->evType,i);
		AcceptBit(filter->port,i);
	}
	for (i = 0; i<16; i++)
		AcceptBit(filter->channel,i);
		
	for (i = 0; i<typeProcess; i++)
		AcceptBit(filter->evType,i);
	
	for (i = typeQuarterFrame; i<=typeRegParam; i++)
		AcceptBit(filter->evType,i);
}

/* -----------------------------------------------------------------------------*/
static Boolean IsSlotConnected (SlotRefNum sref)
{
	short i;
	for (i=0; i<256; i++) {
		if (MidiIsSlotConnected (i, sref))
			return true;
	}
	return false;
}

/* -----------------------------------------------------------------------------*/
static void DisConnectSlot (SlotRefNum sref)
{
	short i;
	for (i=0; i<256; i++) {
		MidiConnectSlot (i, sref, false);
	}
}

/* -----------------------------------------------------------------------------*/
static void DataInit (SerialDrvPtr data)
{
	short i;
	data->refNum = 0;
	data->cnxChange = false;
	data->sleeping = true;
	MidiParseInitTypeTbl (data->s2t);
	MidiParseInitMthTbl (data->rTbl);
	MidiStreamInitMthTbl (data->lTbl);
	for (i=0; i<64; i++) data->slotIndex[i] = -1;
	for (i=0; i<2; i++) {
		data->scc[i].checkTask = 0;
		data->scc[i].state = false;
		data->slot[i] = -1;
	}
}

/* -----------------------------------------------------------------------------*/
Boolean SetUpMidi ()
{
	TDriverInfos infos = { SerialDriverName, 100, 0};
	SlotRefNum sref; short refNum;
	TDriverOperation op; 
	SerialDrvPtr data = GetData ();
	
	DataInit (data);
	
	op.wakeup = WakeUp;
	op.sleep = Sleep;
	op.slotInfo = SlotInfo; 

	if (MidiGetNamedAppl (SerialDriverName) > 0)  // still running
		return true;

	refNum = MidiRegisterDriver(&infos, &op);
	if (refNum == MIDIerrSpace)
		return false;

	data->refNum = refNum;
	sref = MidiAddSlot (refNum);
	if (sref >= 0) {
		data->slot[kModem] = sref;
		data->slotIndex[Slot(sref)] = kModem;
	}
	sref = MidiAddSlot (refNum);
	if (sref >= 0) {
		data->slot[kPrinter] = sref;
		data->slotIndex[Slot(sref)] = kPrinter;
	}

	MidiSetRcvAlarm (refNum, ReceiveEvents);		
	MidiSetApplAlarm (refNum, ApplAlarm);		
	SetupFilter (&data->filter);
	MidiSetFilter (refNum, &data->filter);
	RestoreDriverState (refNum, StateFile);
	if (data->sleeping) data->cnxChange = false;
	return true;
}

/* -----------------------------------------------------------------------------*/
void CloseMidi ()
{
	SerialDrvPtr data = GetData ();
	short ref = data->refNum;
	data->refNum = 0;
	if (ref > 0) {
		SaveDriverState (ref, StateFile, StateCreator, StateType);
		MidiUnregisterDriver (ref);
		CloseLines (data);
	}
}

/* -----------------------------------------------------------------------------*/
void DoIdle()
{
	SerialDrvPtr data = GetData ();
	if (data->cnxChange) {
		if (IsSlotConnected (data->slot[kModem])) {
			if (!State(data, kModem)){
				if (!OpenLineA (data))
					DisConnectSlot (data->slot[kModem]);
			}
		}
		else if (State(data, kModem)) {
			CloseLineA (data);
		}
		if (IsSlotConnected (data->slot[kPrinter])) {
			if (!State(data, kPrinter)) {
				if (!OpenLineB (data))
					DisConnectSlot (data->slot[kPrinter]);
			}
		}
		else if (State(data, kPrinter)) {
			CloseLineB (data);
		}
		data->cnxChange = false;
	}
}

/* -----------------------------------------------------------------------------*/
/* SCC part                                                               */
/* -----------------------------------------------------------------------------*/
Boolean CheckSCC ()
{
	SerialDrvPtr data = GetData ();
	long reply;
	OSErr err = Gestalt (gestaltHardwareAttr, &reply);
	if ((err == noErr) && (reply & (1 << gestaltHasSCC))) {
		err= Gestalt (gestaltPowerMgrAttr, &reply);
		if( err== noErr)
			 data->powerMgr = reply & (1 << gestaltPMgrExists);
		else
			 data->powerMgr = false;
		return true;
	}
	return false;
}
