/*

  Copyright © Grame 1999

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
  
  modifications history:
*/

#include "msFrcv.h"
#include "msEvents.h"
#include "msXmtRcv.h"

static  RcvMethodPtr  	RcvTbl[20];
static  Byte  		TypeTbl[256];

/*===========================================================================
				PROTOTYPES OF INTERNAL FUNCTIONS
  =========================================================================== */

void rcvStatus( RFifoPtr f, char c);

static void rcvUndefStatus ( RFifoPtr f, char c);
static void rcvChan2       ( RFifoPtr f, char c);
static void rcvDataM       ( RFifoPtr f, char c);
static void rcvDataD       ( RFifoPtr f, char c);
static void rcvChan1       ( RFifoPtr f, char c);
static void rcvDataU       ( RFifoPtr f, char c);
static void rcvCommon2     ( RFifoPtr f, char c);
static void rcvCommon1     ( RFifoPtr f, char c);
static void rcvCommon0     ( RFifoPtr f, char c);
static void rcvQFrame      ( RFifoPtr f, char c);
static void rcvDataQ       ( RFifoPtr f, char c);
static void rcvSysExBeg    ( RFifoPtr f, char c);
static void rcvSysExNext   ( RFifoPtr f, char c);
static void rcvSysExEnd    ( RFifoPtr f, char c);
static void rcvPortChg     ( RFifoPtr f, char c);
static void rcvPortNum     ( RFifoPtr f, char c);

static void rcvQFrameSync   (RFifoPtr f, char c);
static void rcvDataQSync    (RFifoPtr f, char c);

//_____________________________________________________________________________
// offset d'un octet de status dans la table des types

#define offset(c) ((unsigned char)c-(unsigned char)0x80)

/*===========================================================================
		 IMPLÉMENTATION DES FONCTIONS D'INITIALISATION EXTERNES
  =========================================================================== */

void RFInit( RFifoPtr f,  TMSGlobalPtr global, short ref)
{
	f->refNum 	= ref;
	f->state     	= kLineOff;     /* Etat du fifo : actif ou pas        */
	f->errCount  	= 0;            /* Nombre d'erreurs en reception      */
	f->global 	= global;

	f->parse     	= rcvStatus;   	/* Etat de l'analyse (adr. routine)   */
	f->ptrCur    	= 0;            /* Pointeur SysEx en cours            */
	f->date      	= 0;            /* Date de l'evt sur 24 bits de pFort */
	f->common.fast 	= 0;
	f->infos.fast 	= 0;
}

//_____________________________________________________________________________

void RFPutByte(RFifoPtr f, Byte c) {(*f->parse)(f,c);}

//_____________________________________________________________________________

void RFInitMthTbl()
{
	register RcvMethodPtr *tbl;
	
	tbl= RcvTbl;

	*tbl++= rcvUndefStatus;    // 0 - statut indéfini
	*tbl++= rcvChan2;          // 1 - typeKeyOn
	*tbl++= rcvChan2;          // 2 - typeKeyOff
	*tbl++= rcvChan2;          // 3 - typeKeyPress
	*tbl++= rcvChan2;          // 4 - typeCtrlChange
	*tbl++= rcvChan1;          // 5 - typeProgChange
	*tbl++= rcvChan1;          // 6 - typeChanPress
	*tbl++= rcvChan2;          // 7 - typePitchWheel
	
	*tbl++= rcvCommon2;        // 8 - typeSongPos
	*tbl++= rcvCommon1;        // 9 - typeSongSel
	*tbl++= rcvCommon0;        // 10 - typeClock
	*tbl++= rcvCommon0;        // 11 - typeStart
	*tbl++= rcvCommon0;        // 12 - typeContinu
	*tbl++= rcvCommon0;        // 13 - typeStop

	*tbl++= rcvCommon0;        // 14 - typeTune
	*tbl++= rcvCommon0;        // 15 - typeActiveSens
	*tbl++= rcvCommon0;        // 16 - typeReset

	*tbl++= rcvSysExBeg;       // 17 - typeSysEx
	*tbl++= rcvQFrame;         // 18 - typeQuarterFrame (en fait 130)
#ifdef __MidiTimePiece__
	*tbl  = rcvPortChg;        // 19 - changement de port MTP
#endif
}


/*__________________________________________________________________________________*/

 void InitTypeTbl( )
{
	register int i;
	
	Byte* table = TypeTbl;
	
	for( i=0; i< 16; i++)
		*table++= typeKeyOff;		
	for( i=0; i< 16; i++)
		*table++= typeKeyOn;
	for( i=0; i< 16; i++)
		*table++= typeKeyPress;
	for( i=0; i< 16; i++)
		*table++= typeCtrlChange;
	for( i=0; i< 16; i++)
		*table++= typeProgChange;
	for( i=0; i< 16; i++)
		*table++= typeChanPress;
	for( i=0; i< 16; i++)
		*table++= typePitchWheel;
	
	*table++= typeSysEx;	// 0xf0
	*table++= 18;		/* typeQuarterFrame (en fait 130) */
	*table++= typeSongPos;
	*table++= typeSongSel;
	*table++= 0;
	*table++= 19;           // pour MTP port change
	*table++= typeTune;
	*table++= 0;		/* end of sysEx pas reçu en tant que tel */
	
	*table++= typeClock;
	*table++= 0;
	*table++= typeStart;
	*table++= typeContinue;
	*table++= typeStop;
	*table++= 0;
	*table++= typeActiveSens;
	*table++= typeReset;
}

/*__________________________________________________________________________________*/

static long AddFSexEv( lifo* fl, MidiEvPtr e, long v)
{
	MidiSEXPtr nouv;
	MidiSEXPtr ext = LinkSE(e);
	int i = ext->data[11];

	if( i < 11) {			          /* If there remains place       */
		ext->data[i] = (Byte)v;	          /* store the value              */
		ext->data[11]++;		  /* update the busy space count  */
	} else { 
		nouv = (MidiSEXPtr)MSNewCell(fl); /* add a new cell               */
		if( nouv) {
			ext->data[11] = (Byte)v;      /* store the value              */
			nouv->data[11] = 0;	      /* busy space count             */
			nouv->link= ext->link;	      /* link the new cell            */
			ext->link= nouv;
			LinkSE(e)= nouv;	/* link header to the last cell */
		}else
			return false;
	}
	return true;
}


/*===========================================================================
			IMPLÉMENTATION DES FONCTIONS UTILITAIRES EXTERNES
  =========================================================================== */
 void SwitchRcvQFrameMethod ( Boolean syncOn)
{
	RcvTbl[18]= syncOn ? rcvQFrameSync : rcvQFrame;
}

static void RFPutEvent ( register RFifoPtr f, register MidiEvPtr e)
{
	MSSend(f->refNum,e,SorterList(f->global));
}

//_____________________________________________________________________________
// rcvStatus : Attend un octet de status

void rcvStatus( register RFifoPtr f, register char c)
{
	if( c < 0) (*RcvTbl[TypeTbl[offset(c)]])( f, c);
}

/*===========================================================================
			IMPLÉMENTATION DES FONCTIONS PUREMENT INTERNES
  =========================================================================== */

//_____________________________________________________________________________
// Ajout d'un événement dans le fifo de réception

#define rcvAddFifo(f, e) RFPutEvent(f, e)

//_____________________________________________________________________________
// Ajout d'un événement sysex dans le fifo de réception

#define rcvAddFifoSysEx(f, e)   Chan(e)=0; rcvAddFifo(f, e)

//_____________________________________________________________________________
// Allocation d'un événement correspondant au message reçu
static void rcvStore( register RFifoPtr f)
{
	register TMidiFastEv e;
	
	e.std= MSNewCell(FreeList(Memory(f->global)));
	if( e.std) {
		Date(e.std)= f->date;
		e.fast->common.longC= f->common.fast;
		e.fast->specific.longF= f->infos.fast;
		rcvAddFifo( f, e.std);
	}
}

//_____________________________________________________________________________
static void rcvUndefStatus( RFifoPtr f, char c){}

//_____________________________________________________________________________
// rcvChan2 : Réception d'un channel message à deux données
static void rcvChan2( register RFifoPtr f, char c)
{
	f->common.std.type= TypeTbl[offset(c)];
	f->common.std.canal= c & 0x0f;
	f->infos.fast= 0;
	f->parse= rcvDataM;
}

//_____________________________________________________________________________
// Réception de la première donnée
static void rcvDataM( register RFifoPtr f, register char c)
{
	if( c < 0) rcvStatus( f, c);
	else {
		f->date= CurrTime(f->global);
		f->infos.data[0]= c;
		f->parse= rcvDataD;
	}
}

//_____________________________________________________________________________
// Réception de la seconde donnée
static void rcvDataD( register RFifoPtr f, char c)
{
	if( c < 0) rcvStatus( f, c);
	else {
		f->infos.data[1]= c;
		f->parse= rcvDataM;
		rcvStore( f);
	}
}

//_____________________________________________________________________________
// rcvChan1 : Réception d'un channel message à une donnée
static void rcvChan1( register RFifoPtr f, char c)
{
	f->common.std.type= TypeTbl[offset(c)];
	f->common.std.canal= c & 0x0f;
	f->infos.fast= 0;
	f->parse= rcvDataU;
}

//_____________________________________________________________________________
// Réception de la seule donnée
static void rcvDataU( register RFifoPtr f, char c)
{
	if( c < 0) rcvStatus( f, c);
	else {
		f->infos.data[0]= c;
		// f->parse= rcvDataU;     en cas de running status
		rcvStore( f);
	}
}

//_____________________________________________________________________________
// rcvCommon2 : Réception d'un common message à deux données
static void rcvCommon2( register RFifoPtr f, char c)
{
	f->common.std.type= TypeTbl[offset(c)];
	f->common.std.canal= 0;
	f->infos.fast= 0;
	f->parse= rcvDataM;
}

//_____________________________________________________________________________
// rcvCommon1 : Réception d'un common message à une donnée
static void rcvCommon1( register RFifoPtr f, char c)
{
	f->common.std.type= TypeTbl[offset(c)];
	f->common.std.canal= 0;
	f->infos.fast= 0;
	f->parse= rcvDataU;
}

//_____________________________________________________________________________
// rcvCommon0 : Réception d'un common message sans données
static void rcvCommon0( register RFifoPtr f, char c)
{
	register TMidiFastEv e;
	
	e.std= MSNewEv( TypeTbl[offset(c)], FreeList(Memory(f->global)));
	if( e.std) {
		Date(e.std)= CurrTime(f->global);
		e.std->chan= 0;
		e.std->port= f->common.std.port;
		e.std->refNum= f->common.std.refNum;
		e.fast->specific.longF=0;
		rcvAddFifo( f, e.std);
	}
}

//_____________________________________________________________________________
// rcvQFrame : Réception d'un quarter frame message à une donnée
static void rcvQFrame( register RFifoPtr f, char c)
{
	f->common.std.type= typeQuarterFrame;
	f->common.std.canal= 0;
	f->infos.fast= 0;
	f->parse= rcvDataQ;
}

//_____________________________________________________________________________
// Réception de la seule donnée
static void rcvDataQ( register RFifoPtr f, register char c)
{	
	if( c < 0) rcvStatus( f, c);
	else {
		f->date= CurrTime(f->global);
		f->infos.data[0]= c >> 4;
		f->infos.data[1]= c & 0x0f;
		rcvStore( f);
	}
}

//_____________________________________________________________________________
// rcvQFrame : Réception d'un quarter frame message à une donnée (sync smpte)
static void rcvQFrameSync( register RFifoPtr f, char c)
{
	f->common.std.type= typeQuarterFrame;
	f->common.std.canal= 0;
	f->infos.fast= 0;
	f->parse= rcvDataQSync;
}

//_____________________________________________________________________________
// Réception de la seule donnée (sync smpte)
static void rcvDataQSync( register RFifoPtr f, register char c)
{	
	if( c < 0) rcvStatus( f, c);
	else {
		f->date= CurrTime(f->global);
		f->infos.data[0]= c >> 4;
		f->infos.data[1]= c & 0x0f;
		rcvStore( f);
/*
A REVOIR  f->global->currTime.syncState( f, &f->global->currTime);
*/
	}
}


//_____________________________________________________________________________
// rcvSysExBeg : Réception d'un message System Exclusive
static void rcvSysExBeg( register RFifoPtr f, char c)
{
	register MidiEvPtr e;

	e= MSNewEv( typeSysEx, FreeList(Memory(f->global)));
	if( e) {
		Date(e) = CurrTime(f->global); /* A REVOIR */
		f->ptrCur= e;
		e->port= f->common.std.port;
		e->refNum= f->common.std.refNum;
		f->parse= rcvSysExNext;
	}
	else { 
		f->parse= rcvStatus;
	}
}

//_____________________________________________________________________________
// Réception d'une donnée de System Exclusive
static void rcvSysExNext( register RFifoPtr f, char c)
{
	if( c < 0) {
		rcvSysExEnd( f, c);
	}
	else if(AddFSexEv(FreeList(Memory(f->global)), f->ptrCur, c)!= true) {
		f->parse= rcvStatus;
		MSFreeEv(f->ptrCur, FreeList(Memory(f->global)));
		f->ptrCur= 0;
	}
}

//_____________________________________________________________________________
// Réception d'un code de statut pendant le sysEx
static void rcvSysExEnd( register RFifoPtr f, char c)
{
	if( (unsigned char)c >= (unsigned char)MClock) {
		rcvStatus( f, c);
	}
	else {
		rcvAddFifoSysEx( f, f->ptrCur);	
		f->ptrCur= 0;
		f->parse= rcvStatus;
		if( c!= (char)EndSysX)
			rcvStatus( f, c);
	}
}

//_____________________________________________________________________________
// rcvPortChg : Réception d'un changement de port courant MTP
static void rcvPortChg( RFifoPtr f, char c)
{
	f->parse= rcvPortNum;
}

//_____________________________________________________________________________
// Réception du numéro de port
static void rcvPortNum( register RFifoPtr f, register char c)
{
	if( c < 0) rcvStatus( f, c);
	else {
		f->common.std.port= c ? f->basePort + c : 0;
	}
}

