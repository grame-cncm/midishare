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
//	TSyncInPlayer.cpp	   			 
// ===========================================================================
//
//	A TSyncInPlayer is the basic synchronisable player.
//
//  A TSyncInPlayer uses three components:
//  - a tickplayer : plays the score where dates are in ticks
// 	- a synchroniser : provides ticks ==> millisecond conversion
//  - a chaser : maintain the "event to chase" state and send events when necessary (start, stop)
//
//  Changing the synchronizer object allows to change the synchronization behaviour of the TSyncInPlayer.
//  


#include "TSyncInPlayer.h"
#include "UDebug.h"

/*--------------------------------------------------------------------------*/

TSyncInPlayer::TSyncInPlayer ( 	TPlayerSynchroniserPtr synchroniser,
								TTickPlayerInterfacePtr player,
								TChaserInterfacePtr chaser)
{
	fSynchroniser = synchroniser;
	fTickPlayer = player;
	fChaser = chaser; 
}

/*--------------------------------------------------------------------------*/

void TSyncInPlayer::Start()
{
	fChaser->Init();
	fSynchroniser->Start();
	fTickPlayer->Start();
}

/*--------------------------------------------------------------------------*/

void TSyncInPlayer::Stop()
{
	fSynchroniser->Stop();
	fTickPlayer->Stop();
	// Chase when Stop
	fChaser->ChaseOff(fTickPlayer->GetPosTicks());
}

/*--------------------------------------------------------------------------*/

void TSyncInPlayer::Pause()
{
	fSynchroniser->Stop();
	fTickPlayer->Stop();
	// Don't chase when Pause
}

/*--------------------------------------------------------------------------*/

void TSyncInPlayer::Cont()
{
	ULONG date_ticks = fSynchroniser->GetPosTicks();
	fChaser->ChaseOn(date_ticks);
	fSynchroniser->Cont(date_ticks);
	fTickPlayer->Cont(date_ticks);
}

/*----------------------------------------------------------------------------*/

void TSyncInPlayer::SetPosTicks (ULONG date_ticks)
{
	fSynchroniser->SetPosTicks(date_ticks);
	fTickPlayer->SetPosTicks(date_ticks);
} 

/*----------------------------------------------------------------------------*/

void  TSyncInPlayer::PlaySliceForward()
{
	// Move a slice forward
	fTickPlayer->PlaySliceForward();
	// Update the synchronizer tick pos
	fSynchroniser->SetPosTicks(fTickPlayer->GetPosTicks());
}

/*----------------------------------------------------------------------------*/

void  TSyncInPlayer::PlaySliceBackward()
{
	// Move a slice backward
	fTickPlayer->PlaySliceBackward();
	// Update the synchronizer tick pos
	fSynchroniser->SetPosTicks(fTickPlayer->GetPosTicks());
}

/*----------------------------------------------------------------------------*/

ULONG TSyncInPlayer::GetPosTicks() { return fSynchroniser->GetPosTicks();}

/*----------------------------------------------------------------------------*/

void TSyncInPlayer::SetTempo(ULONG tempo){ fSynchroniser->SetTempo(tempo); }

/*----------------------------------------------------------------------------*/

void TSyncInPlayer::RcvClock (ULONG date_ms){ fSynchroniser->RcvClock(date_ms); }

/*----------------------------------------------------------------------------*/

ULONG TSyncInPlayer::GetTempo (){ return fSynchroniser->GetTempo(); }

