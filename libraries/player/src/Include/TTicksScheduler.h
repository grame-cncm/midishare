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
//	TTickScheduler.h		  
// ===========================================================================

#ifndef __TTickScheduler__
#define __TTickScheduler__

#include "TScheduler.h"
#include "TSynchroniserInterface.h"
#include "TMidiAppl.h"

//----------------------
// Class TTickScheduler 
//----------------------
/*!
	\brief  Combines a scheduler and a synchroniser.
*/

class TTicksScheduler  :public TSynchroniserInterface, public TSchedulerInterface{

	private:
	
		TSchedulerInterfacePtr 		fScheduler;
		TSynchroniserInterfacePtr	fSynchroniser;
		
	public :
	
 		TTicksScheduler(TSchedulerInterfacePtr sch, TSynchroniserInterfacePtr syn);
 		virtual ~TTicksScheduler();
 		
		ULONG GetTempo() {return fSynchro->GetTempo();}
		
		// non precise (pour les cas ou la date en ticks n'est pas connue)
		void SetTempo (ULONG tempo) {fSynchroniser->SetTempo(fSynchroniser->GetPosTicks(), tempo);}
		void SetTempo (ULONG  date_ticks, ULONG tempo) {fSynchroniser->SetTempo(date_ticks, tempo);}
		
		ULONG GetPosTicks () { return fSynchroniser->GetPosTicks();}
		
		void RcvClock(ULONG date_ms) { fSynchroniser->RcvClock();}
		
		void ScheduleTickTask(TTicksTask* task, ULONG date_ticks) {fScheduler->ScheduleTickTask(task,date_ticks);}
 		void ScheduleTickTaskInt(TTicksTask* task, ULONG date_ticks) {fScheduler->ScheduleTickTaskInt(task,date_ticks);}
};

typedef TTicksScheduler FAR * TTicksSchedulerPtr;

#endif
