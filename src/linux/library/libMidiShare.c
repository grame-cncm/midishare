/*

  Copyright © Grame 1999

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
  grame@rd.grame.fr

*/

#include <pwd.h>
#include <signal.h>

#include "libMidiShare.h"


/*________________________________________________________ */
/*	Globals variables                                  */

TClients   	gLClients = {0};
TClients*	gClients = &gLClients;

int msfd = -1;  /* the MidiShare Device file number */


/*________________________________________________________ */
/* signal handling parameters and variables                */
/* list of signals to be patched */
int gSigList [] = {
	SIGHUP, SIGINT, SIGQUIT, SIGILL, SIGTRAP,
	SIGABRT, SIGBUS, SIGFPE, 
	SIGSEGV, SIGPIPE, SIGTERM, SIGSTKFLT, 
	0
};

/* array to store the previous signal handlers */
typedef void (*sighandler) (int);
#define kMaxSigs  32
sighandler gSigs [kMaxSigs] = { 0 };

/* functions prototypes */
static void panic ();
static void sighandle (int signum);
static void patch_sigs (int * list);
static void restore_sigs ();


/*--------------------------------------------------------------------*/

void MidiInstall()
{
	
  msfd = open("/dev/MidiShare", 0);
  if (msfd < 0) {
	printf("Can't open /dev/MidiShare\n");
	exit(-1);
  }
  
  InitEvents ();
  InitMemory(Memory(gClients), 15000);
  pthread_mutex_init(&gClients->mutex, NULL);
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
	int            	err;
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
	gClients->appls[ref] = appl;
	g->nbAppls++;
}

/*--------------------------------------------------------------------------*/
/* event handler : to be notified by the kernel module */

MidiEvPtr MidiGetCommand(short ref );

void* event_handler(void* arg) 
{
	TAppl* appl = (TAppl*) arg;
	int refNum = appl->refNum;
	TTaskExtPtr task ;
	MidiEvPtr ev;
	
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
					
				case typeReset:
					// quit the thread
					goto exit;	
					break;		
							 
			}
			MSFreeEv(ev, FreeList(Memory(gClients)));
	}

	exit:
	pthread_exit(0);
}


/*******************/
/* MidiGetVersion  */
/*******************/

short MidiGetVersion() 
{
   TMidiGetVersionArgs 	args;
   int err;
   
   CALL(kMidiGetVersion,&args);
   return (short)args.version;
}


/*******************/
/* MidiCountAppls  */
/*******************/

short MidiCountAppls() 
{
   TMidiCountApplsArgs 	args;
   int err;

   CALL(kMidiCountAppls,&args);
   return (short)args.appls;
}

/*******************/
/* MidiGetIndAppl  */
/*******************/

short MidiGetIndAppl(short index) 
{
   TMidiGetIndApplArgs 	args;
   int err;

   args.refnumarg = index;
   CALL(kMidiGetIndAppl,&args);
   return (short)args.refnumres;
}

/********************/
/* MidiGetNamedAppl */
/********************/

short MidiGetNamedAppl(MidiName name)  
{
   TMidiGetNamedApplArgs args;
   int err;

   args.name = name;
   CALL(kMidiGetNamedAppl,&args);
   return (short)args.refnum;
}

/*******************/
/* MidiGetSyncInfo */
/*******************/

void MidiGetSyncInfo(SyncInfoPtr p)  
{
   int err;

   CALL(kMidiGetSyncInfo,p);
}


/*******************/
/* MidiSetSyncMode */
/*******************/

void MidiSetSyncMode(unsigned short mode)  
{
   TMidiSetSyncModeArgs args;
   int err;

   args.mode = mode;
   CALL(kMidiSetSyncMode,&args);
}


/*******************/
/* MidiGetExtTime  */
/*******************/

long MidiGetExtTime ()  
{
   TMidiGetExtTimeArgs args;
   int err;

   CALL(kMidiGetExtTime,&args);
   return args.time;
}


/*******************/
/* MidiInt2ExtTime */
/*******************/

long MidiInt2ExtTime (long date)  
{
   TMidiInt2ExtTimeArgs args;
   int err;

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
   int err;

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
   int err;

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
   int err;

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
   int err;

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
  	int           		err;
   	struct sched_param 	param;  // type defined in  /usr/include/bits/sched.h 
	uid_t uid = getuid ();

	pthread_mutex_lock(&gClients->mutex);
	
 	args.name = name;
  	CALL(kMidiOpen,&args);
	 
	if (gClients->nbAppls == 0) {
		OpenMemory (Memory(gClients));
		patch_sigs (gSigList);
	}
	
	if (gClients->nbAppls < MaxAppls) {
	
		appl = NewAppl (sizeof(TAppl));
		
		if (appl) {
		
			makeAppl(gClients, appl, args.refnum, name); 
			
			param.sched_priority = 99; /* 0 to 99  */
			pthread_create(&appl->rcvThread,NULL,event_handler,(void*)appl);
						
			setuid (name_to_uid ("root")); 
   			err = pthread_setschedparam(appl->rcvThread, SCHED_RR,  &param); 
 			setuid (uid);
		}
	}
	
	pthread_mutex_unlock(&gClients->mutex);
	return args.refnum;
}


  
/*******************/
/* MidiClose       */
/*******************/

void MidiClose (short ref)
{
	TMidiCloseArgs 	args;
    	int            	err;
	TApplPtr 	appl;
	
	pthread_mutex_lock(&gClients->mutex);
	
	if (CheckRefNum(gClients, ref)) {
 		
		appl = gClients->appls[ref];
 		args.refnum = ref;
		
		/* Resume the Real Time thread */
		
		CALL(kMidiQuit,&args);
		
		/* Wait for Real Time thread exit */
		
		pthread_join(appl->rcvThread,NULL);
		
		/* Kernel close */
	
		CALL(kMidiClose,&args);
		
		/* Client close */
		
		FreeAppl(appl) ;
		gClients->appls[ref] = 0;
		gClients->nbAppls--;
	
		if (gClients->nbAppls == 0) {
			CloseMemory(Memory(gClients));
			restore_sigs ();
		} 
	}
	
	pthread_mutex_unlock(&gClients->mutex);
}


/*******************/
/* MidiGetName     */
/*******************/

MidiName MidiGetName(short ref)
{
  TMidiGetNameArgs args;
  int err;
  
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
  int err;
  
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
	if (CheckRefNum(gClients,ref)) {
		info = gClients->appls[ref]->info;
	}
	return info;
}


/*******************/
/* MidiSetInfo     */
/*******************/

void MidiSetInfo (short ref ,void * info)
{
  	if (CheckRefNum(gClients,ref)) {
		gClients->appls[ref]->info = info;
	}
}


/*******************/
/* MidiGetFilter   */
/*******************/

MidiFilterPtr MidiGetFilter (short ref)
{
  TMidiGetFilterArgs args;
  int err;
  
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
  int err;
  
  args.refnum = ref;
  args.filter = filter;
  CALL(kMidiSetFilter ,&args);
}


/*******************/
/* MidiGetRcvAlarm */
/*******************/

RcvAlarmPtr MidiGetRcvAlarm (short ref)
{
	return CheckRefNum(gClients,ref) ? gClients->appls[ref]->rcvAlarm : 0;
}


/*******************/
/* MidiSetRcvAlarm */
/*******************/

void MidiSetRcvAlarm (short ref,RcvAlarmPtr alarm)
{
	TMidiSetRcvAlarmArgs args;
	int err;
	
	if (CheckRefNum(gClients,ref)) {
		args.refnum = ref;
		args.alarm = alarm;
		CALL(kMidiSetRcvAlarm,&args);
		gClients->appls[ref]->rcvAlarm = alarm;
	}
}


/*******************/
/* MidiGetApplAlarm */
/*******************/

ApplAlarmPtr MidiGetApplAlarm (short ref)
{
	return CheckRefNum(gClients,ref) ? gClients->appls[ref]->applAlarm : 0;
}


/*******************/
/* MidiSetApplAlarm */
/*******************/

void MidiSetApplAlarm (short ref,ApplAlarmPtr alarm)
{
	TMidiSetApplAlarmArgs args;
	int err;
	
	if (CheckRefNum(gClients,ref)) {
		args.refnum = ref;
		args.alarm = alarm;
		CALL(kMidiSetApplAlarm,&args);
		gClients->appls[ref]->applAlarm = alarm;
	}
}

/*******************/
/* MidiConnect */
/*******************/

void MidiConnect (short src,short dest , Boolean state)
{
  TMidiConnectArgs args;
  int err;
  
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
  int err;
  
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
  int err;
  
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

void MidiFreeCell (MidiEvPtr e) {  
	if (e) {	
		MSFreeCell(e, FreeList(Memory(gClients)));
	}
}

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
  int err;
  
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
  int            	err;
  
  CALL(kMidiGetTime ,&args);
  return args.time;
}

/*******************/
/* MidiSendIm */
/*******************/

void MidiSendIm (short ref, MidiEvPtr e)
{
	int  err;
	
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
	int  err;
	
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
	int  err;
	
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
  int err;
  
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
  int err;
  
  args.refnum = ref;
  CALL(kMidiFlushEvs ,&args);
}

/*******************/
/* MidiReadSync */
/*******************/

void* MidiReadSync (void* adr)
{
  TMidiReadSyncArgs args;
  int err;
  
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
  int err;
  
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
	int  err;
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
	int  err;
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
	int  err;
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
	int  err;
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
  int err;
  
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
  int err;
  
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
  int err;
  
  CALL(kMidiNewFilter,&args);
  return args.filter;
  
}

/*******************/
/* MidiFreeFilter */
/*******************/

void MidiFreeFilter (MidiFilterPtr filter)
{
  TMidiFreeFilterArgs args;
  int err;
  args.filter = filter;
  CALL(kMidiFreeFilter,&args);
  
}

/*******************/
/* MidiAcceptChan */
/*******************/

void MidiAcceptChan (MidiFilterPtr filter, short chan , Boolean state )
{
  TMidiAcceptChanArgs args;
  int err;
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
  int err;
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
  int err;
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
  int err;
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
  int err;
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
  int err;
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
   CHECK;
   return (msfd >0);
}

/*******************/
/* MidiReset  */
/*******************/

void MidiReset () 
{
  int err;
  TMidiCloseArgs args;
  CALL(kMidiReset,&args);
}


/*--------------------------------------------------------------------*/
static void panic ()
{
	TMidiCloseArgs args;
	int i, err;
	for (i=0; i<MaxAppls; i++) {
		if (gClients->appls[i]) {
			args.refnum = gClients->appls[i]->refNum;
			CALL(kMidiClose,&args);
		}
	}
	if (msfd > 0) close (msfd);
}

/*--------------------------------------------------------------------*/
static void sighandle (int signum)
{
	sighandler prev = gSigs[signum];
	panic ();

	if (prev) {
		prev (signum);
	}
	else if (signal (signum, SIG_DFL) != SIG_ERR) {
		raise (signum);
	}
	else {
		raise (SIGSTOP);
	}
}

/*--------------------------------------------------------------------*/
static void patch_sigs (int * list)
{
	void * sh; int signum;
	while (*list) {
		signum = *list;
		sh = signal (signum, sighandle);
		if (sh != SIG_ERR) {
			gSigs[signum] = sh;
		}
		else {
			gSigs[signum] = 0;
		}
		list++;
	}
}

/*--------------------------------------------------------------------*/
static void restore_sigs ()
{
	int i; sighandler sh;
	for (i=0; i<kMaxSigs; i++) {
		sh = gSigs[i];
		signal (i, sh ? sh : SIG_DFL);
	}
}


