// ===========================================================================
// The Player Library is Copyright (c) Grame, Computer Music Research Laboratory 
// 1996-1999, and is distributed as Open Source software under the Artistic License;
// see the file "Artistic" that is included in the distribution for details.
//
// Grame : Computer Music Research Laboratory
// Web : http://www.grame.fr/Research
// E-mail : MidiShare@rd.grame.fr
//
// modifications history:
//   [11-12-99] SL - Linux adaptation
//
// ===========================================================================


// ===========================================================================
//	TMidiAppl.cpp			    
// ===========================================================================
//
//  A Class to wrap a MidiShare application
//

#define __SupportOldSTDef__
#include "TMidiAppl.h"


/*--------------------------------------------------------------------------*/
void MSALARMAPI TMidiAppl::GenericTask (ULONG date, short refnum, long a1, long a2, long a3) 
{

	TMidiApplPtr appl = (TMidiApplPtr)a1;
	TMidiTaskPtr task = (TMidiTaskPtr)a2;

	task->Clear(); // By default,the task pointer is set to 0
	task->Execute(appl,date);

}
/*--------------------------------------------------------------------------*/

void MSALARMAPI TMidiAppl::GenericReceiveAlarm(short ref) 
{

	((TMidiApplPtr)MidiGetInfo(ref))->ReceiveAlarm (ref);
	
}

/*--------------------------------------------------------------------------*/

void MSALARMAPI TMidiAppl::GenericApplAlarm(short ref, long code) 
{

	((TMidiApplPtr)MidiGetInfo(ref))->ApplAlarm (ref,code);

}

/*--------------------------------------------------------------------------*/

short TMidiAppl::Open (MidiName name) 
{
	short i;
		
	#if GENERATINGCFM
		fUPPGenericTask = NewTaskPtr(GenericTask);
		fUPPGenericReceiveAlarm =  NewRcvAlarmPtr(GenericReceiveAlarm);
		fUPPGenericApplAlarm =  NewApplAlarmPtr(GenericApplAlarm);
	#else
		fUPPGenericTask = (TaskPtr)GenericTask;
		fUPPGenericReceiveAlarm =  (RcvAlarmPtr)GenericReceiveAlarm;
		fUPPGenericApplAlarm = (ApplAlarmPtr)GenericApplAlarm;
	#endif
	
	fRefnum = MidiOpen(name);
	fFilter = MidiNewFilter();
	
	if (fRefnum > 0 && fFilter) {
		MidiSetRcvAlarm(fRefnum, fUPPGenericReceiveAlarm);
		MidiSetApplAlarm(fRefnum, fUPPGenericApplAlarm);
		
		for (i = 0; i<256; i++) {
			MidiAcceptType(fFilter,i,true);
			MidiAcceptPort(fFilter,i,true);
		}
		
		for (i = 0; i<16; i++) {
			MidiAcceptChan(fFilter,i,true);
		}
		
		MidiSetFilter(fRefnum, fFilter);
		MidiSetInfo(fRefnum, this);
		
	}else {
		 Close();
	}
	
	return fRefnum;
}

/*--------------------------------------------------------------------------*/

void TMidiAppl::Close() 
{
	if (fRefnum > 0) {
		MidiSetFilter(fRefnum,0);
		MidiSetRcvAlarm(fRefnum, 0);
		MidiSetApplAlarm(fRefnum, 0);
		
		#if GENERATINGCFM
			if (fUPPGenericTask) DisposeRoutineDescriptor (fUPPGenericTask);
			if (fUPPGenericReceiveAlarm) DisposeRoutineDescriptor (fUPPGenericReceiveAlarm);
			if (fUPPGenericApplAlarm) DisposeRoutineDescriptor (fUPPGenericApplAlarm);
		#endif
	
		if (fFilter) MidiFreeFilter(fFilter);
		MidiClose(fRefnum);
		fRefnum = -1;
	} 
}


/*--------------------------------------------------------------------------*/

#ifdef __Macintosh__
	#if GENERATINGCFM
		void TMidiAppl::NewMidiTask (UPPTaskPtr routine, ULONG date,  long a1,long a2,long a3, MidiEvPtr* adr){
	#else
		void TMidiAppl::NewMidiTask (TaskPtr routine, ULONG date,  long a1,long a2,long a3, MidiEvPtr* adr){
	#endif
	
	MidiEvPtr ev;
	MidiSTPtr ext;
	ev= MidiNewEv(typeProcess);
	
 	if( ev) {
 		Date(ev)= date;
		RefNum(ev)= fRefnum;
		ext= LinkST(ev);
		ext->ptr1= (Ptr)routine;
		ext->ptr2= (Ptr)a1;
		ext->ptr3= (Ptr)a2;
		ext->ptr4= (Ptr)a3;
		*adr = ev;
		MidiSend(fRefnum,ev);
	}else
		*adr = 0;
}
#endif


#ifdef __MSWindows__
	void  TMidiAppl::NewMidiTask(TaskPtr routine, ULONG date,  long a1,long a2,long a3,MidiEvPtr* adr)

{
	MidiEvPtr ev;
	MidiSTPtr ext;
	ev= MidiNewEv(typeProcess);
	
 	if( ev) {
 		Date(ev)= date;
		RefNum(ev)= fRefnum;
		ext= LinkST(ev);
		ext->ptr1= (Ptr)routine;
		ext->ptr2= (Ptr)a1;
		ext->ptr3= (Ptr)a2;
		ext->ptr4= (Ptr)a3;
		*adr = ev;
		MidiSend(fRefnum,ev);
	}else
		*adr = 0;
}

#endif


#ifdef __Linux__

void  TMidiAppl::NewMidiTask(TaskPtr routine, ULONG date,  long a1,long a2,long a3,MidiEvPtr* adr)
{
	*adr = MidiTask(routine, date, fRefnum, a1, a2, a3);
}

#endif

/*--------------------------------------------------------------------------*/

#ifdef __Macintosh__
	#if GENERATINGCFM
		void TMidiAppl::NewMidiCall (UPPTaskPtr routine, ULONG date,  long a1,long a2,long a3)
	#else
		void TMidiAppl::NewMidiCall (TaskPtr routine, ULONG date,  long a1,long a2,long a3)
	#endif

#endif

#ifdef __MSWindows__
	void  TMidiAppl::NewMidiCall(TaskPtr routine, ULONG date,  long a1,long a2,long a3)
#endif

#ifdef __Linux__
	void  TMidiAppl::NewMidiCall(TaskPtr routine, ULONG date,  long a1,long a2,long a3)
#endif


{
	MidiCall(routine,date,fRefnum,a1,a2,a3);
}
