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
//	TPlayerScore.h			    
// ===========================================================================

/*!
 \brief The internal score : a TPlayerScore contains at least one TTempo event, one 
 TTimeSign event and one EndScore event.
 */


#ifndef __TPlayerScore__
#define __TPlayerScore__

#include "TScore.h"
#include "TPlayerConstants.h"
#include "TScoreIterator.h"

//-----------------------
// Class TPlayerScore 
//-----------------------
 
 class TPlayerScore :public TScore{

	private:
		
		TEventPtr fScoreEnd;
		TEventPtr fScoreBegin;
		
		void Init();
		
		void AdjustEndDate(ULONG date); 
		void ClearTrack (short tracknum);
		void CheckAndAllocateMemory();
		Boolean CheckTracknum(short tracknum) { return ((tracknum >= 0)  && (tracknum < kMaxTrack));}
		
		short InsertEventsAtEnd(MidiEvPtr e1);
		short InsertEventsAtEnd(MidiEvPtr e1, short tracknum);
		short InsertEventsIn(TScoreIteratorPtr it,MidiEvPtr* e1);
		short InsertEventsIn(TScoreIteratorPtr it,MidiEvPtr* e1, short tracknum);
		
	public :
	
		TPlayerScore();
		~TPlayerScore(){}
		
		TEventPtr FirstEvent(); 
		TEventPtr LastEvent(); 
		
		void InsertAtBeginEvent(TEventPtr ev);
		void InsertAtEndEvent(TEventPtr ev);
		void RemoveEvent(TEventPtr ev);
		
		void InsertAfterEvent(TEventPtr ev1,TEventPtr ev2);
		void InsertBeforeEvent(TEventPtr ev1,TEventPtr ev2); 
	
		long SetAllTrack(MidiSeqPtr s);
		long SetTrack( short tracknum,MidiSeqPtr s);
		
		MidiSeqPtr GetAllTrack();
		MidiSeqPtr GetTrack(short tracknum);
	
		long InsertAllTrack(TEventPtr cur,MidiSeqPtr s);
		long InsertTrack(TEventPtr cur,short tracknum,MidiSeqPtr s);
		
		ULONG GetLastDate();
};

typedef class TPlayerScore  FAR * TPlayerScorePtr;


#define IsScore(t)  (((EvType(t))<typePrivate || (EvType(t))>typeDProcess) && (EvType(t))!=typeEndTrack)


#endif
