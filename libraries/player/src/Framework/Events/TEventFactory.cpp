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

TEventFactory::TEventFactory ()
{
	// Default constructor
	
	for (short i = 0; i< 256 ; i++) { fBuildTable[i] = CreateEvent;}
	
	// Specific constructor
	
	fBuildTable[typeNote] =  CreateNote;
	fBuildTable[typeKeyOn] = CreateKeyOn;
	fBuildTable[typeKeyOff] = CreateKeyOff;
	fBuildTable[typeProgChange] = CreateProgChange;
	fBuildTable[typeCtrlChange] = CreateCtrlChange;
	fBuildTable[typeKeyPress] = CreateKeyPress;
	fBuildTable[typeChanPress] = CreateChanPress;
	fBuildTable[typePitchWheel] = CreatePitchBend;
	fBuildTable[typeTune] = CreateTune;
	fBuildTable[typeSysEx] = CreateSysEx;
	
	fBuildTable[typeTempo] = CreateTempo;
	fBuildTable[typeTimeSign] = CreateTimeSign;
}
