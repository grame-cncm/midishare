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
//	UScoreManager.cpp			    
// ===========================================================================
//
//	Utility functions for score management
//


#include "UScoreManager.h"

/*----------------------------------------------------------------------------*/
	
void UScoreManager::Apply (TScorePtr score, TScoreVisitorInterfacePtr fun) {
	TScoreIterator it(score);

	TEventPtr cur;
	while ( cur = it.NextEv()) { cur->Accept(fun,true);}
}
	
/*----------------------------------------------------------------------------*/
	
void UScoreManager::Apply (TScorePtr score, TScoreVisitorInterfacePtr fun,ULONG begin, ULONG end) {
	TScoreIterator it(score);
	TEventPtr cur;
	it.SetPosTicks(begin);
	while ((cur = it.NextEv()) && (cur->GetDate() < end)) { cur->Accept(fun,true);}
}
	
