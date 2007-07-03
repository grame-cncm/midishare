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

class TTempoMapBuilder : public TScoreVisitorInterface {

	private:
	
		MidiEvPtr fLastTempo;
		MidiEvPtr fLastTs;
	
	public:
	
		TTempoMapBuilder():fLastTempo(0),fLastTs(0) {}
		virtual ~TTempoMapBuilder() {}
	
		void Visite(TTempo* ev, Boolean  forward)
		{
			if (fLastTempo) {
				ev->SetTempoBackward(Tempo(fLastTempo));
			} else {
				ev->SetTempoBackward (kDefaultTempoEv);
			}	
			fLastTempo = ev->MidiEvent();
		}
		
		void Visite(TTimeSign* ev, Boolean  forward)
		{
			if (fLastTs) {
				ev->SetBNum(TSNum(fLastTs));
				ev->SetBDenom(TSDenom(fLastTs));
				ev->SetBnClocks(TSClocks(fLastTs));
				ev->SetBn32nd(TS32nd(fLastTs)) ;
			} else {
				ev->SetBNum(kDefaultNum);
				ev->SetBDenom(kDefaultDenom);
				ev->SetBnClocks(kDefaultClocks);
				ev->SetBn32nd(kDefaultN32) ;
			}
			fLastTs = ev->MidiEvent();
		}
};

#endif
