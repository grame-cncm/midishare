
#include "Clean.h" 
#include "MidiShare.h"
#include "MidiShare_Clean.h"


#define MidiSharePPC_68k


#define TimeOutType 20

/*__________________________________________________________________________________*/
/* Task extension block                                                             */
typedef struct TTaskExt  *TTaskExtPtr;
typedef struct TTaskExt
{
    TaskPtr fun;
    long    arg1;
    long    arg2;
    long    arg3;
} TTaskExt;


static void NullAlarm(short ref) {}

static void TimeOutTask( long date, short ref, long a1,long a2,long a3 ) {
	printf( "EXEC\n");
	MidiSendIm(ref, MidiNewEv(TimeOutType));
}

MidiEvPtr gTask;
int gDate = 0;


/*--------------------------------------------------------------------------*/
 void CleanToC(char *dest, CleanString str) // chaine Clean dans chaine c
{
	int i;
	char* buffer = CleanStringCharacters(str);
	for (i = 0 ; i<CleanStringLength(str); i++) {dest[i] = buffer[i];}
	dest[i] = 0;
}

/*--------------------------------------------------------------------------*/
 void CToClean(CleanString dest, char * src) // chaine c dans chaine Clean
{
	int i=0;
	char* buffer = CleanStringCharacters(dest);
	while (src[i] != 0) {
		buffer[i] = src[i];
		i++;
	}
	CleanStringLength(dest) = i;
}

/*----------------------------------- MidiShare -------------------------------*/

int Clean_MidiShare	() {return MidiShare();}

/*--------------------------- Global MidiShare environment --------------------*/

int Clean_MidiGetVersion (){return MidiGetVersion();}
int Clean_MidiCountAppls (){return MidiCountAppls();}
int Clean_MidiGetIndAppl (int index){return MidiGetIndAppl(index);}	
int Clean_MidiGetNamedAppl (CleanString name){
	 char buffer [128];
	 CleanToC(buffer,name);
	 return MidiGetNamedAppl(buffer);
}

/*----------------------------- SMPTE synchronization -------------------------*/
// Not defined
/*----------------------------- Open/Close application ----------------------*/


int Clean_MidiOpen (CleanString name)
{
	char buffer [128];
	int ref;
	CleanToC(buffer,name);
	
	ref = MidiOpen1(buffer);
	/*
	printf( "msdisplay %ld \n" ,MidiGetNamedAppl ("msdisplay"));
	MidiConnect (ref, MidiGetNamedAppl ("msdisplay"),1);
	MidiConnect (MidiGetNamedAppl ("mscontrol"),ref ,1);
	MidiSetRcvAlarm(ref,NullAlarm);
	*/
	return ref;
}


void Clean_MidiClose(int ref) {MidiClose1(ref); }


/*--------------------------- Application configuration -----------------------*/

static struct {int length; char buffer[128]; } C_String ;

void Clean_MidiGetName (int refNum, CleanString* str){
	CToClean((CleanString)&C_String, MidiGetName(refNum));
	*str = (CleanString)&C_String; 
}

void Clean_MidiSetName (int refNum, CleanString name){
	 char buffer [128];
	 CleanToC(buffer,name);
	 return MidiSetName(refNum,buffer);
}

void Clean_MidiSetFilter (int refNum, int filter) {MidiSetFilter(refNum, (MidiFilterPtr)filter);}
int Clean_MidiGetFilter (int refNum){ return (int)MidiGetFilter(refNum);}

/*------------------------------- Drivers management --------------------------*/
// Not defined
/*-------------------------------- Slots management ---------------------------*/
// Not defined
/*------------------------- Inter-Application Connections ---------------------*/

void Clean_MidiConnect 	(int src, int dest , Boolean state) { MidiConnect (src,dest,state);}
int Clean_MidiIsConnected (int src, int dest) {return MidiIsConnected (src,dest);}


/*-------------------------------- Serial ports -------------------------------*/
// Not defined
/*-------------------------- Events and memory managing -----------------------*/

int Clean_MidiFreeSpace() {return MidiFreeSpace();}
int Clean_MidiDesiredSpace(){
	//return MidiDesiredSpace();
	return 0;
}
int Clean_MidiNewCell() {return (int) MidiNewCell();}
void Clean_MidiFreeCell(int cell) {MidiFreeCell((MidiEvPtr)cell);}
int Clean_MidiTotalSpace(){return MidiTotalSpace();}
int Clean_MidiGrowSpace(int space){return MidiGrowSpace(space);}

int Clean_MidiNewEv(int type) {return (int)MidiNewEv(type);}
int Clean_MidiCopyEv(int ev){return (int)MidiCopyEv((MidiEvPtr)ev);}
void Clean_MidiFreeEv(int ev){MidiFreeEv((MidiEvPtr)ev);}

void Clean_MidiSetField(int ev, int index, int value) { MidiSetField((MidiEvPtr)ev,index,value) ; }
int Clean_MidiGetField(int ev, int index) { return MidiGetField((MidiEvPtr)ev,index) ; }
void Clean_MidiAddField(int ev, int val) {MidiAddField((MidiEvPtr)ev,val) ; }
int Clean_MidiCountFields(int ev){ return MidiCountFields((MidiEvPtr)ev) ; }

/*------------------------------- Sequence managing ---------------------------*/

int Clean_MidiNewSeq() {return (int)MidiNewSeq();}

void Clean_MidiAddSeq(int seq, int ev)
{
	//printf("Clean_MidiAddSeq %ld\n",seq);
	//printf("Clean_MidiAddSeq %ld\n",ev);
	MidiAddSeq((MidiSeqPtr)seq, (MidiEvPtr)ev);
	//printf("Clean_MidiAddSeq First %ld\n",FirstEv((MidiSeqPtr)seq));
	//printf("Clean_MidiAddSeq Last %ld\n",LastEv((MidiSeqPtr)seq));
}

void Clean_MidiFreeSeq(int seq) {MidiFreeSeq((MidiSeqPtr)seq);}
void Clean_MidiClearSeq(int seq){MidiClearSeq((MidiSeqPtr)seq);}

/*------------------------------------- Time ----------------------------------*/

int Clean_MidiGetTime() {return MidiGetTime();}

/*------------------------------------ Sending --------------------------------*/

void Clean_MidiSendIm(int ref, int ev){MidiSendIm(ref, (MidiEvPtr) ev);}
void Clean_MidiSendAt(int ref, int ev, int date){MidiSendAt(ref, (MidiEvPtr) ev , date);}
void Clean_MidiSend(int ref, int ev){MidiSend(ref, (MidiEvPtr) ev);}

/*------------------------------------ Receving -------------------------------*/

int Clean_MidiCountEvs(int ref) {return MidiCountEvs(ref);}
int Clean_MidiGetEv(int ref){return (int)MidiGetEv(ref);}
void Clean_MidiFlushEvs(int ref){MidiFlushEvs (ref);}
int Clean_MidiAvailEv(int ref){return (int)MidiAvailEv(ref);}

/*----------------------------------- Mail boxes ------------------------------*/
// Not defined
/*---------------------------------- Task Managing ----------------------------*/
// Not defined
/*---------------------------------- Filters ----------------------------------*/

int Clean_MidiNewFilter() {return (int)MidiNewFilter();}
void Clean_MidiFreeFilter(int filter){ MidiFreeFilter((MidiFilterPtr)filter);}

void Clean_MidiAcceptPort(int filter, int port,  Boolean state){MidiAcceptPort((MidiFilterPtr)filter, port,state);}
void Clean_MidiAcceptChan(int filter, int chan,  Boolean state){MidiAcceptChan((MidiFilterPtr)filter, chan,state);}
void Clean_MidiAcceptType(int filter, int type,  Boolean state){MidiAcceptType((MidiFilterPtr)filter, type,state);}

int Clean_MidiIsAcceptedPort(int filter, int port){return MidiIsAcceptedPort((MidiFilterPtr)filter, port);}
int Clean_MidiIsAcceptedChan(int filter, int chan){return MidiIsAcceptedChan((MidiFilterPtr)filter, chan);}
int Clean_MidiIsAcceptedType(int filter, int type){return MidiIsAcceptedType((MidiFilterPtr)filter, type);}

/*---------------------------------- Fields ----------------------------------*/

void Clean_MidiSetDate(int ev, int date) {Date((MidiEvPtr) ev) = date;}
int Clean_MidiGetDate(int ev){return Date((MidiEvPtr) ev);}

void Clean_MidiSetType(int ev, int type){EvType((MidiEvPtr) ev) = type;}
int Clean_MidiGetType(int ev){return EvType((MidiEvPtr) ev);}

void Clean_MidiSetChan(int ev, int chan){Chan((MidiEvPtr) ev) = chan;}
int Clean_MidiGetChan(int ev){return Chan((MidiEvPtr) ev);}

void Clean_MidiSetPort(int ev, int port){Port((MidiEvPtr) ev) = port;}
int Clean_MidiGetPort(int ev){return Port((MidiEvPtr) ev);}

void Clean_MidiSetRefnum(int ev, int ref){RefNum((MidiEvPtr) ev) = ref;}
int Clean_MidiGetRefnum(int ev){return RefNum((MidiEvPtr) ev);}

void Clean_MidiSetLink(int ev, int link){Link((MidiEvPtr) ev) = (MidiEvPtr)link;}
int Clean_MidiGetLink(int ev){return (int)Link((MidiEvPtr) ev);}

void Clean_MidiSetFirstEv(int seq, int link){FirstEv((MidiSeqPtr) seq) = (MidiEvPtr)link;}
int Clean_MidiGetFirstEv(int seq){return (int)FirstEv((MidiSeqPtr)seq);}

void Clean_MidiSetLastEv(int seq, int link){LastEv((MidiSeqPtr) seq) = (MidiEvPtr)link;}
int Clean_MidiGetLastEv(int seq){return (int)LastEv((MidiSeqPtr) seq);}

/*---------------------------------- Special ----------------------------------*/

int Clean_MidiGetEvBlock(int ref) {
	MidiEvPtr ev,ev1 = 0;
	TTaskExtPtr task ;
	
	printf("MidiGetEvBlock 0\n");
	
	if (MidiCountEvs(ref) > 0){
		ev1 = MidiGetEv(ref);
		printf("MidiGetEvBlock 3 %ld \n", EvType(ev1));
		return (int)ev1;
	}else{
		printf("MidiGetEvBlock 3bis\n");
		
	     	while ((ev = (MidiEvPtr)MidiGetCommand(ref)))   { 
			printf("MidiGetEvBlock 1\n");
			
			if (EvType(ev) == typeProcess ){
				task = (TTaskExtPtr)LinkST(ev);	
				printf("TASK \n");
				if (task->fun) (*task->fun) (ev->date, ref, task->arg1, task->arg2, task->arg3);			
			}else if (EvType(ev) == typeRcvAlarm ){
				printf("MidiCountEvs %ld\n", MidiCountEvs(ref));
				ev1 = MidiGetEv(ref);
				if (ev1) printf("MidiGetEvBlock 4 %ld \n", EvType(ev1));
			}else 
				ev1 = 0;
			
			MidiFreeEv(ev);
			if (ev1) return (int)ev1;
		}   
		printf("MidiGetEvBlock 6\n");
	}
}

int Clean_MidiWait(int ref, int dur) {
	usleep(dur*1000);
	return ref;
}

int Clean_MidiWaitTimeOut(int ref, int dur) {
	MidiEvPtr ev;
	int date = MidiGetTime();
	gTask = MidiTask(TimeOutTask, date + dur, ref , 0,0,0);
	ev = (MidiEvPtr)Clean_MidiGetEvBlock(ref);
	if (EvType (ev) != TimeOutType)MidiForgetTask(&gTask);
	Date(ev) -= date;
	return (int)ev;
}

