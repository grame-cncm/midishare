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
//	TTempoMapBuilder.h		    
// ===========================================================================
/*!
  \brief Used for the score TempoMap construction
*/


#ifndef __TTempoMapBuilder__
#define __TTempoMapBuilder__


#include "TScoreVisitorInterface.h"
#include "TPlayerConstants.h"

//-----------------------
// Class TTempoMapBuilder 
//-----------------------

class TTempoMapBuilder :public TScoreVisitorInterface {

	private:
	
		MidiEvPtr lastTempo;
		MidiEvPtr lastTs;
	
	public:
	
		TTempoMapBuilder() {lastTempo = lastTs = 0;}
		~TTempoMapBuilder(){}
	
		void Visite (TTempo* ev, Boolean  forward ){
			if (lastTempo) {
				ev->SetTempoBackward(Tempo(lastTempo));
			}else{
				ev->SetTempoBackward (kDefaultTempoEv);
			}	
			lastTempo = ev->MidiEvent();
		}
		
		void Visite (TTimeSign* ev, Boolean  forward ){
			if (lastTs) {
				ev->SetBNum(TSNum(lastTs));
				ev->SetBDenom(TSDenom(lastTs));
				ev->SetBnClocks(TSClocks(lastTs));
				ev->SetBn32nd(TS32nd(lastTs)) ;
			}else {
				ev->SetBNum(kDefaultNum);
				ev->SetBDenom(kDefaultDenom);
				ev->SetBnClocks(kDefaultClocks);
				ev->SetBn32nd(kDefaultN32) ;
			}
			lastTs = ev->MidiEvent();
		}


};
#endif
