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
//	TEventFactory.h			    
// ===========================================================================

#ifndef __TEventFactory__
#define __TEventFactory__

#include "TEvent.h"


//---------------------
// Class TEventFactory 
//---------------------
/*!
	\brief A TEventFactory object allows to build TEvent objects using the MidiShare event type.
*/

typedef  TEventPtr  (*mthptr)(MidiEvPtr e);


class TEventFactory {

	private:
		
		mthptr fBuildTable[256];
		static TEventFactory* fInstance;
	
	public: 
		
		TEventFactory();
		virtual ~TEventFactory(){}
		
		static TEventPtr GenericCreateEvent(MidiEvPtr e) {return fInstance->fBuildTable[EvType(e)](e);}
		static void Init() {if (fInstance == 0) fInstance = new TEventFactory();}
		static void Destroy() 
		{
			if (fInstance){ 
				delete(fInstance);
			 	fInstance = 0;
			}
		}
};

typedef TEventFactory* TEventFactoryPtr;

#endif
