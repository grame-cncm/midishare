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
//	TClockSender.h		    
// ===========================================================================
//
//	Manage Midi events related to Clock synchronization
//

#ifndef __TClockSender__
#define __TClockSender__

#include "TScheduler.h"
#include "TClockConverter.h"
#include "TEventSenderInterface.h"


//-----------------------------
// Class TClockSender 
//-----------------------------

class TClockSender{

	friend class TClockTask;

	private:
		
		TClockTask* fClockTask;  
		ULONG 		fClockCount;
		
		TSchedulerInterfacePtr	fScheduler;
		TClockConverterPtr 		fClockConverter;
		TEventSenderInterfacePtr  fEventUser;
		
		void NextClock(ULONG date_ms);
	
	public:
	
		TClockSender(TSchedulerInterfacePtr scheduler,
					 TClockConverterPtr converter,
					 TEventSenderInterfacePtr user);
					 
 		~TClockSender();
 		
 		void Start();
 		void Stop();
 		void Cont(ULONG date_ticks);
 		void SendSongPos(ULONG date_ticks);
 		
 		ULONG GetPosTicks();
};

typedef TClockSender FAR * TClockSenderPtr;


//-----------------------
// Class TClockTask 
//-----------------------

class TClockTask :public TTicksTask {

	private:
	
		TClockSenderPtr fSender;

	public : 
		TClockTask (TClockSenderPtr it):TTicksTask() {fSender =  it;}
		void Execute (TMidiApplPtr appl, ULONG date_ms) {fSender->NextClock(date_ms);}
};


typedef TClockTask FAR * TClockTaskPtr;


#endif
