/*

  Copyright © Grame 1996-2004

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
//	TEventRecorder.cpp			    
// ===========================================================================


#ifndef __TEventRecorder__
#define __TEventRecorder__

 
#include "TScoreIterator.h"
#include "TEventReceiverInterface.h"
#include "TPlayerScore.h"
#include "TPlayerSynchroniser.h"

//----------------------
// Class TEventRecorder 
//----------------------
/*!
\brief	Recording management.
*/


class TEventRecorder :public TEventDispatcher {

	private:
		
		short 			fRecordtrack;	// Track in record mode
		short 			fRecordmode;	// Record mode
		MidiFilterPtr 	fRecFilter;     // Record filter
		
		TScoreIterator	 			fIterator;
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
		virtual ~TEventRecorder();
	
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
