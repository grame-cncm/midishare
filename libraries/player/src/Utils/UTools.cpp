// ===========================================================================
// The Player Library is Copyright (c) Grame, Computer Music Research Laboratory 
// 1996-1999, and is distributed as Open Source software under the Artistic License;
// see the file "Artistic" that is included in the distribution for details.
//
// Grame : Computer Music Research Laboratory
// Web : http://www.grame.fr/Research
// E-mail : MidiShare@rd.grame.fr
//
// modifications history:
//   [11-12-99] SL - Linux adaptation
//
// ===========================================================================


// ===========================================================================
//	 UTools.cpp 			    
// ===========================================================================
//
//	Utilities functions
// 


#include "UTools.h"
#include "GenericMidiShare.h"

/*----------------------------------------------------------------------------*/

void UTools::SendVal (ULONG val)
{
	MidiEvPtr  e ;

	if (e= MidiNewEv(typeNote)){
		Dur(e)= (unsigned short)val;
		MidiSendIm (0, e);
	}
}

/*----------------------------------------------------------------------------*/

void UTools::MidiPrintText ( char * s)
{
	MidiEvPtr e;
	ULONG c = 0;

	if (e = MidiNewEv(typeTextual)){
		for (c = 0 ; *s ; s++,c++) MidiAddField (e ,*s);
		MidiSendIm (0, e);
   	}
}
