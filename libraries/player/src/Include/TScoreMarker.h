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
//	TScoreMarker.h			    
// ===========================================================================

#ifndef __TMarker__
#define __TMarker__

#include "TEventFactory.h"
#include "TScore.h"
#include "UDebug.h"

//--------------------
// Class TScoreMarker 
//--------------------
/*!
 \brief	Markers are special Private events which can be inserted in the score.
  Markers are directly inserted in the score and MUST be removed before destroying the score.
*/

class TScoreMarker {

	private:
	
		TEventPtr fEvent;
		
	public:
		
		TScoreMarker(Byte type)
		{
			fEvent = TEventFactory::GenericCreateEvent(MidiNewEv(type));
			if (fEvent) {
				fEvent->SetTrackNum(kTempoMap);
				fEvent->SetDate(0);
			}
		}
		
		// The marker MUST be removed from the score before destroying it.
		virtual ~TScoreMarker() {if (fEvent) delete (fEvent);}
		
		void Insert(TScorePtr score, TEventPtr prev, ULONG date_ticks)
		{ 
			Date(fEvent->MidiEvent()) = date_ticks;  
			score->InsertBeforeEvent(prev, fEvent);
		}
		void Remove (TScorePtr score) {score->RemoveEvent(fEvent);}
		Boolean IsInserted() {return fEvent->fPrev && fEvent->fNext;}
		ULONG GetDateTicks() {return Date(fEvent->MidiEvent());}
};

typedef TScoreMarker FAR * TScoreMarkerPtr;

#endif
