/*

  Copyright © Grame 2002

  This program is free software; you can redistribute it and modify it under 
  the terms of the GNU General Public License as published by the 
  Free Software Foundation version 2 of the License.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for 
  more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

  Grame Research Laboratory, 9, rue du Garet 69001 Lyon - France
  grame@grame.fr


*/

#include <stdio.h>
#include "msDefs.h"
#include "msFunctions.h"
#include "EventToStream.h"
#include "StreamToEvent.h"
#include "StreamDefs.h"

#define midiGetVersion	193	
//#define midiGetVersion	MidiGetVersion()

#define print	printf
#define nil 0
#define flush    fflush(stdout)

#define true	1
#define false	0

/* ========================= variables globales de l'application ==================*/
short version= 0;
long SizeMaxSysEx= 10000;				/* taille du plus grand sysex		*/
char *gOK = " OK";
Boolean gLoopTest = false;
int gEvCount = 1;

#define SizeMedSysEx	40				/* test des sysex par pas de 1		*/

#define noEvts		print("\nno more MidiShare events !\n")
#define ok			print("%s\n", gOK)

msStreamMthTbl	gEv2StreamTbl;
Ev2StreamRec	gEv2Stream;
msStreamParseMethodTbl	gParseTbl;
msStreamBuffer			gStream2Ev;
#define kBufferSize			1024
#define kMediumBufferSize	256
#define kMinBufferSize		sizeof(msStreamHeader)+sizeof(EvCommonPart)
char			gBuffer[kBufferSize];

/* ======================= nom des diffŽrents types d'ŽvŽnements ==================*/
char *typeListe[] =
{
/*  0 */	"Note", 		"KeyOn", 		"KeyOff",		"KeyPress",
/*  4 */	"CtrlChange",	"ProgChange",	"ChanPress",	"PitchWheel",
/*  8 */	"SongPos",		"SongSel",		"Clock",		"Start",
/* 12 */	"Continue",		"Stop",			"Tune",			"ActiveSens",
/* 16 */	"Reset",		"SysEx",		"Stream",		"Private",
/* 20 */	"",	"",	"",	"",	"",	"",	"",	"", "", "", 
/* 30 */	"",	"",	"",	"",	"",	"",	"",	"", "", "",
/* 40 */	"",	"",	"",	"",	"",	"",	"",	"", "", "",
/* 50 */	"",	"",	"",	"",	"",	"",	"",	"", "", "",
/* 60 */	"",	"",	"",	"",	"",	"",	"",	"", "", "",
/* 70 */	"",	"",	"",	"",	"",	"",	"",	"", "", "",
/* 80 */	"",	"",	"",	"",	"",	"",	"",	"", "", "",
/* 90 */	"",	"",	"",	"",	"",	"",	"",	"", "", "",
/*100 */	"",	"",	"",	"",	"",	"",	"",	"", "", "",
/*110 */	"",	"",	"",	"",	"",	"",	"",	"", "", "",
/*120 */	"",	"",	"",	"",	"",	"",	"",	"",
/*128 */	"Process",		"DProcess",		"QFrame",		"Ctrl14b",
/*132 */	"NonRegParam",	"RegParam",		"SeqNum",		"Text",
/*136 */	"Copyright",	"SeqName",		"InstrName",	"Lyric",
/*140 */	"Marker",		"CuePoint",		"ChanPrefix",	"EndTrack",
/*144 */	"Tempo",		"SMPTEOffset",	"TimeSign",		"KeySign",
/*148 */	"Specific",		"PortPrefix",	"RcvAlarm", 	"ApplAlarm", 
/*152 */    "MidiOpen",		"MidiOpenRes", 	"MidiClose", 	"MidiConnect",
/*156*/		"MidiSetName", 	"MidiSetInfo", 	"MidiSetFilter", "Reserved"		
};


/* ============================= functions declarations ========================*/
/*______________________________________________________________________________*/
static int StreamInit(int buffSize)
{
	msStreamParseInitMthTbl (gParseTbl);
	msStreamParseInit (&gStream2Ev, gParseTbl, gBuffer, buffSize);
	msStreamInitMthTbl (gEv2StreamTbl);
	msStreamInit (&gEv2Stream, gEv2StreamTbl, gBuffer, buffSize);
}

/*______________________________________________________________________________*/
static int Initialize()
{	
	StreamInit (kBufferSize);
	return true;
}

/*______________________________________________________________________________*/
static CmpCommon( MidiEvPtr o, MidiEvPtr c, char *src)
{
	int ret= true;
	
	if( EvType(o) != EvType(c)) {
		print("\n%s : modified event type : %d -> %d", src,
					(int)EvType(o), (int)EvType(c));
		ret= false;
	}
	if( ( EvType(o) < typeSongPos) && (Chan(o) != Chan(c))) {
		print("\n%s : modified event channel : %d -> %d", src,
					(int)Chan(o), (int)Chan(c));
		ret= false;
	}
	if( Port(o) != Port(c)) {
		print("\n%s : modified event port : %d -> %d", src,
					(int)Port(o), (int)Port(c));
		ret= false;
	}
	return ret;
}

/*______________________________________________________________________________*/
static int CmpEv (MidiEvPtr o, MidiEvPtr c, char *src)
{
	long f1, f2, i;
	
	f1= MidiCountFields( o);
	f2= MidiCountFields( c);
	if( f1!= f2) {
		print("\n%s : different number of fields : %ld -> %ld", src,f1,f2);
		return false;
	}
	else if( !CmpCommon( o, c, src))
		return false;
	for( i=0; i<f1; i++) {
		if( MidiGetField( o, i)!= MidiGetField( c, i)) {
			print("\n%s : modified value at index %ld", src, i);
			return false;
		}
	}
	return true;
}

/*______________________________________________________________________________*/
static void SetMidiEvent( MidiEvPtr e, Boolean display)
{
	long f, i, v=1;
	
	f= MidiCountFields(e);
	for(i= 0; i<f; i++)
	{
		MidiSetField( e, i, v);
		if( v<127) v++; else v=0;
	}
	if( display)
		print("set "); flush;
}

/*______________________________________________________________________________*/
static int StreamLoop (MidiEvPtr e)
{
	MidiEvPtr get; int ret = false, err, done;
	
	msStreamStart (&gEv2Stream);
	if (msStreamPutEvent (&gEv2Stream, e)) {
		print("\nwarning: msStreamPutEvent didn't failed"); flush;
		goto end;
	}
	get = msStreamStartBuffer(&gStream2Ev, msStreamSize(&gEv2Stream), &err);
//	get = msStreamGetEvent (&gStream2Ev, &err);
	if (!get && (err != kStreamNoMoreData)) {
		print("\nwarning: msStreamGetEvent failed with retcode %d (0 expected)", err); flush;
		goto end;
	}
	do {
		done = msStreamContEvent (&gEv2Stream);
        get = msStreamStartBuffer(&gStream2Ev, msStreamSize(&gEv2Stream), &err);
//		get = msStreamGetEvent (&gStream2Ev, &err);
	} while (!done);
	msStreamParseRewind (&gStream2Ev);
	if (err != kStreamNoError) {
		print("\nwarning: msStreamGetEvent failed with retcode %d", err); flush;
		goto end;
	}
	ret = CmpEv( e, get, "read");
end:
	if (get) MidiFreeEv(get);
	return ret;	
}

/*______________________________________________________________________________*/
static int StreamTest (MidiEvPtr e, int expectedRet)
{
	MidiEvPtr get; int ret = false, err, n = gEvCount;
	
	msStreamStart (&gEv2Stream);
	while (n--) {
		if (!msStreamPutEvent (&gEv2Stream, e)) {
			print("\nwarning: msStreamPutEvent failed"); flush;
			goto err;
		}
	}
	get = msStreamStartBuffer(&gStream2Ev, msStreamSize(&gEv2Stream), &err);
	while (++n < gEvCount) {
		if (!get) {
			if (err != expectedRet) {
				print("\nwarning: msStreamGetEvent failed with retcode %d", err); flush;
			}
			else ret = true;
			goto err;
		}
		ret = CmpEv( e, get, "read");
		if (get) MidiFreeEv(get);
		get = msStreamGetEvent (&gStream2Ev, &err);
	}
//	get = msStreamGetEvent (&gStream2Ev, &err);
	if (get || (err != kStreamNoMoreData))
		print("\nwarning: msStreamGetEvent: kStreamNoMoreData expected, got %d", err); flush;
err:
	return ret;
}

/*______________________________________________________________________________*/
static int TestEvent( short i, Boolean display, int expectedRet)
{
	MidiEvPtr e=0; int ret = false;

	if (display) print("    type %s : ", typeListe[i]); flush;

	e= MidiNewEv(i);
	if (!e) { noEvts; return false; }
	SetMidiEvent( e, display);
	ret = gLoopTest ? StreamLoop(e) : StreamTest (e, expectedRet);
	if (e) MidiFreeEv(e);
	return ret;
}

/*______________________________________________________________________________*/
static void ChanEvents()
{
	short i;
	
	print("\nChannel events :\n");
	/*											typeNote until typePitchWheel */
	for( i= typeNote; i<typeSongPos; i++)
		if( TestEvent( i, true, kStreamNoError)) ok;
		else print("\n");
}

/*______________________________________________________________________________*/
static void CommonEvents()
{
	short i;
	
	print("\nCommon events :\n");	
	/*		typeSongPos until typeReset */
	for( i= typeSongPos; i<typeSysEx; i++)
		if( TestEvent( i, true, kStreamNoError)) ok;
		else print("\n");
}

/*______________________________________________________________________________*/
static void SystemeEx()
{
	MidiEvPtr e=0;
	long v=0, n=0;
	
	print("\nSysteme exclusive :\n");
	
	/*		typeSysEx */
	if( !TestEvent( typeSysEx, true, kStreamNoError)) { print("\n"); return; }

	/*		test of event until n */
	if( !(e= MidiNewEv(typeSysEx))) { noEvts; return; }
	for( n=0; n<SizeMedSysEx; n++) {
		MidiAddField( e, v++);
		if (!StreamTest (e, kStreamNoError)) return;
	}
	print(" 0-%ld ok", n); flush;
	/*		test of max for max length */
	for( ; n< SizeMaxSysEx; n++) {
		MidiAddField( e, v++);
		if( v>126) v=0; else v++;
	}
	print("\n      length %ld", MidiCountFields(e)); flush;
	if (StreamLoop (e)) ok;

	if (e) MidiFreeEv(e);
}

/*______________________________________________________________________________*/
static void Private()
{
	short i;
	Boolean display= true;
	
	print("\nPrivate events :\n");	
	/*													typePrivate */
	for( i= typePrivate; i<typeProcess; i++) {
		if( !TestEvent( i, display, kStreamNoError)) {
			print(" type %d\n", (int)i);
			return;
		}
		display= false;
	}
	ok;
}

/*______________________________________________________________________________*/
static void Process()
{
	short i;
	
	print("\nProcess and DProcess :\n");	
	/*													typePrivate */
	for( i= typeProcess; i<typeQuarterFrame; i++) {
		if( TestEvent( i, true, kStreamNoError)) ok;
		else print("\n");
	}
}

/*______________________________________________________________________________*/
static void QFToMidiFile()
{
	short i;
	
	print("\nQuarterFrame until RegParam :\n");	
	/*													nQuarterFrame until RegParam */
	for( i= typeQuarterFrame; i<=typeRegParam; i++) {
		if( TestEvent( i, true, kStreamNoError)) ok;
		else print("\n");
	}
}

/*______________________________________________________________________________*/
static void MidiFile()
{
	short i;
	
	print("\nMidiFile events :\n");	
	if( midiGetVersion < 160)
		print("not implemented !\n");	
	/*												typeSeqNum until typePortPrefix */
	else for( i= typeSeqNum; i<=typePortPrefix; i++) {
		if( TestEvent( i, true, kStreamNoError)) ok;
		else print("\n");
	}
}

/*______________________________________________________________________________*/
static void Internals()
{
	short i;
	
	print("\nInternal events: (should not be parsed)\n");	
	if( midiGetVersion < 160)
		print("not implemented !\n");	
	/*												typePortPrefix+1 until typeReserved */
	else for( i= typePortPrefix+1; i<=typeApplAlarm; i++) {
		if( TestEvent( i, true, kStreamUnknowParseMeth)) ok;
		else print("\n");
		msStreamParseRewind (&gStream2Ev);
	}
}

/*______________________________________________________________________________*/
static void Fonctions()
{
	int i;
	
	print("\nMidiShare Functions events :\n");
	if( midiGetVersion < 193)
		print("not implemented !\n");	
	else for( i= typeMidiOpen; i<typeReserved; i++) {
		if( TestEvent( i, true, kStreamNoError)) ok;
		else print("\n");
	}
}

/*______________________________________________________________________________*/
static void TestSuite1 ()
{
	ChanEvents(); flush;
	CommonEvents(); flush;			
	SystemeEx(); flush;
	Private(); flush;
	Process(); flush;
	QFToMidiFile(); flush;
	MidiFile(); flush;
	Internals(); flush;
	Fonctions(); flush;
}

/*______________________________________________________________________________*/
static void TestSuite2 ()
{
	ChanEvents(); flush;
	CommonEvents(); flush;			
	QFToMidiFile(); flush;
}

/*______________________________________________________________________________*/
static void TestSuite3 ()
{
	ChanEvents(); flush;
	CommonEvents(); flush;			
	Private(); flush;
	Process(); flush;
	QFToMidiFile(); flush;
	MidiFile(); flush;
	Fonctions(); flush;
}

/*______________________________________________________________________________*/
static int GetLoop (MidiEvPtr e, int size)
{
	int err, r = 0; 
	MidiEvPtr get = msStreamStartBuffer(&gStream2Ev, size, &err);
	
	while (true) {
		if (!get && (err == kStreamNoMoreData))
			return r;
		if (err != kStreamNoError) {
			print("\nwarning: msStreamGetEvent failed with retcode %d (%lx)", err, get);
			return 0;
		}
		if (!CmpEv( e, get, "read")) return 0;
		if (get) MidiFreeEv(get);
		r++;
		get = msStreamGetEvent (&gStream2Ev, &err);
	}
	return r;
}

/*______________________________________________________________________________*/
static void TestSuite4 (int type)
{
	MidiEvPtr e=0; int err, n=0, r=0, tmp;

	print("\n    type %s : ", typeListe[type]); flush;
	msStreamStart (&gEv2Stream);
	e= MidiNewEv(type);
	if (!e) { noEvts; return; }
	SetMidiEvent( e, false);

	while (msStreamPutEvent (&gEv2Stream, e)) n++;
	r = GetLoop (e, msStreamSize(&gEv2Stream));

	if (!msStreamContEvent (&gEv2Stream)) {
		print("\nwarning: msStreamContEvent failed");
		goto end;
	}
	else n++;
	if (!msStreamPutEvent (&gEv2Stream, e)) {
		print("\nwarning: msStreamPutEvent failed");
		goto end;
	}
	else n++;

	r += GetLoop (e, msStreamSize(&gEv2Stream));
	if (r==n) 
		printf ("%d events overflow done", n);
	else
		printf ("\nwarning: %d events expected, read %d", n ,r);
end:
	MidiFreeEv(e);
}


/*______________________________________________________________________________*/
static void TestSuite5 (int buffSize)
{
	MidiEvPtr e=0; int n=0, n2, r=0;
    Ev2StreamRec stream2;

	e= MidiNewEv(typeNote);
	if (!e) { noEvts; return; }
	SetMidiEvent( e, false);

	msStreamInit (&gEv2Stream, gEv2StreamTbl, gBuffer, buffSize);
	msStreamStart (&gEv2Stream);
	if (!msStreamPutEvent (&gEv2Stream, e)) {
		printf ("\nwarning: msStreamPutEvent (stream 1) failed");
        return;
    }
    n = msStreamSize (&gEv2Stream);
	msStreamInit (&stream2, gEv2StreamTbl, &gBuffer[n], buffSize-n);
	msStreamStart (&stream2);
	if (!msStreamPutEvent (&stream2, e)) {
		printf ("\nwarning: msStreamPutEvent (stream 2) failed");
        return;
    }
	n2 = msStreamSize(&stream2);
    printf ("\n    streams written - length %d %d", n, n2);

    printf ("\n    checking with complete streams : ");
	r = GetLoop (e, n + n2);
    if (r != 2)
		printf ("\nwarning: 2 events expected, read %d" ,r);
	else 
        printf ("ok");

    printf ("\n    checking with uncomplete streams 2 : ");
	r = GetLoop (e, n + n2 - 8);
    if (r != 1) printf ("\nwarning: 1 events expected, read %d" ,r);
	r = GetLoop (e, n + msStreamSize(&stream2));
    if (r != 1) printf ("\nwarning: 1 events expected, read %d" ,r);
	else 
        printf ("ok");

	MidiFreeEv(e);
}

/*______________________________________________________________________________*/
main( int argc, char *argv[])
{
	char *s = "\n-----------------------------------------";
	print("           Linearization of MidiShare events.\n");
	print("==========================================================\n");

    version= MidiGetLibVersion();
    print("                MidiShareLib version %d.%d\n", (int)version/100, (int)version%100);
    print("\nWarning : memory must be least 10000 events !\n");

    if( Initialize()) {
 /*       print("\nTest suite with buffer size=%ld%s", kBufferSize, s);
        gLoopTest = false;
        gEvCount = 1;
        TestSuite1 ();

        StreamInit (kMinBufferSize);
        print("\nTest suite with buffer size=%ld (small event overflow)%s", kMinBufferSize, s);
        gLoopTest = true;
        TestSuite2 ();

        StreamInit (kMediumBufferSize);
        gLoopTest = false;
        gEvCount = 5;
        print("\nTest suite with %d events per buffer%s", gEvCount, s);
        TestSuite3 ();

        StreamInit (kMediumBufferSize);
        print("\nTest suite with events overflow%s", s); flush;
        TestSuite4 (typeNote);
        TestSuite4 (typePrivate);
        TestSuite4 (typeSysEx);
*/
        StreamInit (kMediumBufferSize);
        print("\n\nTest suite with multiple streams per buffer%s", s); flush;
        TestSuite5 (kMediumBufferSize);
    }
	print("\n\nEnd of linearization functions test.\n");
	return 0;
}
