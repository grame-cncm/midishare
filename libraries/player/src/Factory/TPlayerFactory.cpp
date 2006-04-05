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
//	TPlayerFactory.cpp			    
// ===========================================================================
//
//	Builds a Player depending of the current synchronization mode
// 
// 

#include "TPlayerFactory.h"
#include "TPlayer.h"

/*--------------------------------------------------------------------------*/

TPlayerFactory::TPlayerFactory (TPlayer* user):fUser(user) {}

/*--------------------------------------------------------------------------*/
// Creation of Players
/*--------------------------------------------------------------------------*/

TGenericPlayerInterfacePtr TPlayerFactory::CreatePlayer()
{ 
	TPlayerSynchroniserPtr 		synchro = 0;
	TEventDispatcherPtr 		receiver = 0;
	TSchedulerInterfacePtr 		scheduler = 0;
	TTickPlayerInterfacePtr 	tickplayer = 0;
	TChaserInterfacePtr 		chaser = 0;
	TPlayerInterfacePtr 		player1 = 0;
	TPlayerInterfacePtr 		player2 = 0;
	TPlayerInterfacePtr 		player3 = 0;
	TClockSenderPtr 			clocksender = 0;
	TClockConverterPtr 			clockconverter = 0;
	TLoopManagerPtr				loopmanager	 = 0;	
	TScoreInserterPtr			inserter = 0;
	TEventRecorderPtr       	recorder = 0;
	TScoreStatePtr          	scorestate = 0;
	TEventSenderInterfacePtr  	eventsender = 0;
	TTimeManagerPtr  			timemanager = 0;
	
	long res;
	
	switch (fUser->fOutput) {
		
		case kMidiShare:
			eventsender = new TMidiPlayer(fUser, &fUser->fTrackTable);
			res = eventsender->Init();
			break;
	}
	
	// If allocation error
	if (res != kNoErr) return 0;
	
	clockconverter = new TClockConverter(fUser->fTick_per_quarter); 
	loopmanager =  new TLoopManager (&fUser->fScore, fUser->fTick_per_quarter); 
	inserter = new TScoreInserter(&fUser->fScore, fUser->fTick_per_quarter);
	scheduler = new TScheduler(); // constructeur vide
	timemanager = new  TTimeManager(&fUser->fScore,fUser->fTick_per_quarter);
	
	
	switch (fUser->fSyncIn) {
		
		case kInternalSync: 
		case kSMPTESync:
			synchro = new TPlayerSynchroniserInt(&fUser->fScore, scheduler, &fUser->fRunningState,fUser->fTick_per_quarter);
			break;
		
		case kExternalSync: 
			synchro = new TPlayerSynchroniserExt(scheduler, &fUser->fRunningState, fUser->fTick_per_quarter);
			break;
			
		case kClockSync: 
			synchro = new TPlayerSynchroniserClock(scheduler, &fUser->fRunningState, fUser->fTick_per_quarter);
			break;
	}
	
	scheduler->Init(synchro, fUser); // Initialisation 
	
	tickplayer 	= new TTickPlayer(&fUser->fScore, eventsender, scheduler);
	chaser 		= new TChaserIterator(&fUser->fScore, eventsender);
	player1 	= new TSyncInPlayer(synchro,tickplayer,chaser);
	
	
	switch (fUser->fSyncOut) {
		
		case kClockSyncOut:
			clocksender = new TClockSender(scheduler, clockconverter, eventsender);
			player2 = new TSyncOutPlayer(player1,clocksender,clockconverter);
			break;
			
		case kNoSyncOut:
			player2 = player1;
			player1 = 0;
			break;
	}
	
	switch (fUser->fSyncIn) {
		
		case kSMPTESync:
			player3 = player2;
			break;
			
		default:
			player3 = new TRunningPlayer(player2,&fUser->fRunningState); // enrobage
			break;
	}
	
	// Incoming events handling
	// Recorder--->EventReceiver
	
	switch (fUser->fSyncIn) {
		
		case kInternalSync: 
			receiver = new TEventReceiver(player3,loopmanager,0);
			break;
		
		case kClockSync: 
			receiver = new TClockEventReceiver(player3,loopmanager,clockconverter,0);
			break;
			
		case  kExternalSync: 
			receiver = new TExtEventReceiver(player3,loopmanager,0);
			break;
		
		case  kSMPTESync: 
			receiver = new TSMPTEEventReceiver(player3,loopmanager,0);
			break;
	}
		
	recorder = new TEventRecorder(&fUser->fScore, synchro, &fUser->fRunningState,receiver);
	scorestate = new TScoreState(&fUser->fScore,fUser->fTick_per_quarter);
	
	// Affectation 
	fUser->fClockConverter =  clockconverter;
	fUser->fLoopManager = loopmanager;
	fUser->fInserter = inserter;
	fUser->fEventReceiver = recorder;
	fUser->fScoreState = scorestate;
	
	// Destructor allocation
	fDestructor = new TDestructor(synchro,receiver,scheduler,tickplayer,
					chaser,player1,player2,player3,clocksender,clockconverter,
					loopmanager,inserter,recorder,scorestate,eventsender,timemanager);
					
	TGenericPlayerInterfacePtr genericplayer = new TGenericPlayer (fUser->fEventReceiver, player3,timemanager);
					
	switch (fUser->fSyncIn) {
		
		case kSMPTESync:
			return new TSMPTEPlayer(genericplayer, &fUser->fRunningState, &fUser->fSmpteInfos,fUser);
			
		default:
			return genericplayer;
	}
}

/*--------------------------------------------------------------------------*/

void TPlayerFactory::DestroyPlayer(TGenericPlayerInterfacePtr* player)
{
	TGenericPlayerInterfacePtr tmp = *player;
	*player = 0; // To avoid access at Interrupt Level
	
	if (tmp) {
		delete(fDestructor);
		delete(tmp);
		fDestructor = 0;
	}
}

