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
/* Event creation methods */
static TEventPtr CreateEvent(MidiEvPtr e)	{ return new TEvent(e);}
static TEventPtr CreateNote(MidiEvPtr e)	{ return new TNote(e);}
static TEventPtr CreateKeyOn(MidiEvPtr e)	{ return new TKeyOn(e);}
static TEventPtr CreateKeyOff(MidiEvPtr e)	{ return new TKeyOff(e);}
static TEventPtr CreateKeyPress(MidiEvPtr e)	{ return new TKeyPress(e);}
static TEventPtr CreateCtrlChange(MidiEvPtr e)	{ return new TCtrlChange(e);}
static TEventPtr CreateProgChange(MidiEvPtr e)	{ return new TProgChange(e);}
static TEventPtr CreateChanPress(MidiEvPtr e)	{ return new TChanPress(e);}
static TEventPtr CreatePitchBend(MidiEvPtr e)	{ return new TPitchBend(e);}
static TEventPtr CreateTune(MidiEvPtr e)	{ return new TTune(e);}
static TEventPtr CreateTempo(MidiEvPtr e)	{ return new TTempo(e);}
static TEventPtr CreateTimeSign(MidiEvPtr e)	{ return new TTimeSign(e);}
static TEventPtr CreateSysEx(MidiEvPtr e)	{ return new TSysEx(e);}


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
