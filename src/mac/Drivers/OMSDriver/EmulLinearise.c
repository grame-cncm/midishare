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


#include "EmulLinearise.h"


//_________________________________________________________________________________
#define NRPLSB		98		/* non registr. param number (lsb)	*/
#define NRPMSB		99		/* non registr. param number (msb)	*/

#define RPLSB		100		/* registred param number (lsb)		*/
#define RPMSB		101		/* registred param number (msb)		*/

#define DENTRYMSB	6		/* data entry value (msb)			*/
#define DENTRYLSB	38		/* data entry value (lsb)			*/


//_________________________________________________________________________________
//							  Linearisation	methods				
//_________________________________________________________________________________

static e2pRet NullE2PMth ( E2PInfosPtr i)
{
	return 0;
}

static e2pRet NoteE2PMth ( E2PInfosPtr i)
{
	unsigned char 	*ptr;
	MidiEvPtr 		off, e;

	e= i->evToSend;
	ptr= i->dest;
	*ptr++ = NoteOn + Chan(e);
	*ptr++ = Pitch(e);
	*ptr = Vel(e);
	
	EvType(e) = typeKeyOn;
	Vel(e) = 0;
	Date(e)+= Dur(e);
	off= MidiNewEv( typePrivate);
	e= MidiCopyEv( e);
	Port(off) = Port(e);
	if( off && e) {		
		LinkST(off)->ptr1= (Ptr)e;
		MidiSendAt( RefNum(e), off, Date(e));
	}
	
	return 3;
}

static e2pRet KeyOnE2PMth ( E2PInfosPtr i)
{
	unsigned char 	*ptr;
	MidiEvPtr 		e;
	
	e= i->evToSend;
	ptr= i->dest;
	*ptr++ = NoteOn + Chan(e);
	*ptr++ = Pitch(e);
	*ptr = Vel(e);
	return 3;
}

static e2pRet KeyOffE2PMth ( E2PInfosPtr i)
{
	unsigned char 	*ptr;
	MidiEvPtr 		e;
	
	e= i->evToSend;	
	ptr= i->dest;
	*ptr++ = NoteOff + Chan(e);
	*ptr++ = Pitch(e);
	*ptr = Vel(e);
	return 3;
}

static e2pRet PrivateE2PMth ( E2PInfosPtr i)
{
	MidiEvPtr priv;
	e2pRet ret;
	
	priv= i->evToSend;
	i->evToSend= (MidiEvPtr)LinkST(priv)->ptr1;
	ret= KeyOnE2PMth( i);
	MidiFreeEv( i->evToSend);
	return ret;
}

static e2pRet KeyPressE2PMth ( E2PInfosPtr i)
{
	unsigned char 	*ptr;
	MidiEvPtr 		e;
	
	e= i->evToSend;	
	ptr= i->dest;
	*ptr++ = PolyTouch + Chan(e);
	*ptr++ = Pitch(e);
	*ptr = Vel(e);
	return 3;
}

static e2pRet CtrlChangeE2PMth ( E2PInfosPtr i)
{
	unsigned char 	*ptr;
	MidiEvPtr 		e;
	
	e= i->evToSend;	
	ptr= i->dest;
	*ptr++ = ControlChg + Chan(e);
	*ptr++ = Pitch(e);
	*ptr = Vel(e);
	return 3;
}

static e2pRet ProgChangeE2PMth ( E2PInfosPtr i)
{
	unsigned char 	*ptr;
	MidiEvPtr 		e;
	
	e= i->evToSend;	
	ptr= i->dest;
	*ptr++ = ProgramChg + Chan(e);
	*ptr = Pitch(e);
	return 2;
}

static e2pRet ChanPressE2PMth ( E2PInfosPtr i)
{
	unsigned char 	*ptr;
	MidiEvPtr 		e;
	
	e= i->evToSend;	
	ptr= i->dest;
	*ptr++ = AfterTouch + Chan(e);
	*ptr = Pitch(e);
	return 2;
}

static e2pRet PitchWheelE2PMth ( E2PInfosPtr i)
{
	unsigned char 	*ptr;
	MidiEvPtr 		e;
	
	e= i->evToSend;	
	ptr= i->dest;
	*ptr++ = PitchBend + Chan(e);
	*ptr++ = Pitch(e);
	*ptr = Vel(e);
	return 3;
}

static e2pRet SongPosE2PMth ( E2PInfosPtr i)
{
	unsigned char 	*ptr;
	MidiEvPtr 		e;
	
	e= i->evToSend;	
	ptr= i->dest;
	*ptr++ = SongPos;
	*ptr++ = Pitch(e);
	*ptr = Vel(e);
	return 3;
}

static e2pRet SongSelE2PMth ( E2PInfosPtr i)
{
	unsigned char 	*ptr;
	
	ptr= i->dest;
	*ptr++ = SongSel;
	*ptr = Pitch(i->evToSend);
	return 2;
}

static e2pRet ClockE2PMth ( E2PInfosPtr i)
{
	*i->dest = MClock;
	return 1;
}

static e2pRet StartE2PMth ( E2PInfosPtr i)
{
	*i->dest = MStart;
	return 1;
}

static e2pRet ContinueE2PMth ( E2PInfosPtr i)
{
	*i->dest = MCont;
	return 1;
}

static e2pRet StopE2PMth ( E2PInfosPtr i)
{
	*i->dest = MStop;
	return 1;
}

static e2pRet TuneE2PMth ( E2PInfosPtr i)
{
	*i->dest = Tune;
	return 1;
}

static e2pRet ActiveSensE2PMth ( E2PInfosPtr i)
{
	*i->dest = ActSense;
	return 1;
}

static e2pRet ResetE2PMth ( E2PInfosPtr i)
{
	*i->dest = MReset;
	return 1;
}

static e2pRet SysExE2PMth ( E2PInfosPtr i)
{
	unsigned char 	*ptr, *data;
	MidiSEXPtr	last, ev;
	short n, j;
	e2pRet ret=0;
	
	ptr= i->dest;
	last = i->evToSend->info.linkSE;
	if( !i->cont){
		*ptr++= SysEx;
		ret++;
		ev = last->link;
	}
	else ev= (MidiSEXPtr)i->cont;
		
	data= ev->data;
	if( ev== last) {
		n= data[11];
		for( j=0; j<n; j++)
			*ptr++= *data++;
		*ptr= EndSysX;
		ret+= n +1;
		i->cont= 0;
		return ret;
	}
	else {
		for( j=0; j<12; j++)
			*ptr++= *data++;
		ret+= 12;
	}
	i->cont= (MidiEvPtr)ev->link;
	return ret;
}

static e2pRet StreamE2PMth ( E2PInfosPtr i)
{
	unsigned char 	*ptr, *data;
	MidiSEXPtr	last, ev;
	e2pRet ret=0, j;
	
	ptr= i->dest;
	last = i->evToSend->info.linkSE;
	if( !i->cont) ev = last->link;
	else ev= (MidiSEXPtr)i->cont;
		
	data= ev->data;
	if( ev== last) {
		short n;
		
		n= data[11];
		for( j=0; j<n; j++)
			*ptr++= *data++;
		ret+= n;
		i->cont= 0;
		return ret;
	}
	else {
		for( j=0; j<12; j++)
			*ptr++= *data++;
		ret+=12;
	}
	i->cont= (MidiEvPtr)ev->link;
	return ret;
}

static e2pRet QuarterFrameE2PMth ( E2PInfosPtr i)
{
	unsigned char 	*ptr;
	MidiEvPtr 		e;
	
	e= i->evToSend;		
	ptr= i->dest;
	*ptr++ = QFrame;
	*ptr = (e->info.data[0] << 4) + e->info.data[1];
	return 2;
}

static e2pRet Ctrl14bE2PMth ( E2PInfosPtr i)
{
	short	n, v;
	unsigned char 	*ptr;
	MidiEvPtr 		e;
	
	e= i->evToSend;		
	n = e->info.seqNum.number & 0x1F;
	v = e->info.seqNum.unused & 0x3FFF;
	ptr= i->dest;
	*ptr++ = ControlChg + Chan(e);
	*ptr++ = n;
	*ptr++ = v >> 7;
	*ptr++ = ControlChg + Chan(e);
	*ptr++ = n+32;
	*ptr = v & 0x7F;
	return 6;
}

static e2pRet NonRegParamE2PMth ( E2PInfosPtr i)
{
	short	n, v;
	unsigned char 	*ptr;
	MidiEvPtr 		e;
	
	e= i->evToSend;		
	n = e->info.seqNum.number & 0x3FFF;
	v = e->info.seqNum.unused & 0x3FFF;
	ptr= i->dest;
	*ptr++ = ControlChg + Chan(e);
	*ptr++ = NRPMSB;
	*ptr++ = n >> 7;
	*ptr++ = ControlChg + Chan(e);
	*ptr++ = NRPLSB;
	*ptr++ = n & 0x7F;
	*ptr++ = ControlChg + Chan(e);
	*ptr++ = DENTRYMSB;
	*ptr++ = v >> 7;
	*ptr++ = ControlChg + Chan(e);
	*ptr++ = DENTRYLSB;
	*ptr = v & 0x7F;
	return 12;
}

static e2pRet RegParamE2PMth ( E2PInfosPtr i)
{
	short	n, v;
	unsigned char 	*ptr;
	MidiEvPtr 		e;
	
	e= i->evToSend;		
	n = e->info.seqNum.number & 0x3FFF;
	v = e->info.seqNum.unused & 0x3FFF;
	ptr= i->dest;
	*ptr++ = ControlChg + Chan(e);
	*ptr++ = RPMSB;
	*ptr++ = n >> 7;
	*ptr++ = ControlChg + Chan(e);
	*ptr++ = RPLSB;
	*ptr++ = n & 0x7F;
	*ptr++ = ControlChg + Chan(e);
	*ptr++ = DENTRYMSB;
	*ptr++ = v >> 7;
	*ptr++ = ControlChg + Chan(e);
	*ptr++ = DENTRYLSB;
	*ptr = v & 0x7F;
	return 12;
}

//_________________________________________________________________________________
//							  Structuration	methods				
//_________________________________________________________________________________

//_________________________________________________________________________________
static p2eRet NoDataP2EMth( MidiEvPtr e, Byte *src, short len)
{
	return 0;
}
	
//_________________________________________________________________________________
static p2eRet OneDataP2EMth( MidiEvPtr e, Byte *src, short len)
{
	if( len) {
		Data(e)[0]= *src;
		return 1;
	}
	return 0;
}
	
//_________________________________________________________________________________
static p2eRet TwoDataP2EMth( MidiEvPtr e, Byte *src, short len)
{
	short i;

	for( i=0; (i<2) && len; i++, len--)
		Data(e)[i]= *src++;
	return i;
}
	
//_________________________________________________________________________________
static p2eRet ThreeDataP2EMth( MidiEvPtr e, Byte *src, short len)
{
	short i;
	
	for( i=0; (i<3) && len; i++, len--)
		Data(e)[i]= *src++;
	return i;
}

//_________________________________________________________________________________
static p2eRet SysExP2EMth( MidiEvPtr e, Byte *src, short len)
{
	short tt;
	Byte v;
	
	tt= len;
	while( len--) {
		v= *src++;
		if( v==	EndSysX)
			break;
		MidiAddField( e, v);
	}
	return tt - len;
}


//_________________________________________________________________________________
static p2eRet QuarterFrameP2EMth( MidiEvPtr e, Byte *src, short len)
{
	if( len) {
		MidiAddField( e, QFrame);
		MidiAddField( e, *src);
		return 1;
	}
	return 0;
}

//_________________________________________________________________________________
void InitLinearizeMthTbl (Ev2PacketFunc* e2p, Packet2EvFunc*	p2e)
{
	int	i;
	
	for (i=0; i<256; i++) 
		e2p[i]= NullE2PMth;

	e2p[typeNote] 		= NoteE2PMth;
	e2p[typeKeyOn] 		= KeyOnE2PMth;
	e2p[typeKeyOff] 	= KeyOffE2PMth;
	e2p[typeKeyPress] 	= KeyPressE2PMth;
	e2p[typeCtrlChange] = CtrlChangeE2PMth;
	e2p[typeProgChange] = ProgChangeE2PMth;
	e2p[typeChanPress] 	= ChanPressE2PMth;
	e2p[typePitchWheel] = PitchWheelE2PMth;
	e2p[typeSongPos] 	= SongPosE2PMth;
	e2p[typeSongSel] 	= SongSelE2PMth;
	e2p[typeClock] 		= ClockE2PMth;
	e2p[typeStart] 		= StartE2PMth;
	e2p[typeContinue] 	= ContinueE2PMth;
	e2p[typeStop] 		= StopE2PMth;
	e2p[typeTune] 		= TuneE2PMth;
	e2p[typeActiveSens] = ActiveSensE2PMth;
	e2p[typeReset] 		= ResetE2PMth;
	e2p[typeSysEx] 		= SysExE2PMth;
	e2p[typeStream] 	= StreamE2PMth;
	e2p[typeQuarterFrame] = QuarterFrameE2PMth;
	e2p[typeCtrl14b] 	= Ctrl14bE2PMth;
	e2p[typeNonRegParam] = NonRegParamE2PMth;
	e2p[typeRegParam] 	= RegParamE2PMth;
	e2p[typePrivate] 	= PrivateE2PMth;

	for (i=0; i<256; i++) 
		p2e[i]= NoDataP2EMth;
	
	p2e[typeKeyOn] 		= TwoDataP2EMth;
	p2e[typeKeyOff] 	= TwoDataP2EMth;
	p2e[typeKeyPress] 	= TwoDataP2EMth;
	p2e[typeCtrlChange] = TwoDataP2EMth;
	p2e[typeProgChange] = OneDataP2EMth;
	p2e[typeChanPress] 	= OneDataP2EMth;
	p2e[typePitchWheel] = TwoDataP2EMth;
	p2e[typeSongPos] 	= TwoDataP2EMth;
	p2e[typeSongSel] 	= OneDataP2EMth;
	p2e[typeClock] 		= NoDataP2EMth;
	p2e[typeStart] 		= NoDataP2EMth;
	p2e[typeContinue] 	= NoDataP2EMth;
	p2e[typeStop] 		= NoDataP2EMth;
	p2e[typeTune] 		= NoDataP2EMth;
	p2e[typeActiveSens] = NoDataP2EMth;
	p2e[typeReset] 		= NoDataP2EMth;
	p2e[typeSysEx] 		= SysExP2EMth;
	p2e[typeQuarterFrame] = QuarterFrameP2EMth;
}

//_________________________________________________________________________________
void InitTypeTbl( Byte*	typeTbl)
{
	register int i;
	Byte *ptr;
	
	ptr= typeTbl;
	for( i=0; i<256; i++) 	*ptr++= 0;
	
	ptr= &typeTbl[NoteOff];
	for( i=0; i< 16; i++)	*ptr++= typeKeyOff;		
	ptr= &typeTbl[NoteOn];
	for( i=0; i< 16; i++)	*ptr++= typeKeyOn;
	ptr= &typeTbl[PolyTouch];
	for( i=0; i< 16; i++)	*ptr++= typeKeyPress;
	ptr= &typeTbl[ControlChg];
	for( i=0; i< 16; i++)	*ptr++= typeCtrlChange;
	ptr= &typeTbl[ProgramChg];
	for( i=0; i< 16; i++)	*ptr++= typeProgChange;
	ptr= &typeTbl[AfterTouch];
	for( i=0; i< 16; i++)	*ptr++= typeChanPress;
	ptr= &typeTbl[PitchBend];
	for( i=0; i< 16; i++)	*ptr++= typePitchWheel;
	
	typeTbl[SysEx]	= typeSysEx;
	typeTbl[QFrame]	= typeStream;			/* typeQuarterFrame (indeed 130) */
	typeTbl[SongPos]	= typeSongPos;
	typeTbl[SongSel]	= typeSongSel;
	typeTbl[Tune]		= typeTune;
	
	typeTbl[MClock]	= typeClock;
	typeTbl[MStart]	= typeStart;
	typeTbl[MCont]	= typeContinue;
	typeTbl[MStop]	= typeStop;
	typeTbl[ActSense]	= typeActiveSens;
	typeTbl[MReset]	= typeReset;
}

//____________________________________________________________________________________
static MidiEvPtr NewEv( unsigned char status,  Byte* typeTbl)
{
	MidiEvPtr e= 0;
	short type;

	type= typeTbl[status];
	if( type) {
		e= MidiNewEv( type);
		if( e) {
			Date(e)= MidiGetTime();
			RefNum(e)= 0;
			Port(e)= ModemPort;
			Chan(e)= (type< typeSongPos) ? status%16 : 0;
			Link(e)= 0;
		}
	}
	return e;
}

//____________________________________________________________________________________
MidiEvPtr Packet2Evs( P2EInfosPtr i,   Packet2EvFunc*	p2e, Byte*	typeTbl)
{
	MidiEvPtr e= 0, prev= 0;
	Byte *src;
	short len, n;
	
	src= i->src;
	len= i->len;
	if( i->cont) e= i->cont;
	else {
		e= NewEv( *src, typeTbl);
		src++; len--; 
	}
	while( e) {
		n= (*p2e[EvType(e)])( e, src, len);
		len -= n;
		src+= n;

		Link(e)= prev;
		prev= e;
		if( len > 0) {
			e= NewEv( *src, typeTbl);
			src++; len--;
		}
		else break;
	}
	return prev;
}

//____________________________________________________________________________________
e2pRet EvToPacket(E2PInfosPtr i, Ev2PacketFunc*	e2p)
{
	return (*e2p[EvType(i->evToSend)])(i);
}
	