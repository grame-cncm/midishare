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
  grame@grame.fr

*/
#include <Gestalt.h>
#include "MidiShare.h"
#include "msMtpDriverAppl.h"
#include "SavingState.h"
#include "SCC.h"
#include "MidiPrint.h"

/* ----------------------------------*/
/* constants definitions             */
#define MidiShareDrvRef		127

MtpDrv gData = { 0 };
static inline MtpDrvPtr GetData ()	{ return &gData; }

/* -----------------------------------------------------------------------------*/
/* Driver required callbacks                                                    */
/* -----------------------------------------------------------------------------*/
static void CloseLines (MtpDrvPtr data)
{
	if (State(data)) CloseLine (data);
}

/* -----------------------------------------------------------------------------*/
static void WakeUp (short r)
{
	MtpDrvPtr data = GetData ();
	OpenLine(data);
	MidiConnect (MidiShareDrvRef, r, true);
	MidiConnect (r, MidiShareDrvRef, true);
}

/* -----------------------------------------------------------------------------*/
static void Sleep (short r)
{
	MtpDrvPtr data = GetData ();
#ifdef __BackgroundOnly__
	CloseMidi ();
	doneFlag = true;
#else
	CloseLines (data);
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
static MidiName MakeSlotName (short index, MidiName name)
{
	PStrCpy (MtpPortName, name);
	if (index > 9) {
		name[0]++;
		name[name[0]] = '1';
		index -= 10;
	}
	name[0]++;
	name[name[0]] = '0' + index;
	return name;
}

/* -----------------------------------------------------------------------------*/
/* MidiShare part                                                               */
/* -----------------------------------------------------------------------------*/

/* -----------------------------------------------------------------------------*/
static pascal void ReceiveEvents (short r)
{
	MtpDrvPtr data = GetData ();
	SCCPtr scc;
	MidiEvPtr e = MidiGetEv(r);
	Boolean rcv;

	rcv = false;
	while (e)
	{
		scc = &data->scc;
		if (scc->state && (scc->outSlots[Port(e)]!=-1)) 
		{
			if((scc->outSlots[Port(e)]) != (scc->MTPOutPort+15))
			{
				MidiEvPtr cp=MidiNewEv(typeStream);
				scc->MTPOutPort = scc->outSlots[Port(e)]-15;
				MidiAddField(cp,0xf5);				
				MidiAddField(cp,scc->MTPOutPort);
				Link(cp) = 0;
				INT_OFF();
				Link(scc->seq.last) = cp;
				scc->seq.last = cp;
				INT_ON();
			}
			Link(e) = 0;
			INT_OFF();
			Link(scc->seq.last) = e;
			scc->seq.last = e;
			INT_ON();
			rcv = true;
		}
		else MidiFreeEv (e);
		e = MidiGetEv(r);
	}
	if (rcv && !MidiStreamCountByte(&scc->xmt))
		StartXmt (scc);
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
static void add16Slot()
{
	MtpDrvPtr data = GetData ();
	short i; SlotRefNum sref; SlotName name;

	for (i=0; i<16; i++) {
		sref = MidiAddSlot (data->refNum, MakeSlotName(i+1, name), MidiInputSlot);
		if (Slot(sref) >= 0) {
			data->scc.slots[i]=Slot(sref);
			data->scc.inSlots[i]=Slot(sref);
		}
	}

	for (i; i<32; i++) {
		sref = MidiAddSlot (data->refNum, MakeSlotName(i-15, name), MidiOutputSlot);
		if (Slot(sref) >= 0) {
			data->scc.slots[i]=Slot(sref);
			data->scc.outSlots[Slot(sref)]= i;
		}
	}
}

/* -----------------------------------------------------------------------------*/
static void DataInit (MtpDrvPtr data)
{
	SCCPtr scc = &data->scc;
	short i;

	data->refNum = 0;			//initialisation du numero de ref du driver
	MidiParseInitTypeTbl (data->s2t);
	MidiParseInitMthTbl (data->rTbl);
	MidiStreamInitMthTbl (data->lTbl);

	scc->MTPOutPort = kNoCurrentPort;
	for (i=0; i<16; i++) scc->inSlots[i] = -1;
	for (i=0; i<256; i++) scc->outSlots[i] = -1;
	for (i=0; i<32; i++) scc->slots[i] = -1;
	scc->checkTask = 0;
	scc->state = false;
	scc->seq.last = (MidiEvPtr)&scc->seq.first;
	scc->waitEvent = 0;
}

/* -----------------------------------------------------------------------------*/
Boolean SetUpMidi ()
{
	TDriverInfos infos = { MtpDriverName, 100, 0};
	short refNum;
	TDriverOperation op = {WakeUp, Sleep, 0, 0, 0};
	MtpDrvPtr data = GetData ();
	
	DataInit (data);
	if (MidiGetNamedAppl (MtpDriverName) > 0) { // still running
		doneFlag = true;
		return true;
	}

	refNum = MidiRegisterDriver(&infos, &op);
	if (refNum == MIDIerrSpace)
		return false;

	data->refNum = refNum;
	data->scc.refNum = refNum;

	MidiSetRcvAlarm (refNum, ReceiveEvents);		
	SetupFilter (&data->filter);
	MidiSetFilter (refNum, &data->filter);	
	return true;
}

/* -----------------------------------------------------------------------------*/
void Start()
{
	MtpDrvPtr data = GetData ();

	add16Slot();
	RestoreDriverState (GetData ()->refNum, StateFile);
}

/* -----------------------------------------------------------------------------*/
void CloseMidi ()
{
	MtpDrvPtr data = GetData ();
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
}
		
/* -----------------------------------------------------------------------------*/
/* SCC part                                                                     */
/* -----------------------------------------------------------------------------*/
Boolean CheckSCC ()
{
	MtpDrvPtr data = GetData ();
	long reply;
	OSErr err = Gestalt (gestaltHardwareAttr, &reply);
	
	if ((err == noErr) && (reply & (1 << gestaltHasSCC))) 
	{
		err= Gestalt (gestaltPowerMgrAttr, &reply);
		if( err== noErr)
			 data->powerMgr = reply & (1 << gestaltPMgrExists);
		else
			 data->powerMgr = false;
		return true;
	}
	return false;
}

/* -----------------------------------------------------------------------------*/
Boolean CheckSCCFree()
{
	return __CheckSCCFree();
}