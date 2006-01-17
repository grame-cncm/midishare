/*

  Copyright © Grame 1996-2004

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
//	Umidi.h			    
// ===========================================================================

#ifndef __UMidi__
#define __UMidi__

#include "PlayerDefs.h"
#include "MidiShare.h"
#include "TPlayerConstants.h"

///------------
// Class UMidi
//-------------
/*!
\brief	Midi utilities : various MidiShare sequence management functions.
*/

class UMidi {

 public:
 
	static MidiSeqPtr 	TrsfNoteToKeyOn (MidiSeqPtr src);
	
 	static void 		MixeSeq(MidiSeqPtr src, MidiSeqPtr dest);
	static MidiEvPtr 	AddSeq(MidiEvPtr e1, MidiEvPtr e2);
	
	static MidiEvPtr	CheckEvType(MidiSeqPtr src, short type);

	static void 		FreeOneLine(MidiEvPtr cur);
 	
 	static ULONG		LengthSeq(MidiSeqPtr s);
 	static void 		SetRefnum(MidiSeqPtr src, short refnum);
 	
 	static MidiEvPtr 	NoteToKeyOff(MidiEvPtr e1);
 	
 	static MidiSeqPtr 	BuildTrack(MidiSeqPtr s);
	static MidiSeqPtr 	BuildAllTrack(MidiSeqPtr s);
	
	static Boolean 		Copy_AddSeq(MidiSeqPtr s, MidiEvPtr e);
	static Boolean 		IsEmpty(MidiSeqPtr s);
	static void 		DelEndTrack(MidiSeqPtr seq);
	static void 		RemoveEv(MidiSeqPtr seq, MidiEvPtr e);
};

#endif
