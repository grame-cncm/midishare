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
//	TTempoMapVisitor.cpp		    
// ===========================================================================
//
// TTempoMapVisitor: an object used to maintain the current Tempo and TimeSign state
// and give conversion functions 


#include "TTempoMapVisitor.h"
#include "UTools.h"


void TTempoMapVisitor::Visite (TTempoPtr ev,Boolean forward)
{
	if (forward) {
		fTempoConverter->Update(ev->GetDate(),ev->GetTempoForward()); 
	}else {
		fTempoConverter->Update(ev->GetDate(),ev->GetTempoBackward());
	}
}

/*--------------------------------------------------------------------------*/

void TTempoMapVisitor::Visite (TTimeSignPtr ev,Boolean forward) 
{
	if (forward) {
		fTimeConverter->Update(ev->GetDate(),ev->GetFNum(),ev->GetFDenom(),ev->GetFnClocks(),ev->GetFn32nd());
	}else {
		fTimeConverter->Update(ev->GetDate(),ev->GetBNum(),ev->GetBDenom(),ev->GetBnClocks(),ev->GetBn32nd());
	}
}

