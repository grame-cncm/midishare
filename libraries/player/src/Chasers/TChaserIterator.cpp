// ===========================================================================
// The Player Library is Copyright (c) Grame, Computer Music Research Laboratory 
// 1996-2003, and is distributed as Open Source software under the Artistic License;
// see the file "Artistic" that is included in the distribution for details.
//
// Grame : Computer Music Research Laboratory
// Web : http://www.grame.fr/Research
// E-mail : MidiShare@rd.grame.fr
// ===========================================================================

// ===========================================================================
//	TChaserIterator.cpp			    
// ===========================================================================
//
//	The chaser iterator use a chaser visitor to keep the state of KeyOn, CtrlChange, 
//  ProgChange, KeyPress, ChanPress, PitchBend, Sysex and Tune events when moving 
//  along the score.
//  The chaser iterator is used when starting or stopping the Player and will send 
//  events that occurred previously in the tracks.
//  


#include "TChaserIterator.h"

/*--------------------------------------------------------------------------*/

TChaserIterator::TChaserIterator(TScorePtr score, TEventSenderInterfacePtr user)
{
	fChaser = new TChaserVisitor(user);
	fIterator = new TScoreIterator(score);
}

/*--------------------------------------------------------------------------*/

TChaserIterator::~TChaserIterator()
{ 
	delete(fChaser);
	delete(fIterator);
}

/*--------------------------------------------------------------------------*/

void TChaserIterator::GoBeginDateTicks(ULONG date_ticks) 
{
	while (!fIterator->IsLastEv() && fIterator->CurDate() < date_ticks){ 
		fIterator->CurEv()->Accept(fChaser,true);
		fIterator->NextEv();
	}
}	

/*--------------------------------------------------------------------------*/

void TChaserIterator::Init()
{
	fChaser->Init();
	fIterator->Init();
	fCurdate_ticks = 0;
}

/*--------------------------------------------------------------------------*/

void TChaserIterator::SetPosTicks (ULONG date_ticks)
{
	if (date_ticks > fCurdate_ticks) {	
		GoBeginDateTicks(date_ticks);
	}else if (date_ticks < fCurdate_ticks){
		Init();
		GoBeginDateTicks(date_ticks);
	}
	fCurdate_ticks = date_ticks;
}

/*--------------------------------------------------------------------------*/

void TChaserIterator::ChaseOn (ULONG date_ticks)
{
	SetPosTicks(date_ticks);
	fChaser->ChaseOn(date_ticks);
}

/*--------------------------------------------------------------------------*/

void TChaserIterator::ChaseOff (ULONG date_ticks)
{
	SetPosTicks(date_ticks);
	fChaser->ChaseOff(date_ticks);
}


	
	