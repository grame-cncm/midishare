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
//	TPlayerMemento.h			    
// ===========================================================================
//
//	Save and restore the Player state between synchro changes
// 


#ifndef __TPlayerMemento__
#define __TPlayerMemento__


#include "TPlayer.h"

//-----------------------
// Class TPlayerMemento
//-----------------------


class TPlayerMemento {

	private:
	
		ULONG 	fCurdate_ticks;
		ULONG	fLoopStart;
		ULONG	fLoopEnd;
		Boolean fLoopState;
		Boolean fRestoreLoop;
			
	public :
		
		TPlayerMemento(){}
		~TPlayerMemento (){}
		
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
		
		
		void DefaultState(TPlayerPtr player)
		{
			player->fTrackTable->Clear();
		}
		
};


typedef TPlayerMemento FAR * TPlayerMementoPtr;

#endif
