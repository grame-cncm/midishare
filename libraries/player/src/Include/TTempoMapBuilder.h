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


#ifndef __TTempoMapBuilder__
#define __TTempoMapBuilder__


#include "TScoreVisitorInterface.h"
#include "TPlayerConstants.h"

//------------------------
// Class TTempoMapBuilder 
//------------------------
/*!
  \brief Used for the score TempoMap construction
*/


class TTempoMapBuilder :public TScoreVisitorInterface {

	private:
	
		MidiEvPtr fLastTempo;
		MidiEvPtr fLastTs;
	
	public:
	
		TTempoMapBuilder():fLastTempo(0),fLastTs(0){}
		virtual ~TTempoMapBuilder(){}
	
		void Visite (TTempo* ev, Boolean  forward)
		{
			if (fLastTempo) {
				ev->SetTempoBackward(Tempo(fLastTempo));
			}else{
				ev->SetTempoBackward (kDefaultTempoEv);
			}	
			fLastTempo = ev->MidiEvent();
		}
		
		void Visite (TTimeSign* ev, Boolean  forward)
		{
			if (fLastTs) {
				ev->SetBNum(TSNum(fLastTs));
				ev->SetBDenom(TSDenom(fLastTs));
				ev->SetBnClocks(TSClocks(fLastTs));
				ev->SetBn32nd(TS32nd(fLastTs)) ;
			}else {
				ev->SetBNum(kDefaultNum);
				ev->SetBDenom(kDefaultDenom);
				ev->SetBnClocks(kDefaultClocks);
				ev->SetBn32nd(kDefaultN32) ;
			}
			fLastTs = ev->MidiEvent();
		}

};


#endif
