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
//	TScoreInserter.cpp			    
// ===========================================================================
//
//  An utility class to insert sequence of events in the score
//


#include "TScoreInserter.h"
#include "UMidi.h"
#include "UTools.h"


/*--------------------------------------------------------------------------*/

 long TScoreInserter::InsertAllTrack(MidiSeqPtr s)
 {
 	MidiSeqPtr s1 = UMidi::BuildAllTrack(s); // Make the sequence
 	if (!s1) return kErrEvent;
 	return fScore->InsertAllTrack(SetPosTicks (Date(FirstEv(s1))), s1);
 }
 
/*--------------------------------------------------------------------------*/

 long TScoreInserter::InsertTrack(short tracknum,MidiSeqPtr s)
 { 
 	MidiSeqPtr s1 = UMidi::BuildAllTrack(s); // Make the sequence
  	if (!s1) return kErrEvent;
  	return  fScore->InsertTrack(SetPosTicks (Date(FirstEv(s1))), tracknum,s1);
 } 