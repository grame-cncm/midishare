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
//	TEventRecorder.cpp			    
// ===========================================================================
//
//	Recording management
// 


#ifndef __TEventRecorder__
#define __TEventRecorder__

 
#include "TScoreIterator.h"
#include "TEventReceiverInterface.h"
#include "TPlayerScore.h"
#include "TPlayerSynchroniser.h"

//-----------------------
// Class TEventRecorder 
//-----------------------

class TEventRecorder :public TEventDispatcher {

	private:
		
		short 			fRecordtrack;	// Track in record mode
		short 			fRecordmode;	// Record mode
		MidiFilterPtr 	fRecFilter;     // Record filter
		
		TScoreIteratorPtr	 		fIterator;
		TPlayerScorePtr			 	fScore;
		TPlayerSynchroniserPtr   	fSynchroniser;
		TRunningStatePtr 			fState;
			
		void Insert(MidiEvPtr e);
		void ReceiveDefaultEvent(MidiEvPtr e);
		void EraseTrack();
		
		Boolean	AcceptEv (MidiEvPtr e) 	{ return MidiIsAcceptedType(fRecFilter,EvType(e));}
		Boolean IsRecording () 			{ return (fRecordtrack > 0);}
		Boolean IsErasing () 			{ return (fRecordmode == kEraseOn);}
	
	public:
	
		TEventRecorder(TPlayerScorePtr score, TPlayerSynchroniserPtr synchro, TRunningStatePtr state, TEventDispatcherPtr successor);
		~TEventRecorder();
	
		void SetRecordMode (short state);
		short GetRecordMode (){return fRecordmode;}
		void SetRecordTrack (short tracknum);
		short GetRecordTrack () {return fRecordtrack;}
		void SetRecordFilter (MidiFilterPtr filter);
		MidiFilterPtr GetRecordFilter (){return fRecFilter;}
		
		void ReceiveEvents (MidiEvPtr e);	
	
};

typedef TEventRecorder FAR * TEventRecorderPtr;


#endif
