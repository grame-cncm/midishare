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
//	TTickConsumer.cpp			    
// ===========================================================================
//
//	Consumer : use a TEventProducer object to produce events to be played
//

#include "TTickConsumer.h"


/*--------------------------------------------------------------------------*/

TTickConsumer::TTickConsumer(TEventSenderInterfacePtr user, TSchedulerInterfacePtr scheduler, TEventProducerInterfacePtr producer)
 {
	fPlayTask = new TPlayTask2(this);	
	fEventUser = user;
	fEventProducer = producer;
	fScheduler = scheduler;
}

/*--------------------------------------------------------------------------*/
		
TTickConsumer::~TTickConsumer() 
{
	delete(fPlayTask);
}

/*--------------------------------------------------------------------------*/

void TTickConsumer::PlaySliceForward () 
{
   /*nothing */
}

/*--------------------------------------------------------------------------*/

void TTickConsumer::PlaySliceBackward () 
{
	/*nothing */
}

/*--------------------------------------------------------------------------*/

void TTickConsumer::PlaySlice (ULONG date_ms) 
{
   MidiEvPtr cur;
   ULONG date_ticks = fEventProducer->GetPosTicks();
    
	// For all events at the same date
	while ((date_ticks ==  fEventProducer->GetPosTicks()) 
		&& (cur = fEventProducer->ProduceEvent(date_ticks))) {
		fEventUser->UseEvent(cur,date_ms);
	}
		
	// Schedule the PlayTask for the date of the next events in ticks, only when date < MAXLONG
	ScheduleTickTask(fEventProducer->GetPosTicks()); 
}

/*--------------------------------------------------------------------------*/

void TTickConsumer::Init () {fEventProducer->Init(0);}

/*--------------------------------------------------------------------------*/

void TTickConsumer::Start (){ ScheduleTickTask(fEventProducer->GetPosTicks());}

/*--------------------------------------------------------------------------*/

void TTickConsumer::Stop (){ fPlayTask->Forget(); }

/*--------------------------------------------------------------------------*/

void TTickConsumer::Cont (ULONG date_ticks) 
{ 
	fEventProducer->SetPosTicks(date_ticks);
	ScheduleTickTask(fEventProducer->GetPosTicks()); 
}

/*--------------------------------------------------------------------------*/

void TTickConsumer::SetPosTicks (ULONG date_ticks) { fEventProducer->SetPosTicks(date_ticks); }

/*--------------------------------------------------------------------------*/

ULONG TTickConsumer::GetPosTicks (){ return fEventProducer->GetPosTicks(); }

/*--------------------------------------------------------------------------*/
/* Don't Schedule a task with an infinite date */

void TTickConsumer::ScheduleTickTask (ULONG date_ticks)  
{
	if (date_ticks < MAXLONG) fScheduler->ScheduleTickTask(fPlayTask, date_ticks);
}
