/*
  MidiShare Project
  Copyright (C) Grame 1999

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
  
*/

#include "MidiStreamToEvent.h"

#ifndef false
enum { false, true };
#endif

/*===========================================================================
	internal functions prototypes
  =========================================================================== */
static MidiEvPtr rcvStatus      ( StreamFifoPtr f, char c);
static MidiEvPtr rcvUndefStatus ( StreamFifoPtr f, char c);
static MidiEvPtr rcvChan2       ( StreamFifoPtr f, char c);
static MidiEvPtr rcvDataM       ( StreamFifoPtr f, char c);
static MidiEvPtr rcvDataD       ( StreamFifoPtr f, char c);
static MidiEvPtr rcvChan1       ( StreamFifoPtr f, char c);
static MidiEvPtr rcvDataU       ( StreamFifoPtr f, char c);
static MidiEvPtr rcvCommon2     ( StreamFifoPtr f, char c);
static MidiEvPtr rcvCommon1     ( StreamFifoPtr f, char c);
static MidiEvPtr rcvCommon0     ( StreamFifoPtr f, char c);
static MidiEvPtr rcvQFrame      ( StreamFifoPtr f, char c);
static MidiEvPtr rcvDataQ       ( StreamFifoPtr f, char c);
static MidiEvPtr rcvSysExBeg    ( StreamFifoPtr f, char c);
static MidiEvPtr rcvSysExNext   ( StreamFifoPtr f, char c);
static MidiEvPtr rcvSysExEnd    ( StreamFifoPtr f, char c);

// MIDI Time Piece support
static MidiEvPtr rcvMTPchange   ( StreamFifoPtr f, char c);
static MidiEvPtr rcvMTPport     ( StreamFifoPtr f, char c);


/*===========================================================================
	external initialization functions
  =========================================================================== */
void MidiParseError(StreamFifoPtr f)
{
	if (f->ptrCur) MidiFreeEv (f->ptrCur);
	f->ptrCur = 0;
	f->parse = rcvStatus;
}

//_____________________________________________________________________________
void MidiParseInit (StreamFifoPtr f, ParseMethodTbl rcv, Byte * typesTbl)
{
	f->parse     = rcvStatus;
	f->rcv       = rcv;
	f->typesTbl  = typesTbl;
	f->ptrCur    	= 0;
	f->date      	= 0;
//	f->common.fast 	= 0;		// removed for 64 bits support
	f->common.field.msType	= 0;
	f->common.field.refNum	= 0;
	f->common.field.port	= 0;
	f->common.field.chan	= 0;

//	f->infos.fast 	= 0;		// removed for 64 bits support
	f->infos.data[0]	= 0;
	f->infos.data[1]	= 0;
	f->infos.data[2]	= 0;
	f->infos.data[3]	= 0;
}

//_____________________________________________________________________________
void MidiParseReset (StreamFifoPtr f)
{
	if (f->ptrCur) MidiFreeEv (f->ptrCur);
	f->ptrCur    	= 0;
	f->date      	= 0;
//	f->common.fast 	= 0;		// removed for 64 bits support
	f->common.field.msType	= 0;
	f->common.field.refNum	= 0;
	f->common.field.port	= 0;
	f->common.field.chan	= 0;
//	f->infos.fast 	= 0;		// removed for 64 bits support
	f->infos.data[0]	= 0;
	f->infos.data[1]	= 0;
	f->infos.data[2]	= 0;
	f->infos.data[3]	= 0;
	f->parse     	= rcvStatus;
}

//_____________________________________________________________________________
void MidiParseInitMthTbl (ParseMethodTbl tbl)
{
	*tbl++= rcvUndefStatus;    // 0 - undefined status
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
	
	*tbl++= rcvMTPchange;      // 19 - MTP port change
}

/*__________________________________________________________________________________*/
void MidiParseInitTypeTbl (Status2TypeTbl table)
{
	int i;
	
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
	*table++= 18;			/* typeQuarterFrame (130) */
	*table++= typeSongPos;
	*table++= typeSongSel;
	*table++= 0;
	*table++= 19;           // pour MTP port change
	*table++= typeTune;
	*table++= 0;
	
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
static Boolean AddFSexEv (MidiEvPtr e, long v)
{
	MidiSEXPtr nouv;
	MidiSEXPtr ext = LinkSE(e);
	int i = ext->data[11];

	if( i < 11) {
		ext->data[i] = (Byte)v;	           /* store the value              */
		ext->data[11]++;		           /* update the busy space count  */
	} else { 
		nouv = (MidiSEXPtr)MidiNewCell();  /* add a new cell               */
		if( nouv) {
			ext->data[11] = (Byte)v;        /* store the value              */
			nouv->data[11] = 0;	            /* busy space count             */
			nouv->link= ext->link;	        /* link the new cell            */
			ext->link= nouv;
			LinkSE(e)= nouv;	            /* link header to the last cell */
		}else
			return false;
	}
	return true;
}


/*===========================================================================
			internal functions implementation
  =========================================================================== */
static MidiEvPtr rcvStatus (StreamFifoPtr f, char c)
{
	return ( c < 0) ? (*f->rcv[type(c, f)])( f, c) : 0;
}

//_____________________________________________________________________________
static MidiEvPtr rcvStore (StreamFifoPtr f)
{
	int i;
	MidiEvPtr e = MidiNewCell();
	if( e) {
		Date(e)= f->date;
		e->evType = f->common.field.msType;
		e->refNum = f->common.field.refNum;
		e->port   = f->common.field.port;
		e->chan   = f->common.field.chan;
		for (i=0; i<4; i++) {
			e->info.data[i] = f->infos.data[i];
		}
		return e;
	}
	return 0;
}

//_____________________________________________________________________________
static MidiEvPtr rcvUndefStatus( StreamFifoPtr f, char c) { return 0; }

//_____________________________________________________________________________
// rcvChan2 : receiving a 2 datas channel message
static MidiEvPtr rcvChan2 (StreamFifoPtr f, char c)
{
	f->common.field.msType = type(c, f);
	f->common.field.chan = c & 0x0f;
//	f->infos.fast 	= 0;		// removed for 64 bits support
	f->infos.data[0]	= 0;
	f->infos.data[1]	= 0;
	f->infos.data[2]	= 0;
	f->infos.data[3]	= 0;
	f->parse = rcvDataM;
	return 0;
}

//_____________________________________________________________________________
// receiving the first data
static MidiEvPtr rcvDataM (StreamFifoPtr f, char c)
{
	if( c < 0) return rcvStatus( f, c);
	f->date = MidiGetTime();
	f->infos.data[0] = c;
	f->parse = rcvDataD;
	return 0;
}

//_____________________________________________________________________________
// receiving the second data
static MidiEvPtr rcvDataD (StreamFifoPtr f, char c)
{
	if( c < 0) return rcvStatus( f, c);
	f->infos.data[1] = c;
	f->parse = rcvDataM;
	return rcvStore( f);
}

//_____________________________________________________________________________
// rcvChan1 : receiving a single data channel message
static MidiEvPtr rcvChan1 (StreamFifoPtr f, char c)
{
	f->common.field.msType = type(c, f);
	f->common.field.chan = c & 0x0f;
//	f->infos.fast 	= 0;		// removed for 64 bits support
	f->infos.data[0]	= 0;
	f->infos.data[1]	= 0;
	f->infos.data[2]	= 0;
	f->infos.data[3]	= 0;
	f->parse= rcvDataU;
	return 0;
}

//_____________________________________________________________________________
// receiving the single data
static MidiEvPtr rcvDataU (StreamFifoPtr f, char c)
{
	if( c < 0) return rcvStatus( f, c);
	f->date = MidiGetTime();
	f->infos.data[0]= c;
	return rcvStore( f);
}

//_____________________________________________________________________________
// rcvCommon2 : receiving a 2 datas common message
static MidiEvPtr rcvCommon2 (StreamFifoPtr f, char c)
{
	f->common.field.msType = type(c, f);
	f->common.field.chan = 0;
//	f->infos.fast 	= 0;		// removed for 64 bits support
	f->infos.data[0]	= 0;
	f->infos.data[1]	= 0;
	f->infos.data[2]	= 0;
	f->infos.data[3]	= 0;
	f->parse= rcvDataM;
	return 0;
}

//_____________________________________________________________________________
// rcvCommon1 : receiving a single data common message
static MidiEvPtr rcvCommon1 (StreamFifoPtr f, char c)
{
	f->common.field.msType = type(c, f);
	f->common.field.chan = 0;
//	f->infos.fast 	= 0;		// removed for 64 bits support
	f->infos.data[0]	= 0;
	f->infos.data[1]	= 0;
	f->infos.data[2]	= 0;
	f->infos.data[3]	= 0;
	f->parse= rcvDataU;
	return 0;
}

//_____________________________________________________________________________
// rcvCommon0 : receiving a common message without data
static MidiEvPtr rcvCommon0 (StreamFifoPtr f, char c)
{
	MidiEvPtr e;
	
	e = MidiNewEv(type(c, f));
	if (e) {
		Date(e) = MidiGetTime();
		Chan(e) = 0;
		Port(e) = f->common.field.port;
		return e;
	}
	return 0;
}

//_____________________________________________________________________________
// rcvQFrame : receiving a quarter frame message
static MidiEvPtr rcvQFrame (StreamFifoPtr f, char c)
{
	f->common.field.msType = typeQuarterFrame;
	f->common.field.chan = 0;
//	f->infos.fast 	= 0;		// removed for 64 bits support
	f->infos.data[0]	= 0;
	f->infos.data[1]	= 0;
	f->infos.data[2]	= 0;
	f->infos.data[3]	= 0;
	f->parse = rcvDataQ;
	return 0;
}

//_____________________________________________________________________________
// receiving a quarter frame data
static MidiEvPtr rcvDataQ (StreamFifoPtr f, char c)
{	
	if( c < 0) return rcvStatus( f, c);
	f->date= MidiGetTime ();
	f->infos.data[0]= c >> 4;
	f->infos.data[1]= c & 0x0f;
	return rcvStore( f);
}

//_____________________________________________________________________________
// rcvSysExBeg : receiving a System Exclusive
static MidiEvPtr rcvSysExBeg (StreamFifoPtr f, char c)
{
	MidiEvPtr e;

	e= MidiNewEv (typeSysEx);
	if( e) {
		Date(e) = MidiGetTime();
		Chan(e) = 0;
		Port(e) = f->common.field.port;
		f->ptrCur= e;
		f->parse= rcvSysExNext;
	}
	else { 
		f->parse = rcvStatus;
	}
	return 0;
}

//_____________________________________________________________________________
// receiving sysex datas
static MidiEvPtr rcvSysExNext (StreamFifoPtr f, char c)
{
	if( c < 0) return rcvSysExEnd( f, c);
	
	if (!AddFSexEv (f->ptrCur, c)) {
		f->parse= rcvStatus;
		MidiFreeEv (f->ptrCur);
		f->ptrCur= 0;
	}
	return 0;
}

//_____________________________________________________________________________
static MidiEvPtr rcvSysExEnd (StreamFifoPtr f, char c)
{
	MidiEvPtr e;
		
	if( (unsigned char)c >= (unsigned char)MClock)
		return rcvStatus (f, c);
	if( c!= (char)EndSysX)
		return rcvStatus( f, c);

	e = f->ptrCur;	
	Chan(e) = 0;
	f->ptrCur= 0;
	f->parse= rcvStatus;
	return e;
}

//_____________________________________________________________________________
// MIDITimePiece port change
static MidiEvPtr rcvMTPchange (StreamFifoPtr f, char c)
{
	f->parse= rcvMTPport;
	return 0;
}

//_____________________________________________________________________________
// MIDITimePiece port number
static MidiEvPtr rcvMTPport (StreamFifoPtr f, char c)
{
	if( c < 0) 
		return rcvStatus( f, c);
	else if( c > 16)
		c = 0;
	else
		c--;
	f->common.field.port= c;
	return 0;
}

