// ===========================================================================
// The Player Library is Copyright (c) Grame, Computer Music Research Laboratory 
// 1996-1999, and is distributed as Open Source software under the Artistic License;
// see the file "Artistic" that is included in the distribution for details.
//
// Grame : Computer Music Research Laboratory
// Web : http://www.grame.fr/Research
// E-mail : MidiShare@rd.grame.fr
// ===========================================================================


// ===========================================================================
//	TClockSender.cpp		    
// ===========================================================================
//
//	Manage Midi events related to Clock synchronization
//


#include "TClockSender.h"
#include "UTools.h"

/*--------------------------------------------------------------------------*/

TClockSender::TClockSender(
	TSchedulerInterfacePtr scheduler, 
	TClockConverterPtr converter,
	TEventSenderInterfacePtr user)
{
	fScheduler = scheduler;
	fClockConverter = converter;
	fEventUser = user;
	fClockTask = new TClockTask(this);
}

/*--------------------------------------------------------------------------*/

TClockSender::~TClockSender(){ delete(fClockTask); }

/*--------------------------------------------------------------------------*/

void TClockSender::Start() 
{
	Stop();
	fClockCount = 0;
	fEventUser->SendEvent(MidiNewEv(typeStart),MidiGetTime());
	fScheduler->ScheduleTickTask(fClockTask, 0);
}

/*--------------------------------------------------------------------------*/

void TClockSender::Stop() 
{ 
	fClockTask->Forget();
	fEventUser->SendEvent(MidiNewEv(typeStop),MidiGetTime());
}

/*--------------------------------------------------------------------------*/
// The date is always an exact multiple of a clock value 
/*--------------------------------------------------------------------------*/

void TClockSender::Cont(ULONG date_ticks)
{
	fEventUser->SendEvent(MidiNewEv(typeContinue),MidiGetTime());
	fClockCount = fClockConverter->ConvertTickToClock(date_ticks);
	fScheduler->ScheduleTickTask(fClockTask, date_ticks);
}

/*--------------------------------------------------------------------------*/
/* Internal functions
/*--------------------------------------------------------------------------*/

void TClockSender::NextClock (ULONG date_ms) 
{
	fClockCount++;
	fEventUser->SendEvent(MidiNewEv(typeClock),date_ms);
	fScheduler->ScheduleTickTaskInt(fClockTask, fClockConverter->ConvertClockToTick(fClockCount));
}

/*--------------------------------------------------------------------------*/

void TClockSender::SendSongPos(ULONG date_ticks)
{
	if (MidiEvPtr e = MidiNewEv(typeSongPos)) {
		ULONG pos = fClockConverter->ConvertTickToSongPos(date_ticks);
		MidiSetField(e,0,pos & 0x7F);
		MidiSetField(e,1,(pos>>7) & 0x7F);
		fEventUser->SendEvent(e,MidiGetTime());     
	}
}

/*--------------------------------------------------------------------------*/

ULONG TClockSender::GetPosTicks() 
{
	return (fClockCount == 0) ? 0 : fClockConverter->ConvertClockToTick(fClockCount - 1);
}