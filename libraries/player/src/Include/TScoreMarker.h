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
//	TScoreMarker.h			    
// ===========================================================================
//
//	Markers are special Private events which can be inserted in the score.
// 
//  Markers are directly inserted in the score and MUST be removed before destroying the score

#ifndef __TMarker__
#define __TMarker__

#include "TEventFactory.h"
#include "TScore.h"
#include "UDebug.h"

//-----------------------
// Class TScoreMarker 
//-----------------------

class TScoreMarker {

	private:
	
		TEventPtr 	fEvent;
		
	public :
		
		TScoreMarker(Byte type)
		{
			fEvent = TEventFactory::GenericCreateEvent(MidiNewEv(type));
			if (fEvent) {
				fEvent->SetTrackNum(kTempoMap);
				fEvent->SetDate(0);
			}
		}
		
		// The marker MUST be removed from the score before destroying it.
		~TScoreMarker () { if (fEvent) delete (fEvent);}
		
		void Insert(TScorePtr score, TEventPtr prev, ULONG date_ticks)
		{ 
			Date(fEvent->MidiEvent()) = date_ticks;  
			score->InsertBeforeEvent(prev, fEvent);
		}
		void Remove (TScorePtr score){score->RemoveEvent(fEvent);}
		Boolean IsInserted(){return fEvent->fPrev && fEvent->fNext;}
		ULONG GetDateTicks(){return Date(fEvent->MidiEvent());}
};


typedef TScoreMarker FAR * TScoreMarkerPtr;


#endif
