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
  grame@grame.fr
  
*/

#include "UDPStreamToEvent.h"

/*===========================================================================
	internal functions prototypes
  =========================================================================== */
static MidiEvPtr rcvIgnore  (UDPStreamPtr f, Byte c);

static MidiEvPtr rcvType    (UDPStreamPtr f, Byte c);
static MidiEvPtr rcv0Byte   (UDPStreamPtr f, Byte c);
static MidiEvPtr rcv1Byte   (UDPStreamPtr f, Byte c);
static MidiEvPtr rcv2Bytes  (UDPStreamPtr f, Byte c);
static MidiEvPtr rcv3Bytes  (UDPStreamPtr f, Byte c);
static MidiEvPtr rcv4Bytes  (UDPStreamPtr f, Byte c);
static MidiEvPtr rcvVarLen  (UDPStreamPtr f, Byte c);

static MidiEvPtr rcvPort  	(UDPStreamPtr f, Byte c);
static MidiEvPtr rcvChan  	(UDPStreamPtr f, Byte c);

static MidiEvPtr rcv1Short       (UDPStreamPtr f, Byte c);
static MidiEvPtr rcv2Shorts      (UDPStreamPtr f, Byte c);
static MidiEvPtr rcv2Bytes1Short (UDPStreamPtr f, Byte c);
static MidiEvPtr rcv1Long        (UDPStreamPtr f, Byte c);

static MidiEvPtr rcvShortHigh  (UDPStreamPtr f, Byte c);
static MidiEvPtr rcvShortLow   (UDPStreamPtr f, Byte c);
static MidiEvPtr rcvLongHH     (UDPStreamPtr f, Byte c);
static MidiEvPtr rcvLongHL     (UDPStreamPtr f, Byte c);
static MidiEvPtr rcvLongLH     (UDPStreamPtr f, Byte c);
static MidiEvPtr rcvLongLL     (UDPStreamPtr f, Byte c);

/*===========================================================================
	external initialization functions
  =========================================================================== */
void UDPParseInit (UDPStreamPtr f, UDPParseMethodTbl rcv)
{
	f->rcv    = rcv;
	f->ptrCur = 0;
	UDPParseReset (f);
}

//_____________________________________________________________________________
void UDPParseReset (UDPStreamPtr f)
{
	if (f->ptrCur) MidiFreeEv (f->ptrCur);
	f->ptrCur = 0;
	f->length = 0;
	f->index  = 0;
	f->shortDst = &f->length;
	f->parse  = rcvShortHigh;
	f->next  = rcvType;
}

//_____________________________________________________________________________
void UDPParseInitMthTbl (UDPParseMethodTbl tbl)
{
	int	i;
	for (i=0; i<256; i++) tbl[i] = 0;

	tbl[typeNote]		= rcv2Bytes1Short;
		
	tbl[typeKeyOn]		= rcv2Bytes;
	tbl[typeKeyOff]		= rcv2Bytes;
	tbl[typeKeyPress] 	= rcv2Bytes;
	tbl[typeCtrlChange]	= rcv2Bytes;
	tbl[typeProgChange]	= rcv1Byte;
	tbl[typeChanPress]	= rcv1Byte;
	tbl[typePitchWheel]	= rcv2Bytes;
		
	tbl[typeSongPos]	= rcv2Bytes;
	tbl[typeSongSel]	= rcv1Byte;
	tbl[typeClock]		= rcv0Byte;
	tbl[typeStart]		= rcv0Byte;
	tbl[typeContinue]	= rcv0Byte;
	tbl[typeStop]		= rcv0Byte;
		
	tbl[typeTune]		= rcv0Byte;
	tbl[typeActiveSens]	= rcv0Byte;
	tbl[typeReset]		= rcv0Byte;
	
	tbl[typeSysEx]		= rcvVarLen;
	tbl[typeStream]		= rcvVarLen;
		
	tbl[typeQuarterFrame] = rcv2Bytes;

	tbl[typeCtrl14b]	= rcv2Shorts;
	tbl[typeNonRegParam]= rcv2Shorts;
	tbl[typeRegParam]	= rcv2Shorts;

	tbl[typeSeqNum]		= rcv1Short;
	tbl[typeTextual]	= rcvVarLen;
	tbl[typeCopyright]	= rcvVarLen;
	tbl[typeSeqName]	= rcvVarLen;
	tbl[typeInstrName]	= rcvVarLen;
	tbl[typeLyric]		= rcvVarLen;
	tbl[typeMarker]		= rcvVarLen;
	tbl[typeCuePoint]	= rcvVarLen;
	tbl[typeChanPrefix]	= rcv1Byte;
	tbl[typeEndTrack]	= rcv0Byte;
	tbl[typeTempo]		= rcv1Long;
	tbl[typeSMPTEOffset]= rcv4Bytes;

	tbl[typeTimeSign]	= rcv4Bytes;
	tbl[typeKeySign]	= rcv2Bytes;
	tbl[typeSpecific]	= rcvVarLen;
}

/*===========================================================================
			internal functions implementation
  =========================================================================== */
static inline MidiEvPtr parseComplete (UDPStreamPtr f)
{
	MidiEvPtr e = f->ptrCur;
	short d = (short)(Date(e) & 0xffff);
	f->ptrCur = 0;
	f->shortDst = &f->length;
	f->parse  = rcvShortHigh;
	f->next  = rcvType;
	f->index = 0;
	if (d < 0) {
		Date(e) = d;
	}
	return e;
}

#if defined(macintosh) || defined(__MacOSX__)
static inline short * LowWord (unsigned long *v) {
	short * ptr = (short *)v;
	return ++ptr;
}
#else
static inline short * LowWord (unsigned long *v) {
	return (short *)v;
}
#endif

//_____________________________________________________________________________
static MidiEvPtr rcvType (UDPStreamPtr f, Byte c)
{
	f->length--;
	if (f->rcv[c]) {
		f->ptrCur = MidiNewEv (c);
		if (f->ptrCur) {
			f->parse = rcvShortHigh;
			f->shortDst = LowWord(&Date(f->ptrCur));
			f->next = rcvPort;
			f->index = 0;
		}
		else f->parse = rcvIgnore;
	}
	// undefined MidiShare type for the udp stream
	else f->parse = rcvIgnore;
	return 0;
}

//_____________________________________________________________________________
static MidiEvPtr rcvIgnore (UDPStreamPtr f, Byte c)
{
	if (! --f->length) 	{
//		parseComplete (f);
		MidiEvPtr e = f->ptrCur;
		f->ptrCur = 0;
		f->shortDst = &f->length;
		f->parse  = rcvShortHigh;
		f->next  = rcvType;
		f->index = 0;
		if (e) MidiFreeEv (e);
	}
	return 0;
}

//_____________________________________________________________________________
// short values parsing
//_____________________________________________________________________________
static MidiEvPtr rcvShortHigh (UDPStreamPtr f, Byte c)
{
	f->length--;
#if defined(macintosh) || defined(__MacOSX__)
	f->data[0] = c;
#else
	f->data[1] = c;
#endif
	f->parse = rcvShortLow;
	return 0;
}

static MidiEvPtr rcvShortLow (UDPStreamPtr f, Byte c)
{
	short * sptr = (short *)f->data;
	f->length--;
#if defined(macintosh) || defined(__MacOSX__)
	f->data[1] = c;
#else
	f->data[0] = c;
#endif
	*f->shortDst = *sptr;
	if (f->next) f->parse = f->next;
	else return  parseComplete (f);
	return 0;
}

//_____________________________________________________________________________
// long values parsing
//_____________________________________________________________________________
static MidiEvPtr rcvLongHH (UDPStreamPtr f, Byte c)
{
	f->length--;
#if defined(macintosh) || defined(__MacOSX__)
	f->data[0] = c;
#else
	f->data[3] = c;
#endif
	f->parse = rcvLongHL;
	return 0;
}

static MidiEvPtr rcvLongHL (UDPStreamPtr f, Byte c)
{
	f->length--;
#if defined(macintosh) || defined(__MacOSX__)
	f->data[1] = c;
#else
	f->data[2] = c;
#endif
	f->parse = rcvLongLH;
	return 0;
}

static MidiEvPtr rcvLongLH (UDPStreamPtr f, Byte c)
{
	f->length--;
#if defined(macintosh) || defined(__MacOSX__)
	f->data[2] = c;
#else
	f->data[1] = c;
#endif
	f->parse = rcvLongLL;
	return 0;
}

static MidiEvPtr rcvLongLL (UDPStreamPtr f, Byte c)
{
	long * lptr = (long *)f->data;
	f->length--;
#if defined(macintosh) || defined(__MacOSX__)
	f->data[3] = c;
#else
	f->data[0] = c;
#endif
	*f->longDst = *lptr;
	if (f->next) f->parse = f->next;
	else return  parseComplete (f);
	return 0;
}

//_____________________________________________________________________________
// port and channel parsing
//_____________________________________________________________________________
static MidiEvPtr rcvPort (UDPStreamPtr f, Byte c)
{
	f->length--;
	Port(f->ptrCur) = c;
	f->parse = rcvChan;
	return 0;
}

//_____________________________________________________________________________
static MidiEvPtr rcvChan (UDPStreamPtr f, Byte c)
{
//	UDPParseMethodPtr next = f->rcv[EvType(f->ptrCur)];
//	MidiEvPtr e = 0;
	f->length--;
	Chan(f->ptrCur) = c;
	if (!f->length)
		return parseComplete (f);
	f->parse = f->rcv[EvType(f->ptrCur)];
	f->next = 0;
	return 0;
}

//_____________________________________________________________________________
// single data byte parsing
//_____________________________________________________________________________
static MidiEvPtr rcv4Bytes (UDPStreamPtr f, Byte c)
{
	Data(f->ptrCur)[f->index++] = c;
	f->parse = rcv3Bytes;
	return 0;
}

static MidiEvPtr rcv3Bytes (UDPStreamPtr f, Byte c)
{
	Data(f->ptrCur)[f->index++] = c;
	f->parse = rcv2Bytes;
	return 0;
}

static MidiEvPtr rcv2Bytes (UDPStreamPtr f, Byte c)
{
	Data(f->ptrCur)[f->index++] = c;
	f->parse = rcv1Byte;
	return 0;
}

static MidiEvPtr rcv1Byte (UDPStreamPtr f, Byte c)
{
	Data(f->ptrCur)[f->index++] = c;
	if (f->next) {
		f->parse = f->next;
		f->next = 0;
	}
	else return parseComplete (f);
	return 0;
}

static MidiEvPtr rcv0Byte (UDPStreamPtr f, Byte c)
{
	// should never be reached
	return parseComplete (f);
}

//_____________________________________________________________________________
// var length events parsing
//_____________________________________________________________________________
static MidiEvPtr rcvVarLen  (UDPStreamPtr f, Byte c)
{
	f->length--;
	MidiAddField (f->ptrCur, c);
	return f->length ? 0 : parseComplete (f);
}

//_____________________________________________________________________________
// parsing short and long values
//_____________________________________________________________________________
static MidiEvPtr rcv1Short (UDPStreamPtr f, Byte c)
{
	f->shortDst = &(f->ptrCur->info.param.num);
	f->next = 0;
	return rcvShortHigh (f, c);
}

//_____________________________________________________________________________
static MidiEvPtr rcv2Shorts (UDPStreamPtr f, Byte c)
{
	f->shortDst = &(f->ptrCur->info.param.val);
	f->next = rcv1Short;
	return rcvShortHigh (f, c);
}

//_____________________________________________________________________________
static MidiEvPtr rcv2Bytes1Short (UDPStreamPtr f, Byte c)
{
	f->shortDst = (short *)&Dur(f->ptrCur);
	f->next = rcvShortHigh;
	return rcv2Bytes (f, c);
}

//_____________________________________________________________________________
static MidiEvPtr rcv1Long (UDPStreamPtr f, Byte c)
{
	f->longDst = &Tempo(f->ptrCur);
	f->next = 0;
	return rcvLongHH (f, c);
}
