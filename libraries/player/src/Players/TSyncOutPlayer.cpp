/*

  Copyright � Grame 1996-2004

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
//	TSyncOutPlayer.cpp	   			 
// ===========================================================================
//
//	A TSyncOutPlayer object send Clock,Start,Stop,Continue and SongPos events
//  
//  A TSyncOutPlayer use the Decorator pattern: additionnal behaviours are added 
//  dynamically on the TPlayerInterface object being decorated.


#include "TSyncOutPlayer.h"
#include "UDebug.h"

/*----------------------------------------------------------------------------*/
	  	
TSyncOutPlayer::TSyncOutPlayer(TPlayerInterfacePtr player, TClockSenderPtr clock, TClockConverterPtr converter)
{ 
	fPlayer = player;
	fClocksender = clock;
	fClockConverter = converter;
	fNeedSongPos = false;
}

/*----------------------------------------------------------------------------*/

void TSyncOutPlayer::Start() 
{ 
	fPlayer->Start();
	fClocksender->Start();
}

/*----------------------------------------------------------------------------*/

void TSyncOutPlayer::Stop()
{
	fClocksender->Stop();
	fPlayer->Stop();
	fPlayer->SetPosTicks(fClocksender->GetPosTicks()); 
}

/*----------------------------------------------------------------------------*/

void TSyncOutPlayer::Pause()
{
	fClocksender->Stop();
	fPlayer->Pause();
	fPlayer->SetPosTicks(fClocksender->GetPosTicks()); 
}

/*----------------------------------------------------------------------------*/

void TSyncOutPlayer::Cont() 
{
	// Set the pos on the previous Clock or SongPos
	
	ULONG new_date_ticks = (fNeedSongPos) 
		? (ULONG) fClockConverter->ConvertTickToTickAtPrevSP((float)fPlayer->GetPosTicks())
		: (ULONG) fClockConverter->ConvertTickToTickAtPrevClock((float)fPlayer->GetPosTicks());
	
	fPlayer->SetPosTicks(new_date_ticks);
	fPlayer->Cont();
	fClocksender->Cont(new_date_ticks);
	fNeedSongPos = false;
}


/*----------------------------------------------------------------------------*/

void TSyncOutPlayer::PlaySliceForward()  
{ 
	fPlayer->PlaySliceForward();
	ULONG new_date_ticks = (ULONG)fClockConverter->ConvertTickToTickAtPrevSP((float)fPlayer->GetPosTicks());
	fClocksender->SendSongPos(new_date_ticks);
	fNeedSongPos = true;
}

/*----------------------------------------------------------------------------*/

void TSyncOutPlayer::PlaySliceBackward() 
{ 
	fPlayer->PlaySliceBackward();
	ULONG new_date_ticks = (ULONG)fClockConverter->ConvertTickToTickAtPrevSP((float)fPlayer->GetPosTicks());
	fClocksender->SendSongPos(new_date_ticks); 
	fNeedSongPos = true;
}

/*----------------------------------------------------------------------------*/

void TSyncOutPlayer::SetPosTicks(ULONG date_ticks) 
{ 
	// Set the pos on the previous SongPos
	
	ULONG new_date_ticks = (ULONG)fClockConverter->ConvertTickToTickAtPrevSP((float)date_ticks);
	fPlayer->SetPosTicks(new_date_ticks);
	fClocksender->SendSongPos(new_date_ticks);
	fNeedSongPos = false;
}

/*----------------------------------------------------------------------------*/

ULONG TSyncOutPlayer::GetPosTicks() {return fPlayer->GetPosTicks();}

/*----------------------------------------------------------------------------*/

void TSyncOutPlayer::SetTempo(ULONG tempo) {fPlayer->SetTempo(tempo);}

/*----------------------------------------------------------------------------*/

void TSyncOutPlayer::RcvClock(ULONG date_ms) {fPlayer->RcvClock(date_ms);}

/*----------------------------------------------------------------------------*/

ULONG TSyncOutPlayer::GetTempo() {return fPlayer->GetTempo();}
