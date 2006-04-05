/*

  Copyright © Grame 1996-2006

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
//	TSMPTEPlayer.cpp	   			 
// ===========================================================================
//
//	Player in kSMPTESync mode
// 

#include "TSMPTEPlayer.h"
#include "UDebug.h"

/*----------------------------------------------------------------------------*/

void TSMPTEPlayer::Start() 
{ 
	Stop();
		
	if (fSmpteInfos->IsSMPTELocked ()) {
		StartAtSMPTEOffset();
	} else {
		fRunningState->SetWaiting();
	}
}

/*----------------------------------------------------------------------------*/

void TSMPTEPlayer::Stop()
{
	if (fRunningState->IsRunning() 
		|| fRunningState->IsPause() 
		|| fRunningState->IsWaiting()) 
	{
		fPlayer->Stop();
	}
}

/*----------------------------------------------------------------------------*/

void TSMPTEPlayer::Pause()
{
	if (fRunningState->IsRunning()) {
		fPlayer->Pause();
		fRunningState->SetPause();
	}
}

/*----------------------------------------------------------------------------*/

void TSMPTEPlayer::Cont() 
{
	if(fRunningState->IsIdle() || fRunningState->IsPause()) {
		if (fSmpteInfos->IsSMPTELocked ()) {
			StartAtSMPTEOffset();
		} else {
			fRunningState->SetWaiting();
		}
	}
}
/*----------------------------------------------------------------------------*/

void TSMPTEPlayer::PlaySliceForward()  
{ 
	if(fRunningState->IsRunning() || fRunningState->IsPause()) Stop();
	fPlayer->PlaySliceForward(); 
}

/*----------------------------------------------------------------------------*/

void TSMPTEPlayer::PlaySliceBackward() 
{ 
	if(fRunningState->IsRunning() || fRunningState->IsPause()) Stop();
	fPlayer->PlaySliceBackward(); 
}

/*----------------------------------------------------------------------------*/

void TSMPTEPlayer::SetPosTicks(ULONG date_ticks) 
{ 
 	if (fRunningState->IsRunning()) {
		fPlayer->Stop();
		fPlayer->SetPosTicks(date_ticks);
		fPlayer->Cont();
	} else {
		fPlayer->SetPosTicks(date_ticks);
	}
}

/*----------------------------------------------------------------------------*/

ULONG TSMPTEPlayer::GetPosTicks() {return fPlayer->GetPosTicks();}

/*----------------------------------------------------------------------------*/

void TSMPTEPlayer::SetTempo(ULONG tempo) {fPlayer->SetTempo(tempo); }

/*----------------------------------------------------------------------------*/
// Nothing to do
void TSMPTEPlayer::RcvClock(ULONG date_ms) {}

/*--------------------------------------------------------------------------*/

void TSMPTEPlayer::StartAtSMPTEOffset()  
{
	ULONG curexttime = MidiGetExtTime();
	fSMPTEtask.Forget();
	
	if (curexttime < fSmpteInfos->GetSMPTEOffset()) {
		fMidiAppl->ScheduleTask(&fSMPTEtask, MidiGetTime() + fSmpteInfos->GetSMPTEOffset() - curexttime);
	} else  if (curexttime == fSmpteInfos->GetSMPTEOffset()) {
		fPlayer->Start();
	} else {
		fPlayer->SetPosMs(curexttime - fSmpteInfos->GetSMPTEOffset());
		fPlayer->Cont();
	}
}

/*----------------------------------------------------------------------------*/

ULONG TSMPTEPlayer::GetTempo() {return fPlayer->GetTempo();}

/*----------------------------------------------------------------------------*/

void TSMPTEPlayer::ReceiveEvents(MidiEvPtr e) {fPlayer->ReceiveEvents(e);}

/*----------------------------------------------------------------------------*/

void TSMPTEPlayer::SetPosBBU(const TPos& pos) {fPlayer->SetPosBBU(pos);}
	
/*----------------------------------------------------------------------------*/

void TSMPTEPlayer::SetPosMs(ULONG date_ms) {fPlayer->SetPosMs(date_ms);}

/*----------------------------------------------------------------------------*/

void TSMPTETask::Execute(TMidiApplPtr appl, ULONG date) {fPlayer->Start();}

