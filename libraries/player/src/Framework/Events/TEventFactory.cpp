/*

  Copyright © Grame 1996-2006

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
static TEventPtr CreateEvent(MidiEvPtr e)	{return new TEvent(e);}
static TEventPtr CreateNote(MidiEvPtr e)	{return new TNote(e);}
static TEventPtr CreateKeyOn(MidiEvPtr e)	{return new TKeyOn(e);}
static TEventPtr CreateKeyOff(MidiEvPtr e)	{return new TKeyOff(e);}
static TEventPtr CreateKeyPress(MidiEvPtr e)	{return new TKeyPress(e);}
static TEventPtr CreateCtrlChange(MidiEvPtr e)	{return new TCtrlChange(e);}
static TEventPtr CreateProgChange(MidiEvPtr e)	{return new TProgChange(e);}
static TEventPtr CreateChanPress(MidiEvPtr e)	{return new TChanPress(e);}
static TEventPtr CreatePitchBend(MidiEvPtr e)	{return new TPitchBend(e);}
static TEventPtr CreateTune(MidiEvPtr e)	{return new TTune(e);}
static TEventPtr CreateTempo(MidiEvPtr e)	{return new TTempo(e);}
static TEventPtr CreateTimeSign(MidiEvPtr e)	{return new TTimeSign(e);}
static TEventPtr CreateSysEx(MidiEvPtr e)	{return new TSysEx(e);}

TEventFactory::TEventFactory()
{
	// Default constructor
	
	for (short i = 0; i< 256 ; i++) {fBuildTable[i] = CreateEvent;}
	
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
