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

#include <stdio.h>

#ifdef __Windows__
#	include <String.h>
#	include <StdLib.h>
#	include <MidiShare.h>
#	define CNAME
#	define CTASKS
#	define PROTOTYPE
#endif


#ifdef __Linux__
#	include "MidiShare.h"
#	define CNAME
#	define CTASKS
#	define nil 0
#	define MidiAvailEv(r)  MidiGetEv(r)
#endif


#ifdef __Macintosh__
#	include <String.h>
#	include <StdLib.h>
#	include "MidiShare.h"
/*
#include <Desk.h>
#include <Dialogs.h>
#include <DiskInit.h>
#include <Events.h>
#include <Files.h>
#include <Fonts.h>
#include <Memory.h>
#include <Menus.h>
#include <OSEvents.h>
#include <OSUtils.h>
#include <Packages.h>
#include <QuickDraw.h>
#include <Resources.h>
#include <ROMDefs.h>
#include <SegLoad.h>
#include <TextEdit.h>
#include <ToolUtils.h>
#include <Traps.h>
#include <Types.h>
#include <Values.h>
#include <Windows.h>
*/
#	define PASCALNAME
#	define PASCALTASKS
#endif

#define true	1
#define false	0

#ifdef PASCALNAME
MidiName ApplName = "\pSender";
MidiName OtherName = "\pReceiver";
MidiName DriverName = "\pMidiShare";
#endif

#ifdef CNAME
MidiName ApplName = "Sender";
MidiName OtherName = "Receiver";
MidiName DriverName = "MidiShare";
#endif

/* ========================= variables globales de l'application ==================*/
short r1= 0, r2= 0, version= 0;
long SizeMaxSysEx= 10000;				/* taille du plus grand sysex		*/
char *gOK = " OK";

#define SizeMedSysEx	40				/* test des sysex par pas de 1		*/

#ifndef typeSeqNum
# define typeSeqNum		134				/* d暫ut des 思始ements MidiFile 	*/
#endif

#define flush			fflush( stdout)
#define noEvts			fprintf( stdout, "\nno more MidiShare events !\n")
#define ok				fprintf( stdout, "%s\n", gOK)


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
/*148 */	"Specific",		"Reserved"
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
/*145 */		GetNoExt, 	GetNoExt, 	GetNoExt, 	GetNoExt, 	0,
/*150 */		0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
/*170 */		0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
/*190 */		0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
/*210 */		0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
/*230 */		0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
/*250 */		0,0,0,0,0, GetNothing
};



/*______________________________________________________________________________*/
static void wait( int d)
{
	long time;
	
	time= MidiGetTime()+ d;
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
			fprintf( stdout, "."); flush;
		t= MidiGetTime();
	}
}

/*______________________________________________________________________________*/
static OpenAppls()
{	
	if( (r1= MidiOpen( ApplName)) < 0)
	{
		fprintf( stdout, "Impossible to open a MidiShare application !\n");
		fprintf( stdout, "Interrupted test !\n");
		return false;
	}
	if( (r2= MidiOpen( OtherName)) < 0)
	{
		fprintf( stdout, "Impossible to open a MidiShare application !\n");
		fprintf( stdout, "Interrupted test !\n");
		MidiClose( r1);
		return false;
	}
	MidiConnect( r1, 0, true);
	MidiConnect( 0, r1, true);
	MidiConnect( r1, r2, true);
	return true;
}

/*______________________________________________________________________________*/
static CmpCommon( MidiEvPtr o, MidiEvPtr c, char *src)
{
	int ret= true;
	
	if( EvType(o) != EvType(c)) {
		fprintf( stdout, "\n%s : modified event type : %d -> %d", src,
					(int)EvType(o), (int)EvType(c));
		ret= false;
	}
	if( ( EvType(o) < typeSongPos) && (Chan(o) != Chan(c))) {
		fprintf( stdout, "\n%s : modified event channel : %d -> %d", src,
					(int)Chan(o), (int)Chan(c));
		ret= false;
	}
	if( Port(o) != Port(c)) {
		fprintf( stdout, "\n%s : modified event port : %d -> %d", src,
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
		fprintf( stdout, "\n%s : different number of fields : %ld -> %ld", src,f1,f2);
		return false;
	}
	else if( !CmpCommon( o, c, src))
		return false;
	for( i=0; i<f1; i++)
	{
		if( MidiGetField( o, i)!= MidiGetField( c, i))
		{
			fprintf( stdout, "\n%s : modified value at index %ld", src, i);
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
	else fprintf( stdout, "\n%s : MidiAvailEv return nil ", src);
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
			fprintf( stdout, "wrong type : %d \n", EvType(get));
			return false;
		}
		f1= MidiCountFields( e)-1;
		f2= MidiCountFields( get)+1;
		if( f1!= f2)
		{
			fprintf( stdout, "\n%s : different number of fields : %ld -> %ld", src,f1,f2);
			return false;
		}
		else if( !CmpCommon( e, get, src))
			return false;
		for( i=1; i<f1; i++)
		{
			if( MidiGetField( e, i)!= MidiGetField( get, i-1))
			{
				fprintf( stdout, "\n%s : modified value at index %ld", src, i);
				return false;
			}
		}
		return true;
	}
	else fprintf( stdout, "\n%s : MidiAvailEv return nil ", src);
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
			fprintf( stdout, "reception of wrong controlers : %ld followed by %ld\n",v1,v2);
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
	else fprintf( stdout, "\n%s : MidiAvailEv return nil ", "ext");
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
			fprintf( stdout, "\nreception of wrong controlers : %ld %ld %ld %ld"
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
	else fprintf( stdout, "\n%s : MidiAvailEv return nil ", "ext");
	return ret;
}

/*______________________________________________________________________________*/
static GetEvents( MidiEvPtr e)
{
	long n;
	int ret= false;
	
	n= MidiCountEvs( r2);
	if(n != 1)
		fprintf( stdout, "\nWarning : wrong fifo content : %ld ", n);
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
		fprintf( stdout, "\nWarning : wrong fifo content : %ld ", n);
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
		fprintf( stdout, "\nWarning : wrong fifo content : rcv=%ld ", n);
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
		fprintf( stdout, "\nWarning : wrong fifo content : rcv=%ld ", n);
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
		fprintf( stdout, "\nWarning : wrong fifo content : %ld ", n);
	else ret= true;
	if( n== 1)
		ret &= CompareEv( e, r1, "int");
	n= MidiCountEvs( r2);
	if( n)
		fprintf( stdout, "\nWarning : event distributed at rcv\n");
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
		fprintf( stdout, "\nWarning : wrong fifos content : appl=%ld rcv=%ld ", n1,n2);
	else ret= true;
	if( type== typeCtrl14b && n1== 2)
		ret &= CompareCtrl14b( e);
	else if( n1== 4)
		ret &= CompareRegParam( e);
	fprintf( stdout, " *");
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
		fprintf( stdout, "\next : wrong types : first=%d next=%d ", (int)EvType(e1), (int)EvType(e2));
		ret= false;
	}
	if( Vel(e2))
	{
		fprintf( stdout, "\next : note off : wrong velocity : %d ", (int)Vel(e2));
		ret= false;
	}
	dc= Date(e2) - Date(e1);
	if( dc!= d)
	{
		fprintf( stdout, "\next : wrong duration : %d instead of %d ", (int)dc, (int)d);
		ret= false;
	}
	if( Pitch(e1)!= Pitch(e2))
	{
		fprintf( stdout, "\next : wrong pitch : %d and %d ", (int)Pitch(e1), (int)Pitch(e2));
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
		fprintf( stdout, "\nWarning : wrong fifo content : %ld ", n);
	else ret = CompareEv( e, r2, "int");
	return ret;
}

/*______________________________________________________________________________*/
static void SetEvent( MidiEvPtr e, Boolean display)
{
	register long f, i, v=1;
	
	f= MidiCountFields(e);
	for(i= 0; i<f; i++)
	{
		MidiSetField( e, i, v);
		if( v<127) v++; else v=0;
	}
	if( display)
		fprintf( stdout, "set "); flush;
}

/*______________________________________________________________________________*/
static TestEvent( short i, Boolean display)
{
	MidiEvPtr e, copy;
	int ret= false;

	MidiFlushEvs( r1);
	MidiFlushEvs( r2);
	if( display)
		fprintf( stdout, "    type %s : ", typeListe[i]); flush;
	e= MidiNewEv(i);
	if( !e) noEvts;
	else{
		SetEvent( e, display);
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
				wait( 6);
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
static void ChanEvents()
{
	short i;
	
	fprintf( stdout, "\nChannel events :\n");
	/*											typeNote until typePitchWheel */
	for( i= typeNote; i<typeSongPos; i++)
		if( TestEvent( i, true)) ok;
		else fprintf( stdout, "\n");
}

/*______________________________________________________________________________*/
static void CommonEvents()
{
	short i;
	
	fprintf( stdout, "\nCommon events :\n");	
	/*											typeSongPos until typeReset */
	for( i= typeSongPos; i<typeSysEx; i++)
		if( TestEvent( i, true)) ok;
		else fprintf( stdout, "\n");
}

/*______________________________________________________________________________*/
static Boolean SystemeEx()
{
	register MidiEvPtr e, copy;
	register long v=0, n=0;
	
	fprintf( stdout, "\nSysteme exclusive :\n");
	
	/*											 			  typeSysEx */
	if( !TestEvent( typeSysEx, true)) {fprintf( stdout, "\n"); return false;}

	/*			  				test of event until n */
	if( !(e= MidiNewEv(typeSysEx))) { noEvts; return false;}
	for( n=0; n<SizeMedSysEx; n++)
	{
		MidiAddField( e, v++);
		if( !(copy= MidiCopyEv( e))) { noEvts; break;}
		else if( CmpEv( e, copy, "copy")){
			MidiSendIm( r1, copy); wait( 40);
			if( !GetEvents( e))
			{
				fprintf( stdout, "\n      loop %d length %ld\n", n+1, MidiCountFields(e));
				MidiFreeEv(e);
				return false;
			}
			MidiFlushEvs(r1);
			MidiFlushEvs(r2);
		}
	}
	fprintf( stdout, " 0-%ld ok", n); flush;
	/*			  				test of max for max length */
	for( ; n< SizeMaxSysEx; n++)
	{
		MidiAddField( e, v++);
		if( v>126) v=0; else v++;
	}
	fprintf( stdout, "\n      length %ld", MidiCountFields(e)); flush;
	if( !(copy= MidiCopyEv( e)))  noEvts;
	else 
	{
		fprintf( stdout, " copy"); flush;
		if( CmpEv( e, copy, "copy")){
			fprintf( stdout, " ok ");
			MidiSendIm( r1, copy); wait2( (SizeMaxSysEx*2)/3+ 10);
			if( !GetEvents( e))
			{
				MidiFreeEv(e);
				return false;
			}
		}
	}
	MidiFreeEv(e);
	MidiFlushEvs( r1);
	MidiFlushEvs( r2);
	ok;
	return true;
}

/*______________________________________________________________________________*/
static Boolean Stream()
{
	register MidiEvPtr e, copy;
	register long v=0, n=0;
	
	/*											 			  typeStream */
	if( !TestEvent( typeStream, true)) {fprintf( stdout, "\n"); return false;}

	/*			  				test of event until n */
	if( !(e= MidiNewEv(typeStream))) { noEvts; return false;}
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
				fprintf( stdout, "\n      loop %d length %ld\n", n+1, MidiCountFields(e));
				MidiFreeEv(e);
				return false;
			}
			MidiFlushEvs(r1);
			MidiFlushEvs(r2);
		}
	}
	fprintf( stdout, " 0-%ld ok", n); flush;
	/*			  				test of max for max length */
	MidiSetField( e, n-1, (long)nil);
	for( ; n< SizeMaxSysEx-1; n++)
	{
		MidiAddField( e, v++);
		if( v>126) v=0; else v++;
	}
	MidiAddField( e, (long)0xF7);
	fprintf( stdout, "\n      length %ld", MidiCountFields(e)); flush;
	if( !(copy= MidiCopyEv( e)))  noEvts;
	else 
	{
		fprintf( stdout, " copy"); flush;
		if( CmpEv( e, copy, "copy")){
			fprintf( stdout, " ok ");
			MidiSendIm( r1, copy); wait2( (SizeMaxSysEx*2)/3+ 10);
			if( !GetStream( e))
			{
				MidiFreeEv(e);
				return false;
			}
		}
	}
	MidiFreeEv(e);
	ok;
	return true;
}

/*______________________________________________________________________________*/
static Boolean  Private()
{
	short i;
	Boolean display= true;
	
	fprintf( stdout, "\nPrivate events :\n");	
	/*													typePrivate */
	for( i= typePrivate; i<typeProcess; i++)
	{
		if( !TestEvent( i, display))
		{
			fprintf( stdout, " type %d\n", (int)i);
			return false;
		}
		display= false;
	}
	ok;
	return true;
}

/*______________________________________________________________________________*/
static void Process()
{
	short i;
	
	fprintf( stdout, "\nProcess and DProcess :\n");	
	/*													typePrivate */
	for( i= typeProcess; i<typeQuarterFrame; i++)
	{
		if( TestEvent( i, true)) ok;
		else fprintf( stdout, "\n");
	}
}

/*______________________________________________________________________________*/
static void QFToMidiFile()
{
	short i;
	
	fprintf( stdout, "\nQuarterFrame until RegParam :\n");	
	/*													nQuarterFrame until RegParam */
	for( i= typeQuarterFrame; i<=typeRegParam; i++)
	{
		if( TestEvent( i, true)) ok;
		else fprintf( stdout, "\n");
	}
}

/*______________________________________________________________________________*/
static void MidiFile()
{
	short i;
	
	fprintf( stdout, "\nMidiFile events :\n");	
	if( version < 160)
		fprintf( stdout, "not implemented !\n");	

	/*												typeSeqNum until typeSpecific */
	else for( i= typeSeqNum; i<=typeSpecific; i++)
	{
		if( TestEvent( i, true)) ok;
		else fprintf( stdout, "\n");
	}
}

/*______________________________________________________________________________*/
static void Reserved()
{
	short ret= true;
	MidiEvPtr e;
	
	fprintf( stdout, "\nReserved events :\n");	
	/*												typeReserved until typeDead */
	fprintf( stdout, "    type %s : ", typeListe[typeReserved]); flush;
	if( e= MidiNewEv(typeReserved)) 
	{
		fprintf( stdout, "\ntype %d : allocation return %lx\n", (int)typeReserved, e);
		ret= false;
	}
	if( e= MidiNewEv( typeDead-1)) 
	{
		fprintf( stdout, "\ntype %d : allocation return %lx\n", (int)typeDead-1, e);
		ret= false;
	}
	if( ret) ok;
	fprintf( stdout, "    type Dead : "); flush;
	if( TestEvent( typeDead, false)) ok;
	else fprintf( stdout, "\n");
}

/*______________________________________________________________________________*/
main( int argc, char *argv[])
{
	fprintf( stdout, "\nAllocation, emission and reception of MidiShare events.\n");
	fprintf( stdout, "==========================================================\n");

	if( MidiShare())
	{
		version= MidiGetVersion();
		fprintf( stdout, "                MidiShare version %d.%d\n", (int)version/100, (int)version%100);
		fprintf( stdout, "\nWarning : MidiShare must have at least 10000 events !\n");

		if( OpenAppls())
		{
			wait( 100);
			ChanEvents(); fflush( stdout);
			CommonEvents(); fflush( stdout);			
			SystemeEx(); fflush( stdout);
			MidiFlushEvs(r1); MidiFlushEvs(r2);
			Stream(); fflush( stdout);
			MidiFlushEvs(r1); MidiFlushEvs(r2);
			Private(); fflush( stdout);
			Process(); fflush( stdout);
			QFToMidiFile(); fflush( stdout);
			MidiFile(); fflush( stdout);
			Reserved(); fflush( stdout);

			MidiClose( r1);
			MidiClose( r2);
		}

	}
	else fprintf( stdout, "MidiShare is not installed !\n");
	fprintf( stdout, "\nEnd of allocation emission and reception test of events.\n");
	return 0;
}
