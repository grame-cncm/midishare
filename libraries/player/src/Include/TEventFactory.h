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
//	TEventFactory.h			    
// ===========================================================================
//
// A TEventFactory object allows to build TEvent objects using the MidiShare event type.
//



#ifndef __TEventFactory__
#define __TEventFactory__

 
#include "TEvent.h"


/*--------------------------------------------------------------------------*/

class TEventFactory;
typedef TEventFactory FAR * TEventFactoryPtr;

/*--------------------------------------------------------------------------*/

typedef  TEventPtr (TEventFactory::*mthptr)(MidiEvPtr e);


//-----------------------
// Class TEventFactory 
//-----------------------

class TEventFactory {

	private:
	
		TEventPtr 	CreateEvent(MidiEvPtr e){ return new TEvent(e);}
		
		TEventPtr 	CreateNote(MidiEvPtr e){ return new TNote(e);}
		TEventPtr 	CreateKeyOn(MidiEvPtr e){ return new TKeyOn(e);}
		TEventPtr 	CreateKeyOff(MidiEvPtr e){ return new TKeyOff(e);}
		TEventPtr	CreateKeyPress(MidiEvPtr e){ return new TKeyPress(e);}
		TEventPtr 	CreateCtrlChange(MidiEvPtr e){ return new TCtrlChange(e);}
		TEventPtr   CreateProgChange(MidiEvPtr e){ return new TProgChange(e);}
		TEventPtr   CreateChanPress(MidiEvPtr e){ return new TChanPress(e);}
		TEventPtr   CreatePitchBend(MidiEvPtr e){ return new TPitchBend(e);}
		TEventPtr 	CreateTune(MidiEvPtr e){ return new TTune(e);}
		TEventPtr 	CreateTempo(MidiEvPtr e){ return new TTempo(e);}
		TEventPtr 	CreateTimeSign(MidiEvPtr e){ return new TTimeSign(e);}
		TEventPtr 	CreateSysEx(MidiEvPtr e){ return new TSysEx(e);}
		
		mthptr 	fBuildTable[256];
		static TEventFactoryPtr fInstance;
	
	public: 
		
		TEventFactory ();
		~TEventFactory() {}
		
		static TEventPtr	GenericCreateEvent(MidiEvPtr e) {return fInstance->GenericCreateEventAux(e);}
		       TEventPtr	GenericCreateEventAux(MidiEvPtr e) {return (this->*fBuildTable[EvType(e)])(e);}
		
		static void Init() { if (fInstance == 0) fInstance = new TEventFactory(); }
};


#endif
