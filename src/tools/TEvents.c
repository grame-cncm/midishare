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

/*
 *
 * Validation and test of MidiShare
 * Event allocation and transmission test.
 *
 * GRAME	Mardi 8 D残embre 1992	DF
 *          Lundi 20 Septembre 1999 SL Adapation Linux (plus de MidiAvailEv)
 * 		    English version 11/11/99 SL
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

#ifdef PASCALNAME
MidiName ApplName = "\pSender";
MidiName OtherName = "\pReceiver";
#endif

#ifdef CNAME
MidiName ApplName = "Sender";
MidiName OtherName = "Receiver";
#endif


/* ========================= variables globales de l'application ==================*/
short r1= 0, r2= 0, version= 0;
long SizeMaxSysEx= 10000;				/* taille du plus grand sysex		*/
char *gOK = " OK";

#define SizeMedSysEx	40				/* test des sysex par pas de 1		*/

#ifndef typeSeqNum
#define typeSeqNum		134				/* d暫ut des 思始ements MidiFile 	*/
#endif

#define noEvts			print("\nno more MidiShare events !\n")
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


/* ============================= functions declarations ========================*/

typedef int (* GetEvFuncPtr)( MidiEvPtr e);

static int GetNote( MidiEvPtr e);
static int GetEvents( MidiEvPtr e);
static int GetNoExt( MidiEvPtr e);
static int GetPrivate( MidiEvPtr e);
static int GetCtrl( MidiEvPtr e);
static int GetNothing( MidiEvPtr e);


/* ============= array of reception functions =============*/

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
/*130 		GetEvents, 	GetCtrl, 	GetCtrl, 	GetCtrl, 	GetNoExt, */
/*130 */		GetEvents, 	GetEvents, 	GetEvents, 	GetEvents, 	GetNoExt,
/*135 */		GetNoExt, 	GetNoExt, 	GetNoExt, 	GetNoExt, 	GetNoExt,
/*140 */		GetNoExt, 	GetNoExt, 	GetNoExt, 	GetNoExt, 	GetNoExt,
/*145 */		GetNoExt, 	GetNoExt, 	GetNoExt, 	GetNoExt, 	GetNoExt,
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
	unsigned long time = MidiGetTime()+ d;
	usleep (d*1000);
	while( MidiGetTime()<= time);
}

/*______________________________________________________________________________*/
static void wait2( int d)
{
	long time, i, t;
	
	time= (i= t= MidiGetTime())+ d;
	while( t<= time)
	{
		if( !((t-i)%10000))
			print("."); flush;
		t= MidiGetTime();
	}
}

/*______________________________________________________________________________*/
int OpenAppls()
{	
	if( (r1= MidiOpen( ApplName)) < 0) {
		print("MidiOpen failed !\n");
		return false;
	}
	if( (r2= MidiOpen( OtherName)) < 0) {
		print("MidiOpen failed !\n");
		MidiClose( r1);
		return false;
	}
//	MidiConnect( r1, 0, true);
//	MidiConnect( 0, r1, true);
	MidiConnect( r1, r2, true);
	usleep (50000);
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
static CompareEv( MidiEvPtr e, short refnum, char *src)
{
	MidiEvPtr get;
	
	if( get= MidiAvailEv( refnum))
		return CmpEv( e, get, src);
	else print("\n%s : MidiAvailEv return nil ", src);
	return false;
}

/*______________________________________________________________________________*/
static CompareStream( MidiEvPtr e, short refnum, char *src)
{
	long f1, f2, i;
	MidiEvPtr get;
	
	if( get= MidiAvailEv( refnum))
	{
		if( EvType(get)== typeSysEx)
			EvType(get)= typeStream;
		else
		{
			print("wrong type : %d \n", EvType(get));
			return false;
		}
		f1= MidiCountFields( e)-1;
		f2= MidiCountFields( get)+1;
		if( f1!= f2)
		{
			print("\n%s : different number of fields : %ld -> %ld", src,f1,f2);
			return false;
		}
		else if( !CmpCommon( e, get, src))
			return false;
		for( i=1; i<f1; i++)
		{
			if( MidiGetField( e, i)!= MidiGetField( get, i-1))
			{
				print("\n%s : modified value at index %ld", src, i);
				return false;
			}
		}
		return true;
	}
	else print("\n%s : MidiAvailEv return nil ", src);
	return false;
}

/*______________________________________________________________________________*/
static CompareCtrl14b( MidiEvPtr e)
{
	MidiEvPtr a, b;
	long v1, v2;
	int ret= false;
	
	if( (a= MidiGetEv( r1)) && (b= MidiGetEv( r1)))
	{
		v1= MidiGetField( a, 0L);
		v2= MidiGetField( b, 0L);
		if( v1!=1 || (v2!= 33))
			print("reception of wrong controlers : %ld followed by %ld\n",v1,v2);
		else
		{
			v2= MidiGetField( b, 1L);
			v2+= MidiGetField( a, 1L)<<7;
			EvType(b)= typeCtrl14b;
			MidiSetField( b, 0L, MidiGetField( a, 0L));
			MidiSetField( b, 1L, v2);
			ret= CmpEv( e, b, "ext");
		}
		MidiFreeEv( a);
		MidiFreeEv( b);
	}
	else print("\n%s : MidiAvailEv return nil ", "ext");
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
			print("\nreception of wrong controlers : %ld %ld %ld %ld"
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
			ret= CmpEv( e, d, "ext");
		}
		MidiFreeEv( a);
		MidiFreeEv( b);
		MidiFreeEv( c);
		MidiFreeEv( d);
	}
	else print("\n%s : MidiAvailEv return nil ", "ext");
	return ret;
}

/*______________________________________________________________________________*/
static GetEvents( MidiEvPtr e)
{
	long n;
	int ret= false;
	
	n= MidiCountEvs( r2);
	if(n != 1)
		print("\nWarning : wrong fifo content : %ld ", n);
	else ret = CompareEv( e, r2, "int");
	return ret;
}

/*______________________________________________________________________________*/
static GetStream( MidiEvPtr e)
{
	long n;
	int ret= false;
	
	n= MidiCountEvs( r2);
	if( n != 1)
		print("\nWarning : wrong fifo content : %ld ", n);
	else ret = CompareEv( e, r2, "int");
	return ret;
}

/*______________________________________________________________________________*/
static GetNothing( MidiEvPtr unused)
{
	long n;
	int ret= false;
	
	n= MidiCountEvs( r2);
	if( n)
		print("\nWarning : wrong fifo content : rcv=%ld ", n);
	else ret= true;
	return ret;
}

/*______________________________________________________________________________*/
static GetNoExt( MidiEvPtr e)
{
	long n;
	int ret= false;
	
	n= MidiCountEvs( r2);
	if( n!= 1)
		print("\nWarning : wrong fifo content : rcv=%ld ", n);
	else ret= true;
	if( n== 1)
		ret &= CompareEv( e, r2, "int");
	return ret;
}

/*______________________________________________________________________________*/
static GetPrivate( MidiEvPtr e)
{
	long n;
	int ret= false;
	
	n= MidiCountEvs( r1);
	if( n!=1)
		print("\nWarning : wrong fifo content : %ld ", n);
	else ret= true;
	if( n== 1)
		ret &= CompareEv( e, r1, "int");
	n= MidiCountEvs( r2);
	if( n)
		print("\nWarning : event distributed at rcv\n");
	return ret;
}

/*______________________________________________________________________________*/
static GetCtrl( MidiEvPtr e)
{
	long n1, n2;
	int ret= false, type;
	
	type= EvType(e);
	wait( 2);
	n1= MidiCountEvs( r1);
	n2= MidiCountEvs( r2);
	if( n2!= 1 || (type== typeCtrl14b && n1!= 2)
			   || (type== typeNonRegParam && n1!= 4)
			   || (type== typeRegParam && n1!= 4))
		print("\nWarning : wrong fifos content : appl=%ld rcv=%ld ", n1,n2);
	else ret= true;
	if( type== typeCtrl14b && n1== 2)
		ret &= CompareCtrl14b( e);
	else if( n1== 4)
		ret &= CompareRegParam( e);
	print(" *");
	if( n2== 1)
		ret &= CompareEv( e, r2, "int");
	return ret;
}

/*______________________________________________________________________________*/
static CmpKeyOnOff( MidiEvPtr e1, MidiEvPtr e2, short d)
{
	short dc;
	int ret= true;
	
	if( EvType(e1)!= typeKeyOn || EvType(e2)!= typeKeyOn)
	{
		print("\next : wrong types : first=%d next=%d ", (int)EvType(e1), (int)EvType(e2));
		ret= false;
	}
	if( Vel(e2))
	{
		print("\next : note off : wrong velocity : %d ", (int)Vel(e2));
		ret= false;
	}
	dc= (short)(Date(e2) - Date(e1));
	if( dc!= d)
	{
		print("\next : wrong duration : %d instead of %d ", (int)dc, (int)d);
		ret= false;
	}
	if( Pitch(e1)!= Pitch(e2))
	{
		print("\next : wrong pitch : %d and %d ", (int)Pitch(e1), (int)Pitch(e2));
		ret= false;
	}
	EvType(e1)= typeNote;
	Dur(e1)= dc;
	return ret;
}

/*______________________________________________________________________________*/
static GetNote( MidiEvPtr e)
{
	long n;	int ret= false;

	wait (Dur(e));
	n= MidiCountEvs( r2);
	if(n != 1)
		print("\nWarning : wrong fifo content : %ld ", n);
	else ret = CompareEv( e, r2, "int");
	return ret;
}

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
static TestEvent( short i, Boolean display)
{
	MidiEvPtr e, copy;
	int ret= false;

	MidiFlushEvs( r1);
	MidiFlushEvs( r2);
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
			if( CmpEv( e, copy, "copy"))
			{
				if( i== typeProcess || i== typeDProcess)
					return true;
				Date( copy)= MidiGetTime()+2;
				MidiSend( r1, copy); 
				wait(50);
				ret= (* GetEvTable[i])( e);
			}
		}
		MidiFreeEv(e);
		MidiFlushEvs(r1);
		MidiFlushEvs(r2);
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
	for( n=0; n<SizeMedSysEx; n++)
	{
		MidiAddField( e, v++);
		if( !(copy= MidiCopyEv( e))) { noEvts; break;}
		else if( CmpEv( e, copy, "copy")){
			MidiSendIm( r1, copy); wait( 20);
			if( !GetEvents( e))
			{
				print("\n      loop %d length %ld\n", n+1, MidiCountFields(e));
				MidiFreeEv(e);
				goto failed;
			}
			MidiFlushEvs(r1);
			MidiFlushEvs(r2);
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
	else 
	{
		print(" copy"); flush;
		if( CmpEv( e, copy, "copy")){
			print(" ok ");
			MidiSendIm( r1, copy); 
			wait2(2000);
//			wait2( (SizeMaxSysEx*2)/3+ 10);
			if( !GetEvents( e))
			{
				MidiFreeEv(e);
				goto failed;
			}
		}
	}
	MidiFreeEv(e);
	MidiFlushEvs( r1);
	MidiFlushEvs( r2);
	ok;
failed:
	MidiFlushEvs(r1); MidiFlushEvs(r2);	
}

/*______________________________________________________________________________*/
void Stream()
{
	register MidiEvPtr e, copy;
	register long v=0, n=0;
	
	/*			  typeStream */
	if( !TestEvent( typeStream, true)) {print("\n"); goto failed;}

	/*			  test of event until n */
	if( !(e= MidiNewEv(typeStream))) { noEvts; goto failed;}
	MidiAddField( e, (long)0xF0);
	MidiAddField( e, (long)0xF7);
	for( n=2; n<SizeMedSysEx; n++)
	{
		MidiSetField( e, n-1, (long)nil);
		MidiAddField( e, (long)0xF7);
		if( !(copy= MidiCopyEv( e))) { noEvts; break;}
		else if( CmpEv( e, copy, "copy")){
			MidiSendIm( r1, copy); wait( 40);
			if( !GetStream( e))
			{
				print("\n      loop %d length %ld\n", n+1, MidiCountFields(e));
				MidiFreeEv(e);
				goto failed;
			}
			MidiFlushEvs(r1);
			MidiFlushEvs(r2);
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
	else 
	{
		print(" copy"); flush;
		if( CmpEv( e, copy, "copy")){
			print(" ok ");
			MidiSendIm( r1, copy); wait2( (SizeMaxSysEx*2)/3+ 10);
			if( !GetStream( e))
			{
				MidiFreeEv(e);
				goto failed;
			}
		}
	}
	MidiFreeEv(e);
	ok;
failed:
	MidiFlushEvs(r1); MidiFlushEvs(r2);	
}

/*______________________________________________________________________________*/
void Private()
{
	short i;
	Boolean display= true;
	
	print("\nPrivate events :\n");	
	/*													typePrivate */
	for( i= typePrivate; i<typeProcess; i++)
	{
		if( !TestEvent( i, display))
		{
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
	/*													typePrivate */
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
	/*													nQuarterFrame until RegParam */
	for( i= typeQuarterFrame; i<=typeRegParam; i++)
	{
		if( TestEvent( i, true)) ok;
		else print("\n");
	}
}

/*______________________________________________________________________________*/
void MidiFile()
{
	short i;
	
	print("\nMidiFile events :\n");	
	if( MidiGetVersion() < 160)
		print("not implemented !\n");	
	/*												typeSeqNum until typePortPrefix */
	else for( i= typeSeqNum; i<=typePortPrefix; i++)
	{
		if( TestEvent( i, true)) ok;
		else print("\n");
	}
}

/*______________________________________________________________________________*/
void Internals()
{
	short i;
	
	print("\nInternal events :\n");	
	if( MidiGetVersion() < 160)
		print("not implemented !\n");	
	/*												typePortPrefix+1 until typeReserved */
	else for( i= typePortPrefix+1; i<=typeApplAlarm; i++)
	{
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
	if( MidiGetVersion() < 193)
		print("not implemented !\n");	
	else for( i= typeMidiOpen; i<typeReserved; i++)
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
	/*												typeReserved until typeDead */
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
void Close ()
{
	MidiClose( r1);
	MidiClose( r2);
}

/*______________________________________________________________________________*/
main( int argc, char *argv[])
{
	print("\nAllocation, emission and reception of MidiShare events.\n");
	print("==========================================================\n");

	if( MidiShare()) {
		version= MidiGetVersion();
		print("                MidiShare version %d.%d\n", (int)version/100, (int)version%100);
		print("\nWarning : MidiShare must have at least 10000 events !\n");

		if( OpenAppls()) {
			wait( 100);

/*			ChanEvents(); flush;
			CommonEvents(); flush;			
*/			SystemeEx(); flush;
			MidiFlushEvs(r1); MidiFlushEvs(r2);
/*			Stream(); flush;
			MidiFlushEvs(r1); MidiFlushEvs(r2);
			Private(); flush;
			Process(); flush;
			QFToMidiFile(); flush;
			MidiFile(); flush;
			Internals(); flush;
			Fonctions(); flush;
			Reserved(); flush;
*/
			Close ();
		}

	}
	else print("MidiShare is not installed !\n");
	print("\nEnd of allocation emission and reception test of events.\n");
	return 0;
}
