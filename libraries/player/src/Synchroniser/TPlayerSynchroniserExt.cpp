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
//	TPlayerSynchroniserExt.h		    
// ===========================================================================
//
//	Synchronizer for kExternalSync mode: 
//


#include "TPlayerSynchroniser.h"
#include "UTools.h"
#include "UDebug.h"

//-----------------------------
// Class TPlayerSynchroniserExt 
//-----------------------------

void TPlayerSynchroniserExt::Start ()
{ 
	fTempoVisitor->Init();
	fOffset = MidiGetTime();
	fState->SetRunning();
}	

/*--------------------------------------------------------------------------*/

void TPlayerSynchroniserExt::Stop () 
{ 
	if (fState->IsRunning()) fTempoVisitor->UpdateMs(MidiGetTime() - fOffset);
	fState->SetIdle();
}

/*--------------------------------------------------------------------------*/

void TPlayerSynchroniserExt::Cont (ULONG date_ticks) 
{ 
	ULONG tempo = fTempoVisitor->GetTempo();
	fTempoVisitor->Init(); // Important  (Tempo Map must be re-initialized)
	fTempoVisitor->SetTempo(date_ticks,tempo);

	SetPosTicks(date_ticks);
	fOffset = MidiGetTime() - fTempoVisitor->CurDateMs();
	fState->SetRunning(); // ordre ??
}

/*--------------------------------------------------------------------------*/

Boolean TPlayerSynchroniserExt::IsSchedulable (ULONG date_ticks) {return fState->IsRunning();}

/*--------------------------------------------------------------------------*/

ULONG TPlayerSynchroniserExt::GetPosTicks () 
{
	if (fState->IsRunning()) fTempoVisitor->UpdateMs(MidiGetTime() - fOffset);
	return fTempoVisitor->CurDateTicks();
}

/*--------------------------------------------------------------------------*/

void  TPlayerSynchroniserExt::SetPosTicks (ULONG date_ticks){ fTempoVisitor->UpdateTicks(date_ticks);}

/*--------------------------------------------------------------------------*/

void TPlayerSynchroniserExt::SetTempo(ULONG  tempo)
{
	if (tempo != fTempoVisitor->GetTempo()) {
		fTempoVisitor->SetTempo(GetPosTicks(),tempo);
		fScheduler->ReScheduleTasks(); 
	}
}

