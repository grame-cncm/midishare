/*

  Copyright � Grame 1999

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
  
  modifications history:
   [08-09-99] DF - adaptation to new data structures

*/


#include <linux/kernel.h>
#include <linux/module.h>

#if CONFIG_MODVERSIONS==1
#define MODVERSIONS
#include <linux/modversions.h>
#endif

#include <linux/fs.h>		/* for devices	*/
#include <asm/uaccess.h>	/* for put_user	*/
#include <linux/sched.h>	/* for current 	*/
#include <linux/tty.h>		/* for tty_struct 	*/
#include <linux/slab.h>

#include "msKernel.h"
#include "msInit.h"
#include "msLoader.h"
#include "msAppFun.h"
#include "msTasks.h"
#include "msXmtRcv.h"
#include "msEvents.h"
#include "msConnx.h"
#include "msFilter.h"

//_________________________________________________________
void CallQuit (TApplContextPtr context);
MidiEvPtr MSGetCommand (short refNum, TClientsPtr g);
MidiEvPtr MSGetDTask (short refNum, TClientsPtr g);

//_________________________________________________________
// DECLARATION DES VARIABLES GLOBALES
TMSGlobal   	gMSMem;
TMSGlobalPtr	gMem = &gMSMem;


/*--------------------------- Global MidiShare environment --------------------*/
MSFunctionType(void) MidiShareSpecialInit(unsigned long defaultSpace) {
  	MSSpecialInit (defaultSpace, gMem);
}


/*__________________________________________________________________________________*/
/* Utilities */


/*__________________________________________________________________________________*/

int mskGetVersion(unsigned long userptr)
{
	TMidiGetVersionArgs args;
	
	args.version = MSGetVersion(gMem);
	if (copy_to_user((TMidiGetVersionArgs *)userptr, &args, sizeof(TMidiGetVersionArgs))) return -EFAULT;
	
	return 0 ;
}
/*__________________________________________________________________________________*/

int mskCountAppls(unsigned long userptr) 
{ 
	TMidiCountApplsArgs args;
	
	args.appls = MSCountAppls(Clients(gMem));
	if (copy_to_user((TMidiCountApplsArgs *)userptr, &args, sizeof(TMidiCountApplsArgs))) return -EFAULT;
	
	return 0 ;
}

/*__________________________________________________________________________________*/

int mskGetIndAppl(unsigned long userptr) 
{ 
	TMidiGetIndApplArgs args;
	
	if (copy_from_user(&args, (TMidiGetIndApplArgs *)userptr, sizeof(TMidiGetIndApplArgs))) return -EFAULT;
	args.refnumres = MSGetIndAppl(args.refnumarg, Clients(gMem));
	if (copy_to_user((TMidiGetIndApplArgs *)userptr, &args, sizeof(TMidiGetIndApplArgs))) return -EFAULT;
	
	return 0 ;
}

/*__________________________________________________________________________________*/

int mskGetNamedAppl(unsigned long userptr)
{ 
	TMidiGetNamedApplArgs args;
	MSName  buffer ;
	
	if (copy_from_user(&args, (TMidiGetNamedApplArgs *)userptr, sizeof(TMidiGetNamedApplArgs))) return -EFAULT;
	
	strncpy_from_user(buffer, args.name, MaxApplNameLen);
	args.refnum = MSGetNamedAppl(buffer, Clients(gMem));
	
	if (copy_to_user((TMidiGetNamedApplArgs *)userptr, &args, sizeof(TMidiGetNamedApplArgs))) return -EFAULT;
	
	return 0 ;
}

/*__________________________________________________________________________________*/

int mskGetSyncInfo (unsigned long userptr)
{ 
	/*
	TMidiGetSyncInfoArgs args;
	
	MSGetSyncInfo(args, gMem);
	if (copy_to_user((TMidiGetSyncInfoArgs *)userptr, &args, sizeof(TMidiGetSyncInfoArgs))) return -EFAULT;
	*/
	
	return 0 ;
}
/*__________________________________________________________________________________*/

int mskSetSyncMode (unsigned long userptr)
{ 
	/*
	TMidiSetSyncModeArgs args;
	
	if (copy_from_user(&args, (TMidiSetSyncModeArgs *)userptr, sizeof(TMidiSetSyncModeArgs))) return -EFAULT;
	MSSetSyncMode(args.mode, gMem);
	*/
	
	return 0 ;
}
/*__________________________________________________________________________________*/
/* unimplemented functions : to be done later */

int mskGetExtTime (unsigned long userptr){ return 0 ;}
int mskInt2ExtTime (unsigned long userptr){ return 0 ;}
int mskExt2IntTime (unsigned long userptr){ return 0 ;}
int mskTime2Smpte (unsigned long userptr){ return 0 ;}
int mskSmpte2Time (unsigned long userptr){ return 0 ;}
int mskGetTimeAddr  (unsigned long userptr){ return 0 ;}

/*__________________________________________________________________________________*/

int mskOpen(unsigned long userptr)
{
	TMidiOpenArgs args;
	MSName  buffer ;
	
	if (copy_from_user(&args, (TMidiOpenArgs *)userptr, sizeof(TMidiOpenArgs))) return -EFAULT;
	
	strncpy_from_user(buffer, args.name, MaxApplNameLen);
	args.refnum = MSOpen (buffer, gMem);
	
	if (copy_to_user((TMidiOpenArgs *)userptr, &args, sizeof(TMidiOpenArgs))) return -EFAULT;

	return 0;	
}

/*__________________________________________________________________________________*/
int mskClose(unsigned long userptr)
{
	TMidiCloseArgs args;
	TClientsPtr clients = Clients(gMem);
	TApplPtr appl;
	
	if (copy_from_user(&args, (TMidiCloseArgs *)userptr, sizeof(TMidiCloseArgs))) return -EFAULT;
	
	appl = clients->appls[args.refnum];
	CallQuit(appl->context);	
	MSClose (args.refnum,gMem);
	
	return 0;	
}

/*__________________________________________________________________________________*/

int mskGetName   (unsigned long userptr){ 

	TMidiGetNameArgs args;
	MidiName resname;
	
	if (copy_from_user(&args, (TMidiGetNameArgs *)userptr, sizeof(TMidiGetNameArgs))) return -EFAULT;
	
	resname = MSGetName(args.refnum, Clients(gMem));
	
	if (resname) {
		if (copy_to_user((TMidiGetNameArgs *)userptr, &args, sizeof(TMidiGetNameArgs))) return -EFAULT;
		if (copy_to_user(((TMidiGetNameArgs *)userptr)->name, resname, MaxApplNameLen)) return -EFAULT;
	}else{
		args.name = 0;
		if (copy_to_user((TMidiGetNameArgs *)userptr, &args, sizeof(TMidiGetNameArgs))) return -EFAULT;
	}
	
	return 0 ;
}

/*__________________________________________________________________________________*/

int mskSetName  (unsigned long userptr){ 

	TMidiSetNameArgs args;
	MSName  buffer ;
	
	if (copy_from_user(&args, (TMidiSetNameArgs *)userptr, sizeof(TMidiSetNameArgs))) return -EFAULT;
	
	strncpy_from_user(buffer, args.name, MaxApplNameLen);
	MSSetName(args.refnum,buffer, Clients(gMem));
	
	if (copy_to_user((TMidiSetNameArgs *)userptr, &args, sizeof(TMidiSetNameArgs))) return -EFAULT;
	
	return 0 ;
}
/*__________________________________________________________________________________*/
/* unimplemented functions : done on the library side */

int mskGetInfo  (unsigned long userptr){ return 0 ;}
int mskSetInfo  (unsigned long userptr){ return 0 ;}

/*__________________________________________________________________________________*/

int mskGetFilter  (unsigned long userptr){ 

	TMidiGetFilterArgs args;
	
	if (copy_from_user(&args, (TMidiGetFilterArgs *)userptr, sizeof(TMidiGetFilterArgs))) return -EFAULT;
	args.filter = MSGetFilter(args.refnum, Clients(gMem));
	if (copy_to_user((TMidiGetFilterArgs *)userptr, &args, sizeof(TMidiGetFilterArgs))) return -EFAULT;
	
	return 0 ;
}

/*__________________________________________________________________________________*/

int mskSetFilter  (unsigned long userptr){ 

	TMidiSetFilterArgs args;
	
	if (copy_from_user(&args, (TMidiSetFilterArgs *)userptr, sizeof(TMidiSetFilterArgs))) return -EFAULT;
	MSSetFilter(args.refnum,args.filter, Clients(gMem));
	if (copy_to_user((TMidiSetFilterArgs *)userptr, &args, sizeof(TMidiSetFilterArgs))) return -EFAULT;
	
	return 0 ;
}

/*__________________________________________________________________________________*/

int mskSetRcvAlarm   (unsigned long userptr)
{	
	TMidiSetRcvAlarmArgs args;
	
	if (copy_from_user(&args, (TMidiSetRcvAlarmArgs *)userptr, sizeof(TMidiSetRcvAlarmArgs))) return -EFAULT;
	MSSetRcvAlarm(args.refnum,args.alarm, Clients(gMem));
	
	return 0 ;
}

/*__________________________________________________________________________________*/

int mskSetApplAlarm   (unsigned long userptr)
{
	TMidiSetApplAlarmArgs args;
	
	if (copy_from_user(&args, (TMidiSetApplAlarmArgs *)userptr, sizeof(TMidiSetApplAlarmArgs))) return -EFAULT;
	MSSetApplAlarm(args.refnum,args.alarm, Clients(gMem));
	
	return 0 ;
}

/*__________________________________________________________________________________*/

int mskConnect(unsigned long userptr)
{
	TMidiConnectArgs args;
	
	if (copy_from_user(&args, (TMidiConnectArgs *)userptr, sizeof(TMidiConnectArgs))) return -EFAULT;
	MSConnect(args.src, args.dest, args.state, Clients(gMem));
	return 0;	
}

/*__________________________________________________________________________________*/

int mskIsConnected(unsigned long userptr)
{
	TMidiIsConnectedArgs args;
	
	if (copy_from_user(&args, (TMidiIsConnectedArgs *)userptr, sizeof(TMidiIsConnectedArgs))) return -EFAULT;
	args.state = MSIsConnected(args.src, args.dest,Clients(gMem));
	if (copy_to_user((TMidiIsConnectedArgs *)userptr, &args, sizeof(TMidiIsConnectedArgs))) return -EFAULT; 
	return 0;	
}

/*__________________________________________________________________________________*/
/* unimplemented functions : to be done later */

int mskGetPortState  (unsigned long userptr){ return 0 ;}
int mskSetPortState (unsigned long userptr){ return 0 ;}

/*__________________________________________________________________________________*/

int mskGetTime(unsigned long userptr)
{
	TMidiGetTimeArgs args;
	
	args.time = CurrTime(gMem);
	if (copy_to_user((TMidiGetTimeArgs *)userptr, &args, sizeof(TMidiGetTimeArgs))) return -EFAULT;
	return 0;	
}

/*__________________________________________________________________________________*/

/* -- Table of event structures */

static char gEvStruct[256];			
static void InitStructTbl();

#define typeLastReserved    typeDead-1    /* last reserved event type      */

/* -- Types of event structures */

#define kUndefStruct 	0
#define kSmallStruct 	1
#define kPrivateStruct 	2
#define kSexStruct 	3
#define kProcessStruct  4
#define kDeadStruct  	5

static MidiEvPtr ev_from_user( MidiEvPtr ue, TMSGlobalPtr g);
static MidiEvPtr cell_from_user (MidiEvPtr uc, TMSGlobalPtr g);

/*__________________________________________________________________________________*/

void InitStructTbl()
{
  int i;

  for (i=typeNote;i<=typeReset;i++)            	gEvStruct[i]= kSmallStruct;
  for (i=typeSysEx;i<=typeStream;i++)          	gEvStruct[i]= kSexStruct;
  for (i=typePrivate;i<=typeDProcess;i++)      	gEvStruct[i]= kPrivateStruct;
  for (i=typeCtrl14b;i<=typeSeqNum;i++)        	gEvStruct[i]= kSmallStruct;
  for (i=typeTextual;i<=typeCuePoint;i++)      	gEvStruct[i]= kSexStruct;
  for (i=typeChanPrefix;i<=typeKeySign;i++)    	gEvStruct[i]= kSmallStruct;
  for (i=typeReserved;i<=typeLastReserved;i++) 	gEvStruct[i]= kUndefStruct;
  gEvStruct[typeQuarterFrame] = kSmallStruct;
  gEvStruct[typeSpecific]     = kSexStruct;
  gEvStruct[typeProcess]    = kProcessStruct;
  gEvStruct[typeDProcess]   = kProcessStruct;
  
  gEvStruct[typeDead]       = kDeadStruct;
}

/*__________________________________________________________________________________*/

int mskSend(unsigned long ue)
{
	MidiEvPtr e;
	
	e = ev_from_user((MidiEvPtr)ue,gMem);
	if (e) MSSend (RefNum(e),e, SorterList(gMem));
	return 0;	
}

/*__________________________________________________________________________________*/

int mskSendIm(unsigned long ue)
{
	MidiEvPtr e;
	
	e = ev_from_user((MidiEvPtr)ue,gMem);
	if (e) MSSendAt (RefNum(e),e, CurrTime(gMem), SorterList(gMem));
	return 0;	
}

/*__________________________________________________________________________________*/

int mskSendAt(unsigned long ue)
{
	MidiEvPtr e;
	
	e = ev_from_user((MidiEvPtr)ue,gMem);
	if (e) MSSend (RefNum(e),e, SorterList(gMem));
	return 0;	
}

/*__________________________________________________________________________________*/

int mskCountEvs(unsigned long userptr)
{ 
	TMidiCountEvsArgs args;
	
	if (copy_from_user(&args, (TMidiCountEvsArgs *)userptr, sizeof(TMidiCountEvsArgs))) return -EFAULT;
	args.evs = MSCountEvs(args.refnum, Clients(gMem));
	if (copy_to_user((TMidiCountEvsArgs *)userptr, &args, sizeof(TMidiCountEvsArgs))) return -EFAULT;
	
	return 0 ;
}

/*__________________________________________________________________________________*/

static MidiEvPtr getUserLink (MidiEvPtr ue)
{
	MidiEvPtr l=0;
	copy_from_user(&l, &ue->link, sizeof(MidiEvPtr));
	return l;
}

/*__________________________________________________________________________________*/

static void setUserLink(MidiEvPtr ue, MidiEvPtr l)
{
	copy_to_user(&ue->link, &l, sizeof(MidiEvPtr));
}


/*__________________________________________________________________________________*/

static int mskGetEvAux(MidiEvPtr ke, TMidiGetEvArgs* arg, TMidiGetEvArgs* userptr)
{
	MidiEvPtr ex, kh, kn, uh, un, ul;
	long i;
	int res = 0;
	
	ex= kh= kn= uh= un= ul = 0;
	if (ke == 0) return 0;
	
	switch (gEvStruct[ke->evType]) {   /* A REVOIR */
	
		case kUndefStruct : 	
	  	case kSmallStruct : 
		  	arg->u = 1;
	  		if 	(arg->d >= 1) {
		  		arg->e = arg->l;
		  		arg->l = getUserLink(arg->l);
		  		arg->d--;
		  		copy_to_user(arg->e, ke, sizeof(TMidiEv));
				res = 0;
			} else {
				res = 1;
			}
		  	copy_to_user(userptr, arg, sizeof(TMidiGetEvArgs));
		  	return res;
			
		case kProcessStruct :
			arg->e = (MidiEvPtr)ke->info.linkST; /* restore the extension (a pointer to a user event) */
			copy_to_user(userptr, arg, sizeof(TMidiGetEvArgs));
			(MidiEvPtr)ke->info.linkST = 0;
			return res;
		
		case kDeadStruct :
			//prnt("GET kDeadStruct ");
			return res;
		
	  	case kPrivateStruct:
			arg->u = 2;
	  		if 	(arg->d >= 2) {
		  		arg->e = arg->l;
				ex = getUserLink(arg->l);
		  		arg->l = getUserLink(ex);
		  		arg->d -= 2;
		  		copy_to_user(ex, ke->info.linkST, sizeof(TMidiEv));
				copy_to_user(arg->e, ke, sizeof(TMidiEv));
				copy_to_user(&arg->e->info.linkST, &ex, sizeof(MidiEvPtr));
				res = 0;
			} else { 
				res = 1;
			}
		  	copy_to_user(userptr, arg, sizeof(TMidiGetEvArgs));
		  	return res;
			
	  	case kSexStruct	:
			/* -- calcul du nombre de cellules necessaires */
			arg->u = 1; kh = (MidiEvPtr) ke->info.linkSE;
			kn = kh; do { arg->u++; } while ((kn=kn->link) != kh);
			if (arg->d >= arg->u) {
				/* -- il y en a suffisamment */ 
				arg->e = arg->l;
				uh = un = getUserLink(arg->l);
				copy_to_user(arg->e, ke, sizeof(TMidiEv));
				copy_to_user(&arg->e->info.linkSE, &uh, sizeof(MidiEvPtr));
				for (kn=kh, i = arg->u - 1; i>0; i--) {
					copy_to_user(&un->date, &kn->date, sizeof(TMidiEv) - sizeof(MidiEvPtr));
					ul = un; 
					un = getUserLink(un); 
					kn = kn->link;
				}
				arg->l = un;
				setUserLink(ul, uh);
				arg->d -= arg->u;
				res = 0;
			} else {
				res = 1;
			}			
			copy_to_user(userptr, arg, sizeof(TMidiGetEvArgs));
		  	return res;
	}
	
	return 0;
}


/*__________________________________________________________________________________*/

int mskGetEv(unsigned long userptr)
{
	TMidiGetEvArgs 	arg;
	MidiEvPtr ev;
	int res;
	
	if (copy_from_user(&arg, (TMidiGetEvArgs* )userptr, sizeof(TMidiGetEvArgs))) return -EFAULT;
	
	ev = MSAvailEv(arg.r, Clients(gMem));
	res = mskGetEvAux(ev, &arg, (TMidiGetEvArgs*)userptr);
	
	if (res == 0) { /* successfull copy */
		ev = MSGetEv(arg.r, Clients(gMem));  /* get the event from the fifo and free it */
		MSFreeEv(ev,FreeList(Memory(gMem)));
	}
	
	return 0;
}

/*__________________________________________________________________________________*/

int mskGetCommand(unsigned long userptr)
{
	TMidiGetEvArgs 	arg;
	MidiEvPtr ev;
	
	if (copy_from_user(&arg, (TMidiGetEvArgs* )userptr, sizeof(TMidiGetEvArgs))) return -EFAULT;
	
	/* check that the event is not a forgeted task */
	while ((ev = MSGetCommand(arg.r, Clients(gMem))) && (EvType(ev) == typeDead)) {
		MSFreeEv(ev,FreeList(Memory(gMem)));
	}
	
	mskGetEvAux(ev, &arg, (TMidiGetEvArgs* )userptr);
	MSFreeEv(ev,FreeList(Memory(gMem)));
	
	return 0;
}


/* A REVOIR */
/*__________________________________________________________________________________*/

int mskAvailEv (unsigned long userptr)
{
	TMidiAvailEvArgs 	arg;

	if (copy_from_user(&arg, (TMidiAvailEvArgs* )userptr, sizeof(TMidiAvailEvArgs))) return -EFAULT;
	
	mskGetEvAux(MSAvailEv(arg.r, Clients(gMem)), &arg, (TMidiAvailEvArgs*)userptr);
	
	return 0;
}


/*__________________________________________________________________________________*/

static MidiEvPtr cell_from_user (MidiEvPtr uc, TMSGlobalPtr g)
{
	MidiEvPtr kc;
	
	if ( uc && (kc = MSNewCell(FreeList(Memory(g)))) && (copy_from_user(kc, uc, sizeof(TMidiEv)) == 0) ) {
		return kc;
	} else {
		return 0;
	}
}

/*__________________________________________________________________________________*/

static MidiEvPtr ev_from_user (MidiEvPtr ue, TMSGlobalPtr g)
{
	MidiEvPtr ke=0, kx, fu, fk, ck, nk;
	
	if ( (ke=cell_from_user(ue,g)) ) {
	
		switch (gEvStruct[ke->evType]) {
				
			case kSmallStruct :
				return ke;
				
			case kProcessStruct :
				(MidiEvPtr)ke->info.linkST = ue; /* keeps the user event in the extension cell */
				return ke;
		
			case kPrivateStruct :
				if ((kx=cell_from_user((MidiEvPtr)ke->info.linkST, g))) {
					ke->info.linkST=(MidiSTPtr)kx;
					return ke;
				} else {
					MSFreeCell(ke,FreeList(Memory(g)));
					return 0;
				}
			
			case kSexStruct :
				fu = (MidiEvPtr)ke->info.linkSE;
				fk = cell_from_user(fu,g); if ( !fk ) { MSFreeCell (ke,FreeList(Memory(g))); return 0; }
				ke->info.linkSE = (MidiSEXPtr)fk;
				ck = fk;
				while (ck->link != fu) {
					nk = cell_from_user(ck->link,g); if (!nk) { ck->link = fk; MSFreeEv(ke,FreeList(Memory(g))); return 0; }
					ck->link = nk; 
					ck = nk;
				}
				ck->link = fk;
				return ke;
			
			case kUndefStruct:
				return 0;
		}
	}
	return 0;
}


/*__________________________________________________________________________________*/

int mskFlushEvs(unsigned long userptr)
{ 
	TMidiFlushEvsArgs args;
	
	if (copy_from_user(&args, (TMidiFlushEvsArgs *)userptr, sizeof(TMidiFlushEvsArgs))) return -EFAULT;
	MSFlushEvs(args.refnum, Clients(gMem));

	return 0 ;
}

/*__________________________________________________________________________________*/
/* unimplemented functions */

int mskReadSync(unsigned long userptr){ return 0 ;}
int mskWriteSync(unsigned long userptr){ return 0 ;}

/*__________________________________________________________________________________*/
/* unimplemented functions : done on the libarary side */

int mskCall(unsigned long userptr){ return 0 ;}

/*__________________________________________________________________________________*/

int mskTask(unsigned long userptr){ 

	TMidiTaskArgs args;
	
	if (copy_from_user(&args, (TMidiTaskArgs *)userptr, sizeof(TMidiTaskArgs))) return -EFAULT;
	
	args.ktask = ev_from_user(args.utask,gMem);
	if (args.ktask) MSSend (args.refnum, args.ktask, SorterList(gMem));
	if (copy_to_user((TMidiTaskArgs *)userptr, &args, sizeof(TMidiTaskArgs))) return -EFAULT;
	
	return 0 ;
}

/*__________________________________________________________________________________*/

int mskDTask(unsigned long userptr){ return mskTask(userptr);}

/*__________________________________________________________________________________*/

static Boolean ForgetTaskSync (MidiEvPtr * taskPtr, MidiEvPtr content)
{
	Boolean ret = false;
	//INT_OFF();
	
	if (*taskPtr == content) {
      	EvType(content) = typeDead;
		/*
    	*taskPtr = 0;
		*/
    	ret = true;
	}
	//INT_ON();
	return ret;
}

/*__________________________________________________________________________________*/

int mskForgetTask (unsigned long userptr){ 

	TMidiForgetTaskArgs args;
	MidiEvPtr ktask; 
	
	if (copy_from_user(&args, (TMidiForgetTaskArgs *)userptr, sizeof(TMidiForgetTaskArgs))) return -EFAULT;
	
	ktask = args.taskptr ? *args.taskptr : 0;
	
	if (ktask && ((EvType(ktask) == typeProcess) || (EvType(ktask) == typeDProcess))) {
      	ForgetTaskSync (args.taskptr, ktask);
	}
	
	args.utask = (MidiEvPtr)LinkST(ktask);
	if (copy_to_user((TMidiForgetTaskArgs *)userptr, &args, sizeof(TMidiForgetTaskArgs))) return -EFAULT;
			
	return 0 ;
}

/*__________________________________________________________________________________*/

int mskCountDTasks(unsigned long userptr)
{ 
	TMidiCountDTasksArgs args;
	
	if (copy_from_user(&args, (TMidiCountDTasksArgs *)userptr, sizeof(TMidiCountDTasksArgs))) return -EFAULT;
	args.res = MSCountDTasks(args.refnum, Clients(gMem));
	if (copy_to_user((TMidiCountDTasksArgs *)userptr, &args, sizeof(TMidiCountDTasksArgs))) return -EFAULT;
	
	return 0 ;
}

/*__________________________________________________________________________________*/

int mskFlushDTasks(unsigned long userptr)
{ 
	TMidiFlushDTasksArgs args;
	
	if (copy_from_user(&args, (TMidiFlushDTasksArgs *)userptr, sizeof(TMidiFlushDTasksArgs))) return -EFAULT;
	MSFlushDTasks(args.refnum, Clients(gMem));
	return 0 ;
}

/*__________________________________________________________________________________*/

int mskGetDTask(unsigned long userptr)
{
	TMidiGetEvArgs 	arg;
	MidiEvPtr ev;

	if (copy_from_user(&arg, (TMidiGetEvArgs* )userptr, sizeof(TMidiGetEvArgs))) return -EFAULT;
	
	ev = MSGetDTask(arg.r, Clients(gMem));
	mskGetEvAux(ev, &arg, (TMidiGetEvArgs*)userptr);
	//MSFreeEv(ev,FreeList(Memory(gMem)));
	
	return 0;
}

/*__________________________________________________________________________________*/

int mskNewFilter (unsigned long userptr)
{
	TMidiNewFilterArgs args;
	
	args.filter = MSNewFilter();
	if (copy_to_user((TMidiNewFilterArgs *)userptr, &args, sizeof(TMidiNewFilterArgs))) return -EFAULT;
	
	return 0 ;
}	

/*__________________________________________________________________________________*/

int mskFreeFilter (unsigned long userptr)
{
	TMidiFreeFilterArgs args;
	
	if (copy_from_user(&args, (TMidiFreeFilterArgs* )userptr, sizeof(TMidiFreeFilterArgs))) return -EFAULT;
	MSFreeFilter(args.filter);
	
	return 0 ;
}	

/*__________________________________________________________________________________*/

int mskAcceptChan (unsigned long userptr)
{
	TMidiAcceptChanArgs args;
	
	if (copy_from_user(&args, (TMidiAcceptChanArgs* )userptr, sizeof(TMidiAcceptChanArgs))) return -EFAULT;
	MSAcceptChan(args.filter,args.generic, args.state);
	
	return 0 ;
}	

/*__________________________________________________________________________________*/

int mskAcceptPort (unsigned long userptr)
{
	TMidiAcceptPortArgs args;
	
	if (copy_from_user(&args, (TMidiAcceptPortArgs* )userptr, sizeof(TMidiAcceptPortArgs))) return -EFAULT;
	MSAcceptPort(args.filter,args.generic, args.state);
	
	return 0 ;
}	

/*__________________________________________________________________________________*/

int mskAcceptType (unsigned long userptr)
{
	TMidiAcceptTypeArgs args;
	
	if (copy_from_user(&args, (TMidiAcceptTypeArgs* )userptr, sizeof(TMidiAcceptTypeArgs))) return -EFAULT;
	MSAcceptType(args.filter,args.generic, args.state);
	
	return 0 ;
}	

/*__________________________________________________________________________________*/

int mskIsAcceptedChan (unsigned long userptr)
{
	TMidiIsAcceptedChanArgs args;
	
	if (copy_from_user(&args, (TMidiIsAcceptedChanArgs* )userptr, sizeof(TMidiIsAcceptedChanArgs))) return -EFAULT;
	args.state = MSIsAcceptedChan(args.filter,args.generic);
	if (copy_to_user((TMidiIsAcceptedChanArgs *)userptr, &args, sizeof(TMidiIsAcceptedChanArgs))) return -EFAULT;
	
	return 0 ;
}	

/*__________________________________________________________________________________*/

int mskIsAcceptedPort (unsigned long userptr)
{
	TMidiIsAcceptedPortArgs args;
	
	if (copy_from_user(&args, (TMidiIsAcceptedPortArgs* )userptr, sizeof(TMidiIsAcceptedPortArgs))) return -EFAULT;
	args.state = MSIsAcceptedPort(args.filter,args.generic);
	if (copy_to_user((TMidiIsAcceptedPortArgs *)userptr, &args, sizeof(TMidiIsAcceptedPortArgs))) return -EFAULT;
	
	return 0 ;
}	

/*__________________________________________________________________________________*/

int mskIsAcceptedType (unsigned long userptr)
{
	TMidiIsAcceptedTypeArgs args;
	
	if (copy_from_user(&args, (TMidiIsAcceptedTypeArgs* )userptr, sizeof(TMidiIsAcceptedTypeArgs))) return -EFAULT;
	args.state = MSIsAcceptedType(args.filter,args.generic);
	if (copy_to_user((TMidiIsAcceptedTypeArgs *)userptr, &args, sizeof(TMidiIsAcceptedTypeArgs))) return -EFAULT;
	
	return 0 ;
}	
