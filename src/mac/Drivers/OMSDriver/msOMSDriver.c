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


#include <QuickTimeMusic.h>
#include <Gestalt.h>
#include <MIDI.h>

#include "MidiShare.h"
#include "msOMSDriver.h"
#include "EmulLinearise.h"

#include "OMS.h"


/* ----------------------------------*/
/* constants definitions             */
#define MidiShareDrvRef	 127

#define MySignature		'Mshd'
#define InputPortID		'in  '
#define OutputPortID	'out '

#define kOMSInputNodes (omsIncludeInputs+omsIncludeReal+omsIncludeVirtual)
#define kOMSOutputNodes (omsIncludeOutputs+omsIncludeReal+omsIncludeVirtual)

typedef struct {
	short			CompatMode;				// current OMS compatibility mode          
	short 			OutputPortRefNum;       // OMS output port refNum
	short			InputPortRefNum;		// OMS input port refNum
	Boolean       	OMSRunning;             // OMS state
	
	Ev2PacketFunc	e2p[256];				// Event to Packet conversion methods
	Packet2EvFunc	p2e[256];				// Packet to Event conversion methods
	Byte			typeTbl[256];			// Type conversion table
	
	long 	numNodeOut;						// Number of OMS output nodes
	long 	numNodeIn;						// Number of OMS input nodes
	long	slot2OMSIn[64];					// SlotRefnum <---> OMS IN ioRefnum table
	long	slot2OMSOut[64];				// SlotRefnum <---> OMS OUT ioRefnum table
	char	slot2InIndex[64];				// SlotRefnum <---> index in OMDNodeInfos table
	char	slot2OutIndex[64];				// SlotRefnum <---> index in OMDNodeInfos table
	
	OMSNodeInfoListH OMSInputNodes;				// OMS Input node list
	OMSNodeInfoListH OMSOutputNodes;			// OMS Output node list 
	OMSConnectionParamsPtr ConnectionParams;	// Connections list
	
	P2EInfos            pei; 				// Packet to event conversion structure
	OMSMIDIPacket255	mPack2;				// Statically allocated OMS packet

} DriverData, *DriverDataPtr;

typedef struct {
	DriverDataPtr	data;
	short			refNum;
	TFilter         filter;
} Storage, * StoragePtr;


/* ----------------------------------*/
/* some macros                       */
#define OMSPacket(data)			&(data)->mPack2
#define P2EInfos(data)			(data)->pei
#define OMSInputNodes(data)	    (data)->OMSInputNodes
#define OMSOutputNodes(data)	(data)->OMSOutputNodes
#define ConnectionParams(data)	(data)->ConnectionParams
#define OutputPortRefNum(data)	(data)->OutputPortRefNum
#define InputPortRefNum(data)	(data)->InputPortRefNum
#define CompatMode(data)		(data)->CompatMode
#define OMSRunning(data)		(data)->OMSRunning
#define slotOutRef(data)		(data)->slotOutRef
#define slotInRef(data)			(data)->slotInRef
#define e2p(data)				(data)->e2p
#define p2e(data)				(data)->p2e
#define typeTbl(data)			(data)->typeTbl
#define slot2OMSIn(data)		(data)->slot2OMSIn
#define slot2OMSOut(data)		(data)->slot2OMSOut
#define slot2InIndex(data)		(data)->slot2InIndex
#define slot2OutIndex(data)		(data)->slot2OutIndex
#define numNodeOut(data)		(data)->numNodeOut
#define numNodeIn(data)			(data)->numNodeIn


/* ----------------------------------*/
/* functions declarations            */
static void 	pTocCopy(char *dest,  unsigned char * src);  
static void 	cTocCopy(char *dest,  char * src); 
static Boolean	pEquals (unsigned char *a,  unsigned char *b);
static void 	cTopCopy(unsigned char *dest,  char * src);
static void		PStrCpy (unsigned char *src, unsigned char * dst, short max);

static void 	SendEvents 		(short slot, OMSMIDIPacket255 *pkt, DriverDataPtr data);
static Boolean	EvToOMSPacket 	(E2PInfos *i, OMSMIDIPacket255 *p, DriverDataPtr data);
static void 	SendDatas		( short refnum, short port, short flag, long date, DriverDataPtr data);
static short 	GetSlotFromRef	( DriverDataPtr data, short refNum);

static pascal void	MyReadHook2(OMSMIDIPacket *pkt, long myRefCon);
static pascal void 	MyAppHook(OMSAppHookMsg *pkt, long myRefCon);

static OSErr GetOutputPorts(DriverDataPtr data);
static OSErr GetInputPorts(DriverDataPtr data);

static Boolean 	OpenOMS(OSType appSignature, OSType inPortID, OSType outPortID, DriverDataPtr data);
static void 	OMSDispose(OSType appSignature, DriverDataPtr data);
static OSErr 	PrepareConnectionsParams(DriverDataPtr data);
static OSErr 	MakeConnections(OSType appSignature, OSType inPortID, DriverDataPtr data);

#ifdef __CodeRsrc__
void __Startup__();
void __Startup__()
{
	if (CheckOMS())
		SetUpMidi ();
}
#endif

static asm void storage () {  	/* total 104 bytes */
	dc.l	0 					/* dynamic memory ptr */
	dc.w	0					/* MidiShare refnum */
	dc.l	0,0,0,0,0,0,0,0		/* MidiShare filter */
	dc.l	0,0,0,0,0,0,0,0
	dc.l	0,0,0,0,0,0,0,0
}

static inline StoragePtr 	GetStorage() 	{ return (StoragePtr)storage; }
static inline short 		GetRefNum ()	{ StoragePtr mem = GetStorage(); return mem->refNum; }
static inline DriverDataPtr GetData ()		{ StoragePtr mem = GetStorage(); return mem->data; }

/* -----------------------------------------------------------------------------*/
/* String utilities                                                    			*/
/* -----------------------------------------------------------------------------*/
static void pTocCopy(  char *dest,  unsigned char * src)
{
	register short i;
	
	i= *src++;
	while( i--)
		*dest++= *src++;
	*dest= 0;
}

/*--------------------------------------------------------------------------*/
static void cTocCopy(  char *dest,  char * src)
{
	register short i = 0;
	
	while (src[i] != 0) {
		dest[i] = src[i];
		i++;
	}
	dest[i] = 0;	
}

/*--------------------------------------------------------------------------*/
static Boolean pEquals( unsigned char *a,  unsigned char *b) {
	int i;
	if (a[0] != b[0]) return false;
	
	for(i = 0 ; i<= a[0]; i++) {
		if (a[i] != b [i]) return false;
	}
	return true;
}

/*--------------------------------------------------------------------------*/
static void cTopCopy(  unsigned char *dest,  char * src)
{
	register short i = 0;
	
	while (src[i] != 0) {
		dest[i+1] = src[i];
		i++;
	}
	dest[0] = i;	
}

/* -----------------------------------------------------------------------------*/
static void PStrCpy (unsigned char *src, unsigned char * dst, short max)
{
	short i = src[0] + 1;
	while (i-- && max--)
		*dst++ = *src++;
}

//____________________________________________________________________________________

/* -----------------------------------------------------------------------------*/
/* Driver required callbacks                                                    */
/* -----------------------------------------------------------------------------*/
static void WakeUp (short r)
{
	int i;
	StoragePtr mem = GetStorage();
	DriverDataPtr data = (DriverDataPtr)NewPtrSys(sizeof(DriverData));
	if (!data) return;
	
	mem->data = data;
	data->OMSRunning = false;
	data->pei.src = 0;
	data->pei.len = 0;
	data->pei.cont = 0;
	
	for (i = 0; i<64; i++) {
		data->slot2OMSOut[i] = -1;
		data->slot2OMSIn[i] = -1;
		data->slot2OutIndex[i] = -1;
		data->slot2InIndex[i] = -1;
	}
	
	if (!mem->refNum) mem->refNum = r;

	InitTypeTbl(data->typeTbl);
	InitLinearizeMthTbl(data->e2p, data->p2e);

	if (OpenOMS (MySignature, InputPortID, OutputPortID, data)){
		MidiConnect (MidiShareDrvRef, r, true);
		MidiConnect (r, MidiShareDrvRef, true);
	}
}

/* -----------------------------------------------------------------------------*/
static void Sleep (short r)
{
	StoragePtr mem = GetStorage();
	DriverDataPtr data = mem->data;
	
	if (data) {
		if (OMSInputNodes(data)) {
			DisposeHandle ((Handle)OMSInputNodes(data));
			OMSInputNodes(data) = 0;
		}
		if (OMSOutputNodes(data)) {
			DisposeHandle ((Handle)OMSOutputNodes(data));
			OMSOutputNodes(data) = 0;
		}
		OMSDispose (MySignature, data);
		DisposePtr ((Ptr)data);
		mem->data = 0;
	}
}

/* -----------------------------------------------------------------------------*/
static Boolean SlotInfo (SlotRefNum slot, TSlotInfos * infos)
{
	DriverDataPtr data = GetData (); short i, ref;

	if (infos) {
		ref = Slot(slot);
		i = slot2InIndex(data)[ref];
		if (i >= 0) {
			PStrCpy ((*OMSInputNodes(data))->info[i].name, infos->name, DrvNameLen);
			infos->direction = MidiInputSlot;
			return true;
		}
		
		i = slot2OutIndex(data)[ref];
		if (i >= 0) {
			PStrCpy ((*OMSOutputNodes(data))->info[i].name, infos->name, DrvNameLen);
			infos->direction = MidiOutputSlot;
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
	MidiEvPtr 	e; OMSMIDIPacket255 *p;
	E2PInfos 	i; Boolean contFlag;
	DriverDataPtr data = GetData();
	
	if (!data) {
		MidiFlushEvs (r);
		return;
	}

	p = OMSPacket(data);

	while (e = MidiGetEv (r)) {
		i.cont = 0;
		i.evToSend = e;
		RefNum(e)= r;
		do {
			contFlag = EvToOMSPacket (&i, p, data);
			if (p->len) SendEvents(Port(e), p, data);
		} while( contFlag);
		MidiFreeEv(e);
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
Boolean SetUpMidi ()
{
	TDriverInfos infos = { OMSDriverName, 100, 0};
	short refNum; TDriverOperation op; 
	StoragePtr mem = GetStorage();

	op.wakeup = WakeUp;
	op.sleep = Sleep;
	op.slotInfo = SlotInfo; 
	mem->refNum = 0;

	if (MidiGetNamedAppl (OMSDriverName) > 0)  // still running
		return true;

	refNum = MidiRegisterDriver(&infos, &op);
	if (refNum == MIDIerrSpace)
		return false;

	mem->refNum = refNum;
	MidiSetRcvAlarm (refNum, ReceiveEvents);		
	SetupFilter (&mem->filter);
	MidiSetFilter (refNum, &mem->filter);
	return true;
}

/* -----------------------------------------------------------------------------*/
void CloseMidi ()
{
	StoragePtr mem = GetStorage();
	MidiUnregisterDriver (mem->refNum);
}

/* -----------------------------------------------------------------------------*/
/* OMS part                                                               		*/
/* -----------------------------------------------------------------------------*/
Boolean CheckOMS()
{
	long OMSnum = OMSVersion();
	// Check is OMS is installed
	if (OMSnum == 0) return false;	/* any old error number */
	// Check OMS version
	if (OMSnum>>16 <= 0x200)  return false;
	return true;
}

/* -----------------------------------------------------------------------------*/
static pascal void	MyReadHook2(OMSMIDIPacket *pkt, long myRefCon)
{
	DriverDataPtr data = (DriverDataPtr)myRefCon;
	short slot = GetSlotFromRef(data, pkt->srcIORefNum);
	
	P2EInfos(data).src= pkt->data;
	P2EInfos(data).len= pkt->len;

	if (slot >= 0) SendDatas(GetRefNum(), slot, pkt->flags & 3, MidiGetTime(), data);
}

/* -----------------------------------------------------------------------------*/
static pascal void MyAppHook(OMSAppHookMsg *pkt, long myRefCon)
{
	DriverDataPtr data = (DriverDataPtr)myRefCon;
	
	switch (pkt->msgType) {
		case omsMsgModeChanged:
			//printf("omsMsgModeChanged\n");
			/* Respond to compatibility mode having changed */
			//gCompatMode = pkt->u.modeChanged.newMode;
			/* this will cause side effects in the event loop */
			break;
		case omsMsgDestDeleted:
			//printf("omsMsgDestDeleted\n");
			//if (gChosenOutputID == pkt->u.nodeDeleted.uniqueID) {
			//	gOutNodeRefNum = -1;	/* invalid */
				//gEmul.OutNodeRefNum = -1; /* invalid */
			///}
			break;
		case omsMsgSourceDeleted:
			//printf("omsMsgSourceDeleted\n");
			break;
			
		case omsMsgNodesChanged:
			//printf("omsMsgNodesChanged\n");
			//GetOutputPorts();
			//GetInputPorts();
			//printf("p\n");
			//gNodesChanged = TRUE;
			break;
		
		case omsMsgDifferentStudioSetup:
			//printf("omsMsgDifferentStudioSetup\n");
			break;
	}
}


/* -----------------------------------------------------------------------------*/
static void NodeInfoCopy (OMSNodeInfoListH dst, OMSNodeInfoListH src, long size)
{
	char * cdst = *(char **)dst;
	char * csrc = *(char **)src;
	while (size--)
		*cdst++ = *csrc++;
}

/* -----------------------------------------------------------------------------*/
static OSErr GetOutputPorts(DriverDataPtr data) 
{
	int i; SlotRefNum sref; OMSNodeInfoListH nodesInfos, h;
	long size; short refNum = GetRefNum();
	
	nodesInfos = OMSGetNodeInfo(kOMSOutputNodes);
	if (!nodesInfos) return 1;

	size = InlineGetHandleSize ((Handle)nodesInfos);
	h = (OMSNodeInfoListH)NewHandleSys (size);
	if (!h) {
		OMSDisposeHandle (nodesInfos);
		return 1;
	}

	NodeInfoCopy (h, nodesInfos, size);
	OMSDisposeHandle (nodesInfos);
	OMSOutputNodes(data) = h;
	
	numNodeOut(data) = (*h)->numNodes;
	for (i = 0; i < numNodeOut(data); i++) {			
		sref = MidiAddSlot (refNum);
		if (sref < 0) return 1;
		slot2OutIndex(data)[Slot(sref)] = i;
		slot2OMSOut(data)[Slot(sref)] = (*h)->info[i].ioRefNum;
	}
	return 0;
}


/* -----------------------------------------------------------------------------*/
static OSErr GetInputPorts (DriverDataPtr data)
{
	int i; SlotRefNum sref; OMSNodeInfoListH nodesInfos, h;
	long size; short refNum = GetRefNum();
	
	nodesInfos = OMSGetNodeInfo(kOMSInputNodes);
	if (!nodesInfos) return 1;
	
	size = InlineGetHandleSize ((Handle)nodesInfos);
	h = (OMSNodeInfoListH)NewHandleSys (size);
	if (!h) {
		OMSDisposeHandle (nodesInfos);
		return 1;
	}
	
	NodeInfoCopy (h, nodesInfos, size);
	OMSDisposeHandle (nodesInfos);
	OMSInputNodes(data) = h;
	
	numNodeIn(data) = (*h)->numNodes;
	for (i = 0; i < numNodeIn(data); i++) {	
		sref = MidiAddSlot (refNum);
		if (sref < 0) return 1;
		slot2InIndex(data)[Slot(sref)] = i;
		slot2OMSIn(data)[Slot(sref)] =  (*h)->info[i].ioRefNum;
	}
	return 0;
}

//__________________________________________________________________________________
 
static void SendDatas( short refnum, short slot, short flag, long date, DriverDataPtr data)
{
	MidiEvPtr e, next; P2EInfosPtr i = &P2EInfos(data);
	
	e= Packet2Evs (i, p2e(data), typeTbl(data));
	
	if( e) {
		
		Port(e)= slot;
		if( (flag== midiNoCont) || (flag== midiEndCont))
			i->cont= 0;
		else {
			i->cont= e;
			e= Link(e);
		}
		while( e) {
			next= Link(e);
			Date(e)= date;
			MidiSend( refnum, e);
			e= next;
		}
	}
	else i->cont= 0;
}


/* -----------------------------------------------------------------------------*/
static short GetSlotFromRef( DriverDataPtr data, short refNum)
{
	int i;

	for(i = 0; i< 64; i++) { if (slot2OMSIn(data)[i] == refNum) return i;}
	return -1;
}


/* -----------------------------------------------------------------------------*/
static OSErr PrepareConnectionsParams (DriverDataPtr data)
{
	int i;

	ConnectionParams(data) = (OMSConnectionParamsPtr)NewPtrSys(numNodeIn(data) * sizeof(OMSConnectionParams));
	
	if (ConnectionParams(data) != NULL) {
		for (i=0; i< numNodeOut(data); i++) {
			ConnectionParams(data)[i].nodeUniqueID = (*OMSOutputNodes(data))->info[i].uniqueID;
			ConnectionParams(data)[i].appRefCon = 0;
		}
	}else {
		return 1;
	}

	return 0;
}

/* -----------------------------------------------------------------------------*/
static OSErr MakeConnections(OSType appSignature, OSType inPortID, DriverDataPtr data)
{
	OSErr err;
	// Gestion des connections
	err = OMSOpenConnections(appSignature,inPortID,(*OMSInputNodes(data))->numNodes,ConnectionParams(data),false);
	if (err != noErr && err != 2) {
		//printf("Error returned from OMSOpenConnections()\n");
		return 1;
	}else 
		return 0;
}

/* -----------------------------------------------------------------------------*/
static Boolean EvToOMSPacket (E2PInfos *i, OMSMIDIPacket255 *p, DriverDataPtr data)
{
	MidiEvPtr oldCont = i->cont;

	i->dest= p->data;
	i->free= 255;
	p->len = EvToPacket (i, e2p(data));
	if (i->cont)
		p->flags= oldCont ? midiMidCont : omsStartCont;
	else
		p->flags= oldCont ? omsEndCont : omsNoCont;
	return p->len ? i->cont != 0 : false;
}

/* -----------------------------------------------------------------------------*/
static void SendEvents (short slot, OMSMIDIPacket255 *pkt, DriverDataPtr data)
{
	if (slot > (*OMSOutputNodes(data))->numNodes) {
	} else {
		OMSWritePacket255 (pkt, slot2OMSOut(data)[slot], OutputPortRefNum(data));
	}
}

/* -----------------------------------------------------------------------------*/

Boolean	OpenOMS(OSType appSignature, OSType inPortID, OSType outPortID, DriverDataPtr data)
{
	OSErr err;
	OMSAppHookUPP appHook; OMSReadHook2UPP readHook2;
	
#ifdef powerc
	appHook = NewOMSAppHook(MyAppHook);
	readHook2 = NewOMSReadHook2(MyReadHook2);
#else
	appHook = MyAppHook;
	readHook2 = MyReadHook2;
#endif
	
	/*	Sign in to OMS */
	err = OMSSignIn(appSignature, (long)data, "\pmsOMS", appHook, &CompatMode(data));
	if (err) return (err!= noErr);
	
	/*	Add an input port */
	err = OMSAddPort(appSignature, inPortID, omsPortTypeInput2, readHook2, (long)data, &InputPortRefNum(data));
	if (err) goto errexit;
	
	/*	Add an output port */
	err = OMSAddPort(appSignature, outPortID, omsPortTypeOutput, NULL, 0L, &OutputPortRefNum(data));
	if (err) goto errexit;
	
	// Info on OMS input/output
	err = GetInputPorts (data);
	if (err) goto errexit;

	err = GetOutputPorts (data);
	if (err) goto errexit;

	// Build connections
	err = PrepareConnectionsParams (data);
	if (err) goto errexit;
	
	// Connections management
	err = MakeConnections (appSignature, inPortID, data);
	if (err) goto errexit;
	 
	OMSRunning(data) = true;
	return true;

errexit:
	OMSSignOut(appSignature);
	return  false;	
}

/* -----------------------------------------------------------------------------*/
static void OMSDispose(OSType appSignature, DriverDataPtr data) 
{
	if (OMSRunning(data)) {
		OMSSignOut(appSignature);
		OMSRunning(data) = false;
	}
}

