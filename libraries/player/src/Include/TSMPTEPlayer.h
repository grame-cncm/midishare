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

class TSMPTETask :public TMidiTask {

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

class TSMPTEPlayer :public TGenericPlayerInterface {

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
