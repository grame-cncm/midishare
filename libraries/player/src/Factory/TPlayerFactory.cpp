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
//	TPlayerFactory.cpp			    
// ===========================================================================
//
//	Builds a Player depending of the current synchronization mode
// 
// 

#include "TPlayerFactory.h"
#include "TPlayer.h"

/*--------------------------------------------------------------------------*/

TPlayerFactory::TPlayerFactory (TPlayer* user) { fUser = user; }

/*--------------------------------------------------------------------------*/
// Creation of Players
/*--------------------------------------------------------------------------*/

TGenericPlayerInterfacePtr TPlayerFactory::CreatePlayer ()
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
	TEventModifierPtr       	modifier = 0;
	TScoreStatePtr          	scorestate = 0;
	TEventSenderInterfacePtr  	eventsender = 0;
	TTimeManagerPtr  			timemanager = 0;
	
	
	long res;
	
	switch (fUser->fOutput) {
		
		case kMidiShare:
			eventsender = new TMidiPlayer(fUser, fUser->fTrackTable);
			res = eventsender->Init();
			break;
	}
	
	// If allocation error
	if (res != kNoErr) return 0;
	
	clockconverter = new TClockConverter(fUser->fTick_per_quarter); 
	loopmanager =  new TLoopManager (fUser->fScore, fUser->fTick_per_quarter); 
	inserter = new TScoreInserter(fUser->fScore, fUser->fTick_per_quarter);
	scheduler= new TScheduler(); // constructeur vide
	timemanager = new  TTimeManager(fUser->fScore,fUser->fTick_per_quarter);
	
	
	switch (fUser->fSyncIn) {
		
		case kInternalSync: 
		case kSMPTESync:
			synchro = new TPlayerSynchroniserInt(fUser->fScore, scheduler, fUser->fRunningState,fUser->fTick_per_quarter);
			break;
		
		case kExternalSync: 
			synchro = new TPlayerSynchroniserExt(scheduler, fUser->fRunningState,fUser->fTick_per_quarter);
			break;
			
		case kClockSync: 
			synchro = new TPlayerSynchroniserClock(scheduler, fUser->fRunningState,fUser->fTick_per_quarter);
			break;
	}
	
	scheduler->Init(synchro, fUser); // Initialisation 
	
	tickplayer 	= new TTickPlayer(fUser->fScore, eventsender, scheduler);
	// A FINIR
	TCombinerPlayerPtr combiner = new TCombinerPlayer(tickplayer, 0);
	chaser 		= new TChaserIterator(fUser->fScore, eventsender);
	player1 	= new TSyncInPlayer(synchro,combiner,chaser);
	

	
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
			player3 = new TRunningPlayer(player2,fUser->fRunningState); // enrobage
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
		
	
	//recorder = new TEventModifier(eventsender,fUser->fScore, synchro, fUser->fRunningState,receiver);
	
	
	// Chaine de traitements des ŽvŽnements
	//--------------------------------------
	// Modifier ==> Recorder ==> Receiver
	
	
	recorder = new TEventRecorder(fUser->fScore, synchro, fUser->fRunningState,receiver); // en fin de chaine cau utiliser les event
	modifier = new TEventModifier(eventsender, synchro, scheduler,fUser->fRunningState,recorder);

	combiner->SetPlayer2(modifier);
	scorestate = new TScoreState(fUser->fScore,fUser->fTick_per_quarter);


	
	// Affectation 
	fUser->fClockConverter =  clockconverter;
	fUser->fLoopManager = loopmanager;
	fUser->fInserter = inserter;
	fUser->fEventReceiver = recorder;
	fUser->fScoreState = scorestate;
	
	// Destructor allocation
	fDestructor = new TDestructor(synchro,receiver,scheduler,tickplayer,
					chaser,player1,player2,player3,clocksender,clockconverter,
					loopmanager,inserter,recorder,modifier,scorestate,eventsender,timemanager);
					
	TGenericPlayerInterfacePtr genericplayer = new TGenericPlayer (modifier, player3,timemanager);
	
	switch (fUser->fSyncIn) {
		
		case kSMPTESync:
			return new TSMPTEPlayer(genericplayer,fUser->fRunningState, fUser->fSmpteInfos,fUser);
			break;
			
		default:
			return genericplayer;
			break;
	}
}

/*--------------------------------------------------------------------------*/

void TPlayerFactory::DestroyPlayer(TGenericPlayerInterfacePtr* player)
{
	TGenericPlayerInterfacePtr tmp = *player;
	*player = 0; // To avoid access at Interrupt Level
	
	if(tmp) {
		delete(fDestructor);
		delete(tmp);
		fDestructor = 0;
	}
}

