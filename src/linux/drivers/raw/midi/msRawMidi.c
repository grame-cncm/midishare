 
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <signal.h> 
#include <pthread.h> 

#include "MidiShare.h"
#include "EventToMidiStream.h"
#include "MidiStreamToEvent.h"


//_______________________________________________________________________
// driver datas
typedef struct {
	int 		devId;
	short		refNum;
	short		done;
	
	pthread_t	inThread;
	MidiFilterPtr	filter;
	
	StreamFifo	inFifo;
	ParseMethodTbl	inMthd;
	Status2TypeTbl	inTypes;
	
	Ev2StreamRec	outFifo;
	LinearizeMthTbl	outMthd;

} DrvMem, * DrvMemPtr;

DrvMem gMem = { 0 };
static inline DrvMemPtr GetData()	{ return &gMem; }

//_______________________________________________________________________
static void closeDriver (DrvMemPtr mem) 
{
	if (mem->devId) {
		close(mem->devId);
		mem->devId = 0;
	}
	if (mem->refNum > 0) {
		MidiClose (mem->refNum);
		mem->refNum = 0;
	}
	if (mem->filter) {
		MidiFreeFilter (mem->filter);
		mem->filter = 0;
	}
}

//_______________________________________________________________________
static void FatalError (const char *msg) 
{
	fprintf (stderr, "%s\n", msg);
	closeDriver (GetData());
	exit (1);
}

//_______________________________________________________________________
static void usage (const char *name) 
{
	char str[256];
	sprintf (str, "usage : %s <device name>\n", name);
	FatalError (str);
}


//_______________________________________________________________________
static void CheckMidiShare () 
{
	if (!MidiShare ()) {
		FatalError ("MidiShare is not installed");
	}
}

//_______________________________________________________________________
static int openDev (const char *name)
{
	int dev = open (name, O_RDWR);
	if (dev == -1) {
		char str[256];
		sprintf (str, "can't open device \"%s\" (errno=%d)", name, errno);
		FatalError (str);
	}
	return dev;
}

//_______________________________________________________________________
static void KeyOffTask (long date, short refNum, long a1,long a2,long a3)
{
	MidiEvPtr e = (MidiEvPtr)a1;
	DrvMemPtr mem = (DrvMemPtr)a2;
	Ev2StreamPtr f = &mem->outFifo;
	Byte c;

	MidiStreamPutEvent (f, e);
	while (MidiStreamGetByte (f, &c)) {
		write (mem->devId, &c, 1);
	}
}

//_______________________________________________________________________
static void RcvAlarm (short refNum)
{
	DrvMemPtr mem = GetData ();
	Ev2StreamPtr f = &mem->outFifo;
	MidiEvPtr off, e = MidiGetEv (refNum);
	Byte c;
	
	while (e) {
		off = MidiStreamPutEvent (f, e);
		while (MidiStreamGetByte (f, &c)) {
			write (mem->devId, &c, 1);
		}
		if (off) MidiTask (KeyOffTask, Date(off), refNum, (long)off, (long)mem, 0);
		e = MidiGetEv (refNum);
	}
}

//_______________________________________________________________________
static void SetupFilter (MidiFilterPtr filter)
{
	short i;
	for (i = 0; i < 256; i++) {
		MidiAcceptPort(filter, i, true);
		MidiAcceptType(filter, i, false);
	}
	for (i = 0; i < 16; i++)
		MidiAcceptChan(filter, i, true);
	for (i = 0; i <= typeProcess; i++)
		MidiAcceptType(filter, i, true);
	for (i = typeQuarterFrame; i <= typeRegParam; i++)
		MidiAcceptType(filter, i, true);
}

//_______________________________________________________________________
static void SetupMidi (char *name, DrvMemPtr mem)
{
	MidiFilterPtr filter;
	short ref = MidiOpen (name);
	if (ref < 0) FatalError ("MidiOpen failed !");
	mem->refNum = ref;

	filter = MidiNewFilter ();
	if (!filter) FatalError ("MidiNewFilter failed !");
	mem->filter = filter;
	SetupFilter (filter);
	MidiSetFilter (ref, filter);

	MidiParseInitMthTbl (mem->inMthd);
	MidiParseInitTypeTbl(mem->inTypes);
	MidiParseInit (&mem->inFifo, mem->inMthd, mem->inTypes);

	MidiStreamInitMthTbl (mem->outMthd);
	MidiStreamInit (&mem->outFifo, mem->outMthd);

	MidiSetRcvAlarm (ref, RcvAlarm);
}

//_______________________________________________________________________
static int checkQuit ()
{
	char c = getc(stdin);
	if ((c == 'q') || (c == 'Q'))
		return 1;
	return 0;
}


//_______________________________________________________________________
static void * inputThread (void * ptr)
{
	DrvMemPtr mem = (DrvMemPtr)ptr;
	StreamFifoPtr f = &mem->inFifo;
	while (!mem->done) {
		char c;
		ssize_t n = read (mem->devId, &c, 1);
		if (n == 1) {
			MidiEvPtr e = MidiParseByte (f, c);
			if (e) MidiSend (mem->refNum, e);
		} else {
			printf ( "input thread read error");
			mem->done = true;
		}
	}
	return 0;
}

//_______________________________________________________________________
static void ThreadCreate (DrvMemPtr mem)
{
	struct sched_param param;  
	int  err, ret = pthread_create(&mem->inThread, NULL, inputThread, mem);
	uid_t uid = getuid ();
	
	param.sched_priority = 99; /* 0 to 99  */
        setuid (name_to_uid ("root")); 
       	err = pthread_setschedparam(mem->inThread, SCHED_RR,  &param); 
        setuid (uid);
	
	if (ret) {
		char str[256];
		sprintf (str, "pthread create failed with err code = %d", ret);
		FatalError (str);		
	}

}

//_______________________________________________________________________
static void run (DrvMemPtr mem, char * dev)
{
	void *threadRet;
	printf ("Midi driver anchored on \"%s\" is running\n", dev);
	printf ("type 'q' to quit\n");
	while (!mem->done)
		mem->done = checkQuit();
	pthread_cancel (mem->inThread);
	pthread_join (mem->inThread, &threadRet);
}

//_______________________________________________________________________
int main (int argc, char *argv[])
{
	DrvMemPtr mem = GetData();
	
	if (argc != 2) usage (argv[0]);
	CheckMidiShare();
	mem->devId = openDev (argv[1]);
	SetupMidi (argv[1], mem);
	ThreadCreate (mem);
	run (mem, argv[1]);
	closeDriver (mem);
	return 0;        
}
