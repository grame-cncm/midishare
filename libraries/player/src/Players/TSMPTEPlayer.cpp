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
//	TSMPTEPlayer.cpp	   			 
// ===========================================================================
//
//	Player in kSMPTESync mode
// 

#include "TSMPTEPlayer.h"
#include "UDebug.h"

/*----------------------------------------------------------------------------*/
	  	
TSMPTEPlayer::TSMPTEPlayer (TGenericPlayerInterfacePtr player, TRunningStatePtr state, TSMPTEInfosPtr smpte,TMidiApplPtr appl)
{ 
	fPlayer = player;
	fRunningState = state;
	fMidiAppl = appl;
	fSmpteInfos = smpte;
	fSMPTEtask = new TSMPTETask(this);
}

/*----------------------------------------------------------------------------*/

TSMPTEPlayer::~TSMPTEPlayer (){ delete(fSMPTEtask);}

/*----------------------------------------------------------------------------*/

void TSMPTEPlayer::Start() 
{ 
	Stop();
		
	if (fSmpteInfos->IsSMPTELocked ()){
		StartAtSMPTEOffset();
	}else{
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
	if(fRunningState->IsIdle() || fRunningState->IsPause()){
		if (fSmpteInfos->IsSMPTELocked ()){
			StartAtSMPTEOffset();
		}else{
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

void TSMPTEPlayer::SetPosTicks (ULONG date_ticks) 
{ 
	
	if (fRunningState->IsRunning()){
		fPlayer->Stop();
		fPlayer->SetPosTicks(date_ticks);
		fPlayer->Cont();
	}else{
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
	fSMPTEtask->Forget();
	
	if (curexttime < fSmpteInfos->GetSMPTEOffset()){
		fMidiAppl->ScheduleTask(fSMPTEtask, MidiGetTime() + fSmpteInfos->GetSMPTEOffset() - curexttime);
	}else  if (curexttime == fSmpteInfos->GetSMPTEOffset()){
		fPlayer->Start();
	}else {
		fPlayer->SetPosMs(curexttime - fSmpteInfos->GetSMPTEOffset());
		fPlayer->Cont();
	}
}

/*----------------------------------------------------------------------------*/

ULONG TSMPTEPlayer::GetTempo() { return fPlayer->GetTempo();}

/*----------------------------------------------------------------------------*/

void TSMPTEPlayer::ReceiveEvents (MidiEvPtr e) { fPlayer->ReceiveEvents(e);}

/*----------------------------------------------------------------------------*/

void TSMPTEPlayer::SetPosBBU (const TPos& pos) {fPlayer->SetPosBBU(pos);}
	
/*----------------------------------------------------------------------------*/

void TSMPTEPlayer::SetPosMs (ULONG date_ms) {fPlayer->SetPosMs(date_ms);}
	
/*----------------------------------------------------------------------------*/

void TSMPTEPlayer::SetTempoFactor (float factor) {fPlayer->SetTempoFactor(factor);}
