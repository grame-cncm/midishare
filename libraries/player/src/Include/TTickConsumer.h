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
//	TTickConsumer.h			    
// ===========================================================================
//
//	Consumer : use a TEventProducer object to produce events to be played
//


#ifndef __TTickConsumer__
#define __TTickConsumer__

 
#include "TTickPlayer.h"
#include "TEventProducerInterface.h"


//-----------------------
// Class TTickConsumer 
//-----------------------

class TTickConsumer : public TTickPlayerInterface{
	
	friend class TPlayTask2;
	
	private:
	
		TPlayTask2*      	      	fPlayTask;
		TEventSenderInterfacePtr  	fEventUser;
		TSchedulerInterfacePtr	  	fScheduler;
		TEventProducerInterfacePtr	fEventProducer;
		
		void PlaySlice (ULONG date_ms);
		
	public :
 
 		TTickConsumer(TEventSenderInterfacePtr user, TSchedulerInterfacePtr scheduler, TEventProducerInterfacePtr producer);
 		~TTickConsumer();
 		
 		void Init();
 		void Start();
		void Stop();	
		void Cont(ULONG date_ticks);
		
 		void PlaySliceForward ();
 		void PlaySliceBackward ();
 		
 		ULONG GetPosTicks () ;
 		void  SetPosTicks (ULONG date_ticks);
  };

typedef TTickConsumer FAR * TTickConsumerPtr;


//-----------------------
// Class TPlayTask2
//-----------------------

class TPlayTask2 : public TTicksTask {

	private :
	
		TTickConsumerPtr fPlayer;

	public : 
	
		TPlayTask2 (TTickConsumerPtr it):TTicksTask() {fPlayer =  it;}
		void Execute (TMidiApplPtr appl , ULONG date_ms) {fPlayer->PlaySlice(date_ms);}
};


typedef TPlayTask2 FAR * TPlayTask2Ptr;


#endif
