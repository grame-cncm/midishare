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
//	TMidiPlayer.h			    
// ===========================================================================
/*!
\brief	Midi events output management.
*/



#ifndef __TEventPlayer__
#define __TEventPlayer__


#include "TEventSenderInterface.h"
#include "TMidiAppl.h"
#include "TTrackTable.h"

//-----------------------
// Class TEventPlayer 
//-----------------------

class TMidiPlayer :public TEventSenderInterface{

	private:
		TMidiApplPtr 	fMidiAppl;
		TTrackTablePtr 	fTrackTable;
		
		Boolean IsPrivate(MidiEvPtr e) { return (EvType (e) >= typePrivate) &&  (EvType (e) < typeProcess);}
	
	public:
		
		TMidiPlayer(TMidiApplPtr appl, TTrackTablePtr table) 
		{
			fMidiAppl = appl;
			fTrackTable = table;
		}
		
		short Init() {return kNoErr;}
		
		~TMidiPlayer(){}
		
		void SendEvent(MidiEvPtr event, ULONG date_ms);

		void UseEvent(TEventPtr event, ULONG date_ms);
		void UseEvent(MidiEvPtr event, ULONG date_ms);
	
		void CopyAndUseEvent(TEventPtr event, ULONG date_ms);
		void CopyAndUseEvent(MidiEvPtr event, ULONG date_ms);
};

typedef TMidiPlayer FAR * TMidiPlayerPtr;

#endif
