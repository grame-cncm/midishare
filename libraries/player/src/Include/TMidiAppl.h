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
//	TMidiAppl.h			    
// ===========================================================================
//
//  A Class to wrap a MidiShare application
//


#ifndef __TMidiAppl__
#define __TMidiAppl__

#include "GenericMidiShare.h"
#include "TPlayerConstants.h"


#ifdef __Macintosh__
#include "filter.h"
#endif


//-----------------------
// Class TMidiTask 
//-----------------------

class TMidiTask {
	
	friend class TMidiAppl;
		
	private:
	
		MidiEvPtr fTask;
		
		void Clear ()  				 {fTask = 0;}
		void SetTask (MidiEvPtr ev)  {fTask = ev;}
		MidiEvPtr GetTask ()  		 {return fTask;}
	
	
	public:
		TMidiTask() {fTask = 0;}
		virtual ~TMidiTask() {MidiForgetTask(&fTask);}
	
		void Forget () {MidiForgetTask(&fTask);}
		virtual void Execute (TMidiAppl* , ULONG date){} // Must be implemented for concrete tasks
};

typedef TMidiTask FAR * TMidiTaskPtr;

//-----------------------
// Class TMidiAppl 
//-----------------------

class TMidiAppl{

	friend class TMidiTask;
	
	private:
		
	#if GENERATINGCFM
		UPPTaskPtr 		fUPPGenericTask;
		UPPRcvAlarmPtr 	fUPPGenericReceiveAlarm;
		UPPApplAlarmPtr fUPPGenericApplAlarm;
	#else
		TaskPtr 		fUPPGenericTask ;
		RcvAlarmPtr 	fUPPGenericReceiveAlarm ;
		ApplAlarmPtr 	fUPPGenericApplAlarm;
	#endif
		
	public :
	
	#ifdef __Macintosh__
		#if GENERATINGCFM
			void NewMidiTask(UPPTaskPtr routine, ULONG date, long a1,long a2,long a3, MidiEvPtr* adr);
			void NewMidiCall(UPPTaskPtr routine, ULONG date, long a1,long a2,long a3);
		#else
			void NewMidiTask(TaskPtr routine, ULONG date, long a1,long a2,long a3, MidiEvPtr* adr);
			void NewMidiCall(TaskPtr routine, ULONG date, long a1,long a2,long a3);
		#endif

	#endif
	
	
	#ifdef __MSWindows__
		void NewMidiTask(TaskPtr routine, ULONG date, long a1,long a2,long a3,MidiEvPtr* adr);
		void NewMidiCall(TaskPtr routine, ULONG date, long a1,long a2,long a3);
	#endif
	
	
	#ifdef __Linux__
		void NewMidiTask(TaskPtr routine, ULONG date, long a1,long a2,long a3,MidiEvPtr* adr);
		void NewMidiCall(TaskPtr routine, ULONG date, long a1,long a2,long a3);
	#endif
	
	
	
		short fRefnum;
		MidiFilterPtr fFilter;
		
		TMidiAppl() 
		{ 
			fRefnum = -1; 
			fFilter = 0;
			fUPPGenericTask = 0;
			fUPPGenericReceiveAlarm = 0;
			fUPPGenericApplAlarm = 0;
		}
		virtual ~TMidiAppl(){ Close();}
		
		virtual short Open (MidiName name);
		virtual void Close();
		
		void Send(MidiEvPtr e) {MidiSend(fRefnum, e);} 
		void SendIm(MidiEvPtr e) {MidiSendIm(fRefnum, e);} 
		void SendAt(MidiEvPtr e, ULONG date) {MidiSendAt(fRefnum, e,date);} 
		void SendToSelfIm(MidiEvPtr e) {MidiSendIm(fRefnum+128, e);} 
		void SendToSelf(MidiEvPtr e) {MidiSend(fRefnum+128, e);} 
		void SendToSelfAt(MidiEvPtr e, ULONG date) {MidiSendAt(fRefnum+128, e,date);} 

		void SendCopyIm(MidiEvPtr e) {MidiSendIm(fRefnum, MidiCopyEv(e));} 
		void SendCopyAt(MidiEvPtr e, ULONG date) {MidiSendAt(fRefnum, MidiCopyEv(e),date);} 

		MidiEvPtr ScheduleTask(TMidiTaskPtr task, ULONG date) 
		{
			task->Forget();
			NewMidiTask(fUPPGenericTask, date,(long)this,(long)task,0, &task->fTask);
			return task->fTask;
		}
		
		MidiEvPtr ScheduleDTask(TMidiTaskPtr task, ULONG date) 
		{
			task->Forget(); 
			task->fTask = MidiDTask(fUPPGenericTask, date,fRefnum,(long)this,(long)task,0);
			return task->fTask;
		}

		virtual void ReceiveAlarm (short ref) {MidiFlushEvs(ref);}
		virtual void ApplAlarm (short ref,long code) {}
		
		/*--------------------------------------------------------------------------*/
		// Prototypes for Midi Callbacks
		/*--------------------------------------------------------------------------*/

		static  void MSALARMAPI GenericTask (ULONG date, short refnum, long a1, long a2, long a3);
		static  void MSALARMAPI GenericReceiveAlarm(short ref);	
		static  void MSALARMAPI GenericApplAlarm(short ref, long code);	
		
};


typedef TMidiAppl FAR * TMidiApplPtr;

#endif
