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
//	TPlayerSynchroniser.h		    
// ===========================================================================


#ifndef __TPlayerSynchroniser__
#define __TPlayerSynchroniser__


#include "TTempoMapVisitor.h"
#include "TScoreIterator.h"

#include "TSynchroniserInterface.h"
#include "TScheduler.h"

#include "TRunningState.h"
#include "TClockConverter.h"
#include "TScoreFollower.h"

//---------------------------
// Class TPlayerSynchroniser 
//---------------------------
/*!
	\brief TPlayerSynchroniser : a component which provides musical time to millisecond time conversion used with a TTickPlayer object.
*/

class TPlayerSynchroniser :public TSynchroniserInterface{

	protected:
	
		TSchedulerInterfacePtr 	fScheduler;
		TRunningStatePtr        fState;
		ULONG  			fOffset;
		TTempoMapVisitor	fTempoVisitor;
			
	public:
		
		TPlayerSynchroniser(TSchedulerInterfacePtr scheduler, TRunningStatePtr state, ULONG tpq)
			:fScheduler(scheduler),fState(state),fTempoVisitor(tpq){}
		virtual ~TPlayerSynchroniser(){}
		
		// Initialisation
		
		virtual void Init() {}
		
		// Time conversion
		
		ULONG ConvertTickToMs (ULONG date_tick) { return fOffset + fTempoVisitor.ConvertTickToMs(date_tick);}
		ULONG ConvertMsToTick (ULONG date_ms)   { return fTempoVisitor.ConvertMsToTick(date_ms - fOffset);}
	
		// Transport
		
		virtual void Start(){}
		virtual void Stop(){}
 		virtual void Cont(ULONG date_ticks){}
 		
		// Tempo management
		
 		virtual void  SetTempo (ULONG tempo) {}
 		virtual void  SetTempo (ULONG date_ticks, ULONG tempo){}
 		virtual	ULONG GetTempo (){return fTempoVisitor.GetTempo();}
 		
 		virtual void  RcvClock (ULONG date_ms){}
 		
 		virtual Boolean IsSchedulable(ULONG date_tick){ return false;};
 		
 		// Position management
 		
 		virtual void SetPosTicks (ULONG date_ticks){}
 		virtual ULONG GetPosTicks (){return 0;}
};

typedef TPlayerSynchroniser FAR * TPlayerSynchroniserPtr;


//------------------
// Class TTempoTask 
//------------------
/*!
  \brief Task to play events at the same date and possibly reschedule pending tick tasks if the Tempo has changed.
*/

class TTempoTask :public TTicksTask {

	friend class TPlayerSynchroniserInt;

	private:
	
		TPlayerSynchroniserInt* fSynchroniser;

	public: 
	
		TTempoTask (TPlayerSynchroniserInt* it):TTicksTask(),fSynchroniser(it){}
		void Execute (TMidiApplPtr appl, ULONG date);
};

typedef TTempoTask FAR * TTempoTaskPtr;

//------------------------------
// Class TPlayerSynchroniserInt 
//------------------------------
/*!
	\brief TPlayerSynchroniserInt : synchroniser for internal (TempoMap baed) synchronization
*/

class TPlayerSynchroniserInt :public TPlayerSynchroniser{

	friend class TTempoTask;

	private:
	
		TTempoTask	fTempoTask;
		TScoreIterator	fIterator;
		TScoreRefFollower fFollower;
		
		void  PlaySlice ();	
	
	public:
		
		TPlayerSynchroniserInt(TScorePtr score, TSchedulerInterfacePtr scheduler, TRunningStatePtr state, ULONG tpq)
			:TPlayerSynchroniser(scheduler,state,tpq),fTempoTask(this),fIterator(score),fFollower(fIterator,fTempoVisitor){}
		virtual ~TPlayerSynchroniserInt(){}
		
		void Init();
		void Start();
 		void Stop();
 		void Cont(ULONG date_ticks);
 		
 		ULONG GetPosTicks ();
 		void SetPosTicks (ULONG date_ticks);
 		
 		Boolean IsSchedulable(ULONG date_tick);
 	
};

typedef TPlayerSynchroniserInt FAR * TPlayerSynchroniserIntPtr;

//--------------------------------
// Class TPlayerSynchroniserClock 
//--------------------------------
/*!
	\brief TPlayerSynchroniserClock : synchroniser for clock synchronization
*/

class TPlayerSynchroniserClock :public TPlayerSynchroniser{

	private:
		
		ULONG	fRcv_clock;	 		// Date in ms of the previous received clock
		ULONG 	fNextdate_ticks;	// Date in ticks of the next (expected) clock
		ULONG 	fClock_count;		// Received clock count
		TClockConverter  fClockConverter;  
		
		void RcvFirstClock(ULONG date_ms);
		void RcvNextClock(ULONG date_ms);
		void NextClock(ULONG date_ms);
			
	public:
		
		TPlayerSynchroniserClock(TSchedulerInterfacePtr scheduler, TRunningStatePtr state, ULONG tpq)
			:TPlayerSynchroniser(scheduler,state,tpq),fClockConverter(tpq){}
		virtual ~TPlayerSynchroniserClock() {}
	
		void Init();
		void Start();
		void Stop();
 		void Cont(ULONG date_ticks);
		
 		ULONG GetPosTicks ();
 		void SetPosTicks (ULONG date_ticks);
 		
 		ULONG GetTempo (){return fTempoVisitor.GetTempo();}
 		
 		Boolean IsSchedulable(ULONG date_tick);
 		
 		void RcvClock (ULONG date_ms);
};


typedef TPlayerSynchroniserClock FAR * TPlayerSynchroniserClockPtr;


//------------------------------
// Class TPlayerSynchroniserExt 
//------------------------------
/*!
	\brief TPlayerSynchroniserExt : synchroniser for external synchronization
*/

class TPlayerSynchroniserExt :public TPlayerSynchroniser{

	public:
		
		TPlayerSynchroniserExt(TSchedulerInterfacePtr scheduler,  TRunningStatePtr state, ULONG tpq)
			:TPlayerSynchroniser(scheduler,state,tpq){} 
		virtual ~TPlayerSynchroniserExt(){}
		
		void Init();
		void Start();
		void Stop();
 		void Cont(ULONG date_ticks);
		
 		ULONG GetPosTicks ();
 		void SetPosTicks (ULONG date_ticks);
 		
 		ULONG GetTempo (){return fTempoVisitor.GetTempo();}
 		void SetTempo (ULONG tempo);
 		
 		Boolean IsSchedulable(ULONG date_tick);
 	
};

typedef TPlayerSynchroniserExt FAR * TPlayerSynchroniserExtPtr;


#endif
