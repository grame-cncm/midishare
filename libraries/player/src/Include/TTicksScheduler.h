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
//	TTickScheduler.cpp			   ©1996-1998 Grame All rights reserved.
// ===========================================================================
//
// Combines a scheduler and a synchroniser

#ifndef __TTickScheduler__
#define __TTickScheduler__


#include "TScheduler.h"
#include "TSynchroniserInterface.h"
#include "TMidiAppl.h"

//-----------------------
// Class TTickScheduler 
//-----------------------

class TTicksScheduler  :public TSynchroniserInterfacePtr, public TSchedulerInterfacePtr{

	private:
	
		TSchedulerInterfacePtr 		fScheduler;
		TSynchroniserInterfacePtr	fSynchroniser;
		
	public :
	
 		TTicksScheduler(TSchedulerInterfacePtr sch, TSynchroniserInterfacePtr syn);
 		virtual ~TTicksScheduler();
 		
		ULONG GetTempo(){return fSynchro->GetTempo();}
		
		// non precise (pour les cas ou la date en ticks n'est pas connue)
		void SetTempo (ULONG tempo){fSynchroniser->SetTempo(fSynchroniser->GetPosTicks(), tempo);}
		void SetTempo (ULONG  date_ticks, ULONG tempo){  fSynchroniser->SetTempo(date_ticks, tempo);}
		
		ULONG GetPosTicks (){ return fSynchroniser->GetPosTicks();}
		
		void RcvClock(ULONG date_ms){ fSynchroniser->RcvClock();}
		
		void ScheduleTickTask(TTicksTask* task, ULONG date_ticks) {	fScheduler->ScheduleTickTask(task,date_ticks);}
 		void ScheduleTickTaskInt(TTicksTask* task, ULONG date_ticks){	fScheduler->ScheduleTickTaskInt(task,date_ticks);}
 		
};


typedef TTicksScheduler FAR * TTicksSchedulerPtr;


#endif
