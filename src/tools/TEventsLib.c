/*

  Copyright ｩ Grame 1999

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

#include "TEvents.h"

#define true	1
#define false	0

/* ========================= variables globales de l'application ==================*/
short version= 0;
long SizeMaxSysEx= 10000;				/* taille du plus grand sysex		*/
char *gOK = " OK";

#define SizeMedSysEx	40				/* test des sysex par pas de 1		*/

#ifndef typeSeqNum
#define typeSeqNum		134				/* d暫ut des 思始ements MidiFile 	*/
#endif

#define noEvts			print("\nno more MidiShare events !")
#define ok			print("%s\n", gOK)


/* ======================= nom des diff屍ents types d'思始ements ==================*/
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

/*______________________________________________________________________________*/
static void SetMidiEvent( MidiEvPtr e, Boolean display)
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
static CmpEv( register MidiEvPtr o, register MidiEvPtr c, register char *src)
{
	long f1, f2, i;
	
	f1= MidiCountFields( o);
	f2= MidiCountFields( c);
	if( f1!= f2)
	{
		print("\n%s : different number of fields : %ld -> %ld", src,f1,f2);
		return false;
	}
	else if( !CmpCommon( o, c, src))
		return false;
	for( i=0; i<f1; i++)
	{
		if( MidiGetField( o, i)!= MidiGetField( c, i))
		{
			print("\n%s : modified value at index %ld", src, i);
			return false;
		}
	}
	return true;
}

/*______________________________________________________________________________*/
static TestEvent( short i, Boolean display)
{
	MidiEvPtr e, copy;
	int ret= false;

	if( display)
		print("    type %s : ", typeListe[i]); flush;
	e= MidiNewEv(i);
	if( !e) noEvts;
	else{
		SetMidiEvent( e, display);
		if( i== typeNote) Dur( e)= 2;
		copy= MidiCopyEv( e);
		if( !copy) noEvts;
		else 
		{
			/* remplissage au dela du nombre de champs de l'思始ement 	*/
			/* shoul not modify  the event								*/
			MidiSetField( e, MidiCountFields(e), 0L);
			if( CmpEv( e, copy, "copy")) {
                                MidiFreeEv(copy);
                                ret = true;
			}
		}
		MidiFreeEv(e);
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
	if( !TestEvent( typeSysEx, true)) {print("\n"); return;}

	/*		test of event until n */
	if( !(e= MidiNewEv(typeSysEx))) { noEvts; return;}
	for( n=0; n<SizeMedSysEx; n++)
	{
		MidiAddField( e, v++);
		if( !(copy= MidiCopyEv( e))) { noEvts; break;}
		else {
                    if (!CmpEv( e, copy, "copy"))
                        print("\n      loop %d length %ld\n", n+1, MidiCountFields(e));
                    MidiFreeEv(copy);
		}
	}
	print(" 0-%ld ok", n); flush;
	/*		test of max for max length */
	for( ; n< SizeMaxSysEx; n++)
	{
		MidiAddField( e, v++);
		if( v>126) v=0; else v++;
	}
	print("\n      length %ld", MidiCountFields(e)); flush;
	if( !(copy= MidiCopyEv( e)))  noEvts;
	else {
		print(" copy"); flush;
		if( CmpEv( e, copy, "copy"))
			print(" ok ");
                MidiFreeEv(copy);
	}
	MidiFreeEv(e);
	ok;
}

/*______________________________________________________________________________*/
void Stream()
{
	register MidiEvPtr e, copy;
	register long v=0, n=0;
	
	/*			  typeStream */
	if( !TestEvent( typeStream, true)) {print("\n"); return;}

	/*			  test of event until n */
	if( !(e= MidiNewEv(typeStream))) { noEvts; return;}
	MidiAddField( e, (long)0xF0);
	MidiAddField( e, (long)0xF7);
	for( n=2; n<SizeMedSysEx; n++)
	{
		MidiSetField( e, n-1, (long)nil);
		MidiAddField( e, (long)0xF7);
		if( !(copy= MidiCopyEv( e))) { noEvts; break;}
		else {
                    if (!CmpEv( e, copy, "copy"))
                            print("\n      loop %d length %ld\n", n+1, MidiCountFields(e));
                    MidiFreeEv(copy);
		}
	}
	print(" 0-%ld ok", n); flush;
	/*			  	test of max for max length */
	MidiSetField( e, n-1, (long)nil);
	for( ; n< SizeMaxSysEx-1; n++)
	{
		MidiAddField( e, v++);
		if( v>126) v=0; else v++;
	}
	MidiAddField( e, (long)0xF7);
	print("\n      length %ld", MidiCountFields(e)); flush;
	if( !(copy= MidiCopyEv( e)))  noEvts;
	else {
		print(" copy"); flush;
		if( CmpEv( e, copy, "copy"))
			print(" ok ");
                MidiFreeEv(copy);
	}
	MidiFreeEv(e);
	ok;
}

/*______________________________________________________________________________*/
void Private()
{
	short i;
	Boolean display= true;
	
	print("\nPrivate events :\n");	
	/* typePrivate */
	for( i= typePrivate; i<typeProcess; i++) {
            if( !TestEvent( i, display)) {
                print(" type %d\n", (int)i);
                return;
            }
            display= false;
	}
	ok;
}

/*______________________________________________________________________________*/
void Process()
{
	short i;
	
	print("\nProcess and DProcess :\n");	
	/*	typePrivate */
	for( i= typeProcess; i<typeQuarterFrame; i++)
	{
		if( TestEvent( i, true)) ok;
		else print("\n");
	}
}

/*______________________________________________________________________________*/
void QFToMidiFile()
{
	short i;
	print("\nQuarterFrame until RegParam :\n");	
	/*	QuarterFrame until RegParam */
	for( i= typeQuarterFrame; i<=typeRegParam; i++) {
		if( TestEvent( i, true)) ok;
		else print("\n");
	}
}

/*______________________________________________________________________________*/
void MidiFile()
{
	short i;
	print("\nMidiFile events :\n");	
	/* typeSeqNum until typePortPrefix */
	for( i= typeSeqNum; i<=typePortPrefix; i++) {
		if( TestEvent( i, true)) ok;
		else print("\n");
	}
}

/*______________________________________________________________________________*/
void Internals()
{
	short i;
	
	print("\nInternal events :\n");	
	/* typePortPrefix+1 until typeReserved */
	for( i= typePortPrefix+1; i<=typeApplAlarm; i++) {
		if( TestEvent( i, true)) ok;
		else print("\n");
	}
}

/*______________________________________________________________________________*/
void Fonctions()
{
	int i; long n1, n2;
	
	print("\nMidiShare Functions events :\n");
	n1 = MidiFreeSpace();
	for( i= typeMidiOpen; i<typeReserved; i++)
	{
		MidiEvPtr e;
		print("    type %s : ", typeListe[i]); flush;
		if( e= MidiNewEv(i)){
			ok;
			MidiFreeEv(e);
		}
		else print("warning: allocation failed\n");
	}
	n2 = MidiFreeSpace();
	if (n1 != n2)
		print("warning: MidiFreeSpace is %ld instead %ld\n", n2, n1);
}

/*______________________________________________________________________________*/
void Reserved()
{
	short ret= true;
	MidiEvPtr e;
	
	print("\nReserved events :\n");	
	/*	typeReserved until typeDead */
	print("    type %s : ", typeListe[typeReserved]); flush;
	if( e= MidiNewEv(typeReserved)) 
	{
		print("\ntype %d : allocation return %lx\n", (int)typeReserved, e);
		ret= false;
	}
	if( e= MidiNewEv( typeDead-1)) 
	{
		print("\ntype %d : allocation return %lx\n", (int)typeDead-1, e);
		ret= false;
	}
	if( ret) ok;
	print("    type Dead : "); flush;
	if( TestEvent( typeDead, false)) ok;
	else print("\n");
}

/*______________________________________________________________________________*/
main( int argc, char *argv[])
{
	print("\nAllocation, emission and reception of MidiShare events.\n");
	print("========================================================\n");
    version = MidiGetVersion(); // MidiGetLibVersion ();
    print("               MidiShare Library version %d.%02d\n", (int)version/100, (int)version%100);
    print("\nWarning : client must have at least 10000 events !\n");

    ChanEvents(); flush;
    CommonEvents(); flush;			
    SystemeEx(); flush;
    Stream(); flush;
    Private(); flush;
    Process(); flush;
    QFToMidiFile(); flush;
    MidiFile(); flush;
    Internals(); flush;
    Fonctions(); flush;
    Reserved(); flush;

	print("\nEnd of allocation emission and reception test of events.\n");
	return 0;
}
