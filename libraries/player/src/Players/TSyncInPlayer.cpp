/*

  Copyright (C) Grame 1996-2012

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

TSyncInPlayer::TSyncInPlayer(TPlayerSynchroniserPtr synchroniser,
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
	// Initialisation 
	fSynchroniser->Init();
	fChaser->Init(); 
	fTickPlayer->Init();
	
	// Start
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

void TSyncInPlayer::SetPosTicks(ULONG date_ticks)
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

ULONG TSyncInPlayer::GetPosTicks() {return fSynchroniser->GetPosTicks();}

/*----------------------------------------------------------------------------*/

void TSyncInPlayer::SetTempo(ULONG tempo) {fSynchroniser->SetTempo(tempo);}

/*----------------------------------------------------------------------------*/

void TSyncInPlayer::RcvClock(ULONG date_ms) {fSynchroniser->RcvClock(date_ms);}

/*----------------------------------------------------------------------------*/

ULONG TSyncInPlayer::GetTempo() {return fSynchroniser->GetTempo();}

