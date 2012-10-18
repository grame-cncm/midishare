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
//	TPlayerSynchroniserExt.h		    
// ===========================================================================
//
//	Synchronizer for kExternalSync mode: 
//

#include "TPlayerSynchroniser.h"
#include "UTools.h"
#include "UDebug.h"

/*--------------------------------------------------------------------------*/

void TPlayerSynchroniserExt::Init() {fTempoVisitor.Init();}

/*--------------------------------------------------------------------------*/

void TPlayerSynchroniserExt::Start()
{ 
	fOffset = MidiGetTime();
	fState->SetRunning();
}	

/*--------------------------------------------------------------------------*/

void TPlayerSynchroniserExt::Stop() 
{ 
	if (fState->IsRunning()) fTempoVisitor.UpdateMs(MidiGetTime() - fOffset);
	fState->SetIdle();
}

/*--------------------------------------------------------------------------*/

void TPlayerSynchroniserExt::Cont(ULONG date_ticks) 
{ 
	ULONG tempo = fTempoVisitor.GetTempo();
	fTempoVisitor.Init(); // Important  (Tempo Map must be re-initialized)
	fTempoVisitor.SetTempo(date_ticks,tempo);

	SetPosTicks(date_ticks);
	fOffset = MidiGetTime() - fTempoVisitor.CurDateMs();
	fState->SetRunning(); // ordre ??
}

/*--------------------------------------------------------------------------*/

Boolean TPlayerSynchroniserExt::IsSchedulable (ULONG date_ticks) {return fState->IsRunning();}

/*--------------------------------------------------------------------------*/

ULONG TPlayerSynchroniserExt::GetPosTicks() 
{
	if (fState->IsRunning()) fTempoVisitor.UpdateMs(MidiGetTime() - fOffset);
	return fTempoVisitor.CurDateTicks();
}

/*--------------------------------------------------------------------------*/

void  TPlayerSynchroniserExt::SetPosTicks(ULONG date_ticks) {fTempoVisitor.UpdateTicks(date_ticks);}

/*--------------------------------------------------------------------------*/

void TPlayerSynchroniserExt::SetTempo(ULONG  tempo)
{
	if (tempo != fTempoVisitor.GetTempo()) {
		fTempoVisitor.SetTempo(GetPosTicks(),tempo);
		fScheduler->ReScheduleTasks(); 
	}
}

