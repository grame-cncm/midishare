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
//	TEventFactory.cpp		    
// ===========================================================================
//
// A TEventFactory object allows to build TEvent object using the MidiShare event type.
//


#include "TEventFactory.h"

/*--------------------------------------------------------------------------*/
// Event Factory global Instance
/*--------------------------------------------------------------------------*/

TEventFactoryPtr TEventFactory::fInstance = 0;

/*--------------------------------------------------------------------------*/

TEventFactory::TEventFactory ()
{
	// Default constructor
	
	for (short i = 0; i< 256 ; i++) { fBuildTable[i] = &TEventFactory::CreateEvent;}
	
	// Specific constructor
	
	fBuildTable[typeNote] =  (mthptr)&TEventFactory::CreateNote;
	fBuildTable[typeKeyOn] = (mthptr)&TEventFactory::CreateKeyOn;
	fBuildTable[typeKeyOff] = (mthptr)&TEventFactory::CreateKeyOff;
	fBuildTable[typeProgChange] = (mthptr)&TEventFactory::CreateProgChange;
	fBuildTable[typeCtrlChange] = (mthptr)&TEventFactory::CreateCtrlChange;
	fBuildTable[typeKeyPress] = (mthptr)&TEventFactory::CreateKeyPress;
	fBuildTable[typeChanPress] = (mthptr)&TEventFactory::CreateChanPress;
	fBuildTable[typePitchWheel] = (mthptr)&TEventFactory::CreatePitchBend;
	fBuildTable[typeTune] = (mthptr)&TEventFactory::CreateTune;
	fBuildTable[typeSysEx] = (mthptr)&TEventFactory::CreateSysEx;

	fBuildTable[typeTempo] = (mthptr)&TEventFactory::CreateTempo;
	fBuildTable[typeTimeSign] = (mthptr)&TEventFactory::CreateTimeSign;
}

