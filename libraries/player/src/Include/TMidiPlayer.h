/*

  Copyright (C) Grame 1996-2012

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
//	TMidiPlayer.h			    
// ===========================================================================

#ifndef __TEventPlayer__
#define __TEventPlayer__

#include "TEventSenderInterface.h"
#include "TMidiAppl.h"
#include "TTrackTable.h"

//--------------------
// Class TEventPlayer 
//--------------------
/*!
\brief	Midi events output management.
*/

class TMidiPlayer : public TEventSenderInterface {

	private:
	
		TMidiApplPtr 	fMidiAppl;
		TTrackTablePtr 	fTrackTable;
		
		Boolean IsPrivate(MidiEvPtr e) {return (EvType (e) >= typePrivate) &&  (EvType (e) < typeProcess);}
	
	public:
		
		TMidiPlayer(TMidiApplPtr appl, TTrackTablePtr table):fMidiAppl(appl),fTrackTable(table) {}
		virtual ~TMidiPlayer() {}
		
		short Init() {return kNoErr;}
		
		void SendEvent(MidiEvPtr event, ULONG date_ms);

		void UseEvent(TEventPtr event, ULONG date_ms);
		void UseEvent(MidiEvPtr event, ULONG date_ms);
	
		void CopyAndUseEvent(TEventPtr event, ULONG date_ms);
		void CopyAndUseEvent(MidiEvPtr event, ULONG date_ms);
};

typedef TMidiPlayer FAR * TMidiPlayerPtr;

#endif
