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
#include "MidiPrint.h"

//_______________________________________________________________________
#define SPConfig		0x1FB	/* config des 2 ports (A: MSnibble, B: LSnibble) */
#define useFree			0x0		/* usage libre d'un port (ˆ placer dans SPConfig) (dŽjˆ dŽfini) */

/*------------------------------------------------------------------------*/
/* Vecteurs sur les registres Hardware du Macintosh */
#define SCCptr		0x1d8
#define SCCreadptr	0x1d8
#define SCCwriteptr	0x1dc

//_______________________________________________________________________
#define WriteData	((*(volatile unsigned char **)SCCwriteptr) + Data_)
#define ReadData	((*(volatile unsigned char **)SCCreadptr) + Data_)

#define WriteCtl  	((*(char **)SCCwriteptr) + Ctl)

#define LockedCall(f)	INT_OFF(); f; INT_ON()

//_______________________________________________________________________
static asm void SCCDataConstants () {
	dc.b	0x04,0x04,0x01,0x00,0x03,0xc0,0x05,0x62
	dc.b	0x0b,0x28,0x03,0xc1,0x05,0x6a,0x0f,0x08,0x00,0x10
	dc.b	0x00,0x10,0x01,0x13,0x09,0x0a,0x28,0x00
}

//_______________________________________________________________________
static asm void SCCIntHandlerData () { 
	dc.l	0, 0 
}
static inline SCCPtr GetData() 
				{ 	SCCPtr * ptr = (SCCPtr *)SCCIntHandlerData; return *ptr; }

static void SetData(SCCPtr data) 
{ 	
	SCCPtr * ptr = (SCCPtr *)SCCIntHandlerData;
	*ptr = data; 
}

static inline void Send (MidiEvPtr e, SCCPtr scc) 
{ 
	Port(e)=scc->inSlots[Port(e)];
	MidiSend (scc->refNum, e);
}

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
static void xmtHandler()
{
	Byte c; SCCPtr scc = GetData ();
	if (GetByte(scc, &c))
		*WriteData= c;
	else 
		*WriteCtl= 0x28;
}

//_______________________________________________________________________
static void rcvHandler()
{
	SCCPtr scc = GetData ();
	MidiEvPtr e = MidiParseByte (&scc->rcv, *ReadData);
	if (e) Send (e , scc);
}

//_______________________________________________________________________
static void rcvSpecial()
{
	SCCPtr scc = GetData ();
	char c = *ReadData; 
	MidiParseError(&scc->rcv);
}

//_______________________________________________________________________
static void StopSCC (char *WriteCtrl)
{
	*WriteCtrl = 0x01;
	wait();
	*WriteCtrl = 0xe1;
}

//_______________________________________________________________________
static void ResetSCC (char *WriteCtrl)
{
	*WriteCtrl = 0x09;
	wait();
	*WriteCtrl = WCL;
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
static void InitVect ()
{
	*(SCCIntHandler *)xmtVector= xmtHandler;
	*(SCCIntHandler *)rcvVector= rcvHandler;
	*(SCCIntHandler *)SpecialVector= rcvSpecial;
}

//_______________________________________________________________________
static void SetupPort()
{
	InitVect();          		 // init vecteurs d'interruptions port modem
	ResetSCC( WriteCtl);       // reset controleur serie port modem
	InitSCC( WriteCtl, (char *)SCCDataConstants);
}

//_______________________________________________________________________
static void ClearSeq (MidiSeqPtr seq)
{
	if (seq->first) {
		MidiClearSeq (seq);
		seq->last = (MidiEvPtr)&seq->first;
	}
}

//_______________________________________________________________________
static pascal void CheckFifo (long date, short refNum, long a1, long a2, long a3)
{
	SCCPtr scc = (SCCPtr)a1;
	MidiEvPtr e = scc->seq.first;
	
	if (e && (MidiGetTime() - Date(e) > 1000))
		ClearSeq (&scc->seq);
	if(scc->seq.last == (MidiEvPtr)&scc->seq.first)
		scc->MTPOutPort = -1;
	scc->checkTask= MidiTask (CheckFifo, date + 500, refNum, a1, a2, a3);
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
	if (GetByte (scc, &c)) *WriteData= c;
}

//_______________________________________________________________________
Boolean __CheckSCCFree ()
{
	char used;
	used= *(char *)PortUse;
	return ( used & 0x80 ) ? true : false;
}

//_______________________________________________________________________
static Boolean __OpenLine ()
{
	char used, nibble; 
	Boolean ret = false;
	
	INT_OFF();
	used= *(char *)PortUse;
	if( used & 0x80) {                    // si le port n'est pas en service
		used &= 0x7f;                     // bit 7 ˆ 0
		*(char *)PortUse= used;           // indique port occupe
		nibble= *(char *)SPConfig;        // indique que le port
		nibble &= NBL | useMidiShare;     // utilise le driver MidiShare
		*(char *)SPConfig= nibble;
		ret = true;
	}
	INT_ON();
	return ret;
}

//_______________________________________________________________________
Boolean OpenLine (MtpDrvPtr data)
{
	SCCPtr scc = &data->scc;

	if (__OpenLine()) {
        if( data->powerMgr) PwMngON;
		
		SetData	(scc);	
		MidiParseInit (&scc->rcv, data->rTbl, data->s2t);
		MidiStreamInit (&scc->xmt, data->lTbl);
		scc->seq.last = (MidiEvPtr)&scc->seq.first;
		scc->refNum = data->refNum;

		LockedCall(SetupPort());
		scc->checkTask = MidiTask (CheckFifo, MidiGetTime()+100, data->refNum, (long)scc, 0, 0);
		scc->state = true;				
		return true;
	}
	return false;
}

//_______________________________________________________________________
static void __CloseLine()
{
	char used, nibble;
	used= *(char *)PortUse;
	used|= 0x80;
	*(char *)PortUse= used;
	nibble= *(char *)SPConfig;
	nibble&= NBL;
	*(char *)SPConfig= nibble;
	StopSCC( WriteCtl);
}

//_______________________________________________________________________
void CloseLine (MtpDrvPtr data)
{
	SCCPtr scc = &data->scc;
	scc->state = false;
	LockedCall(__CloseLine());
    if (data->powerMgr) PwMngOFF;
	ResetData (scc);
}
