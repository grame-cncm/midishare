#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <signal.h> 
#include <pthread.h> 
#include <pwd.h>
#include <termios.h>

#include "MidiShare.h"
#include "EventToMidiStream.h"
#include "MidiStreamToEvent.h"


//_______________________________________________________________________
#define SIGDRVQUIT	SIGUSR1
#define BAUDRATE B38400
typedef struct termios * TermiosPtr;
enum  { kOld, kNew };

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
	
	struct termios  tio[2];

} DrvMem, * DrvMemPtr;

DrvMem gMem = { 0 };
static inline DrvMemPtr GetData()	{ return &gMem; }

//_______________________________________________________________________
static void showTConf (TermiosPtr tio)
{
	printf ("termios struct content : \n");
    	printf ("        c_iflag : %x\n", tio->c_iflag);       /* input mode flags */
    	printf ("        c_oflag : %x\n", tio->c_oflag);       /* output mode flags */
    	printf ("        c_cflag : %x\n", tio->c_cflag);       /* control mode flags */
    	printf ("        c_lflag : %x\n", tio->c_lflag);       /* local mode flags */
    	printf ("        c_line  : %x\n", (int)tio->c_line);   /* line discipline */
     	printf ("        c_ispeed : %x\n", tio->c_ispeed);       /* input speed */
     	printf ("        c_ospeed : %x\n", tio->c_ospeed);       /* output speed */
}

//_______________________________________________________________________
static void closeDriver (DrvMemPtr mem) 
{
	if (mem->devId) {
		tcflush(mem->devId, TCIFLUSH);
		tcsetattr (mem->devId, TCSANOW, &mem->tio[kOld]);
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
static void SetupSerial (DrvMemPtr mem)
{
	char str[256];
 	int id = mem->devId;
	TermiosPtr tio = &mem->tio[kNew];
 
	if (tcgetattr (id,&mem->tio[kOld])) {
		sprintf (str, "SetupSerial: tcgetattr error (errno %d)\n", errno);
		FatalError (str);
	}
	
	*tio = mem->tio[kOld];
	tcsetattr(id,TCSANOW, tio);
	
	cfsetispeed (tio, BAUDRATE);
	cfsetospeed (tio, BAUDRATE);
	tio->c_cflag |= (CLOCAL | CREAD);
	tio->c_cflag &= ~PARENB;
	tio->c_cflag |= CSTOPB;
	tio->c_cflag &= ~CSIZE;
	tio->c_cflag |= CS8;
	tio->c_cflag &= ~CRTSCTS;
	
	tio->c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
	tio->c_iflag &= ~(IXON | IXOFF | IXANY);
	tio->c_oflag &= ~OPOST;
	
	tio->c_cc[VTIME]    = 0;   /* inter-character timer unused */
	tio->c_cc[VMIN]     = 1;   /* blocking read until 1 chars received */

	tcflush(id, TCIFLUSH);
	if (tcsetattr(id,TCSANOW, tio)) {
		sprintf (str, "SetupSerial: tcsetattr error (errno %d)\n", errno);
		FatalError (str);
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
	SetupSerial (GetData ());
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
void QuitHandler (int sigNum)
{
	pthread_exit (0);	
}

//_______________________________________________________________________
static void * inputThread (void * ptr)
{
	DrvMemPtr mem = (DrvMemPtr)ptr;
	StreamFifoPtr f = &mem->inFifo;
	signal(SIGDRVQUIT, QuitHandler);
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
	int  ret = pthread_create(&mem->inThread, NULL, inputThread, mem);
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
//	printf ("Midi driver anchored on \"%s\" is running\n", dev);
	printf ("type 'q' to quit ");
	while (!mem->done)
		mem->done = checkQuit();
	pthread_kill (mem->inThread, SIGDRVQUIT);
	pthread_join (mem->inThread, &threadRet);
}

//_______________________________________________________________________
uid_t name_to_uid (char * name)
{
	struct passwd *p;
	p = getpwnam (name);
	return p ? p->pw_uid : -1;
}

//_______________________________________________________________________
int main (int argc, char *argv[])
{
	DrvMemPtr mem = GetData();
	uid_t uid = getuid ();
	
	if (argc != 2) usage (argv[0]);
	CheckMidiShare();
	setuid (name_to_uid ("root")); 
	mem->devId = openDev (argv[1]);
	SetupMidi (argv[1], mem);
	ThreadCreate (mem);
	run (mem, argv[1]);
	closeDriver (mem);
 	setuid (uid);
	return 0;        
}
