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
//	TEventSenderInterface.h			    
// ===========================================================================
//
//	Interface for events users
// 


#ifndef __TEventSenderInterface__
#define __TEventSenderInterface__

#include "TEvent.h"

//---------------------------
// Class TEventSenderInterface
//---------------------------

class TEventSenderInterface{

	public:
	
		virtual ~TEventSenderInterface(){};
	
		virtual short Init() = 0;
		virtual void SendEvent(MidiEvPtr event, ULONG date_ms) = 0;
		
		virtual void UseEvent(TEventPtr event, ULONG date_ms) = 0;
		virtual void UseEvent(MidiEvPtr event, ULONG date_ms) = 0;

		virtual void CopyAndUseEvent(TEventPtr event, ULONG date_ms) = 0;
		virtual void CopyAndUseEvent(MidiEvPtr event, ULONG date_ms) = 0;
};

typedef TEventSenderInterface FAR * TEventSenderInterfacePtr;


#endif
