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
	  	
TSyncOutPlayer::TSyncOutPlayer (TPlayerInterfacePtr player, TClockSenderPtr clock,TClockConverterPtr converter)
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
	
	ULONG new_date_ticks;
	
	if (fNeedSongPos){
		new_date_ticks = fClockConverter->ConvertTickToTickAtPrevSP(fPlayer->GetPosTicks());
	}else {
		new_date_ticks = fClockConverter->ConvertTickToTickAtPrevClock(fPlayer->GetPosTicks());
	}
		
	fPlayer->SetPosTicks (new_date_ticks);
	fPlayer->Cont();
	fClocksender->Cont(new_date_ticks);
	fNeedSongPos = false;
}


/*----------------------------------------------------------------------------*/

void TSyncOutPlayer::PlaySliceForward()  
{ 
	fPlayer->PlaySliceForward();
	ULONG new_date_ticks = fClockConverter->ConvertTickToTickAtPrevSP(fPlayer->GetPosTicks());
	fClocksender->SendSongPos(new_date_ticks);
	fNeedSongPos = true;
}

/*----------------------------------------------------------------------------*/

void TSyncOutPlayer::PlaySliceBackward() 
{ 
	fPlayer->PlaySliceBackward();
	ULONG new_date_ticks = fClockConverter->ConvertTickToTickAtPrevSP(fPlayer->GetPosTicks());
	fClocksender->SendSongPos(new_date_ticks); 
	fNeedSongPos = true;
}

/*----------------------------------------------------------------------------*/

void TSyncOutPlayer::SetPosTicks (ULONG date_ticks) 
{ 
	// Set the pos on the previous SongPos
	
	ULONG new_date_ticks = fClockConverter->ConvertTickToTickAtPrevSP(date_ticks);
	fPlayer->SetPosTicks(new_date_ticks);
	fClocksender->SendSongPos(new_date_ticks);
	fNeedSongPos = false;
}

/*----------------------------------------------------------------------------*/

ULONG TSyncOutPlayer::GetPosTicks() { return fPlayer->GetPosTicks();}

/*----------------------------------------------------------------------------*/

void TSyncOutPlayer::SetTempo(ULONG tempo) { fPlayer->SetTempo(tempo); }

/*----------------------------------------------------------------------------*/

void TSyncOutPlayer::RcvClock(ULONG date_ms) { fPlayer->RcvClock(date_ms);}

/*----------------------------------------------------------------------------*/

ULONG TSyncOutPlayer::GetTempo() { return fPlayer->GetTempo();}

/*----------------------------------------------------------------------------*/

void TSyncOutPlayer::SetTempoFactor(float factor) { fPlayer->SetTempoFactor(factor); }
