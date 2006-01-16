/*

  Copyright  Grame 1999-2005

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
  
   modifications history:
   
 	 [19-02-01] SL - CallQuitAction removed, use of pthread_cancel in the library
	 [22-06-01] SL - Remove signal handling code, now done in the kernel module
	 [03-07-01] SL - Add field access functions

*/

#include <pwd.h>
#include <signal.h>

#include "libMidiShare.h"

/*________________________________________________________ */
/*	Globals variables                                  */

TClients   	gLClients = {0};
TClients*	gClients = &gLClients;          /* The global MidiShare structure   */


/*--------------------------------------------------------------------*/
/* memory allocation implementation                                   */
/*--------------------------------------------------------------------*/

FarPtr(void) AllocateMemory (MemoryType type, unsigned long size){ return (void*)malloc(size); }
void DisposeMemory  (FarPtr(void) memPtr){if (memPtr) free(memPtr); }

/*--------------------------------------------------------------------*/

void MidiInstall()
{
  Device(gClients) = open("/dev/MidiShare", 0);
  if (Device(gClients) < 0) return;
  InitEvents ();
  InitMemory(Memory(gClients), 15000);
  pthread_mutex_init(Mutex(gClients), NULL);
}

/*--------------------------------------------------------------------*/

void FreeList1( MidiEvPtr l)
{
	MidiEvPtr next;
	
	while (l) {
		next = Link(l);
		MSFreeCell(l,FreeList(Memory(gClients)));
		l=next;
	}
}

/*--------------------------------------------------------------------*/

MidiEvPtr MidiGetEvAux(short ref, int command)
{
	TMidiGetEvArgs 	arg;
	MidiEvPtr	c;
	
	c = MSNewCell(FreeList(Memory(gClients)));
	
	if (c == 0) return 0;
	Link(c)=0;
	arg.r = ref;
	arg.d = 1;
	arg.l = c; 
	arg.e = 0;
	arg.u = 0;
	
	CALL (command, &arg);
	
	if (arg.e == 0 && arg.u > 0) {
		/* -- Not enough memrory cells available */
		while ((arg.d < arg.u) && (c = MSNewCell(FreeList(Memory(gClients))))) {
			Link(c) = arg.l;
			arg.l = c;
			arg.d++;
		}
		if (arg.d == arg.u) {
			CALL (command, &arg);
		}
	}

 	FreeList1(arg.l);
	return arg.e;
}

/*--------------------------------------------------------------------*/
uid_t name_to_uid (char * name)
{
	struct passwd *p;
	p = getpwnam (name);
	return p ? p->pw_uid : -1;
}

/*--------------------------------------------------------------------------*/

void makeAppl(TClientsPtr g, TApplPtr appl, short ref, MidiName n)
{
	appl->info = 0;
	appl->refNum = (uchar)ref;
	appl->rcvAlarm = 0;
	appl->applAlarm = 0;
	Appls(gClients)[ref] = appl;
	ApplsCount(gClients)++;
}

/*--------------------------------------------------------------------------*/
/* event handler : to be notified by the kernel module */

MidiEvPtr MidiGetCommand(short ref );

void* CmdHandler(void* arg) 
{
	TAppl* appl = (TAppl*) arg;
	int refNum = appl->refNum;
	TTaskExtPtr task ;
	MidiEvPtr ev;
	int retrycount;
	
	pthread_setcanceltype( PTHREAD_CANCEL_ASYNCHRONOUS,NULL);

	while (1) {	// added to avoid exiting from the CmdHandler
	
		while ((ev = MidiGetCommand(refNum))) {         		
	
				switch (ev->evType) {
				
					case typeRcvAlarm: 		
						// execute rcv alarm 
						if (appl->rcvAlarm) (*appl->rcvAlarm) (refNum);  
						break;
				
					case typeProcess : 
						// execute pending real-time tasks 
						task = (TTaskExtPtr)LinkST(ev);	
						if (task->fun) (*task->fun) (ev->date, refNum, task->arg1, task->arg2, task->arg3);			
						break;						
						
					case typeApplAlarm:
						// execute application alarm 
						if (appl->applAlarm) (*appl->applAlarm) (refNum, MSGetField(ev,0)); 	
						break;	
								
				}
				MSFreeEv(ev, FreeList(Memory(gClients)));
		}
	}
	pthread_exit(0);
}


/*******************/
/* MidiGetVersion  */
/*******************/

short MidiGetVersion() 
{
   TMidiGetVersionArgs 	args;
   CALL(kMidiGetVersion,&args);
   return (short)args.version;
}


/*******************/
/* MidiCountAppls  */
/*******************/

short MidiCountAppls() 
{
   TMidiCountApplsArgs 	args;
   CALL(kMidiCountAppls,&args);
   return (short)args.count;
}

/*******************/
/* MidiGetIndAppl  */
/*******************/

short MidiGetIndAppl(short index) 
{
   TMidiGetIndApplArgs 	args;

   args.index = index;
   CALL(kMidiGetIndAppl,&args);
   return (short)args.refnum;
}

/********************/
/* MidiGetNamedAppl */
/********************/

short MidiGetNamedAppl(MidiName name)  
{
   TMidiGetNamedApplArgs args;

   args.name = name;
   CALL(kMidiGetNamedAppl,&args);
   return (short)args.refnum;
}

/*******************/
/* MidiGetSyncInfo */
/*******************/

void MidiGetSyncInfo(SyncInfoPtr p)  
{
   CALL(kMidiGetSyncInfo,p);
}

/*******************/
/* MidiSetSyncMode */
/*******************/

void MidiSetSyncMode(unsigned short mode)  
{
   TMidiSetSyncModeArgs args;

   args.mode = mode;
   CALL(kMidiSetSyncMode,&args);
}

/*******************/
/* MidiGetExtTime  */
/*******************/

long MidiGetExtTime ()  
{
   TMidiGetExtTimeArgs args;
   CALL(kMidiGetExtTime,&args);
   return args.time;
}

/*******************/
/* MidiInt2ExtTime */
/*******************/

long MidiInt2ExtTime (long date)  
{
   TMidiInt2ExtTimeArgs args;
   args.datein = date;
   CALL(kMidiInt2ExtTime,&args);
   return args.dateout;
}

/*******************/
/* MidiExt2IntTime */
/*******************/

long MidiExt2IntTime (long date)  
{
   TMidiExt2IntTimeArgs args;
   args.datein = date;
   CALL(kMidiExt2IntTime,&args);
   return args.dateout;
}

/*******************/
/* MidiTime2Smpte  */
/*******************/

void MidiTime2Smpte (long time, short format, SmpteLocPtr loc)  
{
   TMidiTime2SmpteArgs args;

   args.time = time;
   args.format = format;
   args.loc = loc;
   CALL(kMidiTime2Smpte,&args);
 }

/*******************/
/* MidiSmpte2Time  */
/*******************/

long MidiSmpte2Time (SmpteLocPtr loc)  
{
   TMidiSmpte2TimeArgs args;

   args.loc = loc;
   CALL(kMidiSmpte2Time,&args);
   return args.time;
 }

/*******************/
/* MidiGetTimeAddr */
/*******************/

long* MidiGetTimeAddr ()  
{
   TMidiGetTimeAddrArgs args;

   CALL(kMidiGetTimeAddr,&args);
   return args.timeadr;
 }
 
/*******************/
/* MidiOpen 	   */
/*******************/

 short MidiOpen (MidiName name)
{
	TApplPtr 		appl;
	TMidiOpenArgs 		args;
  	struct sched_param 	param;   
	int           		err;
	
	uid_t uid = getuid ();
	pthread_mutex_lock(Mutex(gClients));
	
	args.name = name;
  	CALL(kMidiOpen,&args);
	 
	if (ApplsCount(gClients) == 0) OpenMemory (Memory(gClients));
	if (ApplsCount(gClients) < MaxAppls) {
	
		appl = NewAppl (sizeof(TAppl));
		
		if (appl) {
		
			makeAppl(gClients, appl, args.refnum, name); 
			
			param.sched_priority = 99; /* 0 to 99  */
			pthread_create(&appl->rcvThread,NULL,CmdHandler,(void*)appl);
			setuid (name_to_uid ("root")); 
   			err = pthread_setschedparam(appl->rcvThread, SCHED_RR,  &param); 
			setuid (uid);
		}
	}
	pthread_mutex_unlock(Mutex(gClients));
	return args.refnum;
}


  
/*******************/
/* MidiClose       */
/*******************/

void MidiClose (short ref)
{
	TMidiCloseArgs 	args;
	TApplPtr 	appl;
	
	pthread_mutex_lock(Mutex(gClients));
	
	if (CheckRefNum(gClients, ref)) {
		appl = Appls(gClients)[ref];
 		args.refnum = ref;
		
		/* Wait for Real Time thread exit */
		pthread_cancel(appl->rcvThread);
		pthread_join(appl->rcvThread,NULL);
		
		/* Kernel close */
		CALL(kMidiClose,&args);
		
		/* Client close */
		FreeAppl(appl) ;
		Appls(gClients)[ref] = 0;
		ApplsCount(gClients)--;
	
		if (ApplsCount(gClients) == 0) CloseMemory(Memory(gClients));
		
	}
	pthread_mutex_unlock(Mutex(gClients));
}

/*******************/
/* MidiGetName     */
/*******************/

MidiName MidiGetName(short ref)
{
  TMidiGetNameArgs args;

  args.refnum = ref;
  args.name = gClients->nameTable[ref];
  CALL(kMidiGetName,&args);
  return args.name;
}

/*******************/
/* MidiSetName     */
/*******************/

void MidiSetName (short ref, MidiName name)
{
  TMidiSetNameArgs args;
  
  args.refnum = ref;
  args.name = name;
  CALL(kMidiSetName ,&args);
}

/*******************/
/* MidiGetInfo     */
/*******************/

void* MidiGetInfo (short ref)
{
	void* info = 0;
	if (CheckRefNum(gClients,ref)) info = Appls(gClients)[ref]->info;
	return info;
}


/*******************/
/* MidiSetInfo     */
/*******************/

void MidiSetInfo (short ref ,void * info)
{
  	if (CheckRefNum(gClients,ref)) Appls(gClients)[ref]->info = info;
}


/*******************/
/* MidiGetFilter   */
/*******************/

MidiFilterPtr MidiGetFilter (short ref)
{
  TMidiGetFilterArgs args;
  
  args.refnum = ref;
  CALL(kMidiGetFilter ,&args);
  return args.filter;
}


/*******************/
/* MidiSetFilter   */
/*******************/

void MidiSetFilter (short ref, MidiFilterPtr filter)
{
  TMidiSetFilterArgs args;
  
  args.refnum = ref;
  args.filter = filter;
  CALL(kMidiSetFilter ,&args);
}


/*******************/
/* MidiGetRcvAlarm */
/*******************/

RcvAlarmPtr MidiGetRcvAlarm (short ref)
{
	return CheckRefNum(gClients,ref) ? Appls(gClients)[ref]->rcvAlarm : 0;
}


/*******************/
/* MidiSetRcvAlarm */
/*******************/

void MidiSetRcvAlarm (short ref,RcvAlarmPtr alarm)
{
	TMidiSetRcvAlarmArgs args;
	
	if (CheckRefNum(gClients,ref)) {
		args.refnum = ref;
		args.alarm = alarm;
		CALL(kMidiSetRcvAlarm,&args);
		Appls(gClients)[ref]->rcvAlarm = alarm;
	}
}


/*******************/
/* MidiGetApplAlarm */
/*******************/

ApplAlarmPtr MidiGetApplAlarm (short ref)
{
	return CheckRefNum(gClients,ref) ? Appls(gClients)[ref]->applAlarm : 0;
}


/*******************/
/* MidiSetApplAlarm */
/*******************/

void MidiSetApplAlarm (short ref,ApplAlarmPtr alarm)
{
	TMidiSetApplAlarmArgs args;
	
	if (CheckRefNum(gClients,ref)) {
		args.refnum = ref;
		args.alarm = alarm;
		CALL(kMidiSetApplAlarm,&args);
		Appls(gClients)[ref]->applAlarm = alarm;
	}
}

/*******************/
/* MidiConnect */
/*******************/

void MidiConnect (short src,short dest , Boolean state)
{
  TMidiConnectArgs args;
   
  args.src = src;
  args.dest = dest;
  args.state = state;
  CALL(kMidiConnect ,&args);
 
}

/*******************/
/* MidiIsConnected  */
/*******************/

Boolean MidiIsConnected  (short src,short dest)
{
  TMidiIsConnectedArgs args;
  
  args.src = src;
  args.dest = dest;
  CALL(kMidiIsConnected  ,&args);
  return args.state;
}


/*******************/
/* MidiGetPortState */
/*******************/

Boolean MidiGetPortState (short port)
{
  TMidiGetPortStateArgs args;
  
  args.port = port;
  CALL(kMidiGetPortState ,&args);
  return args.state;
}


/*******************/
/* MidiFreeSpace */
/*******************/

long MidiFreeSpace (){ return MSFreeSpace(Memory(gClients)); }
 
/*******************/
/* MidiTotalSpace */
/*******************/

long MidiTotalSpace (){ return MSTotalSpace(Memory(gClients)); }

/*******************/
/* MidiGrowSpace */
/*******************/

long MidiGrowSpace (unsigned long nbev){ return MSGrowSpace(nbev, Memory(gClients)); }

/*******************/
/* MidiNewCell */
/*******************/

MidiEvPtr MidiNewCell (){ return MSNewCell(FreeList(Memory(gClients))); }

/*******************/
/* MidiFreeCell */
/*******************/

void MidiFreeCell (MidiEvPtr e) {  if (e) MSFreeCell(e, FreeList(Memory(gClients)));}

/*******************/
/* MidiNewEv */
/*******************/

MidiEvPtr MidiNewEv (short type){ return MSNewEv(type, FreeList(Memory(gClients))); }

/*******************/
/* MidiCopyEv */
/*******************/

MidiEvPtr MidiCopyEv (MidiEvPtr ev){ return MSCopyEv(ev, FreeList(Memory(gClients))); }

/*******************/
/* MidiFreeEv */
/*******************/

void MidiFreeEv (MidiEvPtr ev){  MSFreeEv(ev, FreeList(Memory(gClients))); }

/*******************/
/* MidiSetField */
/*******************/

void MidiSetField (MidiEvPtr ev, unsigned long f, long v){  MSSetField(ev, f,v); }

/*******************/
/* MidiGetField */
/*******************/

long MidiGetField (MidiEvPtr ev, unsigned long f){ return MSGetField(ev, f); }

/*******************/
/* MidiCountFields */
/*******************/

long MidiCountFields (MidiEvPtr ev){ return MSCountFields(ev); }

/*******************/
/* MidiAddField */
/*******************/

void MidiAddField (MidiEvPtr ev, long v){  MSAddField(ev,v,FreeList(Memory(gClients))); }

/*******************/
/* MidiNewSeq */
/*******************/

MidiSeqPtr MidiNewSeq (){ return MSNewSeq(FreeList(Memory(gClients))); }

/*******************/
/* MidiAddSeq */
/*******************/

void MidiAddSeq (MidiSeqPtr s, MidiEvPtr ev){  MSAddSeq(s,ev); }

/*******************/
/* MidiFreeSeq */
/*******************/

void MidiFreeSeq (MidiSeqPtr s){  MSFreeSeq(s,FreeList(Memory(gClients))); }

/*******************/
/* MidiClearSeq */
/*******************/

void MidiClearSeq (MidiSeqPtr s){  MSClearSeq(s,FreeList(Memory(gClients))); }

/*******************/
/* MidiApplySeq */
/*******************/

void MidiApplySeq (MidiSeqPtr s, ApplyProcPtr proc){  MSApplySeq(s,proc); }

/*******************/
/* MidiSetPortState */
/*******************/

void MidiSetPortState (short port,Boolean state)
{
  TMidiSetPortStateArgs args;
   
  args.port = port;
  args.state = state;
  CALL(kMidiSetPortState ,&args);
 
}

/*******************/
/* MidiGetTime */
/*******************/

unsigned long MidiGetTime()
{
  TMidiGetTimeArgs 	args;
  CALL(kMidiGetTime ,&args);
  return args.time;
}

/*******************/
/* MidiSendIm */
/*******************/

void MidiSendIm (short ref, MidiEvPtr e)
{
	if (e) {
		RefNum(e) = ref;
		CALL(kMidiSendIm ,e);
		MSFreeEv(e, FreeList(Memory(gClients)));
	}
}
		

/*******************/
/* MidiSend */
/*******************/
		
void MidiSend (short ref, MidiEvPtr e)
{
	if (e) {
		RefNum(e) = ref;
		CALL(kMidiSend ,e);
		MSFreeEv(e, FreeList(Memory(gClients)));
	}
}

/*******************/
/* MidiSendAt */
/*******************/

void MidiSendAt (short ref, MidiEvPtr e, long d)
{
	if (e) {
		RefNum(e) = ref;
		Date(e) = d;
		CALL(kMidiSend ,e);
		MSFreeEv(e, FreeList(Memory(gClients)));
	}
}	

/*******************/
/* MidiCountEvs */
/*******************/

long MidiCountEvs (short ref)
{
  TMidiCountEvsArgs args;
  
  args.refnum = ref;
  CALL(kMidiCountEvs ,&args);
  return args.evs;
}


MidiEvPtr MidiGetEv(short ref ) { return MidiGetEvAux(ref, kMidiGetEv); }
MidiEvPtr MidiGetCommand(short ref ) { return MidiGetEvAux(ref, kMidiGetCommand); }

/*******************/
/* MidiAvailEv */
/*******************/

MidiEvPtr MidiAvailEv   (short ref)  {return MidiGetEvAux(ref, kMidiAvailEv); }

/*******************/
/* MidiFlushEvs */
/*******************/

void MidiFlushEvs   (short ref)
{
  TMidiFlushEvsArgs args;
  
  args.refnum = ref;
  CALL(kMidiFlushEvs ,&args);
}

/*******************/
/* MidiReadSync */
/*******************/

void* MidiReadSync (void* adr)
{
  TMidiReadSyncArgs args;
  
  args.adr = adr;
  CALL(kMidiReadSync ,&args);
  return args.res;
  
}

/*******************/
/* MidiWriteSync  */
/*******************/

void* MidiWriteSync (void* adr, void* val)
{
  TMidiWriteSyncArgs args;
  
  args.adr = adr;
  args.val = val;
  CALL(kMidiWriteSync  ,&args);
  return args.res;
  
}

/*******************/
/* MidiCall  */
/*******************/

void MidiCall (TaskPtr routine, long date, short ref, long a1,long a2,long a3)
{
  	MidiEvPtr ev;
	MidiSTPtr ext;
	
	ev= MidiNewEv (typeProcess);
	if( ev) {
		ev->date= date;
		ev->refNum= ref;
		ext= LinkST(ev);
		ext->val[0]= (long)routine;
		ext->val[1]= a1;
		ext->val[2]= a2;
		ext->val[3]= a3;
		CALL(kMidiSend ,ev);
	}
}

/*******************/
/* MidiTask */
/*******************/

MidiEvPtr MidiTask   (TaskPtr routine, long date, short ref, long a1,long a2,long a3)
{
  	MidiEvPtr ev;
	MidiSTPtr ext;
	TMidiTaskArgs args;
	
	ev= MidiNewEv (typeProcess);
	
	if( ev) {
		ev->date= date;
		ev->refNum= ref;
		ext= LinkST(ev);
		ext->val[0]= (long)routine;
		ext->val[1]= a1;
		ext->val[2]= a2;
		ext->val[3]= a3;
		
		args.refnum = ref;
		args.utask = ev;
		CALL (kMidiTask, &args);
		MSFreeEv(ev, FreeList(Memory(gClients)));
		return args.ktask;
	}else
		return 0;	
}

/*******************/
/* MidiDTask */
/*******************/

MidiEvPtr MidiDTask   (TaskPtr routine, long date, short ref, long a1,long a2,long a3)
{
  	MidiEvPtr ev;
	MidiSTPtr ext;
	TMidiDTaskArgs args;
	
	ev= MidiNewEv (typeDProcess);
	
	if( ev) {
		ev->date= date;
		ev->refNum= ref;
		ext= LinkST(ev);
		ext->val[0]= (long)routine;
		ext->val[1]= a1;
		ext->val[2]= a2;
		ext->val[3]= a3;
		args.refnum = ref;
		args.utask = ev;
		CALL (kMidiDTask, &args);
		MSFreeEv(ev, FreeList(Memory(gClients)));
		return args.ktask;
	}else
		return 0;
}

/*******************/
/* MidiForgetTask */
/*******************/

void  MidiForgetTask  (MidiEvPtr *taskptr)
{
    	TMidiForgetTaskArgs args;
	
	if (taskptr && *taskptr) {
		args.utask = 0;
		args.taskptr = taskptr;
		CALL(kMidiForgetTask,&args);
	}
}

/*******************/
/* MidiCountDTasks */
/*******************/

long MidiCountDTasks (short ref)
{
  TMidiCountDTasksArgs args;
  
  args.refnum = ref;
  CALL(kMidiCountDTasks,&args);
  return args.res; 
}

/*******************/
/* MidiFlushDTasks  */
/*******************/

void MidiFlushDTasks  (short ref)
{
  TMidiFlushDTasksArgs args;
  
  args.refnum = ref;
  CALL(kMidiFlushDTasks,&args);  
}

/*******************/
/* MidiExec1DTask  */
/*******************/

void MidiExec1DTask   (short ref)
{
	MidiEvPtr dTask = MidiGetEvAux(ref, kMidiGetDTask);
	
	if (dTask) {
		TTaskExtPtr task= (TTaskExtPtr)LinkST(dTask);
		(*task->fun)(dTask->date, ref, task->arg1, task->arg2, task->arg3);
		MSFreeEv(dTask, FreeList(Memory(gClients)));
	}
}

/*******************/
/* MidiNewFilter */
/*******************/

MidiFilterPtr MidiNewFilter ()
{
  TMidiNewFilterArgs args;
  CALL(kMidiNewFilter,&args);
  return args.filter;
  
}

/*******************/
/* MidiFreeFilter */
/*******************/

void MidiFreeFilter (MidiFilterPtr filter)
{
  TMidiFreeFilterArgs args;
  args.filter = filter;
  CALL(kMidiFreeFilter,&args);
  
}

/*******************/
/* MidiAcceptChan */
/*******************/

void MidiAcceptChan (MidiFilterPtr filter, short chan , Boolean state )
{
  TMidiAcceptChanArgs args;

  args.filter = filter;
  args.generic = chan;
  args.state = state;
  CALL(kMidiAcceptChan,&args);
  
}

/*******************/
/* MidiAcceptPort */
/*******************/

void MidiAcceptPort (MidiFilterPtr filter, short port , Boolean state )
{
  TMidiAcceptPortArgs args;

  args.filter = filter;
  args.generic = port;
  args.state = state;
  CALL(kMidiAcceptPort,&args);
  
}

/*******************/
/* MidiAcceptType */
/*******************/

void MidiAcceptType (MidiFilterPtr filter, short type , Boolean state )
{
  TMidiAcceptTypeArgs args;

  args.filter = filter;
  args.generic = type;
  args.state = state;
  CALL(kMidiAcceptType,&args);  
}

/*******************/
/* MidiIsAcceptedChan */
/*******************/

Boolean MidiIsAcceptedChan (MidiFilterPtr filter, short chan )
{
  TMidiIsAcceptedChanArgs args;

  args.filter = filter;
  args.generic = chan;
  CALL(kMidiIsAcceptedChan,&args);
  return  args.state;
}

/*******************/
/* MidiIsAcceptedPort */
/*******************/

Boolean MidiIsAcceptedPort (MidiFilterPtr filter, short port )
{
  TMidiIsAcceptedPortArgs args;

  args.filter = filter;
  args.generic = port;
  CALL(kMidiIsAcceptedPort,&args);
  return  args.state;
}

/*******************/
/* MidiIsAcceptedChan */
/*******************/

Boolean MidiIsAcceptedType (MidiFilterPtr filter, short type )
{
  TMidiIsAcceptedTypeArgs args;

  args.filter = filter;
  args.generic = type;
  CALL(kMidiIsAcceptedType,&args);
  return  args.state;
}

/*******************/
/* MidiShare  */
/*******************/

Boolean MidiShare () 
{
   CHECK_DEVICE;
   return (Device(gClients) >0);
}

/*******************/
/* MidiReset  */
/*******************/

void MidiReset () 
{
  TMidiCloseArgs args;
  CALL(kMidiReset,&args);
}


/*--------------------------------------------------------------------*/
/* release 1.80 additionnal entry points */
/*--------------------------------------------------------------------*/
short MidiRegisterDriver (TDriverInfos * infos, TDriverOperation *op)
{
	TMidiRegisterDriverArgs args;
  	args.infos = *infos;
  	args.op.wakeup = op->wakeup;
  	args.op.sleep = op->sleep;
 	CALL(kMidiRegisterDriver,&args);
	return args.refnum;
}

/*--------------------------------------------------------------------*/
void MidiUnregisterDriver (short refnum)
{
	CALL(kMidiUnregisterDriver,&refnum);
}

/*--------------------------------------------------------------------*/
short MidiCountDrivers ()
{
	short count = 0;
	CALL(kMidiCountDrivers,&count);
	return count;
}

/*--------------------------------------------------------------------*/
short MidiGetIndDriver (short index)
{
	TMidiGetIndDriverArgs args;
 	args.index = index;
	CALL(kMidiGetIndDriver,&args);
	return args.refnum;
}

/*--------------------------------------------------------------------*/
Boolean MidiGetDriverInfos (short refnum, TDriverInfos * infos)
{
	TMidiGetDriverInfosArgs args;
 	args.refnum = refnum;
	CALL(kMidiGetDriverInfos,&args);
	if (infos) *infos = args.infos;
 	return args.result;
}

/*--------------------------------------------------------------------*/
SlotRefNum MidiAddSlot  (short refnum, MidiName name, SlotDirection direction)
{
	TMidiAddSlotArgs args;
 	args.refnum = refnum;
	args.name = name;
	args.direction = direction;
	CALL(kMidiAddSlot,&args);
	return args.slotRef;
}

/*--------------------------------------------------------------------*/
SlotRefNum MidiGetIndSlot (short refnum, short index)
{
	TMidiGetIndSlotArgs args;
	args.refnum = refnum;
	args.index = index;
	CALL(kMidiGetIndSlot,&args);
	return args.slotRef;
}

/*--------------------------------------------------------------------*/
void MidiRemoveSlot (SlotRefNum slot)
{
	CALL(kMidiRemoveSlot,&slot);
}

/*--------------------------------------------------------------------*/
Boolean MidiGetSlotInfos (SlotRefNum slot, TSlotInfos * infos)
{
	TMidiGetSlotInfosArgs args;
	args.slotRef = slot;
	args.infos = *infos;
	args.result = false;
	CALL(kMidiGetSlotInfos,&args);
	return args.result;
}

/*--------------------------------------------------------------------*/
void MidiConnectSlot (short port, SlotRefNum slot, Boolean state)
{
	TMidiConnectSlotArgs args;
	args.slotRef = slot;
	args.port = port;
	args.state = state;
	CALL(kMidiConnectSlot,&args);
}

/*--------------------------------------------------------------------*/
Boolean MidiIsSlotConnected	(short port, SlotRefNum slot)
{
	TMidiIsSlotConnectedArgs args;
	args.slotRef = slot;
	args.port = port;
	args.result = false;
	CALL(kMidiIsSlotConnected,&args);
	return args.result;
}

/*--------------------------------------------------------------------*/
/* release 1.82 additionnal entry points */
/*--------------------------------------------------------------------*/
void MidiSetSlotName (SlotRefNum slot, MidiName name)
{
	TMidiSetSlotNameArgs args;
	args.slotRef = slot;
	args.name = name;
	CALL(kMidiSetSlotName,&args);
}


/*--------------------------------------------------------------------*/
/* Field access functions */
/*--------------------------------------------------------------------*/

MidiEvPtr  MidiGetLink (MidiEvPtr e)                  { return Link(e); }
void       MidiSetLink (MidiEvPtr e, MidiEvPtr next)  { Link(e) = next; }

long  MidiGetDate   (MidiEvPtr e)                { return Date(e); }
void  MidiSetDate   (MidiEvPtr e, ulong date)    { Date(e) = date; }
short MidiGetRefNum (MidiEvPtr e)                { return RefNum(e); }
void  MidiSetRefNum (MidiEvPtr e, short ref)     { RefNum(e) = (Byte)ref; }
short MidiGetType   (MidiEvPtr e)                { return EvType(e); }
void  MidiSetType   (MidiEvPtr e, short type)    { EvType(e) = (Byte)type; }
short MidiGetChan   (MidiEvPtr e)                { return Chan(e); }
void  MidiSetChan   (MidiEvPtr e, short chan)    { Chan(e) = (Byte)chan; }
short MidiGetPort   (MidiEvPtr e)                { return Port(e); }
void  MidiSetPort   (MidiEvPtr e, short port)    { Port(e) = (Byte)port; }

long  MidiGetData0 (MidiEvPtr e)            { return Data(e)[0]; }
long  MidiGetData1 (MidiEvPtr e)            { return Data(e)[1]; }
long  MidiGetData2 (MidiEvPtr e)            { return Data(e)[2]; }
long  MidiGetData3 (MidiEvPtr e)            { return Data(e)[3]; }
void  MidiSetData0 (MidiEvPtr e, long v)    { Data(e)[0] = (Byte)v; }
void  MidiSetData1 (MidiEvPtr e, long v)    { Data(e)[1] = (Byte)v; }
void  MidiSetData2 (MidiEvPtr e, long v)    { Data(e)[2] = (Byte)v; }
void  MidiSetData3 (MidiEvPtr e, long v)    { Data(e)[3] = (Byte)v; }

MidiEvPtr  MidiGetFirstEv (MidiSeqPtr seq)               { return seq->first; }
void       MidiSetFirstEv (MidiSeqPtr seq, MidiEvPtr e)  { seq->first = e; }
MidiEvPtr  MidiGetLastEv  (MidiSeqPtr seq)               { return seq->last; }
void       MidiSetLastEv  (MidiSeqPtr seq, MidiEvPtr e)  { seq->last = e; }




/*******************/
/* MidiGetError  */
/*******************/

long MidiGetError() 
{
   TMidiGetErrorArgs 	args;
   CALL(kMidiGetError,&args);
   return args.error;
}
