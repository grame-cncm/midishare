/*
  MidiShare Project
  Copyright (C) Grame 1999

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
  
*/

#include "EventToMidiStream.h"

#ifndef false
enum { false, true };
#endif

/*------------------------------------------------------------------------*/
#define NRPLSB		98		/* non registr. param number (lsb)	*/
#define NRPMSB		99		/* non registr. param number (msb)	*/

#define RPLSB		100		/* registred param number (lsb)		*/
#define RPMSB		101		/* registred param number (msb)		*/

#define DENTRYMSB	6		/* data entry value (msb)		*/
#define DENTRYLSB	38		/* data entry value (lsb)		*/

/*===========================================================================
	internal functions prototypes
  =========================================================================== */

static void     NormalContinuation     (Ev2StreamPtr f);
static void     SysExContinuation      (Ev2StreamPtr f);
static void     StreamContinuation     (Ev2StreamPtr f);

static void     NullLinearizeMth         (MidiEvPtr e, Ev2StreamPtr f);
static void     NoteLinearizeMth         (MidiEvPtr e, Ev2StreamPtr f);
static void     KeyOnLinearizeMth        (MidiEvPtr e, Ev2StreamPtr f);
static void     KeyOffLinearizeMth       (MidiEvPtr e, Ev2StreamPtr f);
static void     KeyPressLinearizeMth     (MidiEvPtr e, Ev2StreamPtr f);
static void     CtrlChangeLinearizeMth   (MidiEvPtr e, Ev2StreamPtr f);
static void     ProgChangeLinearizeMth   (MidiEvPtr e, Ev2StreamPtr f);
static void     ChanPressLinearizeMth    (MidiEvPtr e, Ev2StreamPtr f);
static void     PitchWheelLinearizeMth   (MidiEvPtr e, Ev2StreamPtr f);
static void     SongPosLinearizeMth      (MidiEvPtr e, Ev2StreamPtr f);
static void     SongSelLinearizeMth      (MidiEvPtr e, Ev2StreamPtr f);
static void     ClockLinearizeMth        (MidiEvPtr e, Ev2StreamPtr f);
static void     StartLinearizeMth        (MidiEvPtr e, Ev2StreamPtr f);
static void     ContinueLinearizeMth     (MidiEvPtr e, Ev2StreamPtr f);
static void     StopLinearizeMth         (MidiEvPtr e, Ev2StreamPtr f);
static void     TuneLinearizeMth         (MidiEvPtr e, Ev2StreamPtr f);
static void     ActiveSensLinearizeMth   (MidiEvPtr e, Ev2StreamPtr f);
static void     ResetLinearizeMth        (MidiEvPtr e, Ev2StreamPtr f);
static void     SysExLinearizeMth        (MidiEvPtr e, Ev2StreamPtr f);
static void     StreamLinearizeMth       (MidiEvPtr e, Ev2StreamPtr f);
static void     QuarterFrameLinearizeMth (MidiEvPtr e, Ev2StreamPtr f);
static void     Ctrl14bLinearizeMth      (MidiEvPtr e, Ev2StreamPtr f);
static void     NonRegParamLinearizeMth  (MidiEvPtr e, Ev2StreamPtr f);
static void     RegParamLinearizeMth     (MidiEvPtr e, Ev2StreamPtr f);


/*===========================================================================
		initialization functions
  =========================================================================== */
void MidiStreamInitMthTbl (LinearizeMthTbl lin)
{
	int	i;
	
	for (i=0; i<256; i++) lin[i] = NullLinearizeMth;
	lin[typeNote]         = NoteLinearizeMth;
	lin[typeKeyOn]        = KeyOnLinearizeMth;
	lin[typeKeyOff]       = KeyOffLinearizeMth;
	lin[typeKeyPress]     = KeyPressLinearizeMth;
	lin[typeCtrlChange]   = CtrlChangeLinearizeMth;
	lin[typeProgChange]   = ProgChangeLinearizeMth;
	lin[typeChanPress]    = ChanPressLinearizeMth;
	lin[typePitchWheel]   = PitchWheelLinearizeMth;
	lin[typeSongPos]      = SongPosLinearizeMth;
	lin[typeSongSel]      = SongSelLinearizeMth;
	lin[typeClock]        = ClockLinearizeMth;
	lin[typeStart]        = StartLinearizeMth;
	lin[typeContinue]     = ContinueLinearizeMth;
	lin[typeStop]         = StopLinearizeMth;
	lin[typeTune]         = TuneLinearizeMth;
	lin[typeActiveSens]   = ActiveSensLinearizeMth;
	lin[typeReset]        = ResetLinearizeMth;
	lin[typeSysEx]        = SysExLinearizeMth;
	lin[typeStream]       = StreamLinearizeMth;
	lin[typeQuarterFrame] = QuarterFrameLinearizeMth;
	lin[typeCtrl14b]      = Ctrl14bLinearizeMth;
	lin[typeNonRegParam]  = NonRegParamLinearizeMth;
	lin[typeRegParam]     = RegParamLinearizeMth;
}

void MidiStreamInit (Ev2StreamPtr f, LinearizeMthTbl lin)
{
	f->cont     = NormalContinuation;
	f->lin      = lin;
	f->nextCell = 0;
	f->runStat	= 0;
	f->count	= 0;
}

void MidiStreamReset (Ev2StreamPtr f)
{
	MidiEvPtr next, e = (MidiEvPtr)f->nextCell;
	while (e) {
		next = Link(e);
		MidiFreeCell(e);
		e = next;
	}
	f->cont     = NormalContinuation;
	f->nextCell = 0;
	f->runStat	= 0;
	f->count    = 0;
}

/*===========================================================================
		 external functions implementation
  =========================================================================== */
MidiEvPtr MidiStreamPutEvent (Ev2StreamPtr f, MidiEvPtr e)
{
	if (f->count)  return e;
	if (EvType(e) == typeNote) {
		(f->lin[EvType(e)]) (e, f);
		EvType(e) = typeKeyOn;
		Vel(e) = 0;
		Date(e) += Dur(e);
		return e;
	}
	(f->lin[EvType(e)]) (e, f);
	return 0;
}

Boolean	MidiStreamGetByte (Ev2StreamPtr f, Byte *code)
{	
 	if (!f->count) return false;
	*code = f->data[f->count--];
	if (!f->count) (*f->cont)( f);
	return true;
}
		
/*===========================================================================
		internal functions implementation
  =========================================================================== */

//___________________________________________________________________________
//	continuations					
//___________________________________________________________________________
static void NormalContinuation (Ev2StreamPtr f)
{
	f->runStat= 0;
	f->count= 0;
}

static void SysExContinuation (Ev2StreamPtr f)
{
	MidiSEXPtr	c;
	
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
		f->cont = NormalContinuation;
	} else {
		int i = 0;
		int j = 12;
		f->count = j;
		do {
			f->data[j] = c->data[i];
			i++;
		} while (--j);
	}
	MidiFreeCell((MidiEvPtr)c);	
}

static void StreamContinuation (Ev2StreamPtr f)
{
	MidiSEXPtr	c;
	int			i=0, j;
	
	c = f->nextCell;
	
	if ((f->nextCell = c->link) == 0) {
		f->cont = NormalContinuation;
		if ((f->count = j = c->data[11]) == 0) {
			(*NormalContinuation)(f);
		} else {
			do { f->data[j] = c->data[i]; i++; } while (--j);
		}
	} else {
		f->count = j = 12;
		do { f->data[j] = c->data[i]; i++; } while (--j);
	}
	MidiFreeCell((MidiEvPtr)c);	
}

//___________________________________________________________________________
//	linearization methods					
//___________________________________________________________________________
static void NullLinearizeMth (MidiEvPtr e, Ev2StreamPtr f)
{
	MidiFreeEv (e);
}

static void NoteLinearizeMth (MidiEvPtr e, Ev2StreamPtr f)
{
	Byte status = NoteOn + Chan(e);
	if( f->runStat== status)
		f->count= 2;
	else {
		f->runStat= f->data[3] = status;
		f->count = 3;
	}
	f->data[2] = Pitch(e);
	f->data[1] = Vel(e);
}

static void KeyOnLinearizeMth (MidiEvPtr e, Ev2StreamPtr f)
{
	Byte status = NoteOn + Chan(e);
	if( f->runStat== status)
		f->count= 2;
	else {
		f->runStat= f->data[3] = status;
		f->count = 3;
	}
	f->data[2] = Pitch(e);
	f->data[1] = Vel(e);
	MidiFreeCell (e);	
}

static void KeyOffLinearizeMth (MidiEvPtr e, Ev2StreamPtr f)
{
	Byte status = NoteOff + Chan(e);
	if( f->runStat== status)
		f->count= 2;
	else {
		f->runStat= f->data[3] = status;
		f->count = 3;
	}
	f->data[2] = Pitch(e);
	f->data[1] = Vel(e);
	MidiFreeCell (e);	
}

static void KeyPressLinearizeMth (MidiEvPtr e, Ev2StreamPtr f)
{
	Byte status = PolyTouch + Chan(e);
	if( f->runStat== status)
		f->count= 2;
	else {
		f->runStat= f->data[3] = status;
		f->count = 3;
	}
	f->data[2] = Pitch(e);
	f->data[1] = Vel(e);
	MidiFreeCell (e);	
}

static void CtrlChangeLinearizeMth (MidiEvPtr e, Ev2StreamPtr f)
{
	Byte status = ControlChg + Chan(e);
	if( f->runStat== status)
		f->count= 2;
	else {
		f->runStat= f->data[3] = status;
		f->count = 3;
	}
	f->data[2] = Pitch(e);
	f->data[1] = Vel(e);
	MidiFreeCell (e);	
}

static void ProgChangeLinearizeMth (MidiEvPtr e, Ev2StreamPtr f)
{
	Byte status = ProgramChg + Chan(e);
	if( f->runStat== status)
		f->count= 1;
	else {
		f->runStat= f->data[2] = status;
		f->count = 2;
	}
	f->data[1] = Pitch(e);
	MidiFreeCell (e);	
}

static void ChanPressLinearizeMth (MidiEvPtr e, Ev2StreamPtr f)
{
	Byte status = AfterTouch + Chan(e);
	if( f->runStat== status)
		f->count= 1;
	else {
		f->runStat= f->data[2] = status;
		f->count = 2;
	}
	f->data[1] = Pitch(e);
	MidiFreeCell (e);	
}

static void PitchWheelLinearizeMth (MidiEvPtr e, Ev2StreamPtr f)
{
	Byte status = PitchBend + Chan(e);
	if( f->runStat== status)
		f->count= 2;
	else {
		f->runStat= f->data[3] = status;
		f->count = 3;
	}
	f->data[2] = Pitch(e);
	f->data[1] = Vel(e);
	MidiFreeCell (e);	
}

static void SongPosLinearizeMth (MidiEvPtr e, Ev2StreamPtr f)
{
	f->count = 3;
	f->data[3] = SongPos;
	f->data[2] = Pitch(e);
	f->data[1] = Vel(e);
	f->runStat = 0;
	MidiFreeCell (e);	
}

static void SongSelLinearizeMth (MidiEvPtr e, Ev2StreamPtr f)
{
	f->count = 2;
	f->data[2] = SongSel;
	f->data[1] = Pitch(e);
	f->runStat = 0;
	MidiFreeCell (e);	
}

static void ClockLinearizeMth (MidiEvPtr e, Ev2StreamPtr f)
{
	f->count = 1;
	f->data[1] = MClock;
	MidiFreeCell (e);	
}

static void StartLinearizeMth (MidiEvPtr e, Ev2StreamPtr f)
{
	f->count = 1;
	f->data[1] = MStart;
	MidiFreeCell (e);	
}

static void ContinueLinearizeMth (MidiEvPtr e, Ev2StreamPtr f)
{
	f->count = 1;
	f->data[1] = MCont;
	MidiFreeCell (e);	
}

static void StopLinearizeMth (MidiEvPtr e, Ev2StreamPtr f)
{	
	f->count = 1;
	f->data[1] = MStop;
	MidiFreeCell (e);	
}

static void TuneLinearizeMth (MidiEvPtr e, Ev2StreamPtr f)
{	
	f->count = 1;
	f->data[1] = Tune;
	MidiFreeCell (e);	
}

static void ActiveSensLinearizeMth (MidiEvPtr e, Ev2StreamPtr f)
{	
	f->count = 1;
	f->data[1] = ActSense;
	MidiFreeCell (e);	
}

static void ResetLinearizeMth (MidiEvPtr e, Ev2StreamPtr f)
{	
	f->count = 1;
	f->data[1] = MReset;
	MidiFreeCell (e);	
}

static void SysExLinearizeMth (MidiEvPtr e, Ev2StreamPtr f)
{
	MidiSEXPtr	last, first;
	
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
		f->cont = SysExContinuation;
	}
	MidiFreeCell (e);	
	MidiFreeCell ((MidiEvPtr)first);	
}

static void StreamLinearizeMth (MidiEvPtr e, Ev2StreamPtr f)
{
	MidiSEXPtr	last, first;
	int		i=0, j;
	
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
		f->cont = StreamContinuation;
	}
	MidiFreeCell (e);	
	MidiFreeCell ((MidiEvPtr)first);	
}

static void QuarterFrameLinearizeMth (MidiEvPtr e, Ev2StreamPtr f)
{	
	f->count = 2;
	f->data[2] = QFrame;
	f->data[1] = (e->info.data[0] << 4) + e->info.data[1];
	f->runStat = 0;
	MidiFreeCell (e);	
}

static void Ctrl14bLinearizeMth (MidiEvPtr e, Ev2StreamPtr f)
{
	Byte status;
	short	n = e->info.param.num & 0x1F;
	short	v = e->info.param.val & 0x3FFF;
	
	status = ControlChg + Chan(e);
	if( f->runStat== status)
		f->count= 4;
	else {
		f->runStat= f->data[5]= status;
		f->count= 5;
	}
	f->data[5] = ControlChg + Chan(e);
	f->data[4] = (Byte)n;
	f->data[3] = v >> 7;
	f->data[2] = n+32;
	f->data[1] = v & 0x7F;
	MidiFreeCell (e);	
}

static void NonRegParamLinearizeMth (MidiEvPtr e, Ev2StreamPtr f)
{
	Byte status;
	short	n = e->info.param.num & 0x3FFF;
	short	v = e->info.param.val & 0x3FFF;
	
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
	MidiFreeCell (e);	
}

static void RegParamLinearizeMth (MidiEvPtr e, Ev2StreamPtr f)
{
	Byte status;
	short	n = e->info.param.num & 0x3FFF;
	short	v = e->info.param.val & 0x3FFF;
	
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
	MidiFreeCell (e);	
}
