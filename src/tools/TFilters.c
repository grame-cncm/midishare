/*

  Copyright © Grame 2000

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
  grame@rd.grame.fr


*/

#ifdef __Windows__
#	include <stdio.h>
#	include <String.h>
#	include <StdLib.h>
#	include <MidiShare.h>
#	define CNAME
#	define CTASKS
#	define PROTOTYPE
#endif


#ifdef __Linux__
#ifdef MODULE
# 	ifdef MODVERSIONS
# 		include <linux/modversions.h>
# 	endif
#	include <linux/module.h>
#	include <linux/kernel.h>
#	include "MidiShareKernel.h"
#	define flush
#	define print(args...)	printk(## args)
inline Boolean MidiShare() { return true; }
#else
#	include <stdio.h>
#	include "MidiShare.h"
#	define flush		fflush( stdout)
#	define print(args...)	fprintf(stdout, ## args)
#endif
#	define CNAME
#	define CTASKS
#	define MidiAvailEv(r)  MidiGetEv(r)
#endif


#if macintosh
#	include <stdio.h>
#	include <String.h>
#	include "MidiShare.h"
#	define PASCALNAME
#	define PASCALTASKS
#	define flush	fflush( stdout)
#	define print	printf
#endif

#include "TFilters.h"

#define true	1
#define false	0

static short 	refNum= 0;
static short 	version= 0;
static MidiFilterPtr myFilterPtr = 0;

#ifdef PASCALNAME
MidiName ApplName = "\pFiltres";
#endif

#ifdef CNAME
MidiName ApplName = "Filtres";
#endif

#define LastPort 32

/* =========================== events names ===========================*/
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
/*148 */	"Specific",		"Reserved"
};


#ifdef _Filter_Direct_Access_
#include "FilterUtils.h"
static TFilter	myFilter;					/* application filter */
#define NewFilter()	&myFilter
#define FreeFilter(f)	
#define AcceptType(f,i)	AcceptBit(f->evType, i)
#define AcceptChan(f,i)	AcceptBit(f->channel, i)
#define AcceptPort(f,i)	AcceptBit(f->port, i)
#define RejectType(f,i)	RejectBit(f->evType, i)
#define RejectChan(f,i)	RejectBit(f->channel, i)
#define RejectPort(f,i)	RejectBit(f->port, i)

#else
#define NewFilter()	MidiNewFilter()
#define FreeFilter(f)	MidiFreeFilter (f);
#define AcceptType(f,i)	MidiAcceptType(f, i, true)
#define AcceptChan(f,i)	MidiAcceptChan(f, i, true)
#define AcceptPort(f,i)	MidiAcceptPort(f, i, true)
#define RejectType(f,i)	MidiAcceptType(f, i, false)
#define RejectChan(f,i)	MidiAcceptChan(f, i, false)
#define RejectPort(f,i)	MidiAcceptPort(f, i, false)
#endif

/*____________________________________________________________________*/
static void ResetFilter (void)
{
	short i;

	for (i = 0; i<256; i++) { 										
		AcceptType(myFilterPtr,i);		/* accepte tous les types d'ŽvŽnements	*/
		AcceptPort(myFilterPtr,i);		/* en provenance de tous les ports		*/
	}
	for (i = 0; i<16; i++)
		AcceptChan(myFilterPtr,i);		/* et sur tous les canaux Midi			*/		
}

/*____________________________________________________________________*/
static void wait( int v)
{
	long time;
	
	time= MidiGetTime();
	while( (MidiGetTime() - time) < v);
}

/*____________________________________________________________________*/
static MidiEvPtr GetReceived(void)
{
	long n;
	
	if( (n= MidiCountEvs( refNum)) > 1) {
		print ("\nwarning: %ld evts recus\n", n);
		MidiFlushEvs( refNum);
		return 0;
	}
	return MidiGetEv( refNum);
}

/*____________________________________________________________________*/
static int TestEv( int i, short t1, short t2)
{
	MidiEvPtr e;
	int n, result= true;
	
	for( n=t1; n<=t2; n++) {
		e= MidiNewEv( n);
		if( e) {
			MidiSendIm( refNum, e);
			wait( 2);
			e= GetReceived();
			if( e) {
				if( n== i) {
					if( n== typePrivate)
						print (" not filtered ");
					else {
						print ("\n    error: type %s not filtered\n", typeListe[n]);
						result= false;
					}
				}
				MidiFreeEv( e);
			}
			else if( n!= i) {
				print ("\n    error: type %s filtered\n", typeListe[n]);
				result= false;
			}
		}
		else {
			print ("\n  cannot allocate a new event !\n");
			return false;
		}
	}
	return result;
}

/*____________________________________________________________________*/
static void TestEvent( int i)
{
	if( TestEv( i, typeNote, typePrivate) && 
		TestEv( i, typeQuarterFrame, typeSpecific))
		print ("ok\n");
}

/*____________________________________________________________________*/
void Events( void)
{
	int i;
	
	print ("\nChecking events types\n");
	print ("-----------------------------\n");
	ResetFilter(); 
	for(i=typeNote; i<=typePrivate; i++) {
		print ("  %s : ", typeListe[i]);
		RejectType(myFilterPtr,i);
		TestEvent( i);
		AcceptType(myFilterPtr,i);
		flush;
	}
	for(i=typeQuarterFrame; i<=typeSpecific; i++) {
		print ("  %s : ", typeListe[i]);
		RejectType(myFilterPtr,i);
		TestEvent( i);
		AcceptType(myFilterPtr,i);
		flush;
	}
}

/*____________________________________________________________________*/
static void TestCanal( int i)
{
	MidiEvPtr e;
	int n, result= true;
	
	for( n=0; n<16; n++) {
		e= MidiNewEv( typeKeyOn);
		if( e) {
			Chan(e)= n;
			MidiSendIm( refNum, e);
			wait( 2);
			e= GetReceived();
			if( e) {
				if( n== i) {
					print ("\n    error: canal %d not filtered\n", n);
					result= false;
				}
				MidiFreeEv( e);
			}
			else if( n!= i) {
				print ("\n    error: canal %d filtered\n", n);
				result= false;
			}
		}
		else {
			print ("\n  cannot allocate a new event !\n");
			break;
		}
	}
	if( result) print ("ok\n");
}

/*____________________________________________________________________*/
void Canaux( void)
{
	int i;
	
	print ("\nChecking channels\n");
	print ("-----------------------------\n");
	ResetFilter();
	for(i=0; i<16; i++) {
		print ("  canal %d : ", i);
		RejectChan(myFilterPtr,i);
		TestCanal( i);
		AcceptChan(myFilterPtr,i);
		flush;
	}
}

/*____________________________________________________________________*/
static void TestPort( int i)
{
	MidiEvPtr e;
	int n, result= true;
	
	for( n=0; n<LastPort; n++) {
		e= MidiNewEv( typeKeyOn);
		if( e) {
			Port(e)= n;
			MidiSendIm( refNum, e);
			wait( 2);
			e= GetReceived();
			if( e) {
				if( n== i) {
					print ("\n    error: port %d not filtered\n", n);
					result= false;
				}
				MidiFreeEv( e);
			}
			else if( n!= i) {
				print ("\n    error: port %d filtered\n", n);
				result= false;
			}
		}
		else {
			print ("\n  cannot allocate a new event !\n");
			break;
		}
	}
	if( result) print ("ok\n");
}

/*____________________________________________________________________*/
void Ports( void)
{
	int i;
	
	print ("\nChecking ports\n");
	print ("-----------------------------\n");
	ResetFilter();
	for(i=0; i<LastPort; i++) {
		print ("  port %d : ", i);
		RejectPort(myFilterPtr,i);
		TestPort( i);
		AcceptPort(myFilterPtr,i);
		flush;
	}
}

/*____________________________________________________________________*/
int Open (void)
{
	refNum= MidiOpen( ApplName);
	if (refNum > 0) {
		myFilterPtr = NewFilter();
		if (!myFilterPtr) {
			Close ();
			return false;
		}
		MidiConnect( refNum, refNum, true);
		MidiSetFilter( refNum, myFilterPtr);
		return true;
	}
	return false;
}

/*____________________________________________________________________*/
void Close (void)
{
	if (myFilterPtr) FreeFilter (myFilterPtr);
	MidiClose( refNum);
}

#include <Memory.h>
/*____________________________________________________________________*/
main(void)
{
	print ("\nMidiShare filters tests.\n");
	print ("================================\n");
	
	if( MidiShare())
	{
		version= MidiGetVersion();
		print ("MidiShare version %d.%d\n\n", (int)version/100, (int)version%100);
		if( Open()) {
			Ports();
			Canaux();
			Events();
			Close();
		}
		else print ("Can't open a MidiShare application !\n");
	}
	else print ("MidiShare is not installed !\n");
	print ("\nEnd of filters tests.\n");
	return 0;
}
