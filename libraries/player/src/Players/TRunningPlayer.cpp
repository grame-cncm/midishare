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
//	TRunningPlayer.cpp		    
// ===========================================================================
//
//  A TRunningPlayer use the Decorator pattern: additionnal behaviours are added 
//  dynamically on the TPlayerInterface object being decorated.
//
//  A TRunningPlayer calls the decorated player methods (Start, Stop, SetPosTicks)
//  depending of the current running state, read in the TRunningState object. 


#include "TRunningPlayer.h"
#include "UDebug.h"

/*----------------------------------------------------------------------------*/
	  	
TRunningPlayer::TRunningPlayer (TPlayerInterfacePtr player, TRunningStatePtr state)
{ 
	fPlayer = player;
	fRunningState = state;
}

/*----------------------------------------------------------------------------*/

void TRunningPlayer::Start() 
{ 
	Stop();
	fPlayer->Start();
}

/*----------------------------------------------------------------------------*/

void TRunningPlayer::Stop()
{
	if (fRunningState->IsRunning() 
		|| fRunningState->IsPause() 
		|| fRunningState->IsWaiting()) 
	{
		fPlayer->Stop();
	}
}

/*----------------------------------------------------------------------------*/

void TRunningPlayer::Pause()
{
	if (fRunningState->IsRunning() 
		|| fRunningState->IsIdle() 
		|| fRunningState->IsWaiting()) 
	{
		fPlayer->Pause();
		fRunningState->SetPause();
	}
}

/*----------------------------------------------------------------------------*/

void TRunningPlayer::Cont() 
{
	if(fRunningState->IsIdle() || fRunningState->IsPause()) {fPlayer->Cont();}
}

/*----------------------------------------------------------------------------*/

void TRunningPlayer::PlaySliceForward()  
{ 
	if(fRunningState->IsIdle()) fPlayer->PlaySliceForward(); 
}

/*----------------------------------------------------------------------------*/

void TRunningPlayer::PlaySliceBackward() 
{ 
	if(fRunningState->IsIdle()) fPlayer->PlaySliceBackward(); 
}

/*----------------------------------------------------------------------------*/

void TRunningPlayer::SetPosTicks (ULONG date_ticks) 
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

ULONG TRunningPlayer::GetPosTicks() {return fPlayer->GetPosTicks();}

/*----------------------------------------------------------------------------*/

void TRunningPlayer::SetTempo(ULONG tempo) {fPlayer->SetTempo(tempo); }

/*----------------------------------------------------------------------------*/

void TRunningPlayer::RcvClock(ULONG date_ms) {fPlayer->RcvClock(date_ms);}

/*----------------------------------------------------------------------------*/

ULONG TRunningPlayer::GetTempo() {  return fPlayer->GetTempo();}