/*

  Copyright � Grame 1996-2004

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
//	TDestructor.h			    
// ===========================================================================

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

/*!
\brief	Keeps trace of all objects allocated by the TPlayerFactory.
*/

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
		TClockSenderPtr				fClockSender;
		TClockConverterPtr			fClockConverter;
		TLoopManagerPtr				fLoopManager;
		TScoreFollowerPtr			fInserter;
		TEventRecorderPtr			fRecorder;
		TScoreStatePtr				fScoreState;
		TEventSenderInterfacePtr	fEventSender;
		TTimeManagerPtr				fTimeManager;
		
	public:
		
		TDestructor(TPlayerSynchroniserPtr 	synchro,
                            TEventDispatcherPtr	 	receiver,
                            TSchedulerInterfacePtr 	scheduler,
                            TTickPlayerInterfacePtr tickplayer,
                            TChaserInterfacePtr 	chaser,
                            TPlayerInterfacePtr 	player1,
                            TPlayerInterfacePtr 	player2,
                            TPlayerInterfacePtr 	player3,
                            TClockSenderPtr 		clocksender,
                            TClockConverterPtr      clockconverter,
                            TLoopManagerPtr         loopmanager,
                            TScoreFollowerPtr       inserter,
                            TEventRecorderPtr		recorder,
                            TScoreStatePtr   		scorestate,
                            TEventSenderInterfacePtr eventsender,
                            TTimeManagerPtr	timemanager
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
			fScoreState = scorestate;
			fEventSender = eventsender;
			fTimeManager = timemanager;
		}
				
		virtual ~TDestructor()
		{
			// The scheduler must be desallocated first
			
			if (fScheduler)		delete(fScheduler);
			if (fSynchroniser) 	delete(fSynchroniser);
			if (fReceiver)		delete(fReceiver);
			if (fTickplayer)	delete(fTickplayer);
			if (fChaser)		delete(fChaser);
			if (fPlayer1)		delete(fPlayer1);
			if (fPlayer2)		delete(fPlayer2);
			if (fPlayer3)		delete(fPlayer3);
			if (fClockSender)	delete(fClockSender);
			if (fClockConverter)	delete(fClockConverter);
			if (fLoopManager)	delete(fLoopManager);
			if (fInserter)		delete(fInserter);
			if (fRecorder)		delete(fRecorder);
			if (fScoreState )	delete(fScoreState);
			if (fEventSender) 	delete(fEventSender);
			if (fTimeManager) 	delete(fTimeManager);
		}

};

typedef TDestructor FAR * TDestructorPtr;

#endif
