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
//	Umidi.h			    
// ===========================================================================
/*!
\brief	Midi utilities : various MidiShare sequence management functions.
*/



#ifndef __UMidi__
#define __UMidi__


#include "GenericMidiShare.h"
#include "TPlayerConstants.h"


//------------------------------------------------------------------
// Fonctions utilitaires sur les sequences et gestion de la memoire
//------------------------------------------------------------------

class UMidi{

 public :
 
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
