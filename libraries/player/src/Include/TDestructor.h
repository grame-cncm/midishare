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
//	TDestructor.h			    
// ===========================================================================
//
//	Keeps trace of all objects allocated by the PlayerFactory
// 

#ifndef __TDestructor__
#define __TDestructor__


#include "TPlayerSynchroniser.h"
#include "TSynchroniserInterface.h"
#include "TTickPlayer.h"
#include "TChaserIterator.h"
#include "TPlayerInterface.h"
#include "TClockSender.h"
#include "TLoopManager.h"
#include "TEventRecorder.h"
#include "TScoreState.h"
#include "TTimeManager.h"
#include "TGenericPlayer.h"
#include "TSynchroniserInterface.h"
#include "TScoreInserter.h"

#include "TMidiPlayer.h"
#include "TSyncOutPlayer.h"
#include "TSyncInPlayer.h"
#include "TSMPTEPlayer.h"
#include "TEventReceiver.h"
#include "TRunningPlayer.h"
#include "TEventModifier.h"
#include "TCombinerPlayer.h"



class TDestructor {

	private:
	
		TPlayerSynchroniserPtr 		fSynchroniser;
		TEventDispatcherPtr 	  	fReceiver;
		TSchedulerInterfacePtr 		fScheduler;
		TTickPlayerInterfacePtr 	fTickplayer;
		TChaserInterfacePtr 		fChaser;
		TPlayerInterfacePtr 		fPlayer1;
		TPlayerInterfacePtr 		fPlayer2;
		TPlayerInterfacePtr 		fPlayer3;
		TClockSenderPtr 			fClockSender;
		TClockConverterPtr      	fClockConverter;
		TLoopManagerPtr         	fLoopManager;
		TScoreFollowerPtr       	fInserter;
		TEventRecorderPtr			fRecorder;
		TEventModifierPtr			fModifier;
		TScoreStatePtr   			fScoreState;
		TEventSenderInterfacePtr  	fEventSender;
		TTimeManagerPtr				fTimeManager;
		
	public:
		
		TDestructor(TPlayerSynchroniserPtr 		synchro,
					TEventDispatcherPtr	 		receiver,
					TSchedulerInterfacePtr 		scheduler,
					TTickPlayerInterfacePtr 	tickplayer,
					TChaserInterfacePtr 		chaser,
					TPlayerInterfacePtr 		player1,
					TPlayerInterfacePtr 		player2,
					TPlayerInterfacePtr 		player3,
					TClockSenderPtr 			clocksender,
					TClockConverterPtr      	clockconverter,
					TLoopManagerPtr         	loopmanager,
					TScoreFollowerPtr       	inserter,
					TEventRecorderPtr			recorder,
					TEventModifierPtr			modifier,
					TScoreStatePtr   			scorestate,
					TEventSenderInterfacePtr  	eventsender,
					TTimeManagerPtr				timemanager
					)

		{
			fSynchroniser = synchro;
			fReceiver = receiver;
			fScheduler = scheduler;
			fTickplayer = tickplayer;
			fChaser = chaser;
			fPlayer1 = player1;
			fPlayer2 = player2;
			fPlayer3 = player3;
			fClockSender = clocksender;
			fClockConverter = clockconverter;
			fLoopManager = loopmanager;
			fInserter = inserter;
			fRecorder = recorder;
			fModifier = modifier;
			fScoreState = scorestate;
			fEventSender = eventsender;
			fTimeManager = timemanager;
		}
				
		~TDestructor()
		{
			// the scheduler must be desallocated first
			
			if (fScheduler)		delete(fScheduler);
			if (fSynchroniser) 	delete(fSynchroniser);
			if (fReceiver)		delete(fReceiver);
			if (fTickplayer)	delete(fTickplayer);
			if (fChaser)		delete(fChaser);
			if (fPlayer1)		delete(fPlayer1);
			if (fPlayer2)		delete(fPlayer2);
			if (fPlayer3)		delete(fPlayer3);
			if (fClockSender)	delete(fClockSender);
			if (fClockConverter)delete(fClockConverter);
			if (fLoopManager)	delete(fLoopManager);
			if (fInserter)		delete(fInserter);
			if (fRecorder)		delete(fRecorder);
			if (fModifier)		delete(fModifier);
			if (fScoreState )	delete(fScoreState);
			if (fEventSender) 	delete(fEventSender);
			if (fTimeManager) 	delete(fTimeManager);
		}

};

typedef TDestructor FAR * TDestructorPtr;
#endif
