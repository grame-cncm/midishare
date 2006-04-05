/*

  Copyright © Grame 1996-2006

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
//	TGenericPlayer.h			    
// ===========================================================================

#ifndef __TGenericPlayer__
#define __TGenericPlayer__

#include "TEventRecorder.h"
#include "TPlayerInterface.h"
#include "TTimeManager.h"
#include "TGenericPlayerInterface.h"


//----------------------
// Class TGenericPlayer 
//----------------------
/*!
  \brief A TGenericPlayer uses three components :
  <UL>
  	<LI> a synchronizable player implementing the TPlayerInterface 
  	<LI> a event receiver : a component implementing the TEventReceiverInterface
  	<LI> a time manager : a component which converts date in BBU and Ms in ticks date (and vice-versa)
  </UL>
*/


class TGenericPlayer : public TGenericPlayerInterface {

	private :
	
		TEventReceiverInterfacePtr 	fReceiver;
		TPlayerInterfacePtr 		fPlayer;
		TTimeManagerPtr			  	fTimeManager;
		
	
	public:
	 
		TGenericPlayer (TEventReceiverInterfacePtr receiver, TPlayerInterfacePtr player, TTimeManagerPtr timemanager)
			:fReceiver(receiver),fPlayer(player),fTimeManager(timemanager) {}
		virtual ~TGenericPlayer() {} 
			
		//------------------
		// Player Interface 
		//------------------
		
		void Start() {fPlayer->Start();}
		void Stop()  {fPlayer->Stop();}
		void Pause() {fPlayer->Pause();}
		void Cont()  {fPlayer->Cont();}
		
		void PlaySliceForward()  {fPlayer->PlaySliceForward();}
 		void PlaySliceBackward() {fPlayer->PlaySliceBackward();}

		void SetPosTicks(ULONG date_ticks) {fPlayer->SetPosTicks(date_ticks);}
		void SetPosBBU(const TPos& pos)	{fPlayer->SetPosTicks(fTimeManager->ConvertBBUToTick(pos));}
		void SetPosMs(ULONG date_ms)		{fPlayer->SetPosTicks(fTimeManager->ConvertMsToTick(date_ms));}
		
		ULONG GetPosTicks() { return fPlayer->GetPosTicks();}
		
		void RcvClock(ULONG date_ms) {fPlayer->RcvClock(date_ms);}
		
		void SetTempo(ULONG tempo)	{fPlayer->SetTempo(tempo);}
		ULONG GetTempo() 			{return fPlayer->GetTempo();}
		 
		//--------------------
		// Receiver Interface  
		//--------------------
		
		void ReceiveEvents (MidiEvPtr e) {fReceiver->ReceiveEvents(e);}
};

typedef TGenericPlayer FAR * TGenericPlayerPtr;

#endif

