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

#include "msFxmt.h"
#include "msEvents.h"
#include "msXmtRcv.h"


static LinearizeMethodPtr LinearizeTbl[256];

/*===========================================================================
			PROTOTYPES DES FONCTIONS PUREMENT INTERNES
  =========================================================================== */

void     XFNormalContinuation     (XFifoPtr f);
void     XFSysExContinuation      (XFifoPtr f);
void     XFStreamContinuation     (XFifoPtr f);

void     NullLinearizeMth         (MidiEvPtr e, XFifoPtr f);
void     NoteLinearizeMth         (MidiEvPtr e, XFifoPtr f);
void     KeyOnLinearizeMth        (MidiEvPtr e, XFifoPtr f);
void     KeyOffLinearizeMth       (MidiEvPtr e, XFifoPtr f);
void     KeyPressLinearizeMth     (MidiEvPtr e, XFifoPtr f);
void     CtrlChangeLinearizeMth   (MidiEvPtr e, XFifoPtr f);
void     ProgChangeLinearizeMth   (MidiEvPtr e, XFifoPtr f);
void     ChanPressLinearizeMth    (MidiEvPtr e, XFifoPtr f);
void     PitchWheelLinearizeMth   (MidiEvPtr e, XFifoPtr f);
void     SongPosLinearizeMth      (MidiEvPtr e, XFifoPtr f);
void     SongSelLinearizeMth      (MidiEvPtr e, XFifoPtr f);
void     ClockLinearizeMth        (MidiEvPtr e, XFifoPtr f);
void     StartLinearizeMth        (MidiEvPtr e, XFifoPtr f);
void     ContinueLinearizeMth     (MidiEvPtr e, XFifoPtr f);
void     StopLinearizeMth         (MidiEvPtr e, XFifoPtr f);
void     TuneLinearizeMth         (MidiEvPtr e, XFifoPtr f);
void     ActiveSensLinearizeMth   (MidiEvPtr e, XFifoPtr f);
void     ResetLinearizeMth        (MidiEvPtr e, XFifoPtr f);
void     SysExLinearizeMth        (MidiEvPtr e, XFifoPtr f);
void     StreamLinearizeMth       (MidiEvPtr e, XFifoPtr f);
void     QuarterFrameLinearizeMth (MidiEvPtr e, XFifoPtr f);
void     Ctrl14bLinearizeMth      (MidiEvPtr e, XFifoPtr f);
void     NonRegParamLinearizeMth  (MidiEvPtr e, XFifoPtr f);
void     RegParamLinearizeMth     (MidiEvPtr e, XFifoPtr f);


/*===========================================================================
		IMPLÉMENTATION DES FONCTIONS D'INITIALISATION EXTERNES
  =========================================================================== */

void	XFInitMthTbl ()
{
	int	i;
	
	for (i=0; i<256; i++) LinearizeTbl[i] = NullLinearizeMth;
	LinearizeTbl[typeNote]         = NoteLinearizeMth;
	LinearizeTbl[typeKeyOn]        = KeyOnLinearizeMth;
	LinearizeTbl[typeKeyOff]       = KeyOffLinearizeMth;
	LinearizeTbl[typeKeyPress]     = KeyPressLinearizeMth;
	LinearizeTbl[typeCtrlChange]   = CtrlChangeLinearizeMth;
	LinearizeTbl[typeProgChange]   = ProgChangeLinearizeMth;
	LinearizeTbl[typeChanPress]    = ChanPressLinearizeMth;
	LinearizeTbl[typePitchWheel]   = PitchWheelLinearizeMth;
	LinearizeTbl[typeSongPos]      = SongPosLinearizeMth;
	LinearizeTbl[typeSongSel]      = SongSelLinearizeMth;
	LinearizeTbl[typeClock]        = ClockLinearizeMth;
	LinearizeTbl[typeStart]        = StartLinearizeMth;
	LinearizeTbl[typeContinue]     = ContinueLinearizeMth;
	LinearizeTbl[typeStop]         = StopLinearizeMth;
	LinearizeTbl[typeTune]         = TuneLinearizeMth;
	LinearizeTbl[typeActiveSens]   = ActiveSensLinearizeMth;
	LinearizeTbl[typeReset]        = ResetLinearizeMth;
	LinearizeTbl[typeSysEx]        = SysExLinearizeMth;
	LinearizeTbl[typeStream]       = StreamLinearizeMth;
	LinearizeTbl[typeQuarterFrame] = QuarterFrameLinearizeMth;
	LinearizeTbl[typeCtrl14b]      = Ctrl14bLinearizeMth;
	LinearizeTbl[typeNonRegParam]  = NonRegParamLinearizeMth;
	LinearizeTbl[typeRegParam]     = RegParamLinearizeMth;
}


#define ProtectedFastFreeCell(fl, e)  MSFreeCell(e,fl)

/*===========================================================================
		 IMPLÉMENTATION DES FONCTIONS UTILITAIRES EXTERNES
  =========================================================================== */

void XFInit( XFifoPtr f, TMSGlobalPtr g, short refnum)
{
	f->global 	= g;
	f->state	= kLineOff;
	f->cont         = XFNormalContinuation;
	f->nextCell 	= 0;
	f->runStat	= 0;
	f->port		= 0;
	f->count	= 0;
	f->lastXmtDate	= 0;
	f->refNum 	= refnum;
}

void XFPutEvent( register XFifoPtr f, register MidiEvPtr e)
{
	if(f->count==0)  {
		f->runStat= 0;
		(*LinearizeTbl[EvType(e)]) (e, f);
	}
	
}

Boolean	XFGetByte( register XFifoPtr f, Byte FAR *code, Byte FAR *port)
{	
 	if( !f->count) return false;
	
	*port = f->port;
	*code = f->data[f->count--];
	
	if( !f->count) (*f->cont)( f);
	return true;
}

int	XFCountByte( register XFifoPtr f){ return f->count;}

		
/*===========================================================================
			IMPLÉMENTATION DES FONCTIONS PUREMENT INTERNES
  =========================================================================== */

//___________________________________________________________________________
//					  Les Continuations d'émission					
//___________________________________________________________________________

void XFNormalContinuation ( register XFifoPtr f)
{
	f->runStat= 0;
	f->count= 0;
}

void XFSysExContinuation (register XFifoPtr f)
{
	register MidiSEXPtr	c;
	
	c = f->nextCell;
	if ((f->nextCell = c->link) == 0) {
		int i = 0;
		int j = c->data[11]+1;
		f->count = j;
		while(j>1) {
			f->data[j] = c->data[i];
			i++; j--;
		}
		f->data[1] = EndSysX;
		f->cont = XFNormalContinuation;
	} else {
		int i = 0;
		int j = 12;
		f->count = j;
		do {
			f->data[j] = c->data[i];
			i++;
		} while (--j);
	}
	MSFreeCell(c, FreeList(Memory(f->global)));	
}

void XFStreamContinuation ( register XFifoPtr f)
{
	register MidiSEXPtr	c;
	int			i=0, j;
	
	c = f->nextCell;
	
	if ((f->nextCell = c->link) == 0) {
		f->cont = XFNormalContinuation;
		if ((f->count = j = c->data[11]) == 0) {
			(*XFNormalContinuation)(f);
		} else {
			do { f->data[j] = c->data[i]; i++; } while (--j);
		}
	} else {
		f->count = j = 12;
		do { f->data[j] = c->data[i]; i++; } while (--j);
	}
	MSFreeCell(c, FreeList(Memory(f->global)));		
}

//___________________________________________________________________________
//				Les méthodes de linearisation					
//___________________________________________________________________________

void NullLinearizeMth (MidiEvPtr e, XFifoPtr f)
{
	MSFreeEv( e, FreeList(Memory(f->global)));
}

void NoteLinearizeMth ( register MidiEvPtr e, register XFifoPtr f)
{
	register uchar status;
	
	f->port = Port(e);
	status = NoteOn + Chan(e);
	if( f->runStat== status)
		f->count= 2;
	else {
		f->runStat= f->data[3] = status;
		f->count = 3;
	}
	f->data[2] = Pitch(e);
	f->data[1] = Vel(e);
	
	EvType(e) = typeKeyOn;
	Vel(e) = 0;
	Date(e) += Dur(e);
	MSSend(f->refNum | 0x80,e,SorterList(f->global));
}

void KeyOnLinearizeMth ( register MidiEvPtr e, register XFifoPtr f)
{
	register uchar status;
	
	f->port = Port(e);
	status = NoteOn + Chan(e);
	if( f->runStat== status)
		f->count= 2;
	else {
		f->runStat= f->data[3] = status;
		f->count = 3;
	}
	f->data[2] = Pitch(e);
	f->data[1] = Vel(e);
	MSFreeCell(e, FreeList(Memory(f->global)));	
}

void KeyOffLinearizeMth (register MidiEvPtr e, register XFifoPtr f)
{
	register uchar status;
	
	f->port = Port(e);
	status = NoteOff + Chan(e);
	if( f->runStat== status)
		f->count= 2;
	else {
		f->runStat= f->data[3] = status;
		f->count = 3;
	}
	f->data[2] = Pitch(e);
	f->data[1] = Vel(e);
	MSFreeCell(e, FreeList(Memory(f->global)));	
}

void KeyPressLinearizeMth (register MidiEvPtr e, register XFifoPtr f)
{
	register uchar status;
	
	f->port = Port(e);
	status = PolyTouch + Chan(e);
	if( f->runStat== status)
		f->count= 2;
	else {
		f->runStat= f->data[3] = status;
		f->count = 3;
	}
	f->data[2] = Pitch(e);
	f->data[1] = Vel(e);
	MSFreeCell(e, FreeList(Memory(f->global)));	
}

void CtrlChangeLinearizeMth (register MidiEvPtr e, register XFifoPtr f)
{
	register uchar status;
	
	f->port = Port(e);
	status = ControlChg + Chan(e);
	if( f->runStat== status)
		f->count= 2;
	else {
		f->runStat= f->data[3] = status;
		f->count = 3;
	}
	f->data[2] = Pitch(e);
	f->data[1] = Vel(e);
	MSFreeCell(e, FreeList(Memory(f->global)));	
}

void ProgChangeLinearizeMth (register MidiEvPtr e, register XFifoPtr f)
{
	register uchar status;
	
	f->port = Port(e);
	status = ProgramChg + Chan(e);
	if( f->runStat== status)
		f->count= 1;
	else {
		f->runStat= f->data[2] = status;
		f->count = 2;
	}
	f->data[1] = Pitch(e);
	MSFreeCell(e, FreeList(Memory(f->global)));	
}

void ChanPressLinearizeMth (register MidiEvPtr e, register XFifoPtr f)
{
	register uchar status;
	
	f->port = Port(e);
	status = AfterTouch + Chan(e);
	if( f->runStat== status)
		f->count= 1;
	else {
		f->runStat= f->data[2] = status;
		f->count = 2;
	}
	f->data[1] = Pitch(e);
	MSFreeCell(e, FreeList(Memory(f->global)));	
}

void PitchWheelLinearizeMth (register MidiEvPtr e, register XFifoPtr f)
{
	register uchar status;
	
	f->port = Port(e);
	status = PitchBend + Chan(e);
	if( f->runStat== status)
		f->count= 2;
	else {
		f->runStat= f->data[3] = status;
		f->count = 3;
	}
	f->data[2] = Pitch(e);
	f->data[1] = Vel(e);
	MSFreeCell(e, FreeList(Memory(f->global)));	
}

void SongPosLinearizeMth (register MidiEvPtr e, register XFifoPtr f)
{
	f->port = Port(e);
	
	f->count = 3;
	f->data[3] = SongPos;
	f->data[2] = Pitch(e);
	f->data[1] = Vel(e);
	f->runStat = 0;
	MSFreeCell(e, FreeList(Memory(f->global)));	
}

void SongSelLinearizeMth (register MidiEvPtr e, register XFifoPtr f)
{
	f->port = Port(e);
	
	f->count = 2;
	f->data[2] = SongSel;
	f->data[1] = Pitch(e);
	f->runStat = 0;
	MSFreeCell(e, FreeList(Memory(f->global)));	
}

void ClockLinearizeMth (register MidiEvPtr e, register XFifoPtr f)
{
	f->port = Port(e);
	
	f->count = 1;
	f->data[1] = MClock;
	MSFreeCell(e, FreeList(Memory(f->global)));	
}

void StartLinearizeMth (register MidiEvPtr e, register XFifoPtr f)
{
	f->port = Port(e);
	
	f->count = 1;
	f->data[1] = MStart;
	MSFreeCell(e, FreeList(Memory(f->global)));	
}

void ContinueLinearizeMth (register MidiEvPtr e, register XFifoPtr f)
{
	f->port = Port(e);
	
	f->count = 1;
	f->data[1] = MCont;
	MSFreeCell(e, FreeList(Memory(f->global)));	
}

void StopLinearizeMth (register MidiEvPtr e, register XFifoPtr f)
{
	f->port = Port(e);
	
	f->count = 1;
	f->data[1] = MStop;
	MSFreeCell(e, FreeList(Memory(f->global)));	
}

void TuneLinearizeMth (register MidiEvPtr e, register XFifoPtr f)
{
	f->port = Port(e);
	
	f->count = 1;
	f->data[1] = Tune;
	MSFreeCell(e, FreeList(Memory(f->global)));	
}

void ActiveSensLinearizeMth (register MidiEvPtr e, register XFifoPtr f)
{
	f->port = Port(e);
	
	f->count = 1;
	f->data[1] = ActSense;
	MSFreeCell(e, FreeList(Memory(f->global)));	
}

void ResetLinearizeMth (register MidiEvPtr e, register XFifoPtr f)
{
	f->port = Port(e);
	
	f->count = 1;
	f->data[1] = MReset;
	MSFreeCell(e, FreeList(Memory(f->global)));	
}

void SysExLinearizeMth (register MidiEvPtr e, register XFifoPtr f)
{
	MidiSEXPtr	last, first;
	
	f->port = Port(e);
	last = e->info.linkSE;
	first = last->link;
	last->link = 0;
	f->runStat = 0;
	if (first == last) {
		int i = 0;
		int j = first->data[11]+2;

		f->count = j;
		f->data[j--] = SysEx;
		while(j>1) f->data[j--] = first->data[i++];
		f->data[1] = EndSysX;
	} else {
		int i = 0;
		int j = 13;

		f->count = j;
		f->data[j--] = SysEx;
		while(j>0) f->data[j--] = first->data[i++];
		f->nextCell = first->link;
		f->cont = XFSysExContinuation;
	}
	MSFreeCell(e, FreeList(Memory(f->global)));	
	MSFreeCell(first, FreeList(Memory(f->global)));	
}

void StreamLinearizeMth (register MidiEvPtr e, register XFifoPtr f)
{
	MidiSEXPtr	last, first;
	int		i=0, j;
	
	f->port = Port(e);
	f->runStat = 0;	
	last = e->info.linkSE;
	first = last->link;
	last->link = 0;
	
	if ((f->nextCell = first->link) == 0) {
		f->count = j = first->data[11];
		while (j) { f->data[j] = first->data[i]; i++; j--; } 
	} else {
		f->count = j = 12;
		do { f->data[j] = first->data[i]; i++; } while (--j);
		f->cont = XFStreamContinuation;
	}
	MSFreeCell(e, FreeList(Memory(f->global)));	
	MSFreeCell(first, FreeList(Memory(f->global)));	
}

void QuarterFrameLinearizeMth (register MidiEvPtr e, register XFifoPtr f)
{
	f->port = Port(e);
	
	f->count = 2;
	f->data[2] = QFrame;
	f->data[1] = (e->info.data[0] << 4) + e->info.data[1];
	f->runStat = 0;
	MSFreeCell(e, FreeList(Memory(f->global)));	
}

void Ctrl14bLinearizeMth (register MidiEvPtr e, register XFifoPtr f)
{
	register uchar status;
	short	n = e->info.shortFields[0] & 0x1F;
	short	v = e->info.shortFields[1] & 0x3FFF;
	
	f->port = Port(e);
	status = ControlChg + Chan(e);
	if( f->runStat== status)
		f->count= 4;
	else {
		f->runStat= f->data[5]= status;
		f->count= 5;
	}
	f->data[5] = ControlChg + Chan(e);
	f->data[4] = n;
	f->data[3] = v >> 7;
	f->data[2] = n+32;
	f->data[1] = v & 0x7F;
	MSFreeCell(e, FreeList(Memory(f->global)));	
}

void NonRegParamLinearizeMth (register MidiEvPtr e, register XFifoPtr f)
{
	register uchar status;
	short	n = e->info.shortFields[0] & 0x3FFF;
	short	v = e->info.shortFields[1] & 0x3FFF;
	
	f->port = Port(e);
	status = ControlChg + Chan(e);
	if( f->runStat== status)
		f->count= 8;
	else {
		f->runStat= f->data[9]= status;
		f->count= 9;
	}
	f->data[8] = NRPMSB;
	f->data[7] = n >> 7;

	f->data[6] = NRPLSB;
	f->data[5] = n & 0x7F;

	f->data[4] = DENTRYMSB;
	f->data[3] = v >> 7;

	f->data[2] = DENTRYLSB;
	f->data[1] = v & 0x7F;
	MSFreeCell(e, FreeList(Memory(f->global)));	
}

void RegParamLinearizeMth (register MidiEvPtr e, register XFifoPtr f)
{
	register uchar status;
	short	n = e->info.shortFields[0] & 0x3FFF;
	short	v = e->info.shortFields[1] & 0x3FFF;
	
	f->port = Port(e);
	status = ControlChg + Chan(e);
	if( f->runStat== status)
		f->count= 8;
	else {
		f->runStat= f->data[9]= status;
		f->count= 9;
	}
	f->data[8] = RPMSB;
	f->data[7] = n >> 7;

	f->data[6] = RPLSB;
	f->data[5] = n & 0x7F;

	f->data[4] = DENTRYMSB;
	f->data[3] = v >> 7;

	f->data[2] = DENTRYLSB;
	f->data[1] = v & 0x7F;
	MSFreeCell(e, FreeList(Memory(f->global)));
}
