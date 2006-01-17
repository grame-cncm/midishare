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
  research@grame.fr


*/

#ifdef __Windows__
#	include <stdio.h>
#	include <String.h>
#	include <StdLib.h>
#	include <MidiShare.h>
#	define CNAME
#	define CTASKS
#	define nil 0
#	define flush    fflush(stdout)
#	define print	printf
#else
#	define MSALARMAPI
#endif


#ifdef __linux__
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
#	define nil 0
#	define MidiAvailEv(r)  MidiGetEv(r)
#endif


#if macintosh
#	include <stdio.h>
#	include <String.h>
#	include <StdLib.h>
#	include "MidiShare.h"
# if defined(__ppc__) && defined(__GNUC__)
#	define CNAME
#	define CTASKS
#	define nil 0
# else
#	define PASCALNAME
#	define PASCALTASKS
# endif
#	define flush	fflush( stdout)
#	define print	printf
#endif

#include "TOutEvents.h"

#define true	1
#define false	0

#ifdef PASCALNAME
MidiName ApplName = "\pTest Out Evts";
#endif

#ifdef CNAME
MidiName ApplName = "Test Out Evts";
#endif

/* ============================================================================*/
short r1= 0, version= 0;
long SizeMaxSysEx= 10000;
char *gOK = " OK";

#define SizeMedSysEx	40	

#define noEvts		print("\nno more MidiShare events !\n")
#define ok			print("%s\n", gOK)
#define WrongCount(expected,received)	print("\nWarning: wrong fifo content: expected %ld, received %ld", expected,received)

/* ================================ ev type names ==============================*/
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
/*148 */	"Specific",		"PortPrefix",	"RcvAlarm", 	"ApplAlarm", "Reserved"		
};



/* ============================= functions declarations ========================*/

typedef int (* GetEvFuncPtr)( MidiEvPtr e);

static int GetNote( MidiEvPtr e);
static int GetEvents( MidiEvPtr e);
static int GetNoExt( MidiEvPtr e);
static int GetPrivate( MidiEvPtr e);
static int GetCtrl( MidiEvPtr e);
static int GetNothing( MidiEvPtr e);


GetEvFuncPtr GetEvTable[] = {
/*  0 */		GetNote, 	GetEvents, 	GetEvents, 	GetEvents, 	GetEvents,
/*  5 */		GetEvents, 	GetEvents, 	GetEvents, 	GetEvents, 	GetEvents,
/* 10 */		GetEvents, 	GetEvents, 	GetEvents, 	GetEvents, 	GetEvents,
/* 15 */		GetEvents, 	GetEvents, 	GetEvents, 	GetNoExt, 	GetPrivate,
/* 20 */		GetPrivate, GetPrivate, GetPrivate, GetPrivate, GetPrivate,
/* 25 */		GetPrivate, GetPrivate, GetPrivate, GetPrivate, GetPrivate,
/* 30 */		GetPrivate, GetPrivate, GetPrivate, GetPrivate, GetPrivate,
/* 35 */		GetPrivate, GetPrivate, GetPrivate, GetPrivate, GetPrivate,
/* 40 */		GetPrivate, GetPrivate, GetPrivate, GetPrivate, GetPrivate,
/* 45 */		GetPrivate, GetPrivate, GetPrivate, GetPrivate, GetPrivate,
/* 50 */		GetPrivate, GetPrivate, GetPrivate, GetPrivate, GetPrivate,
/* 55 */		GetPrivate, GetPrivate, GetPrivate, GetPrivate, GetPrivate,
/* 60 */		GetPrivate, GetPrivate, GetPrivate, GetPrivate, GetPrivate,
/* 65 */		GetPrivate, GetPrivate, GetPrivate, GetPrivate, GetPrivate,
/* 70 */		GetPrivate, GetPrivate, GetPrivate, GetPrivate, GetPrivate,
/* 75 */		GetPrivate, GetPrivate, GetPrivate, GetPrivate, GetPrivate,
/* 80 */		GetPrivate, GetPrivate, GetPrivate, GetPrivate, GetPrivate,
/* 85 */		GetPrivate, GetPrivate, GetPrivate, GetPrivate, GetPrivate,
/* 90 */		GetPrivate, GetPrivate, GetPrivate, GetPrivate, GetPrivate,
/* 95 */		GetPrivate, GetPrivate, GetPrivate, GetPrivate, GetPrivate,
/*100 */		GetPrivate, GetPrivate, GetPrivate, GetPrivate, GetPrivate,
/*105 */		GetPrivate, GetPrivate, GetPrivate, GetPrivate, GetPrivate,
/*110 */		GetPrivate, GetPrivate, GetPrivate, GetPrivate, GetPrivate,
/*115 */		GetPrivate, GetPrivate, GetPrivate, GetPrivate, GetPrivate,
/*120 */		GetPrivate, GetPrivate, GetPrivate, GetPrivate, GetPrivate,
/*125 */		GetPrivate, GetPrivate, GetPrivate, 0, 			0,
/*130 */		GetEvents, 	GetCtrl, 	GetCtrl, 	GetCtrl, 	GetNoExt,
/*135 */		GetNoExt, 	GetNoExt, 	GetNoExt, 	GetNoExt, 	GetNoExt,
/*140 */		GetNoExt, 	GetNoExt, 	GetNoExt, 	GetNoExt, 	GetNoExt,
/*145 */		GetNoExt, 	GetNoExt, 	GetNoExt, 	GetNoExt, 	0,
/*150 */		GetNoExt,	GetNoExt,	0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
/*170 */		0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
/*190 */		0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
/*210 */		0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
/*230 */		0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
/*250 */		0,0,0,0,0, GetNothing
};



/*______________________________________________________________________________*/
static void wait( int d)
{
	unsigned long time;
	time= MidiGetTime()+ d;
	while( MidiGetTime()<= time);
}

/*______________________________________________________________________________*/
int OpenAppl()
{	
	if( (r1= MidiOpen( ApplName)) < 0)
	{
		print("Impossible to open a MidiShare application !\n");
		print("Interrupted test !\n");
		return false;
	}
	MidiConnect( r1, 0, true);
	MidiConnect( 0, r1, true);
	return true;
}

/*______________________________________________________________________________*/
static CmpCommon( MidiEvPtr o, MidiEvPtr c)
{
	int ret= true;
	
	if( EvType(o) != EvType(c)) {
		print("\nWarning : event type modified : %d instead %d",
					(int)EvType(c), (int)EvType(o));
		ret= false;
	}
	if( ( EvType(o) < typeSongPos) && (Chan(o) != Chan(c))) {
		print("\nWarning : event channel modified : %d instead %d",
					(int)Chan(c), (int)Chan(o));
		ret= false;
	}
	if( Port(o) != Port(c)) {
		print("\nWarning : event port modified : %d instead %d",
					(int)Port(c), (int)Port(o));
		ret= false;
	}
	return ret;
}

/*______________________________________________________________________________*/
static CmpEv( register MidiEvPtr o, register MidiEvPtr c)
{
	long f1, f2, i;
	
	f1= MidiCountFields( o);
	f2= MidiCountFields( c);
	if( f1!= f2) {
		print("\nWarning : fields count differs : %ld -> %ld", f1, f2);
		return false;
	}
	else if( !CmpCommon( o, c))
		return false;
	for( i=0; i<f1; i++) {
		if( MidiGetField( o, i)!= MidiGetField( c, i)) {
			print("\nWarning : modified value at index %ld", i);
			return false;
		}
	}
	return true;
}

/*______________________________________________________________________________*/
static CompareEv( MidiEvPtr e, short refnum)
{
	Boolean ret = false;
	MidiEvPtr get = MidiGetEv (refnum);
	
	if (get) {
		ret = CmpEv (e, get);
		MidiFreeEv (get);
	}
	else print("\nWarning: MidiGetEv returned nil ");
	return ret;
}


/*______________________________________________________________________________*/
static CompareCtrl14b (MidiEvPtr e)
{
	MidiEvPtr a, b;
	long v1, v2;
	int ret= false;
	
	if( (a= MidiGetEv( r1)) && (b= MidiGetEv( r1)))
	{
		v1= MidiGetField( a, 0L);
		v2= MidiGetField( b, 0L);
		if( v1!=1 || (v2!= 33))
			print("\nWarning: wrong controlers received: %ld, %ld", v1, v2);
		else
		{
			v2= MidiGetField( b, 1L);
			v2+= MidiGetField( a, 1L)<<7;
			EvType(b)= typeCtrl14b;
			MidiSetField( b, 0L, MidiGetField( a, 0L));
			MidiSetField( b, 1L, v2);
			ret= CmpEv( e, b);
		}
		MidiFreeEv( a);
		MidiFreeEv( b);
	}
	else print ("\nWarning : MidiGetEv return nil");
	return ret;
}

/*______________________________________________________________________________*/
static CompareRegParam( MidiEvPtr e)
{
	MidiEvPtr a, b, c, d;
	long v1, v2, v3, v4;
	int ret= false;
	
	if( (a= MidiGetEv( r1)) && (b= MidiGetEv( r1))
		&& (c= MidiGetEv( r1)) && (d= MidiGetEv( r1)))
	{
		v1= MidiGetField( a, 0L);
		v2= MidiGetField( b, 0L);
		v3= MidiGetField( c, 0L);
		v4= MidiGetField( d, 0L);
		ret= (v3==6) && (v4==38);
		ret &= ((EvType(e)== typeRegParam) && (v1==101) && (v2==100))
			|| ((v1==99) && (v2==98));
		if( !ret)
			print("\nWarning: wrong controlers received: %ld, %ld, %ld, %ld"
						, v1, v2, v3, v4);
		else
		{
			v2= MidiGetField( b, 1L);
			v2+= MidiGetField( a, 1L)<<7;
			v4= MidiGetField( d, 1L);
			v4+= MidiGetField( c, 1L)<<7;
			EvType(d)= EvType(e);
			MidiSetField( d, 0L, v2);
			MidiSetField( d, 1L, v4);
			ret= CmpEv( e, d);
		}
		MidiFreeEv( a);
		MidiFreeEv( b);
		MidiFreeEv( c);
		MidiFreeEv( d);
	}
	else print ("\nWarning : MidiGetEv return nil");
	return ret;
}

/*______________________________________________________________________________*/
static CompareStream( MidiEvPtr e, short refnum)
{
	Boolean ret = false; long f1, f2, i;
	MidiEvPtr get = MidiGetEv( refnum);
	
	if (get) {
		if (EvType(get) == typeSysEx) {
			EvType(get) = typeStream;
			f1= MidiCountFields (e)-1;
			f2= MidiCountFields (get)+1;
			if( f1!= f2) {
				print("\nWarning : fields count differs : %ld -> %ld", f1, f2);
				goto err;
			}
			else if( !CmpCommon (e, get))
				goto err;
			for( i=1; i<f1; i++) {
				if (MidiGetField (e, i) != MidiGetField (get, i-1)) {
					print ("\nWarning : event modified at position %ld", i);
					goto err;
				}
			}
			MidiFreeEv (get);
			return true;
		}
		else print ("\nWarning: wrong type : %d \n", EvType(get));
	}
	else print ("\nWarning : MidiGetEv return nil");
err:
	MidiFreeEv (get);
	return false;
}

/*______________________________________________________________________________*/
static GetRegAndNonReg( MidiEvPtr e)
{
	return CompareEv (e, r1);
}

/*______________________________________________________________________________*/
static GetEvents( MidiEvPtr e)
{
	long n = MidiCountEvs (r1);
	if (n!= 1) WrongCount(1, n);
	else return CompareEv (e, r1);
	return false;
}

/*______________________________________________________________________________*/
static GetStream (MidiEvPtr e)
{
	long n = MidiCountEvs (r1);
	if (n!= 1) WrongCount(1, n);
	else return CompareStream (e, r1);
	return false;
}

/*______________________________________________________________________________*/
static GetNothing( MidiEvPtr unused)
{
	long n = MidiCountEvs (r1);
	if( n) WrongCount(0, n);
	else return true;
	return false;
}

/*______________________________________________________________________________*/
static GetNoExt( MidiEvPtr e)
{
	long n = MidiCountEvs (r1);
	if (n!= 1) WrongCount(1, n);
	else return CompareEv (e, r1);
	return false;
}

/*______________________________________________________________________________*/
static GetPrivate( MidiEvPtr e)
{
	print("\nWarning : private event received\n");
	return false;
}

/*______________________________________________________________________________*/
static GetCtrl (MidiEvPtr e)
{
	long n, expected;
	int ret= false, type;
	
	type= EvType(e);
	wait (10);
	switch (type) {
		case typeCtrl14b:		expected = 2;
			 break;
		case typeNonRegParam:
		case typeRegParam:		expected = 4;
			break;
		default:				expected = 1;			
	}
	n= MidiCountEvs (r1);
	if (n!= expected)
		WrongCount(expected, n);
	else ret= true;
	if ((type == typeCtrl14b) && (n == 2))
		ret &= CompareCtrl14b( e);
	else if (n == 4)
		ret &= CompareRegParam (e);
	print(" *");
	return ret;
}

/*______________________________________________________________________________*/
static GetNote( MidiEvPtr e)
{
	long n;	int ret= false;

	wait (Dur(e));
	n= MidiCountEvs( r1);
	if(n != 2)
		WrongCount(2, n);
	else {
		EvType(e) = typeKeyOn;
		ret = CompareEv (e, r1);
		if (ret) {
			MidiEvPtr off = MidiGetEv (r1);
			if (off) {
				switch (EvType(off)) {
					case typeKeyOn:
						Vel(e) = 0;
						ret = CmpEv (e, off);
						break;
					case typeKeyOff:
						EvType(e) = typeKeyOff;
						ret = CmpEv (e, off);
						break;
				}
				MidiFreeEv (off);
			}
			else print("\nWarning: Key Off not received");
		}
	}
	return ret;
}

/*______________________________________________________________________________*/
static void SetMSEvent( MidiEvPtr e, Boolean display)
{
	register long f, i, v=1;
	
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
static TestEvent( short i, Boolean display)
{
	MidiEvPtr e, copy;
	int ret= false;

	MidiFlushEvs( r1);
	if( display)
		print("    type %s : ", typeListe[i]); flush;
	e= MidiNewEv(i);
	if( !e) noEvts;
	else{
		SetMSEvent (e, display);
		if( i== typeNote) Dur (e)= 2;
		copy= MidiCopyEv( e);
		
		if( !copy) noEvts;
		else 
		{
			MidiSend( r1, copy); 
			wait (10);
			ret= (* GetEvTable[i])(e);
		}
		MidiFreeEv (e);
		MidiFlushEvs (r1);
	}
	return ret;
}

/*______________________________________________________________________________*/
void ChanEvents()
{
	short i;
	
	print("\nChannel events :\n");
	/*											typeNote until typePitchWheel */
	for( i= typeNote; i<typeSongPos; i++)
		if( TestEvent( i, true)) ok;
		else print("\n");
}

/*______________________________________________________________________________*/
void CommonEvents()
{
	short i;
	
	print("\nCommon events :\n");	
	/*		typeSongPos until typeReset */
	for( i= typeSongPos; i<typeSysEx; i++)
		if( TestEvent( i, true)) ok;
		else print("\n");
}

/*______________________________________________________________________________*/
void SystemeEx()
{
	register MidiEvPtr e, copy;
	register long v=0, n=0;
	
	print("\nSysteme exclusive :\n");
	
	/*		typeSysEx */
	if( !TestEvent( typeSysEx, true)) {print("\n"); goto failed;}

	/*		test of event until n */
	if( !(e= MidiNewEv(typeSysEx))) { noEvts; goto failed;}
	for (n=0; n<SizeMedSysEx; n++) {
		MidiAddField( e, v++);
		copy= MidiCopyEv (e);
		if (!copy) { noEvts; break;}
		else {
			MidiSendIm (r1, copy); wait( 40);
			if (!GetEvents(e)) {
				print("\n      loop %d length %ld\n", n+1, MidiCountFields(e));
				MidiFreeEv(e);
				goto failed;
			}
			MidiFlushEvs(r1);
		}
	}
	print(" 0-%ld ok", n); flush;
	/*		test of max for max length */
	for( ; n< SizeMaxSysEx; n++) {
		MidiAddField( e, v++);
		if( v>126) v=0; else v++;
	}
	print("\n      length %ld", MidiCountFields(e)); flush;
	if( !(copy= MidiCopyEv( e)))  noEvts;
	else {
		print(" send "); flush;
		MidiSendIm (r1, copy); wait( (SizeMaxSysEx*2)/3+ 100);
		if (!GetEvents (e)) {
			MidiFreeEv(e);
			goto failed;
		}
	}

	MidiFreeEv(e);
	ok;
failed:
	MidiFlushEvs(r1);
}

/*______________________________________________________________________________*/
void Stream()
{
	register MidiEvPtr e, copy;
	register long v=0, n=0;
	
	/*			  typeStream */
	print("\nStream :\n");
	print("    type %s : ", typeListe[typeStream]); flush;
	
	/*			  test of event until n */
	if( !(e= MidiNewEv(typeStream))) { noEvts; goto failed;}
	MidiAddField( e, 0xf0);
	MidiAddField( e, 0);
	for( n=2; n<SizeMedSysEx; n++)
	{
		MidiAddField( e, 0xf7);
		copy = MidiCopyEv (e);
		if (!copy) { noEvts; break;}
		else {
			MidiSendIm( r1, copy); wait(50);
			if (!GetStream( e)) {
				print("\n      loop %d length %ld\n", n+1, MidiCountFields(e));
				MidiFreeEv(e);
				goto failed;
			}
			MidiFlushEvs(r1);
		}
		MidiSetField( e, n, (long)nil);
	}
	print(" 0-%ld ok", n); flush;
	/*			  	test of max for max length */
	for( ; n< SizeMaxSysEx-1; n++) {
		MidiAddField( e, v++);
		if( v>126) v=0; else v++;
	}
	MidiAddField( e, 0xf7);
	print("\n      length %ld", MidiCountFields(e)); flush;
	if (!(copy= MidiCopyEv( e)))  noEvts;
	else {
		print(" send "); flush;
		MidiSendIm (r1, copy); wait( (SizeMaxSysEx*2)/3+ 100);
		if( !GetStream( e)) {
			MidiFreeEv(e);
			goto failed;
		}
	}

	MidiFreeEv(e);
	ok;
failed:
	MidiFlushEvs(r1);
}

/*______________________________________________________________________________*/
void QFToMidiFile()
{
	short i;
	
	print("\nQuarterFrame until RegParam :\n");	
	/*													nQuarterFrame until RegParam */
	for( i= typeQuarterFrame; i<=typeRegParam; i++)
	{
		if (TestEvent( i, true)) ok;
		else print("\n");
	}
}

/*______________________________________________________________________________*/
void Close ()
{
	MidiClose( r1);
}

/*______________________________________________________________________________*/
main( int argc, char *argv[])
{
	print("\nMidiShare drivers transmission and reception tests.\n");
	print("===================================================\n");

	if( MidiShare())
	{
		version= MidiGetVersion();
		print("                MidiShare version %d.%d\n", (int)version/100, (int)version%100);
		print("\nWarning : MidiShare must have at least 10000 events !\n");

		if( OpenAppl()) {
			wait (100);
			ChanEvents(); flush;
			CommonEvents(); flush;			
			SystemeEx(); flush;
			MidiFlushEvs(r1);
			Stream(); flush;
			MidiFlushEvs(r1);
			QFToMidiFile(); flush;
			Close ();
		}
	}
	else print("MidiShare is not installed !\n");
	print("\nEnd of MidiShare drivers transmission and reception tests.\n");
	return 0;
}
