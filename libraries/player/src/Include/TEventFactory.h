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
/*!
	\brief A TEventFactory object allows to build TEvent objects using the MidiShare event type.
*/



#ifndef __TEventFactory__
#define __TEventFactory__

 
#include "TEvent.h"


/*--------------------------------------------------------------------------*/

class TEventFactory;
typedef TEventFactory FAR * TEventFactoryPtr;

/*--------------------------------------------------------------------------*/

typedef  TEventPtr  (*mthptr)(MidiEvPtr e);

/*--------------------------------------------------------------------------*/
/* Event creation methods */
static TEventPtr CreateEvent(MidiEvPtr e)		{ return new TEvent(e);}
static TEventPtr CreateNote(MidiEvPtr e)		{ return new TNote(e);}
static TEventPtr CreateKeyOn(MidiEvPtr e)		{ return new TKeyOn(e);}
static TEventPtr CreateKeyOff(MidiEvPtr e)		{ return new TKeyOff(e);}
static TEventPtr CreateKeyPress(MidiEvPtr e)	{ return new TKeyPress(e);}
static TEventPtr CreateCtrlChange(MidiEvPtr e)	{ return new TCtrlChange(e);}
static TEventPtr CreateProgChange(MidiEvPtr e)	{ return new TProgChange(e);}
static TEventPtr CreateChanPress(MidiEvPtr e)	{ return new TChanPress(e);}
static TEventPtr CreatePitchBend(MidiEvPtr e)	{ return new TPitchBend(e);}
static TEventPtr CreateTune(MidiEvPtr e)		{ return new TTune(e);}
static TEventPtr CreateTempo(MidiEvPtr e)		{ return new TTempo(e);}
static TEventPtr CreateTimeSign(MidiEvPtr e)	{ return new TTimeSign(e);}
static TEventPtr CreateSysEx(MidiEvPtr e)		{ return new TSysEx(e);}
		
//-----------------------
// Class TEventFactory 
//-----------------------

class TEventFactory {

	private:
		
		mthptr fBuildTable[256];
		static TEventFactoryPtr fInstance;
	
	public: 
		
		TEventFactory ();
		~TEventFactory() {}
		
		static TEventPtr GenericCreateEvent(MidiEvPtr e) {return fInstance->fBuildTable[EvType(e)](e);}
		static void Init() { if (fInstance == 0) fInstance = new TEventFactory(); }
};


#endif
