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
//	TMidiAppl.h			    
// ===========================================================================

#ifndef __TMidiAppl__
#define __TMidiAppl__

#include "PlayerDefs.h"
#include "MidiShare.h"
#include "TPlayerConstants.h"

//-----------------
// Class TMidiTask 
//-----------------
/*!
  \brief  A Class to wrap a MidiShare task.
*/

class TMidiAppl;

class TMidiTask {
	
	friend class TMidiAppl;
		
	private:
	
		MidiEvPtr fTask;
		
		void Clear()  				 {fTask = 0;}
		void SetTask(MidiEvPtr ev)   {fTask = ev;}
		MidiEvPtr GetTask ()  		 {return fTask;}
	
	
	public:
	
		TMidiTask() {fTask = 0;}
		virtual ~TMidiTask() {MidiForgetTask(&fTask);}
	
		void Forget () {MidiForgetTask(&fTask);}
		virtual void Execute (TMidiAppl* , ULONG date) {} // Must be implemented for concrete tasks
};

typedef TMidiTask FAR * TMidiTaskPtr;

//-----------------
// Class TMidiAppl 
//-----------------

/*!
  \brief A Class to wrap a MidiShare application.
*/

class TMidiAppl {

	friend class TMidiTask;
	
	private:
		
	#if defined (__Macintosh__) && defined (__MacOS9__)
		UPPTaskPtr 	fUPPGenericTask;
		UPPRcvAlarmPtr 	fUPPGenericReceiveAlarm;
		UPPApplAlarmPtr fUPPGenericApplAlarm;
	#else
		TaskPtr 	fUPPGenericTask;
		RcvAlarmPtr 	fUPPGenericReceiveAlarm;
		ApplAlarmPtr 	fUPPGenericApplAlarm;
	#endif
		
	public :
	
	#if defined (__Macintosh__) 
		#if defined (__MacOS9__)
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
		
		TMidiAppl():fUPPGenericTask(0),fUPPGenericReceiveAlarm(0),fUPPGenericApplAlarm(0),fRefnum(-1),fFilter(0) {}
		virtual ~TMidiAppl() {Close();}
		
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

		static  void MSALARMAPI GenericTask(ULONG date, short refnum, long a1, long a2, long a3);
		static  void MSALARMAPI GenericReceiveAlarm(short ref);	
		static  void MSALARMAPI GenericApplAlarm(short ref, long code);	
		
};

typedef TMidiAppl FAR * TMidiApplPtr;

#endif
