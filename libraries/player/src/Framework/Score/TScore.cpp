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
//	TScore.cpp			    
// ===========================================================================

#include "TScore.h"
#include "TScoreIterator.h"
#include "UDebug.h"
#include "UTools.h"

/*----------------------------------------------------------------------------*/

void  TScore::AttachIterator(TScoreIteratorPtr it)
{
	it->SetNextIterator(fHeadtIt);
	fHeadtIt = it;
}

/*----------------------------------------------------------------------------*/

void  TScore::DetachIterator(TScoreIteratorPtr it)
{
	TScoreIteratorPtr currIter = fHeadtIt;
	TScoreIteratorPtr prevIter = 0;
	
	while (currIter) {		// Search for the specified Iterator
		if (currIter == it) {
									// Found it. Adjust linked list.
			if (prevIter) {
				prevIter->SetNextIterator(currIter->GetNextIterator());
			} else {
				fHeadtIt = currIter->GetNextIterator();
			}
			break;
			
		} else {					// Continue search
			prevIter = currIter;
			currIter = currIter->GetNextIterator();
		}
	}
}

/*----------------------------------------------------------------------------*/

void  TScore::DetachIterators()
{
	TScoreIteratorPtr it = fHeadtIt;
	while (it) {
		it->Detach();
		it = it->GetNextIterator();
	}
}

/*----------------------------------------------------------------------------*/

void  TScore::AttachIterators()
{
	TScoreIteratorPtr it = fHeadtIt;
	while (it) {
		it->Init();
		it = it->GetNextIterator();
	}
}


/*----------------------------------------------------------------------------*/
//	Notify Iterators associated with an score that items have been inserted
/*----------------------------------------------------------------------------*/

void  TScore::ItemsInserted(TEventPtr ev)
{
	TScoreIteratorPtr it = fHeadtIt;
	assert(ev);
	while (it) {
		it->ItemsInserted(ev);
		it = it->GetNextIterator();
	}
}

/*----------------------------------------------------------------------------*/
//	Notify Iterators associated with an score that items have been removed
/*----------------------------------------------------------------------------*/

void  TScore::ItemsRemoved(TEventPtr ev)
{
	TScoreIteratorPtr it = fHeadtIt;
	assert(ev);
	while (it) {
		it->ItemsRemoved(ev);
		it = it->GetNextIterator();
	}
}

/*----------------------------------------------------------------------------*/
TEventPtr TScore::FirstEvent()
{ 
	return fLast ? fLast->fNext : 0;

}
/*----------------------------------------------------------------------------*/
TEventPtr TScore::LastEvent()
{ 
	return fLast;
} 
/*----------------------------------------------------------------------------*/
void TScore::Clear () 
{
	TEventPtr cur;
	TScoreIterator1 it(this);
	while ((cur = it.NextEv())) {RemoveEvent(cur); delete cur;}
	fLast = 0;
}

/*----------------------------------------------------------------------------*/
// insert at the begining of the score

void TScore::InsertAtBeginEvent(TEventPtr ev) 
{
	assert(ev);
	
	if (fLast) { 	// If one element at least
		TEventPtr first = fLast->fNext;
		ev->fNext = first;
		ev->fPrev = fLast;
		first->fPrev = ev;
		fLast->fNext = ev;
		
	}else {     	// Empty score
		ev->fNext = ev;
		ev->fPrev = ev;
		fLast = ev;
	}
}

/*----------------------------------------------------------------------------*/
// insert at the end of the score

void TScore::InsertAtEndEvent(TEventPtr ev) 
{
	assert(ev);
	
	if (fLast) { 	// If one element at least
		TEventPtr first = fLast->fNext;
		ev->fNext = first;
		ev->fPrev = fLast;
		first->fPrev = ev;
		fLast->fNext = ev;
		fLast = ev;
	}else {     	// Empty score
		ev->fNext = ev;
		ev->fPrev = ev;
		fLast = ev;
	}
	
}

/*----------------------------------------------------------------------------*/
// insert ev2 after ev1, update fLast if necessary

void TScore::InsertAfterEvent(TEventPtr ev1, TEventPtr ev2) {

	assert(ev1);
	assert(ev2);
	
	if (ev1 == fLast){
		InsertAtEndEvent(ev2);
	}else{
		ev2->fNext = ev1->fNext;
		ev2->fPrev = ev1;
		ev1->fNext->fPrev = ev2;
		ev1->fNext = ev2;
	}
}

/*----------------------------------------------------------------------------*/
//  insert ev2 before ev1

void TScore::InsertBeforeEvent(TEventPtr ev1, TEventPtr ev2) 
{
	assert(ev1);
	assert(ev2);
	
	ev2->fNext = ev1;
	ev2->fPrev = ev1->fPrev;
	ev1->fPrev->fNext = ev2; // attention ˆ l'ordre!!
	ev1->fPrev = ev2;
}

/*----------------------------------------------------------------------------*/

void TScore::RemoveEvent(TEventPtr ev) 
{
	assert(ev);
	
	if (ev->fPrev && ev->fNext) { // if the event is insered 
	
		ItemsRemoved(ev); // notify the Iterators
		
		if (ev == fLast) {        // if last
			if (fLast->fNext == fLast) {  // only 1 element
				ev->fPrev = 0;
				ev->fNext = 0;
				fLast = 0;
				return;
			}else {
				fLast = fLast->fPrev;
			}
		}
		
		ev->fNext->fPrev = ev->fPrev;
		ev->fPrev->fNext = ev->fNext;
		ev->fPrev = 0;
		ev->fNext = 0;
	}
}

/*----------------------------------------------------------------------------*/

ULONG TScore::Length() {
	TScoreIterator it(this); 
	ULONG count = 0;
	
	while (it.NextEv()) {count ++;}
	return count;
}
