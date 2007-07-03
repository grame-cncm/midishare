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

 long TScoreInserter::InsertTrack(short tracknum, MidiSeqPtr s)
 { 
 	MidiSeqPtr s1 = UMidi::BuildAllTrack(s); // Make the sequence
  	if (!s1) return kErrEvent;
  	return  fScore->InsertTrack(SetPosTicks (Date(FirstEv(s1))), tracknum,s1);
 }

