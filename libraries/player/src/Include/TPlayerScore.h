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
//	TPlayerScore.h			    
// ===========================================================================

#ifndef __TPlayerScore__
#define __TPlayerScore__

#include "TScore.h"
#include "TPlayerConstants.h"
#include "TScoreIterator.h"

//--------------------
// Class TPlayerScore 
//--------------------
/*!
 \brief The internal score : a TPlayerScore contains at least one TTempo event, one 
 TTimeSign event and one EndScore event.
*/

class TPlayerScore : public TScore {

	private:
		
		TEventPtr fScoreEnd;
		TEventPtr fScoreBegin;
		
		void Init();
		
		void AdjustEndDate(ULONG date); 
		void ClearTrack (short tracknum);
		void CheckAndAllocateMemory();
		Boolean CheckTracknum(short tracknum) { return ((tracknum >= 0)  && (tracknum < kMaxTrack));}
		
		short InsertEventsAtEnd(MidiEvPtr e1);
		short InsertEventsAtEnd(MidiEvPtr e1, short tracknum);
		short InsertEventsIn(TScoreIteratorPtr it,MidiEvPtr* e1);
		short InsertEventsIn(TScoreIteratorPtr it,MidiEvPtr* e1, short tracknum);
		
	public:
	
		TPlayerScore();
		virtual ~TPlayerScore(){}
		
		TEventPtr FirstEvent(); 
		TEventPtr LastEvent(); 
		
		void InsertAtBeginEvent(TEventPtr ev);
		void InsertAtEndEvent(TEventPtr ev);
		void RemoveEvent(TEventPtr ev);
		
		void InsertAfterEvent(TEventPtr ev1,TEventPtr ev2);
		void InsertBeforeEvent(TEventPtr ev1,TEventPtr ev2); 
	
		long SetAllTrack(MidiSeqPtr s);
		long SetTrack( short tracknum,MidiSeqPtr s);
		
		MidiSeqPtr GetAllTrack();
		MidiSeqPtr GetTrack(short tracknum);
	
		long InsertAllTrack(TEventPtr cur,MidiSeqPtr s);
		long InsertTrack(TEventPtr cur,short tracknum,MidiSeqPtr s);
		
		ULONG GetLastDate();
};

typedef class TPlayerScore  FAR * TPlayerScorePtr;

#define IsScore(t)  (((EvType(t))<typePrivate || (EvType(t))>typeDProcess) && (EvType(t))!=typeEndTrack)

#endif
