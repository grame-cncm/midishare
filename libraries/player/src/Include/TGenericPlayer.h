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
//	TPlayerGeneric.h			    
// ===========================================================================
//  A TPlayerGeneric uses three components:
//  - a synchronizable player implementing the TPlayerInterface 
// 	- a event receiver : a component implementing the TEventReceiverInterface
//  - a time manager : a component which converts dates in BBU and Ms in ticks dates (and vice-versa)
//

#ifndef __TGenericPlayer__
#define __TGenericPlayer__

#include "TEventRecorder.h"
#include "TPlayerInterface.h"
#include "TTimeManager.h"
#include "TGenericPlayerInterface.h"


//----------------------
// Class TGenericPlayer 
//----------------------

class TGenericPlayer :public TGenericPlayerInterface{

	private :
	
		TEventReceiverInterfacePtr 	fReceiver;
		TPlayerInterfacePtr 		fPlayer;
		TTimeManagerPtr			  	fTimeManager;
		
	
	public:
	 
		TGenericPlayer (TEventReceiverInterfacePtr receiver, TPlayerInterfacePtr player,
		 	TTimeManagerPtr timemanager)
		{
			fReceiver = receiver;
			fPlayer = player;
			fTimeManager = timemanager;
		}
		
		~TGenericPlayer (){} 
			
		//----------------------
		// Player Interface 
		//----------------------
			
		void Start() {fPlayer->Start();}
		void Stop()  {fPlayer->Stop();}
		void Pause() {fPlayer->Pause();}
		void Cont()  {fPlayer->Cont();}
		
		void PlaySliceForward()  {fPlayer->PlaySliceForward();}
 		void PlaySliceBackward() {fPlayer->PlaySliceBackward();}

		void SetPosTicks (ULONG date_ticks) {fPlayer->SetPosTicks(date_ticks);}
		void SetPosBBU (const TPos& pos)	{fPlayer->SetPosTicks(fTimeManager->ConvertBBUToTick(pos));}
		void SetPosMs (ULONG date_ms)		{fPlayer->SetPosTicks(fTimeManager->ConvertMsToTick(date_ms));}
		
		ULONG GetPosTicks(){ return fPlayer->GetPosTicks();}
		
		void RcvClock (ULONG date_ms) {fPlayer->RcvClock(date_ms);}
		
		void SetTempo (ULONG tempo)	{fPlayer->SetTempo(tempo);}
		ULONG GetTempo () 			{return fPlayer->GetTempo();}
		 
		//----------------------
		// Receiver Interface  
		//----------------------
		
		void ReceiveEvents (MidiEvPtr e) {fReceiver->ReceiveEvents(e);}
};

typedef TGenericPlayer FAR * TGenericPlayerPtr;


#endif

