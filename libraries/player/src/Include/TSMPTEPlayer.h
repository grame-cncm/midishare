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
//	TSMPTEPlayer.h	   			 
// ===========================================================================

#ifndef __TSMPTEPlayer__
#define __TSMPTEPlayer__

#include "TSyncInPlayer.h"
#include "TRunningState.h"
#include "TSMPTEInfos.h"
#include "TGenericPlayerInterface.h"

//------------------
// Class TSMPTETask 
//------------------
/*!
\brief	Task used in kSMPTESync mode.
*/

class TSMPTETask : public TMidiTask {

	friend class TSMPTEPlayer;
	
	private:
	
		TSMPTEPlayer* fPlayer;

	public: 
	
		TSMPTETask (TSMPTEPlayer* it):fPlayer(it){}
		virtual ~TSMPTETask(){}
		void Execute (TMidiApplPtr appl, ULONG date);
};

typedef TSMPTETask FAR * TSMPTETaskPtr;


//-----------------------
// Class TSMPTEPlayer
//-----------------------
/*!
\brief	Player in kSMPTESync mode.
*/

class TSMPTEPlayer : public TGenericPlayerInterface {

	friend class TSMPTETask;
	
	 private:
	 	
	 	TGenericPlayerInterfacePtr  fPlayer;
        TRunningStatePtr  			fRunningState;
	 	TMidiApplPtr        		fMidiAppl;
	 	
	 	TSMPTEInfosPtr	fSmpteInfos;
		TSMPTETask		fSMPTEtask;
	 	
	 	void StartAtSMPTEOffset();  
	 	
	  public:
	  	
	 	TSMPTEPlayer (TGenericPlayerInterfacePtr player, TRunningStatePtr state, TSMPTEInfosPtr smpte, TMidiApplPtr appl)
	 		:fPlayer(player),fRunningState(state),fMidiAppl(appl),fSmpteInfos(smpte),fSMPTEtask(this){}
	 	virtual ~TSMPTEPlayer(){}
	  
	  	void Start();
		void Stop();
		void Pause();
		void Cont();
		
		void PlaySliceForward();
 		void PlaySliceBackward() ;

		void SetPosTicks (ULONG date_ticks) ;
		void SetPosBBU (const TPos& pos) ;
		void SetPosMs (ULONG date_ms);
		
		ULONG GetPosTicks();
		
		void SetTempo (ULONG tempo);
		ULONG GetTempo ();
		void RcvClock (ULONG date_ms);
		
		void ReceiveEvents (MidiEvPtr e);
};
 
typedef TSMPTEPlayer FAR * TSMPTEPlayerPtr;

#endif
