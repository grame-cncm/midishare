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
//   [11-12-99] SL - Definition of MidiIsAcceptedType, 
//					 MidiIsAcceptedPort and MidiIsAcceptedChan
// ===========================================================================

#ifndef __Filter__
#define __Filter__

#include "GenericMidiShare.h"


#ifdef __cplusplus
extern "C" {
#endif

#ifdef __Macintosh__
MidiFilterPtr MidiNewFilter();
void MidiFreeFilter(MidiFilterPtr filter);

void MidiAcceptPort (MidiFilterPtr filter, short port, Boolean state);
void MidiAcceptChan (MidiFilterPtr filter, short chan, Boolean state);
void MidiAcceptType (MidiFilterPtr filter, short type, Boolean state);

Boolean MidiIsAcceptedType (MidiFilterPtr filter, short type);
Boolean MidiIsAcceptedPort (MidiFilterPtr filter, short port);
Boolean MidiIsAcceptedChan (MidiFilterPtr filter, short chan);


#endif

#ifdef __cplusplus
}
#endif

#endif
