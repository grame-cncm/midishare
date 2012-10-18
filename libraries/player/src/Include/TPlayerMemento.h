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
//	TPlayerMemento.h			    
// ===========================================================================

#ifndef __TPlayerMemento__
#define __TPlayerMemento__

#include "TPlayer.h"

//----------------------
// Class TPlayerMemento
//----------------------
/*!
\brief	Save and restore the Player state between synchro changes.
*/

class TPlayerMemento {

	private:
	
		ULONG 	fCurdate_ticks;
		ULONG	fLoopStart;
		ULONG	fLoopEnd;
		Boolean fLoopState;
		Boolean fRestoreLoop;
			
	public :
		
		TPlayerMemento():fCurdate_ticks(0),fLoopStart(0),fLoopEnd(0),fLoopState(false),fRestoreLoop(false) {}
		virtual ~TPlayerMemento () {}
		
		void SaveState(TPlayerPtr player)
		{
			fCurdate_ticks = player->fPlayer->GetPosTicks();
			fRestoreLoop = player->fLoopManager->IsLoopPlaced();
			
			// Save the Loop points only if necessary
			if (fRestoreLoop) {
				fLoopStart = player->fLoopManager->GetLoopStartTicks();
				fLoopEnd = player->fLoopManager->GetLoopEndTicks();
				fLoopState = player->fLoopManager->GetLoop();
			}
		}
		
		void RestoreState(TPlayerPtr player)
		{
			player->fPlayer->SetPosTicks(fCurdate_ticks); 
			
			// Restore the Loop points only if necessary
			if (fRestoreLoop) {
				player->fLoopManager->SetLoopEndTicks(fLoopEnd);
				player->fLoopManager->SetLoopStartTicks(fLoopStart);
				player->fLoopManager->SetLoop(fLoopState);
			}
		}
			
		void DefaultState(TPlayerPtr player) {player->fTrackTable.Clear();}
};

typedef TPlayerMemento FAR * TPlayerMementoPtr;

#endif
