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
//	TTickPlayer.h			    
// ===========================================================================
//
//	Player in ticks 
//


#ifndef __TTickPlayer__
#define __TTickPlayer__

 
#include "TScoreIterator.h"
#include "TSliceVisitor.h"
#include "TScheduler.h"


//----------------------------
// Class TTickPlayerInterface 
//----------------------------

class TTickPlayerInterface {
	
	public :
 
 		virtual ~TTickPlayerInterface() {}
 		
 		virtual void Start() = 0;
		virtual void Stop() = 0;
		virtual void Cont(ULONG date_ticks) = 0;
		
 		virtual void PlaySliceForward () = 0;
 		virtual void PlaySliceBackward () = 0;
 		
 		virtual ULONG GetPosTicks () = 0;
 		virtual void  SetPosTicks (ULONG date_ticks) = 0;
  };

typedef TTickPlayerInterface FAR * TTickPlayerInterfacePtr;


//-----------------------
// Class TTickPlayer 
//-----------------------

class TTickPlayer : public TTickPlayerInterface{
	
	friend class TPlayTask;
	
	private:
	
		TPlayTask*      	    fPlayTask;
		TScoreIteratorPtr	    fIterator;
		TSliceVisitorPtr   		fSliceVisitor;
		TEventSenderInterfacePtr  fEventUser;
		TSchedulerInterfacePtr	fScheduler;
		
		void PlaySlice (ULONG date_ms);
		
	public :
 
 		TTickPlayer(TScorePtr score, TEventSenderInterfacePtr user, TSchedulerInterfacePtr scheduler);
 		~TTickPlayer();
 		
 		void Start();
		void Stop();	
		void Cont(ULONG date_ticks);
		
 		void PlaySliceForward ();
 		void PlaySliceBackward ();
 		
 		ULONG GetPosTicks () ;
 		void  SetPosTicks (ULONG date_ticks);
  };

typedef TTickPlayer FAR * TTickPlayerPtr;


//-----------------------
// Class TPlayTask 
//-----------------------

class TPlayTask : public TTicksTask {

	private :
	
		TTickPlayerPtr fPlayer;

	public : 
	
		TPlayTask (TTickPlayerPtr it):TTicksTask() {fPlayer =  it;}
		void Execute (TMidiApplPtr appl , ULONG date_ms) {fPlayer->PlaySlice(date_ms);}
};


typedef TPlayTask FAR * TPlayTaskPtr;


#endif
