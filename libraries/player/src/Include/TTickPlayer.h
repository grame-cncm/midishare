/*

  Copyright © Grame 1996-2004

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
//	TTickPlayer.h			    
// ===========================================================================

#ifndef __TTickPlayer__
#define __TTickPlayer__

#include "TScoreIterator.h"
#include "TSliceVisitor.h"
#include "TScheduler.h"

//----------------------------
// Class TTickPlayerInterface 
//----------------------------
/*!
 \brief An interface for ticks based players.
*/

class TTickPlayerInterface {
	
	public :
 
 		virtual ~TTickPlayerInterface() {}
 		
 		virtual void Init() = 0;
 		virtual void Start() = 0;
		virtual void Stop() = 0;
		virtual void Cont(ULONG date_ticks) = 0;
		
 		virtual void PlaySliceForward () = 0;
 		virtual void PlaySliceBackward () = 0;
 		
 		virtual ULONG GetPosTicks () = 0;
 		virtual void  SetPosTicks (ULONG date_ticks) = 0;
  };

typedef TTickPlayerInterface FAR * TTickPlayerInterfacePtr;


//-----------------
// Class TPlayTask 
//-----------------
/*!
  \brief Task to play events at the same date.
*/

class TPlayTask : public TTicksTask {

	friend class TTickPlayer;

	private :
	
		TTickPlayer* fPlayer;

	public : 
	
		TPlayTask (TTickPlayer* it):TTicksTask(),fPlayer(it){}
		void Execute (TMidiApplPtr appl , ULONG date_ms);
};

typedef TPlayTask FAR * TPlayTaskPtr;

//-------------------
// Class TTickPlayer 
//-------------------
/*!
\brief	Player using date in ticks. 
*/

class TTickPlayer : public TTickPlayerInterface {

	friend class TPlayTask;
	
	private:
	
        TSliceVisitor  	fSliceVisitor;
		TScoreIterator	fIterator;
		TPlayTask      	fPlayTask;
        TEventSenderInterfacePtr  	fEventUser;
		TSchedulerInterfacePtr		fScheduler;
		
		void PlaySlice (ULONG date_ms);
		
	public:
 
 		TTickPlayer(TScorePtr score, TEventSenderInterfacePtr user, TSchedulerInterfacePtr scheduler)
 			:fSliceVisitor(user),fIterator(score),fPlayTask(this),fEventUser(user),fScheduler(scheduler){}
 		virtual ~TTickPlayer(){}
 		
 		void Init();
 		void Start();
		void Stop();	
		void Cont(ULONG date_ticks);
		
 		void PlaySliceForward();
 		void PlaySliceBackward();
 		
 		ULONG GetPosTicks() ;
 		void  SetPosTicks(ULONG date_ticks);
  };

typedef TTickPlayer FAR * TTickPlayerPtr;

#endif
