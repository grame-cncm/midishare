// ===========================================================================
// The Player Library is Copyright (c) Grame, Computer Music Research Laboratory 
// 1996-2003, and is distributed as Open Source software under the Artistic License;
// see the file "Artistic" that is included in the distribution for details.
//
// Grame : Computer Music Research Laboratory
// Web : http://www.grame.fr/Research
// E-mail : MidiShare@rd.grame.fr
// ===========================================================================


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
