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
//	TEventProducer.h			    
// ===========================================================================


#ifndef __TEventProducer__
#define __TEventProducer__


#include "TEventProducerInterface.h"
#include "TEventReceiverInterface.h"
#include "TFilterInterface.h"


//-------------------
// Class UEventTools 
//-------------------

class UEventTools {

	public :

		static Boolean CheckEnd (MidiEvPtr ev) { return (EvType(ev) == typeScoreEnd);}
		
		static MidiEvPtr MakeEnd () 
		{ 
			MidiEvPtr ev =  MidiNewEv(typeScoreEnd);
			assert(ev);
			return ev;
		}
		
		static MidiEvPtr CopyAndOffset (MidiEvPtr ev, ULONG off) 
		{ 
			MidiEvPtr copy = MidiCopyEv(ev);
	 		assert(copy);
	 		Date(copy) += off;
	 		return copy;
	 	}
	
};


//------------------------------
// Class TEventReceiverProducer 
//------------------------------
// Base class for all event producers and receivers

class TEventReceiverProducer : public TEventProducerInterface , public TEventReceiverInterface{

	public:

		virtual ~TEventReceiverProducer(){}
		
		void ReceiveEvents(MidiEvPtr e) {}
		MidiEvPtr ProduceEvent(ULONG date_ticks){return 0;}

};

typedef TEventReceiverProducer FAR * TEventReceiverProducerPtr;


//----------------------------
// Class TReactiveProducer 
//----------------------------
// Returns its producer event until the event filter returns true

class TReactiveProducer : public TEventReceiverProducer {

	private:
	
		TEventReceiverProducerPtr 	fProducer;	// Decorated producer
		MidiEvPtr  					fEnd;		// End event
		Boolean 					fStatus;    // Current status
		TFilterInterfacePtr			fFilter;	// An event matcher
		
	public:
	
		TReactiveProducer(TEventReceiverProducerPtr producer, TFilterInterfacePtr filter) 
		{
 			fProducer = producer;
 			fFilter = filter;
 			fEnd = UEventTools::MakeEnd();
 			fStatus = false;
 		}
 		
 		~TReactiveProducer() { if (fProducer) delete(fProducer); }
 		
 		MidiEvPtr ProduceEvent(ULONG date_ticks) 
 		{
 			if (fStatus) {
 				Date (fEnd)+=1;
 				return MidiCopyEv(fEnd);
 			}else {
 				MidiEvPtr res = fProducer->ProduceEvent(date_ticks);
 				Date(fEnd) = Date(res);
 				return res;
 			}
 		}
 		 				
 		void Init(ULONG  date_ticks) 
 		{ 
 			fStatus = false;
 			fProducer->Init(date_ticks); 
 		}
 		 		 
 		void ReceiveEvents(MidiEvPtr e) { fStatus = fFilter->FilterEv(e); }
 		
 		ULONG GetPosTicks(){ return (fStatus) ? Date(fEnd) : fProducer->GetPosTicks() ;}
 		// A REVOIR
 		void SetPosTicks(ULONG date_ticks){fProducer->SetPosTicks(date_ticks);}
	
  };


typedef TReactiveProducer FAR * TReactiveProducerPtr;


//----------------------------
// Class TScoreProducer 
//----------------------------
// Always returns and event : the last one (typeScoreEnd) will be returned with a date that increments each time

class TScoreProducer : public TEventReceiverProducer {

	private:
	
		MidiSeqPtr fScore;		// Internal score
		MidiEvPtr  fPos;		// Current pos
		MidiEvPtr  fEnd;		// End event
		ULONG 	   fOffset;    	// Offset in ticks
		
	public:
		
		TScoreProducer(MidiSeqPtr score) 
		{
			// Assume the score contains as least one event
			assert(score);
			assert(LastEv(score));
			
 			fScore = score;
 			fOffset = 0;
 			fEnd = UEventTools::MakeEnd();
 			Date (fEnd) = Date (LastEv(score));
 		}
 		
 		~TScoreProducer() { if (fScore) MidiFreeSeq(fScore); }
 		
 			
 		MidiEvPtr ProduceEvent(ULONG date_ticks)
 		{
 			MidiEvPtr curEv;
 			
 			if (fPos) {
 				curEv = UEventTools::CopyAndOffset(fPos,fOffset);
 				fPos = Link(fPos);
 				return curEv;
 			}else {
 				Date (fEnd)+=1;
 				return MidiCopyEv(fEnd);
 			}
 		}
 		
 				
 		void Init(ULONG  date_ticks) 
 		{
 			fOffset = date_ticks;
 			fPos = FirstEv(fScore);
 			Date (fEnd) = Date (LastEv(fScore));
 		}
 		
 		void ReceiveEvents(MidiEvPtr e){}
 		
 		ULONG GetPosTicks(){ return (fPos) ? Date(fPos)+fOffset : Date(fEnd)+fOffset;}
 		
 		void SetPosTicks(ULONG date_ticks)
 		{
			MidiEvPtr cur = FirstEv(fScore);
			while (cur && (Date (cur) < date_ticks)) { cur = Link(cur);}
			fPos = cur;
		}
};

typedef TScoreProducer FAR * TScoreProducerPtr;


//---------------------
// Class TSeqProducer 
//---------------------
// Sequence of producers


class TSeqProducer : public TEventReceiverProducer {

	private:
	
		TEventReceiverProducerPtr fProducer1;
		TEventReceiverProducerPtr fProducer2;
		TEventReceiverProducerPtr fCurrent;
		
	public:
		
		TSeqProducer (TEventReceiverProducerPtr exp1, TEventReceiverProducerPtr exp2) 
		{
	 		fProducer1 = exp1;
	 		fProducer2 = exp2;
	 		fCurrent = exp1;
	 	}
	 	
	 	~TSeqProducer () 
	 	{
	 		if (fProducer1) delete(fProducer1);
	 		if (fProducer2) delete(fProducer2);
	 	}
		 	
	 	MidiEvPtr ProduceEvent(ULONG date_ticks)
		{ 
			MidiEvPtr curEv =  fCurrent->ProduceEvent(date_ticks);
			
	 		if (!UEventTools::CheckEnd(curEv)) {
				return curEv;
			}else if (fCurrent == fProducer1) {
				fCurrent = fProducer2;
				fCurrent->Init(Date(curEv));
				return  fCurrent->ProduceEvent(date_ticks);
			}else {
				return  curEv;
			}
		}
	 		
		 void Init(ULONG date_ticks) 
		{
			if (fProducer1) {
	 			fCurrent = fProducer1;
	 			fCurrent->Init(date_ticks);
	 		}
	 	}
	 	
	 	void ReceiveEvents(MidiEvPtr e){fCurrent->ReceiveEvents(e);}	
 		
 		ULONG GetPosTicks(){return fCurrent->GetPosTicks();}
 		// A REVOIR
		void SetPosTicks(ULONG date_ticks){fCurrent->SetPosTicks(date_ticks);}
 };

typedef TSeqProducer FAR * TSeqProducerPtr;



//--------------------
// Class TMixProducer 
//--------------------
// Mix of producers


class TMixProducer  : public  TEventReceiverProducer {

	private:
	
		TEventReceiverProducerPtr fProducer1;
		TEventReceiverProducerPtr fProducer2;
		MidiEvPtr fLastEv1, fLastEv2;  	// Last produced event on each branch
		
		ULONG min (ULONG a, ULONG b) {return (a <= b) ? a : b;}
	
	public :

		TMixProducer (TEventReceiverProducerPtr exp1, TEventReceiverProducerPtr exp2) 
		{
	 		fProducer1 = exp1;
	 		fProducer2 = exp2;
	 		fLastEv1 = 0;
	 		fLastEv2 = 0;
	 	}
	 	
	 	~TMixProducer ()
	 	{
	 		if (fProducer1) delete(fProducer1);
	 		if (fProducer2) delete(fProducer2);
	 	}
	 		
		MidiEvPtr ProduceEvent(ULONG date_ticks)
	 	{ 
	 		MidiEvPtr ev1, ev2;
	 		
	 		if (!fLastEv1) fLastEv1 = fProducer1->ProduceEvent(date_ticks);
	 		if (!fLastEv2) fLastEv2 = fProducer2->ProduceEvent(date_ticks);
	 		
	 		ev1 =  fLastEv1;
	 		ev2 =  fLastEv2;
	 			
			if(UEventTools::CheckEnd(ev1)){
				if (UEventTools::CheckEnd(ev2)){
					return (Date(ev1) < Date(ev2)) ? ev2 : ev1;
				}else {
					fLastEv2 = 0;
					return ev2;
				}
			}else if(UEventTools::CheckEnd(ev2)){
				fLastEv1 = 0;
				return ev1;
			}else if (Date(ev1) < Date(ev2))  {
				fLastEv1 = 0;
				return ev1;
			}else {
				fLastEv2 = 0;
				return ev2;
			}
		}
		
	 		
	 	void Init(ULONG date_ticks) 
	 	{	
	 		fProducer1->Init(date_ticks);
	 		fProducer2->Init(date_ticks);
	 		fLastEv1 = 0;
	 		fLastEv2 = 0;
	 	}
	 	
	 	void ReceiveEvents(MidiEvPtr e)
	 	{
	 		fProducer1->ReceiveEvents(e);
	 		fProducer2->ReceiveEvents(e);
	 	}	
	 	
	 	
	 	ULONG GetPosTicks()
	 	{
	 		if (fLastEv1) 
	 			return min (Date(fLastEv1), fProducer2->GetPosTicks());
	 		else if (fLastEv2)
	 			return min (fProducer1->GetPosTicks(), Date(fLastEv2));
	 		else
	 			return min (fProducer1->GetPosTicks(), fProducer2->GetPosTicks());
	 	}
	 	
		void SetPosTicks(ULONG date_ticks)
		{
			fProducer1->SetPosTicks(date_ticks);
			fProducer2->SetPosTicks(date_ticks);
			fLastEv1 = 0;
	 		fLastEv2 = 0;
		}
	 		
 };
 
 typedef TMixProducer FAR * TMixProducerPtr;

//---------------------------------
// Class TDefEventReceiverProducer 
//---------------------------------
// Default event producer/receiver

class TDefEventReceiverProducer : public  TEventReceiverProducer{
	
	public :
 
 		TDefEventReceiverProducer() {}
 		~TDefEventReceiverProducer() {}
 		
 		void Init(ULONG date_ticks){}
 		ULONG GetPosTicks() { return 0;}
		void SetPosTicks(ULONG date_ticks){}
		MidiEvPtr ProduceEvent(ULONG date_ticks){ return UEventTools::MakeEnd();}
		
		void ReceiveEvents(MidiEvPtr e) {}
			
 };
  
 typedef TDefEventReceiverProducer FAR * TDefEventReceiverProducerPtr;

 
//--------------------------------
// Class TGenEventReceiverProducer 
//--------------------------------
// Generic producer/receiver : decorates an internal producer/receiver that can be changed (SetProducer/GetProducer)

class TGenEventReceiverProducer : public  TEventProducerInterface, public TEventDispatcher{

	private :
	
		TEventReceiverProducerPtr 	fEventProducerReceiver;
		TPlayerInterfacePtr  		fPlayer;
		ULONG						fLast_date;
	
	public :
 
 		TGenEventReceiverProducer() : TEventDispatcher(){ fEventProducerReceiver = new TDefEventReceiverProducer();}
 		
 		TGenEventReceiverProducer(TEventReceiverProducerPtr producer, TPlayerInterfacePtr player, TEventDispatcherPtr successor) 
 			: TEventDispatcher(successor) { 
 			fEventProducerReceiver = producer;
 			fPlayer = player;
 			fLast_date = 0;
 		}
 		virtual ~TGenEventReceiverProducer() {if (fEventProducerReceiver) delete(fEventProducerReceiver);}
 		
 		void Init(ULONG date_ticks){ fEventProducerReceiver->Init(date_ticks);}
 		
 		ULONG GetPosTicks() { 
 			fLast_date = fEventProducerReceiver->GetPosTicks();
 			return fLast_date;
 		}
		void SetPosTicks(ULONG date_ticks){ fEventProducerReceiver->SetPosTicks(date_ticks);}
		MidiEvPtr ProduceEvent(ULONG date_ticks){ return fEventProducerReceiver->ProduceEvent(date_ticks);}
		
		void ReceiveEvents(MidiEvPtr e)
		{ 
			fEventProducerReceiver->ReceiveEvents(e);
			TEventDispatcher::ReceiveEvents(e); // Calls the next object ReceiveEvents method
		
			// Stops the player and continues if the next event is before the already scheduled one
			if (fEventProducerReceiver->GetPosTicks() < fLast_date) {
				fPlayer->Stop();
				fPlayer->Cont();
			}
		}	
		
		void SetEventProducer(TEventReceiverProducerPtr arg) 
		{ 
			if (fEventProducerReceiver) delete (fEventProducerReceiver);
			fEventProducerReceiver = arg;
		}
		
		TEventProducerInterfacePtr GetEventProducer() { return fEventProducerReceiver;}
		
		void SetPlayer(TPlayerInterfacePtr player) {fPlayer = player;}
		TPlayerInterfacePtr GetPlayer() { return fPlayer; }
	
 };
 
 
typedef TGenEventReceiverProducer FAR * TGenEventReceiverProducerPtr;



//-------------------------
// Class TProducerFactory
//--------------------------

 class TProducerFactory  {
 
 	public:

		static TEventReceiverProducerPtr MakeScoreProducer (MidiSeqPtr seq) { return new TScoreProducer(seq);}
		
		static TEventReceiverProducerPtr MakeReactiveProducer (TEventReceiverProducerPtr p1, TFilterInterfacePtr f) 
		{ 
			return new TReactiveProducer (p1,f);
		}
	
		static TEventReceiverProducerPtr MakeSeqProducer (TEventReceiverProducerPtr p1, TEventReceiverProducerPtr p2) 
		{ 
			return new TSeqProducer(p1,p2);
		}

		static TEventReceiverProducerPtr MakeMixProducer(TEventReceiverProducerPtr p1, TEventReceiverProducerPtr p2) 
		{ 
			return new TMixProducer(p1,p2);
		}
		
};
 
 
 #endif
 
 
 
 
 
 
