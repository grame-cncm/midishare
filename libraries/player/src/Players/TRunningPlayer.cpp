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

void TRunningPlayer::SetPosTicks(ULONG date_ticks) 
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

ULONG TRunningPlayer::GetTempo() {return fPlayer->GetTempo();}
