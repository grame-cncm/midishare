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
//	TEventModifier.cpp			    
// ===========================================================================
//
//	Real-time transformation of incoming events : a TEventModifier object
//  implements both TEventDispatcher and TTickPlayerInterface interfaces
// 

#include "TEventModifier.h"
#include "TEventFactory.h"
#include "UTools.h"
#include "UDebug.h"

/*--------------------------------------------------------------------------*/

void TFun::InsertEvent( MidiEvPtr ev)
{
	MidiEvPtr e = MidiCopyEv(ev);
	
	if (e) {
		IncRefCount();
		Link(e) = fListEvent;
		fListEvent = e;
	}
}

/*--------------------------------------------------------------------------*/

MidiEvPtr TFun::RemoveEventAux( MidiEvPtr e)
{
	MidiEvPtr prev, cur;
	cur = prev = fListEvent;

	if (cur && MatchEvent2(cur,e)){  // Match on the first ev
		fListEvent = Link(cur);
		return cur;
	}
	
	while (cur && !MatchEvent2(cur,e)) {
		prev = cur;
		cur = Link (cur);
	}
	
	if (cur) Link(prev) = Link(cur);
	return cur;
}


/*--------------------------------------------------------------------------*/

MidiEvPtr TFun::RemoveEvent( MidiEvPtr e)
{
	MidiEvPtr key = RemoveEventAux(e);
	if (key) DecRefCount();
	return key;
}

/*----------------------------------------------------------------------------*/

TEventModifier::TEventModifier(TEventSenderInterfacePtr sender, TPlayerSynchroniserPtr synchro, 
TSchedulerInterfacePtr	scheduler, TRunningStatePtr state, TEventDispatcherPtr successor) 
	:TEventDispatcher(successor)
{
	fEventUser = sender;
	fSynchroniser = synchro;
	fScheduler = scheduler;
	fRunningState = state;
	
	// Initialisation 
	
	fScore = new TPlayerScore();
	fIterator = new TScoreIterator(fScore);
	fIterator1 = new TScoreIterator(fScore);
	fPlayTask = new TPlayTask1(this);	
	fChaser = new TChaserIterator(fScore, sender);
	
	fTransSeq = 0;
	fLast_task_ticks = 0;
	
	Init();
}

/*----------------------------------------------------------------------------*/

TEventModifier::~TEventModifier()
{
	// Warning : fScore must be desallocated the last 
	
	delete(fIterator1);
	delete(fIterator);
	delete(fPlayTask);
	delete(fChaser);
	delete(fScore);	
}
	
/*----------------------------------------------------------------------------*/

void TEventModifier::ReceiveEvents(MidiEvPtr e)
{
	MidiEvPtr e1;
		
	if (fRunningState->IsRunning()) {
	
		switch (EvType(e)) {
			
				case typeNote:
					if (isFunOn() && (e1 = UMidi::NoteToKeyOff(e))) {
						Modify(MidiCopyEv(e));   // keyOn
						Modify(e1);  			 // keyOff
					}
					break;
					
				case typeKeyOn:
				case typeKeyOff:
					if (isFunOn()) { Modify(MidiCopyEv(e));}
					break;
					
				case typeFunOn:
					InsertFun(e);
					break;
					
				case typeFunOff:
					RemoveFun(Generation(e));
					break;
				
				/*
				A REVOIR
				default:
					fEventUser->CopyAndUseEvent(e,Date(e));
					break;
				*/
					
		}
	}
	TEventDispatcher::ReceiveEvents(e);
}

/*--------------------------------------------------------------------------*/

void TEventModifier::InsertFun(MidiEvPtr e)  
{
	MidiEvPtr copy = MidiCopyEv(e);

	if (copy) {
		Date(copy) = fSynchroniser->ConvertMsToTick(Date(e));
		TFunPtr fun = new TFun(copy);
		fun->SetNext(fTransSeq);
		fTransSeq = fun;
	}
}

/*--------------------------------------------------------------------------*/

void TEventModifier::RemoveFun(ULONG generation)  
{
	TFunPtr prev = 0;
	TFunPtr cur = fTransSeq;
	
	while (cur) {
		if (cur->GetGeneration() == generation) {
		
			// No running KeyOn
			
			if(cur->GetRefCount() == 0) {   
			
				if(prev)								
					prev->SetNext( cur->GetNext());				
				else									
					fTransSeq =  cur->GetNext();	
					
				delete(cur);
				return;
				
			}else{
				cur->SetEvType(typeFunOff);
			}
		}
		prev = cur;
		cur = cur->GetNext();
	}
}

/*--------------------------------------------------------------------------*/
	
void TEventModifier::Init()
{
	MidiSeqPtr seq = MidiNewSeq();
	MidiEvPtr ev = MidiNewEv(typeEndTrack);
	TFunPtr next,cur = fTransSeq;
	
	// Put an empty seq with a last event with "infinite" date in the score
	
	if (seq && ev) {
		Date(ev) = MAXLONG;
		MidiAddSeq(seq,ev);
		fScore->SetAllTrack(seq); 
	}
	
	// Delete list of transformation
	
	while(cur) {
		next = cur->GetNext();
		delete(cur);
		cur = next;
	}
	
	// Initialisation 
	
	fIterator->Init();
	fIterator1->Init();
	fChaser->Init();
	
	fTransSeq = 0;
	fLast_task_ticks = 0;
}

/*--------------------------------------------------------------------------*/

void TEventModifier::Start(){ ScheduleTickTask(fIterator->CurDate()); }
	
/*--------------------------------------------------------------------------*/

void TEventModifier::Stop ()
{ 
	fPlayTask->Forget(); 
	
	// Init the chaser each time
	fChaser->Init();
	fChaser->ChaseOff(fIterator->CurDate());
}

/*--------------------------------------------------------------------------*/

void TEventModifier::Cont (ULONG date_ticks) 
{ 
	fChaser->Init();
	fChaser->ChaseOn(date_ticks);
	fIterator->SetPosTicks(date_ticks);
	ScheduleTickTask(fIterator->CurDate()); 
}

/*--------------------------------------------------------------------------*/

void TEventModifier::SetPosTicks (ULONG date_ticks) 
{ 
	// mettre ˆ jour les fonctions (integrer les ev privŽs dans le chaser)
	fIterator->SetPosTicks(date_ticks);
 }

/*--------------------------------------------------------------------------*/

ULONG TEventModifier::GetPosTicks (){ return fIterator->CurDate(); }

/*--------------------------------------------------------------------------*/

void TEventModifier::PlaySlice (ULONG date_ms) 
{
	if (fIterator->IsLastEv()) {
		fEventUser->CopyAndUseEvent(fIterator->CurEv(),date_ms);
	}else {
	
		// Keep the date of played events 
		fLast_task_ticks =  fIterator->CurDate();
	
		// For all events at the same date
		while (TEventPtr cur = fIterator->NextDateEv()) { fEventUser->CopyAndUseEvent(cur,date_ms); }
		
		// Schedule the PlayTask for the date of the next events in ticks
		ScheduleTickTask(fIterator->CurDate());
	}
}

/*--------------------------------------------------------------------------*/

MidiEvPtr  TEventModifier::HandleKeyOn(MidiEvPtr ev, TFunPtr fun)
{
	MidiEvPtr copy = MidiCopyEv(ev);
	if (!copy) return copy;
	
	Pitch(copy) = Range ((signed)Pitch(copy) + fun->GetPitch(),0,127);
	Vel(copy) = Range ((signed)Vel(copy) + fun->GetVel(),0,127);
	Chan(copy) = Range((signed)Chan(copy)+ fun->GetChan(), 0,15);
	Port(copy) = Range((signed)Port(copy)+ fun->GetPort(), 0,15);
	Date(copy) = ((fSynchroniser->ConvertMsToTick(Date(copy)) - fun->GetDate())
		 * fun->GetCoeff())/1000 + fun->GetOffset() + fun->GetDate();
	
	fun->InsertEvent(ev);
	return copy;
}


/*--------------------------------------------------------------------------*/

MidiEvPtr  TEventModifier::HandleKeyOff(MidiEvPtr ev, TFunPtr fun)
{
	MidiEvPtr key;
	
	if ((fun->GetRefCount() > 0) && (key = fun->RemoveEvent(ev)))  { // si c'est un ŽvŽnement transformŽ par cette fonction
		
		EvType(key) = typeKeyOff;
		Pitch(key) = Range ((signed)Pitch(key) + fun->GetPitch(),0,127);
		Chan(key) = Range((signed)Chan(key)+ fun->GetChan(), 0,15);
		Port(key) = Range((signed)Port(key)+ fun->GetPort(), 0,15);
		Date(key) = ((fSynchroniser->ConvertMsToTick(Date(ev)) - fun->GetDate()) 
			* fun->GetCoeff())/1000 + fun->GetOffset() + fun->GetDate() ;
		
		if (fun->isLastEvent()) { // si dernier : supprime la fonction 
			RemoveFun(fun->GetGeneration()); 
		}
	
		return key;
	}else {
		return 0;
	}
}

/*--------------------------------------------------------------------------*/
/*
Dans une interruption, les taches sont effectuŽes avant l'alarme de reception:

==> pour un tick donnŽ, la situation suivante peut se produire:

- jeu de events par la tache de jeu
- alarme de reception : insertion ˆ la mme date 
	jeu des events ˆ cette date ==> les events dŽja dans la sŽquence sont jouŽs 2 fois  
	
Solution : variable fLast_task_ticks qui contient la dernire date des ŽvŽnements 
dans la tache de jeu, si on insre ˆ la mme date, la date d'insertion est dŽcalŽe de 1.
	
/*--------------------------------------------------------------------------*/
	
void TEventModifier::Modify(MidiEvPtr ev)  
{
	if (!ev) return;
	
	TFunPtr next, fun = fTransSeq;
	MidiEvPtr transEv;
	TEventPtr curEv;
	ULONG min_date_ticks = MAXLONG;
	ULONG cur_date_ticks = fSynchroniser->ConvertMsToTick(Date(ev));
	
	// Force le refnum
	RefNum(ev) = trackNum;
	
	// Si des events ˆ cette date ont ŽtŽ jouŽs, dŽcale la date d'insertion de 1
	if (cur_date_ticks == fLast_task_ticks) cur_date_ticks++;
	
	while(fun) {
	
		next = fun->GetNext();
		transEv = 0;
		
		// Transformation
		switch (EvType(ev)) {
			
			case typeKeyOn:
				if (Vel(ev) == 0) {
					transEv = HandleKeyOff(ev, fun);
				}else{	
					if (fun->isStartFun())  
						transEv = HandleKeyOn(ev, fun);
				}
				break;
		
			case typeKeyOff:
				transEv = HandleKeyOff(ev, fun);
				break;
		}
	
		if (transEv) {
		 
		 	// Insere au minimum ˆ la date courante
			Date(transEv) = UTools::Max (cur_date_ticks, Date(transEv)); 
			
			// Insertion
			if (curEv = fIterator1->SetPosTicks(Date(transEv))) {
				fScore->InsertBeforeEvent(curEv, TEventFactory::GenericCreateEvent(transEv));
				min_date_ticks = UTools::Min (min_date_ticks, Date(transEv));
			}
		}
		
		fun = next;
	}
	
	if ((min_date_ticks <= fIterator->CurDate()) || fPlayTask->IsIdle ()) {
		fPlayTask->Forget();
		fIterator->SetPosTicks(min_date_ticks);
		ScheduleTickTask(fIterator->CurDate()); 
	}
	
	MidiFreeEv(ev);
}	

		
/*--------------------------------------------------------------------------*/
/* Don't Schedule a task with an infinite date */

void TEventModifier::ScheduleTickTask (ULONG date_ticks)  
{
	if (date_ticks < MAXLONG) fScheduler->ScheduleTickTask(fPlayTask, date_ticks);
}

