/*

  Copyright © Grame 1996-2004

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
  research@grame.fr

*/


// ===========================================================================
//	TClockSender.cpp		    
// ===========================================================================
//
//	Manage Midi events related to Clock synchronization
//

#include "TClockSender.h"
#include "UTools.h"

/*--------------------------------------------------------------------------*/

void TClockSender::Start() 
{
	Stop();
	fClockCount = 0;
	fEventUser->SendEvent(MidiNewEv(typeStart),MidiGetTime());
	fScheduler->ScheduleTickTask(&fClockTask, 0);
}

/*--------------------------------------------------------------------------*/

void TClockSender::Stop() 
{ 
	fClockTask.Forget();
	fEventUser->SendEvent(MidiNewEv(typeStop),MidiGetTime());
}

/*--------------------------------------------------------------------------*/
// The date is always an exact multiple of a clock value 
/*--------------------------------------------------------------------------*/

void TClockSender::Cont(ULONG date_ticks)
{
	fEventUser->SendEvent(MidiNewEv(typeContinue),MidiGetTime());
	fClockCount = (ULONG)fClockConverter->ConvertTickToClock((float)date_ticks);
	fScheduler->ScheduleTickTask(&fClockTask, date_ticks);
}

/*--------------------------------------------------------------------------*/
// Internal functions
/*--------------------------------------------------------------------------*/

void TClockSender::NextClock (ULONG date_ms) 
{
	fClockCount++;
	fEventUser->SendEvent(MidiNewEv(typeClock),date_ms);
	fScheduler->ScheduleTickTask(&fClockTask, (ULONG)fClockConverter->ConvertClockToTick((float)fClockCount));
}

/*--------------------------------------------------------------------------*/

void TClockSender::SendSongPos(ULONG date_ticks)
{
	if (MidiEvPtr e = MidiNewEv(typeSongPos)) {
		ULONG pos = (ULONG)fClockConverter->ConvertTickToSongPos((float)date_ticks);
		MidiSetField(e,0,pos & 0x7F);
		MidiSetField(e,1,(pos>>7) & 0x7F);
		fEventUser->SendEvent(e,MidiGetTime());     
	}
}

/*--------------------------------------------------------------------------*/

ULONG TClockSender::GetPosTicks() 
{
	return (fClockCount == 0) ? 0 : (ULONG)fClockConverter->ConvertClockToTick((float)(fClockCount - 1));
}

/*--------------------------------------------------------------------------*/

void TClockTask::Execute (TMidiApplPtr appl, ULONG date_ms) {fSender->NextClock(date_ms);}
