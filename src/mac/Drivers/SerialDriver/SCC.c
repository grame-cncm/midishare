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


#include <Power.h>

#include "SCC.h"

//_______________________________________________________________________
#define PortAUse	0x290	/* bit 7 = 1 signifie : NOT in use */
#define PortBUse	0x291	/* bit 7 = 1 signifie : NOT in use */
#define SPConfig	0x1FB	/* config des 2 ports (A: MSnibble, B: LSnibble) */

#define useFree		0x0	/* usage libre d'un port (ˆ placer dans SPConfig) (dŽjˆ dŽfini) */
#define useAMidiShare	0x70	/* usage MidiShare du port A dans SPConfig */
#define useBMidiShare	0x07	/* usage MidiShare du port B dans SPConfig */

/*------------------------------------------------------------------------*/
/* Vecteurs sur les registres Hardware du Macintosh */

#define SCCptr		0x1d8
#define SCCreadptr	0x1d8
#define SCCwriteptr	0x1dc

#define ACtl		2		/* Offset des registres SCC */
#define AData		6
#define BCtl		0
#define BData		4

/*------------------------------------------------------------------------*/
/* Vecteurs d'interruptions du Macintosh */
#define xmtVectorA	    0x1c2	/* Modem Port : send			*/
#define rcvVectorA	    0x1ca	/* Modem Port : normal rcv		*/
#define SpecialVectorA	0x1ce	/* Modem Port : special rcv		*/

#define xmtVectorB	    0x1b2	/* Printer Port : send			*/
#define rcvVectorB	    0x1ba	/* Printer Port : normal rcv	*/
#define SpecialVectorB	0x1be	/* Printer Port : special rcv	*/

//_______________________________________________________________________
#define WriteDataA	((*(unsigned char **)SCCwriteptr) + AData)
#define WriteDataB	((*(unsigned char **)SCCwriteptr) + BData)
#define ReadDataA	((*(unsigned char **)SCCreadptr) + AData)
#define ReadDataB	((*(unsigned char **)SCCreadptr) + BData)

#define WriteCtlA  	((*(char **)SCCwriteptr) + ACtl)
#define WriteCtlB  	((*(char **)SCCwriteptr) + BCtl)

#define LockedCall(f)	INT_OFF(); f; INT_ON()

//_______________________________________________________________________
static asm void SCCDataConstants () {
	dc.b	0x04,0x84,0x01,0x00,0x03,0xc0,0x05,0x62
	dc.b	0x0b,0x28,0x03,0xc1,0x05,0x6a,0x0f,0x08,0x00,0x10
	dc.b	0x00,0x10,0x01,0x13,0x09,0x0a,0x28,0x00
}

//_______________________________________________________________________
static asm void SCCIntHandlerData () { 
	dc.l	0, 0 
}
static inline SCCPtr GetData(short port) 
				{ 	SCCPtr * ptr = (SCCPtr *)SCCIntHandlerData; return ptr[port]; }

static void SetData(short port, SCCPtr data) 
{ 	
	SCCPtr * ptr = (SCCPtr *)SCCIntHandlerData;
	ptr[port] = data; 
}

static inline void Send (MidiEvPtr e, SCCPtr scc) 
				{ Port(e) = scc->slot; MidiSend (scc->refNum, e); }

//_______________________________________________________________________
static void wait()
{
	int  i = 0xfff;
	while (i--)
		;
}

//_______________________________________________________________________
static pascal void KeyOffTask (long date, short refNum, long s, long ev, long a3)
{
	SCCPtr scc = (SCCPtr)s;
	MidiEvPtr e = (MidiEvPtr)ev;

	Link(e) = 0;
	INT_OFF();
	Link(scc->seq.last) = e;
	scc->seq.last = e;
	INT_ON();
	if (!MidiStreamCountByte(&scc->xmt))
		StartXmt (scc);
}

//_______________________________________________________________________
static MidiEvPtr Pop (MidiSeqPtr seq) 
{
	MidiEvPtr e;
	INT_OFF();
	e = seq->first;
	if (e) {
		seq->first = Link(e);
		if (!seq->first)
			seq->last = (MidiEvPtr)&seq->first;
	}
	INT_ON();
	return e;
}

//_______________________________________________________________________
static Boolean GetByte (SCCPtr scc, Byte *c)
{
	if (MidiStreamGetByte(&scc->xmt, c))
		return true;
	else {
		MidiEvPtr e = Pop(&scc->seq);
		if (e) {
			e = MidiStreamPutEvent	(&scc->xmt, e);
			if (e) MidiTask (KeyOffTask, Date(e), 0, (long)scc, (long)e, 0);
			return MidiStreamGetByte(&scc->xmt, c);
		}
	}
	return false;
}

//_______________________________________________________________________
static void xmtHandlerA()
{
	Byte c; SCCPtr scc = GetData (kModem);
	if (GetByte(scc, &c))
		*WriteDataA= c;
	else 
		*WriteCtlA= 0x28;
}

//_______________________________________________________________________
static void xmtHandlerB()
{
	Byte c; SCCPtr scc = GetData (kPrinter);
	if (GetByte (scc, &c))
		*WriteDataB= c;
	else
		*WriteCtlB= 0x28;
}

//_______________________________________________________________________
static void rcvHandlerA()
{
	SCCPtr scc = GetData (kModem);
	MidiEvPtr e = MidiParseByte (&scc->rcv, *ReadDataA);
	if (e) Send (e , scc);
}

//_______________________________________________________________________
static void rcvHandlerB()
{
	SCCPtr scc = GetData (kPrinter);
	MidiEvPtr e = MidiParseByte (&scc->rcv, *ReadDataB);
	if (e) Send (e , scc);
}

//_______________________________________________________________________
static void rcvSpecialA()
{
	SCCPtr scc = GetData (kModem); MidiEvPtr e;
	char read= *ReadDataA;
	wait();
	e = MidiParseByte (&scc->rcv, *ReadDataA);
	if (e) Send (e , scc);
}

//_______________________________________________________________________
static void rcvSpecialB()
{
	SCCPtr scc = GetData (kPrinter); MidiEvPtr e;
	char read= *ReadDataA;
	wait();
	e = MidiParseByte (&scc->rcv, *ReadDataB);
	if (e) Send (e , scc);
}

//_______________________________________________________________________
static void StopSCC (char *WriteCtrl)
{
	*WriteCtrl = 0x01;
	wait();
	*WriteCtrl = 0xe1;
}

//_______________________________________________________________________
static void ResetSCCA (char *WriteCtrl)
{
	*WriteCtrl = 0x09;
	wait();
	*WriteCtrl = 0x80;
}

//_______________________________________________________________________
static void ResetSCCB (char *WriteCtrl)
{
	*WriteCtrl = 0x09;
	wait();
	*WriteCtrl = 0x40;
}

//_______________________________________________________________________
static void InitSCC (char *WriteCtrl, char *SCCData)
{
	int i = 25;
	while (i--) {
		*WriteCtrl = *SCCData++;
		wait();
	}
}

//_______________________________________________________________________
static void InitVectA ()
{
	*(SCCIntHandler *)xmtVectorA= xmtHandlerA;
	*(SCCIntHandler *)rcvVectorA= rcvHandlerA;
	*(SCCIntHandler *)SpecialVectorA= rcvSpecialA;
}

//_______________________________________________________________________
static void InitVectB ()
{
	*(SCCIntHandler *)xmtVectorB= xmtHandlerB;
	*(SCCIntHandler *)rcvVectorB= rcvHandlerB;
	*(SCCIntHandler *)SpecialVectorB= rcvSpecialB;
}

//_______________________________________________________________________
static void SetupPortA()
{
	InitVectA();          		 // init vecteurs d'interruptions port modem
	ResetSCCA( WriteCtlA);       // reset controleur serie port modem
	InitSCC( WriteCtlA, (char *)SCCDataConstants);
}

//_______________________________________________________________________
static void SetupPortB()
{
	InitVectB();          		 // init vecteurs d'interruptions port modem
	ResetSCCB( WriteCtlB);       // reset controleur serie port modem
	InitSCC( WriteCtlB, (char *)SCCDataConstants);
}

/*__________________________________________________________________________________*/
static void ClearSeq (MidiSeqPtr seq)
{
	if (seq->first) {
		MidiClearSeq (seq);
		seq->last = (MidiEvPtr)&seq->first;
	}
}

/*__________________________________________________________________________________*/
static pascal void CheckFifo (long date, short refNum, long a1, long a2, long a3)
{
	SCCPtr scc = (SCCPtr)a1;
	MidiEvPtr e = scc->seq.first;
	
	if (e && (MidiGetTime() - Date(e) > 100)) {
		ClearSeq (&scc->seq);
	}
	scc->checkTask= MidiTask (CheckFifo, date + 50, refNum, a1, a2, a3);
}

//_______________________________________________________________________
static void ResetData (SCCPtr scc)
{
	MidiForgetTask (&scc->checkTask);
	ClearSeq (&scc->seq);
	MidiStreamReset (&scc->xmt);
	MidiParseReset (&scc->rcv);
}

//_______________________________________________________________________
void StartXmt (SCCPtr scc) 
{
	Byte c;
	if (GetByte (scc, &c)) {
		if (scc->port)
			*WriteDataB= c;
		else
			*WriteDataA= c;
	}
}

//_______________________________________________________________________
static Boolean __OpenLineA ()
{
	char used, nibble; 
	Boolean ret = false;
	
	INT_OFF();
	used= *(char *)PortAUse;
	if( used & 0x80) {                    // si le port n'est pas en service
		used &= 0x7f;                     // bit 7 ˆ 0
		*(char *)PortAUse= used;          // indique port occupe
		nibble= *(char *)SPConfig;        // indique que le port A
		nibble &= 0x0f | useAMidiShare;   // utilise le driver MidiShare
		*(char *)SPConfig= nibble;
		ret = true;
	}
	INT_ON();
	return ret;
}

//_______________________________________________________________________
Boolean OpenLineA (SerialDrvPtr data)
{
	SCCPtr scc = &data->scc[kModem];

	if (__OpenLineA()) {
        if( data->powerMgr) AOnIgnoreModem();
		
		SetData	(kModem, scc);	
		MidiParseInit (&scc->rcv, data->rTbl, data->s2t);
		MidiStreamInit (&scc->xmt, data->lTbl);
		scc->seq.last = (MidiEvPtr)&scc->seq.first;
		scc->port = kModem;
		scc->slot = Slot(data->slot[kModem]);
		scc->refNum = data->refNum;

		LockedCall(SetupPortA());
		scc->checkTask = MidiTask (CheckFifo, MidiGetTime()+100, data->refNum, (long)scc, 0, 0);
		scc->state = true;
		return true;
	}
	return false;
}

//_______________________________________________________________________
static void __CloseLineA()
{
	char used, nibble;
	used= *(char *)PortAUse;
	used|= 0x80;
	*(char *)PortAUse= used;
	nibble= *(char *)SPConfig;
	nibble&= 0x0f;
	*(char *)SPConfig= nibble;
	StopSCC( WriteCtlA);
}

//_______________________________________________________________________
void CloseLineA (SerialDrvPtr data)
{
	SCCPtr scc = &data->scc[kModem];
	scc->state = false;
	LockedCall(__CloseLineA());
    if (data->powerMgr) AOff();
	ResetData (scc);
}

//_______________________________________________________________________
static Boolean __OpenLineB ()
{
	char used, nibble; 
	Boolean ret = false;
	
	INT_OFF();
	used= *(char *)PortBUse;
	if( used & 0x80) {
		used&= 0x7f;
		*(char *)PortBUse= used;
		nibble= *(char *)SPConfig;
		nibble&= 0xf0 | useBMidiShare;
		*(char *)SPConfig= nibble;
		ret = true;
	}
	INT_ON();
	return ret;
}

//_______________________________________________________________________
Boolean OpenLineB (SerialDrvPtr data)
{
	SCCPtr scc = &data->scc[kPrinter];

	if (__OpenLineB()) {
        if (data->powerMgr) BOn();

		SetData	(kPrinter, scc);	
		MidiParseInit (&scc->rcv, data->rTbl, data->s2t);
		MidiStreamInit (&scc->xmt, data->lTbl);
		scc->seq.last = (MidiEvPtr)&scc->seq.first;
		scc->port = kPrinter;
		scc->slot = Slot(data->slot[kPrinter]);
		scc->refNum = data->refNum;
		
		LockedCall(SetupPortB());
		scc->checkTask = MidiTask (CheckFifo, MidiGetTime()+100, data->refNum, (long)scc, 0, 0);
		scc->state = true;
		return true;
	}
	return false;
}

//_______________________________________________________________________
static void __CloseLineB ()
{
	char used, nibble;
	used= *(char *)PortBUse;
	used|= 0x80;
	*(char *)PortBUse= used;
	nibble= *(char *)SPConfig;
	nibble&= 0xf0;
	*(char *)SPConfig= nibble;
	StopSCC( WriteCtlB);
}

//_______________________________________________________________________
void CloseLineB (SerialDrvPtr data)
{
	SCCPtr scc = &data->scc[kPrinter];
	scc->state = false;
	LockedCall(__CloseLineB());
    if (data->powerMgr) BOff();
	ResetData (scc);
}
