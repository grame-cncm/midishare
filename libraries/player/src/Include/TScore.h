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
//	TScore.h			    
// ===========================================================================
//
// A double linked list of TEvent objects
//


#ifndef __TScore__
#define __TScore__


#include "TEvent.h"

//-----------------------
// Class TScore 
//-----------------------
 
class TScore {
	
	friend class TScoreIterator;

	protected:
	
		TEventPtr 		fLast;
		TScoreIterator* fHeadtIt;
		
		void ItemsInserted(TEventPtr ev);
		void ItemsRemoved(TEventPtr ev);
		
		void DetachIterators();
		void AttachIterators();
		
		void AttachIterator(TScoreIterator* it);
		void DetachIterator(TScoreIterator* it);
		
	public:
	
		TScore() {fLast = 0; fHeadtIt = 0;}
		virtual ~TScore() {  DetachIterators(); Clear ();}
		
		void Clear ();
		
		virtual TEventPtr FirstEvent(); 
		virtual TEventPtr LastEvent(); 
		
		virtual void InsertAtBeginEvent(TEventPtr ev);
		virtual void InsertAtEndEvent(TEventPtr ev);
		
		virtual void InsertAfterEvent(TEventPtr ev1,TEventPtr ev2);
		virtual void InsertBeforeEvent(TEventPtr ev1,TEventPtr ev2);
		
		virtual void RemoveEvent(TEventPtr ev);
		
		ULONG Length();
};

typedef class TScore  FAR * TScorePtr;


#endif
