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
//	TClockSender.h		    
// ===========================================================================

#ifndef __TClockSender__
#define __TClockSender__

#include "TScheduler.h"
#include "TClockConverter.h"
#include "TEventSenderInterface.h"


//------------------
// Class TClockTask 
//------------------

/*!
  \brief Task to send clocks events.
*/

class TClockTask :public TTicksTask {

	friend class TClockSender;

	private:
	
		TClockSender* fSender;

	public : 
		TClockTask (TClockSender* it):TTicksTask(),fSender(it){}
		void Execute (TMidiApplPtr appl, ULONG date_ms);
};


typedef TClockTask FAR * TClockTaskPtr;


//--------------------
// Class TClockSender 
//--------------------
/*!
	\brief Manage Midi events related to Clock synchronization.
*/

class TClockSender{

	friend class TClockTask;

	private:
		
		TSchedulerInterfacePtr		fScheduler;
		TClockConverterPtr 			fClockConverter;
		TEventSenderInterfacePtr  	fEventUser;
                
        TClockTask  fClockTask;  
		ULONG 	fClockCount;
		
		void NextClock(ULONG date_ms);
	
	public:
	
		TClockSender(TSchedulerInterfacePtr scheduler,TClockConverterPtr converter,TEventSenderInterfacePtr user)
			:fScheduler(scheduler),fClockConverter(converter),fEventUser(user),fClockTask(this),fClockCount(0){}
		virtual ~TClockSender(){}
 		
 		void Start();
 		void Stop();
 		void Cont(ULONG date_ticks);
 		void SendSongPos(ULONG date_ticks);
 		
 		ULONG GetPosTicks();
};

typedef TClockSender FAR * TClockSenderPtr;


#endif
