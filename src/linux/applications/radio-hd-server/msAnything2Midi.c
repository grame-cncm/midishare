/*

  Copyright © Grame 2001

  Grame Research Laboratory, 9, rue du Garet 69001 Lyon - France
  grame@grame.fr

*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

#include "MidiShare.h"
#include "MidiStreamToEvent.h"
#include "server.h"

#define kMaxCnx	64

StreamFifo 		gStream;
ParseMethodTbl	gParse;
Status2TypeTbl	gTypeTbl;
short			gRefNum, gOffset=0;
unsigned long	gTimeOffset;
char *			gCurFile = 0;
char *			gFrom = 0;

#define ApplName	"RadioHDServer"

//_______________________________________________________________________
static void FatalError (char * name, const char *msg) 
{
	fprintf (stderr, "Cannot launch %s: %s\n", name, msg);
	exit (1);
}

//_______________________________________________________________________
static void usage (const char *name) 
{
	fprintf (stderr, "usage : %s [options] [files]\n", name);
	fprintf (stderr, "        option: -f <from> to override the file src parsing\n");
	fprintf (stderr, "        %s parses the input files and produces the corresponding\n", ApplName);
	fprintf (stderr, "               MidiShare stream on output\n");
	fprintf (stderr, "        %s parses its standard input when no file is specified\n", ApplName);
	fprintf (stderr, "               on the command line\n");
	exit (1);
}

//_______________________________________________________________________
// returns the input files index or 0 when unspecified
static void checknoopts (int argc, char *argv[], int i)
{
	for (i; i< argc; i++) {
		char * arg = argv[i];
		if (*arg == '-') usage (argv[0]);
	}
}

//_______________________________________________________________________
static char * getvalue (int argc, char *argv[], int i)
{
	if (i < argc)
		return argv[i];
	usage (argv[0]);
	return 0;
}

//_______________________________________________________________________
// returns the input files index or 0 when unspecified
static int getopts (int argc, char *argv[])
{
	int i = 1;
	char * arg = argv[i];
	if (*arg == '-') {
		if (!strcmp (arg, "-f")) {
			gFrom = getvalue (argc, argv, ++i);
			i++;
		}
		else usage (argv[0]);
	}
	checknoopts (argc, argv, i);
	return i;
}

//_______________________________________________________________________
static Boolean Equal (MidiEvPtr e1, MidiEvPtr e2)
{
	long n1, n2;
	if (!e1 || !e2) return false;
	if (EvType(e1) != EvType(e2)) return false;
	if (Chan(e1) != Chan(e2)) return false;
	n1 = MidiCountFields (e1);
	n2 = MidiCountFields (e2);
	if (n1 != n2) return false;
	while (n1--) {
		if (MidiGetField (e1, n1) != MidiGetField (e2, n1))
			return false;
	}
	return true;
}

//_______________________________________________________________________
static Boolean SendCtrl (MidiEvPtr e)
{
	short type = Data(e)[0];
	short val = Data(e)[1];
	switch (type) {
		case 0:			// bank select
		case 1:			// modulation wheel
		case 2:			// breath controler
//		case 4:			// foot pedal
		case 5:			// portamento time
		case 8:			// balance
		case 10:		// pan position
//		case 11:		// expression (coarse)
		case 65:		// portamento 	on/off
//		case 68:		// legato pedal on/off
		case 91:		// effects level
		case 92:		// tremolo level
		case 93:		// chorus level
		case 94:		// celeste level
		case 95:		// phaser level
			MidiSend (gRefNum, e);
			break;
		case 7:
			if (val < 40) Data(e)[1] += 64;
			MidiSend (gRefNum, e);
			break;
		default:
			MidiFreeEv(e);
			return false;
	}
	return true;
}

//_______________________________________________________________________
static Boolean SendNote (MidiEvPtr e)
{
	long dur;
	EvType(e) = typeNote;
	if (Vel(e) < 20) 	Vel(e) += 80;
	if (Pitch(e) < 5)  	Pitch(e) += 40;
	dur = random() >> 20;
	Dur(e) = dur ? dur : 10000;
	MidiSend (gRefNum, e);
	return true;
}

//_______________________________________________________________________
static Boolean SendProg (MidiEvPtr e)
{
	static unsigned long lastProg = 0;
	unsigned long t = MidiGetTime();
	if ((t - lastProg) > 500) {
		MidiSend (gRefNum, e);
		lastProg = t;
		return true;
	}
	MidiFreeEv (e);
	return false;
}

//_______________________________________________________________________
static Boolean Send (MidiEvPtr e)
{
	static MidiEvPtr previous = 0;
	Boolean equal = previous ? Equal(previous, e) : false;

	if (!equal) {
		if (previous) MidiFreeEv (previous);
		previous = MidiCopyEv(e);
	}
	Port(e) = 0;
	switch (EvType(e)) {
		case typeSysEx:
		case typeStream:
		case typeReset:
		case typeActiveSens:
		case typeClock:
		case typeStart:
		case typeStop:
		case typeTune:
		case typeContinue:
		case typeSongPos:
		case typeSongSel:
		case typeQuarterFrame:
			MidiParseReset (&gStream);
			goto ignore;

		case typeNote:
		case typeKeyOn:
		case typeKeyOff:
			return SendNote (e);


		case typeProgChange:
			MidiParseReset (&gStream);
			if (!equal) return SendProg (e);
			goto ignore;

		case typeCtrlChange:
			MidiParseReset (&gStream);
			if (!equal) return SendCtrl (e);
			goto ignore;

		case typeKeyPress:
		case typeChanPress:
			if (Data(e)[0] < 20) Data(e)[0] = 100;
		default:
			MidiParseReset (&gStream);
			if (!equal) MidiSend (gRefNum, e);
			else goto ignore;
	}
	return true;
	
ignore:
	MidiFreeEv(e);
	return false;
}

//_______________________________________________________________________
static long ParseByte (FILE *fd, unsigned char c, unsigned long date)
{
	unsigned long nextTime = 0; MidiEvPtr e;
	e = MidiParseByte (&gStream, c);
	if (e) {
		Date(e) = date + gOffset++;
		if (Send (e)) {
			c = getc(fd);
			if (feof(fd)) return 0;
			nextTime = (c > 5) ? c : 5;
		}
	}
	return nextTime * 1000;
}

//_______________________________________________________________________
static void ParseIn (FILE *fd, short ref)
{
	unsigned long sleepTime = 0;
	unsigned long date = MidiGetTime();
	while (true) {
		unsigned char c = getc(fd);
		if (feof(fd)) break;
		sleepTime += ParseByte (fd, c, date);
		if (sleepTime >= 10000) {
			usleep (sleepTime);
			sleepTime = 0;
			gOffset = 0;
			date = MidiGetTime();
		}
	}
}

//_______________________________________________________________________
static MidiEvPtr Text2Ev (char * text, short type)
{
	MidiEvPtr e = MidiNewEv (type);
	if (e) {
		while (*text)
			MidiAddField (e, *text++);
	}
	return e;
}

//_______________________________________________________________________
static MidiEvPtr SeqName (char * file)
{
	char * name = strrchr (file, '/');
	MidiEvPtr e = Text2Ev (name ? ++name : file, typeSeqName);
	if (e) Port(e) = kTCPPort;
	return e;
}

//_______________________________________________________________________
static MidiEvPtr SeqFrom (char * file)
{
	MidiEvPtr e; 
	if (gFrom) e = Text2Ev (gFrom, typeCopyright);
	else {
		char * end = strrchr (file, '/');
		if (!end) e = Text2Ev ("/", typeCopyright);
		else {
			char c = *end;
			*end = 0;
			e = Text2Ev (file, typeCopyright);
			if (e) Port(e) = kTCPPort;
			*end = c;
		}
	}
	return e;
}

//_______________________________________________________________________
static MidiEvPtr SeqOffset (unsigned long time)
{
	MidiEvPtr e = MidiNewEv (typeTempo);
	if (e) {
		long elapsed = (time >= gTimeOffset) ? time - gTimeOffset :
			time + (0xffffffff - gTimeOffset);
		Port(e) = kTCPPort;
		Tempo(e) = elapsed;
	}
	return e;
}

//_______________________________________________________________________
void sendinfos (short ref, char * file)
{
	long date = MidiGetTime() + 500;
	MidiEvPtr e = SeqName (file);
	if (e) {
		MidiSendAt (ref, e, date);
		MidiSendAt (ref, SeqFrom(file), date);
		MidiSendAt (ref, SeqOffset (MidiGetTime()), date);		
	}
}

//_______________________________________________________________________
static void SendAllChans (short ref, MidiEvPtr e)
{
	short i;
	for (i=0; i<16; i++) {
		Chan(e) = i;
		MidiSendIm (ref, MidiCopyEv (e));
	}	
}

//_______________________________________________________________________
static void initChannel (short ref)
{
	MidiEvPtr e =  MidiNewEv (typeProgChange);
	if (e) {
		Data(e)[0] = 0;
		SendAllChans (ref, e);	
		MidiFreeEv (e);
	}
	e = MidiNewEv (typeCtrlChange);
	if (e) {
		Data(e)[0] = 7;
		Data(e)[1] = 127;
		SendAllChans (ref, e);	
		MidiFreeEv (e);
	}
}

//_______________________________________________________________________
static void stop (short ref)
{
	short i;
	MidiEvPtr e = MidiNewEv (typeEndTrack);
	if (e) {
		Port(e) = kTCPPort;
		MidiSendIm (ref, e);
	}
}

//_______________________________________________________________________
main (int argc, char *argv[])
{
	int i;
	if (!MidiShare()) FatalError (argv[0], "MidiShare is not installed");

	i = (argc > 1) ? getopts (argc, argv) : 1;
	gRefNum = MidiOpen (ApplName);
	if (gRefNum < 0) FatalError (argv[0], "can't open a MidiShare client");
	
	MidiParseInitMthTbl (gParse);
	MidiParseInitTypeTbl (gTypeTbl);
	MidiParseInit (&gStream, gParse, gTypeTbl);
	CnxInit (gRefNum);
	sleep (1);
	if (i < argc) {
		FILE *fd;
		for (i; i<argc; i++) {
			gCurFile = argv[i];
			fd = fopen (gCurFile, "r");
			if (fd) {
				gTimeOffset = MidiGetTime();
				sendinfos (gRefNum, gCurFile);
				usleep (500);
				initChannel (gRefNum);
				ParseIn (fd, gRefNum);
				gCurFile = 0;
				stop (gRefNum);
				fclose (fd);
				sleep (1);
			}
			else {
				fprintf (stderr, "can't read \"%s\": ", argv[i]);
				perror ("");
			}
		}
	}
	else  ParseIn (stdin, gRefNum);
	MidiClose (gRefNum);
	exit (0);
}
