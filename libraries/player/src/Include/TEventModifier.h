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
// TEventModifier.h			    
// ===========================================================================
//
//	Real-time transformation of incoming events : a TEventModifier object
//  implements both TEventDispatcher and TTickPlayerInterface interfaces
// 

#ifndef __TEventModifier__
#define __TEventModifier__

 
#include "TEventSenderInterface.h"
#include "TScoreIterator.h"
#include "TEventReceiverInterface.h"
#include "TPlayerScore.h"
#include "TPlayerSynchroniser.h"
#include "TTickPlayer.h"
#include "TScheduler.h"
#include "TChaserIterator.h"
#include "UMidi.h"

#define MAXLONG 0x7fffffff  


#define trackNum		1
#define typeFunOn		21
#define typeFunOff		22

#define MatchEvent2(e1,e2) ((Pitch((e1)) == Pitch((e2))) && (Chan ((e1))== Chan ((e2))) &&(Port ((e1)) == Port ((e2))))


//-----------------------
// Class TFun
//-----------------------

class TFun {

	public :

	 	TFun* 		fNext;  	// Pointers for linked list 
	 	MidiEvPtr 	fFun;   	// Transformation
	 	MidiEvPtr 	fListEvent; // List of transformed events
	 	
	 	// Use of the MidiShare memory allocator

		void*	operator new (size_t)				{ return MidiNewCell(); }  
		void 	operator delete(void* p, size_t) 	{ MidiFreeCell((MidiEvPtr)p); }
			
		TFun	() 				{ fNext = 0; fFun = fListEvent = 0; }
		TFun	(MidiEvPtr e) 	{ fNext = 0; fFun = e; fListEvent = 0; }

		virtual ~TFun()  		{ MidiFreeEv(fFun); UMidi::FreeOneLine(fListEvent);} 
		
		void SetNext(TFun* fun) {fNext = fun;}
		TFun* GetNext() { return fNext;}
		
		ULONG GetGeneration() {return (MidiGetField(fFun,0)/65536);}
		
		short GetPitch() {return (MidiGetField(fFun,1)/65536)-128;}
		short GetVel() {return (MidiGetField(fFun,1)%65536)-128;}
		short GetChan() {return Chan(fFun);}
		short GetPort() {return Port(fFun);}
		ULONG GetCoeff(){return MidiGetField(fFun,3);}
		ULONG GetOffset(){return MidiGetField(fFun,2);}
		
		short GetEvType() {return EvType(fFun);}
		void SetEvType(short type) { EvType(fFun) = type;}
		ULONG GetDate(){return Date(fFun);}
		
		void IncRefCount() { MidiSetField(fFun,0, MidiGetField(fFun,0)+1);}
		void DecRefCount() { MidiSetField(fFun,0, MidiGetField(fFun,0)-1);}
		short GetRefCount()   { return (MidiGetField(fFun,0)%65536);}
		
		Boolean isStartFun () {return (EvType(fFun) == typeFunOn);}
		Boolean isEndFun () {return (EvType(fFun) == typeFunOff);}
		Boolean isLastEvent() {return ((EvType(fFun) == typeFunOff) && (GetRefCount() == 0));}
		
		void InsertEvent(MidiEvPtr e);
		MidiEvPtr RemoveEvent(MidiEvPtr e);
		MidiEvPtr RemoveEventAux(MidiEvPtr e);
		
		
};

typedef TFun FAR * TFunPtr;


//-----------------------
// Class TEventModifier
//-----------------------
// 
// Event reception is always on : it has to be filtered
//

class TEventModifier :public TEventDispatcher, public TTickPlayerInterface {

	friend class TPlayTask1;

	private:
		
		TFunPtr		fTransSeq;			// List of Transformations
		TPlayTask1* fPlayTask;			// Playing task
		ULONG		fLast_task_ticks; 	// Date of the last playing task	
	
		TScoreIteratorPtr	 		fIterator,fIterator1;
		TPlayerScorePtr			 	fScore;
		TPlayerSynchroniserPtr   	fSynchroniser;
		TEventSenderInterfacePtr    fEventUser;
		TSchedulerInterfacePtr		fScheduler;
		TChaserInterfacePtr 	  	fChaser;
		
		
		void RemoveFun(ULONG generation);
		void InsertFun(MidiEvPtr e);
		
		void Modify(MidiEvPtr e);
		void PlaySlice (ULONG date_ms);
		
		Boolean isFunOn() {return (fTransSeq != 0);}
		
		short Generation(MidiEvPtr e) 	{return (MidiGetField(e,0)/65536);}
		
		MidiEvPtr HandleKeyOn(MidiEvPtr ev, TFunPtr fun);
		MidiEvPtr HandleKeyOff(MidiEvPtr ev, TFunPtr fun);
		
		short Range(short val, short min, short max) { return (val < min) ? min : (val > max) ? max : val;}
		
		void ScheduleTickTask (ULONG date_ticks);
		
	public:
	
		TEventModifier(TEventSenderInterfacePtr sender, TPlayerSynchroniserPtr synchro,
		TSchedulerInterfacePtr	scheduler, TEventDispatcherPtr successor) ;
		
		~TEventModifier();
	
		// Interface TEventDispatcher
		void ReceiveEvents (MidiEvPtr e);	
		
		// Interface TTickPlayerInterface
		void Init();
		void Start();
		void Stop();
		void Cont(ULONG date_ticks);
		
		void PlaySliceForward(){}
		void PlaySliceBackward(){}
	
		void SetPosTicks (ULONG date_ticks);		
		ULONG GetPosTicks();
	
};

typedef TEventModifier FAR * TEventModifierPtr;


//-----------------------
// Class TPlayTask1
//-----------------------

class TPlayTask1 : public TTicksTask {

	private :
	
		TEventModifierPtr fPlayer;

	public : 
	
		TPlayTask1 (TEventModifierPtr it):TTicksTask() {fPlayer =  it;}
		void Execute (TMidiApplPtr appl , ULONG date_ms) {fPlayer->PlaySlice(date_ms);}
};


typedef TPlayTask1 FAR * TPlayTask1Ptr;



#endif
