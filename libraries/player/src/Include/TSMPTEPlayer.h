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
//	TSMPTEPlayer.h	   			 
// ===========================================================================
//
//	Player in kSMPTESync mode
// 


#ifndef __TSMPTEPlayer__
#define __TSMPTEPlayer__


#include "TSyncInPlayer.h"
#include "TRunningState.h"
#include "TSMPTEInfos.h"
#include "TGenericPlayerInterface.h"


//-----------------------
// Class TSMPTEPlayer
//-----------------------


class TSMPTEPlayer :public TGenericPlayerInterface {

	friend class TSMPTETask;
	
	 private:
	 
	 	TRunningStatePtr  			fRunningState;
	 	TGenericPlayerInterfacePtr  fPlayer;
	 	TMidiApplPtr        		fMidiAppl;
	 	
	 	TSMPTEInfosPtr	fSmpteInfos;
		TSMPTETask* 	fSMPTEtask;
	 	
	 	void StartAtSMPTEOffset();  
	 	
	  public:
	  	
	 	TSMPTEPlayer (TGenericPlayerInterfacePtr player, TRunningStatePtr state, TSMPTEInfosPtr smpte, TMidiApplPtr appl);
	 	~TSMPTEPlayer ();
	  
	  	void Start() ;
		void Stop();
		void Pause();
		void Cont() ;
		
		void PlaySliceForward();
 		void PlaySliceBackward() ;

		void SetPosTicks (ULONG date_ticks) ;
		void SetPosBBU (const TPos& pos) ;
		void SetPosMs (ULONG date_ms);
		
		ULONG GetPosTicks();
		
		void SetTempo (ULONG tempo);
		ULONG GetTempo ();
		void  SetTempoFactor (float factor);
		void RcvClock (ULONG date_ms);
		
		void ReceiveEvents (MidiEvPtr e);
};
 
 
 typedef TSMPTEPlayer FAR * TSMPTEPlayerPtr;
 
 
//-----------------------
// Class TSMPTETask 
//-----------------------

class TSMPTETask :public TMidiTask {
	TSMPTEPlayerPtr fPlayer;

	public : 
		TSMPTETask (TSMPTEPlayerPtr it) {fPlayer =  it;}
		void Execute (TMidiApplPtr appl, ULONG date) {fPlayer->Start();}
};

typedef TSMPTETask FAR * TSMPTETaskPtr;


#endif
