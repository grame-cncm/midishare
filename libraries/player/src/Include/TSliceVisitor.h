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
//	TSliceVisitor.h			    
// ===========================================================================
/*!
	\brief Visitor for Step playing mode.
  Only KeyOn event are played, actually KeyOn are "transformed" in Note event
  with a fixed duration.
*/


#ifndef __TSliceVisitor__
#define __TSliceVisitor__

 
#include "TScoreVisitorInterface.h"
#include "TEventSenderInterface.h"


//-----------------------
// Class TSliceVisitor 
//-----------------------

class TSliceVisitor :public TScoreVisitorInterface {
	
	private:
	
		TEventSenderInterfacePtr fUser;
			
	public :
 
 		TSliceVisitor(TEventSenderInterfacePtr user){ fUser = user;}
 		~TSliceVisitor (){}
 		
 		void Visite (TKeyOn* ev, Boolean forward) { 
 			MidiEvPtr note = MidiCopyEv(ev->MidiEvent());
 			// Force the event to typeNote with a default duration
			if (note) { 
				EvType(note) = typeNote;
				MidiSetField(note,2,150);
				fUser->UseEvent(note, MidiGetTime());
			}
 		}
 	
};

typedef TSliceVisitor FAR * TSliceVisitorPtr;

#endif
