/*

  Copyright © Grame 1999

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
/*
 *
 * Validation and test of MidiShare
 * Test of MidiShare functions.
 *
 * GRAME	Lundi 7 DŽcembre 1992	DF
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

#if macintosh
#	include <stdio.h>
#	include "MidiShare.h"
# if defined(__ppc__) && defined(__GNUC__)
#	define __POWERPC__
#	define CNAME
#	define CTASKS
#	define nil 0
# else
#	define PASCALNAME
#	define PASCALTASKS
#	define unused1
#	define unused2
#	define unused3
#	define unused4
# endif
#	define TestMidiCall
#	define flush        fflush(stdout)
#	define print        printf
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
#	define flush           fflush(stdout)
#	define print(args...)	fprintf(stdout, ## args)
#endif
#	define CNAME
#	define CTASKS
#	define nil 0
#endif

#include "TFonctions.h"

#define true	1
#define false	0

char *OK = "OK", *Erreur= "ERREUR";
short refNum= nil;

#ifdef PASCALNAME
MidiName ApplName   = "\pFonctions";
MidiName NewName    = "\pNewName";
MidiName MidiShareName = "\pMidiShare";
MidiName TestName   = "\pTest";
#endif

#ifdef CNAME
MidiName ApplName   = "Fonctions";
MidiName NewName    = "NewName";
MidiName MidiShareName = "MidiShare";
MidiName TestName   = "Test";
#endif


/*____________________________________________________________________*/
#ifdef PASCALTASKS
static pascal void RcvAlarm( short unused1)
#endif
#ifdef CTASKS
void MSALARMAPI RcvAlarm( short unused1)
#endif
{
}


/*____________________________________________________________________*/
#ifdef PASCALTASKS
static pascal void ApplAlarm( short unused1, long unused2)
#endif
#ifdef CTASKS
void MSALARMAPI ApplAlarm( short unused1, long unused2)
#endif
{
}

/*____________________________________________________________________*/
short Environment()
{
	int version, count; short n;
	
	print ("\nGlobal MidiShare environment :\n");
	print ("    MidiGetVersion : ");flush;
	version= MidiGetVersion();
	print ("%d.%d\n", version/100, version%100);

	print ("    MidiCountAppls : ");flush;
	count= MidiCountAppls();
	print ("%d  %s\n", count, OK);

	print ("    MidiOpen : ");flush;
	if( refNum= MidiOpen( ApplName))
	{
		print ("%d %s\n", (int)refNum, OK);
		
		count+= count ? 1 : 2;
		n= MidiCountAppls();
		if( n!= count)
		{
			print ("    MidiCountAppls : %d\n");flush;
			print ("Warning : inconsistant number of opened applications !\n", n);
		}

		print ("    MidiGetIndAppl : ");flush;
		n= MidiGetIndAppl(n);
		print ("%d  %s\n", n, OK);
		if( n!= refNum)
			print ("Warning : wrong index : %d return %d\n", count, n);
		n= MidiGetIndAppl( 1);
		if( n!= 0)
			print ("Warning : wrong index : 1 return %d\n", n);
		
		print ("    MidiGetNamedAppl : ");flush;
		n= MidiGetNamedAppl( ApplName);
		print ("%d  %s\n", n, OK);
		if( n!= refNum)
			print ("Warning : wrong refNum\n");
		n= MidiGetNamedAppl( MidiShareName);
		if( n!= 0)
			print ("Warning : wrong refNum for %s : %d\n", MidiShareName, n);
	}
	else
	{
		print ("MidiOpen failed\n");
		return 0;
	}
	return refNum;
}

/*____________________________________________________________________*/
void OpenClose()
{
	int i, n;
	short r[256];
	
	print ("\nMidiOpen and MidiClose :\n");flush;
	for( i=0; i<256; i++)
	{
		if( (r[i]= MidiOpen( TestName)) < 0)
			break;
	}
	print ("    max number of opened applications : %d\n", i+2);
	n= MidiCountAppls();
	print ("    number returned by MidiShare : %d\n", n);
	for( ; i>=0; i--)
		MidiClose( r[i]);
	n= MidiCountAppls();
	if( n!= 2)
		print ("Warning : not all appplications are closed : remain %d\n", n);
}

/*____________________________________________________________________*/
static int pascalcmp( unsigned char *s1, unsigned char *s2)
{
	int i1, i2;
	
	i1= *s1++;
	i2= *s2++;
	if( i1== i2)
	{
		while( i1--)
		{
			if( *s1++ != *s2++)
				return 1;
		}
	}
	else return 1;
	return 0;
}

/*____________________________________________________________________*/
static int equal(char *s1, char *s2)
{
	while (*s1) {
		if (*s1 != *s2) return 1;
		s1++; s2++;
	}
	if (*s1 != *s2) return 1;
	return 0;
}

/*____________________________________________________________________*/
void ApplConfiguration()
{
	MidiName s;
	void* info;
	MidiFilterPtr f;
	RcvAlarmPtr rcv;
	ApplAlarmPtr appl;
	
	print ("\nApplication configuration :\n");
	
	print ("    MidiGetName : ");flush;
	if( s= MidiGetName( refNum))
	{
#ifdef CNAME
		print ("%s %s\n", s, OK);
		if( equal( s, ApplName))
			print ("Warning : incoherent name !\n");
#endif
#ifdef PASCALNAME
		s[s[0]+1]=0;
		print ("%s %s\n", &s[1], OK);
		if( pascalcmp( s, ApplName))
			print ("Warning : incoherent name !\n");
#endif
	}
	else print ("nil  %s\n", Erreur);
	
	print ("    MidiSetName : ");flush;
	MidiSetName( refNum, NewName);
	print ("%s : ", OK);
	if( s= MidiGetName( refNum))
	{
#ifdef CNAME
		print ("%s\n", s);
		if( equal( s, NewName))
			print ("Warning : incoherent name !\n");
#endif
#ifdef PASCALNAME
		s[s[0]+1]=0;
		print ("%s\n", &s[1]);
		if( pascalcmp( s, NewName))
			print ("Warning : incoherent name !\n");
#endif
	}
	else print ("MidiGetName nil  %s\n", Erreur);
	MidiSetName( refNum, ApplName);
	
	print ("    MidiGetInfo : ");flush;
	info= MidiGetInfo( refNum);
	print ("%lx %s\n", info, OK);
	if( info)
		print ("Warning : info zone not set and not null\n");
	
	print ("    MidiSetInfo : ");flush;
	MidiSetInfo( refNum, (void* )s);
	print ("%s\n", OK);
	if( (info= MidiGetInfo( refNum))!= s)
		print ("Warning : incorrect info zone  : contains %lx instead of %lx\n"
						, info, s);
	MidiSetInfo( refNum, (void* )nil);
	if( info= MidiGetInfo( refNum))
		print ("Warning : incorrect info zone : contains %lx instead of nil\n"
						, info);
	
	print ("    MidiGetFilter : ");flush;
	f= MidiGetFilter( refNum);
	print ("%lx %s\n", f, OK);
	if( f)
		print ("Warning : filter not set and not null\n");
	
	print ("    MidiSetFilter : ");flush;
	MidiSetFilter( refNum, (MidiFilterPtr)s);
	print ("%s\n", OK);
	if( (f= MidiGetFilter( refNum))!= (MidiFilterPtr)s)
		print ("Warning : incorrect filter : %lx instead of %lx\n", f, s);
	MidiSetFilter( refNum, (MidiFilterPtr)nil);
	if( f= MidiGetFilter( refNum))
		print ("Warning : incorrect filter : %lx instead of nil\n", f);
	
	print ("    MidiGetRcvAlarm : ");flush;
	rcv= MidiGetRcvAlarm( refNum);
	print ("%lx %s\n", rcv, OK);
	if( rcv)
		print ("Warning : alarm not set and not null \n");
	
	print ("    MidiSetRcvAlarm : ");flush;
	MidiSetRcvAlarm( refNum, RcvAlarm);
	print ("%s\n", OK);
	if( (rcv= MidiGetRcvAlarm( refNum))!= RcvAlarm)
		print ("Warning : incorrect alarm : %lx  instead of %lx\n", rcv, RcvAlarm);
	MidiSetRcvAlarm( refNum, (RcvAlarmPtr)nil);
	if( rcv= MidiGetRcvAlarm( refNum))
		print ("Warning : incorrect alarm : %lx  instead of nil\n", rcv);
	
	print ("    MidiGetApplAlarm : ");flush;
	appl= MidiGetApplAlarm( refNum);
	print ("%lx %s\n", appl, OK);
	if( appl)
		print ("Warning : alarm not set and not null \n");
	
	print ("    MidiSetApplAlarm : ");flush;
	MidiSetApplAlarm( refNum, ApplAlarm);
	print ("%s\n", OK);
	if( (appl= MidiGetApplAlarm( refNum))!= ApplAlarm)
		print ("Warning : incorrect alarm : %lx instead of %lx\n", appl, ApplAlarm);
	MidiSetApplAlarm( refNum, (ApplAlarmPtr)nil);
	if( appl= MidiGetApplAlarm( refNum))
		print ("Warning : incorrect alarm : %lx instead of nil\n", appl);
}

/*____________________________________________________________________*/
void Connections( short isFreeMem)
{
	Boolean in, out;
	
	print ("\nConnections :\n");
	if( MidiIsConnected( 0, refNum) || MidiIsConnected( refNum, 0))
		print ("Warning : application already connected !");
	
	print ("    MidiConnect : ");flush;
	MidiConnect( 0, refNum, true);
	MidiConnect( refNum, 0, true);
	print ("%s\n", OK);
	
	print ("    MidiIsConnected : ");flush;
	in= MidiIsConnected( 0, refNum);
	out= MidiIsConnected( refNum, 0);
	print ("%s\n", OK);
	if( !in || !out)
		if( isFreeMem)
			print ("Warning : connections not made : in %d, out %d\n",
					(int)in, (int)out);

	MidiConnect( 0, refNum, false);
	MidiConnect( refNum, 0, false);
	in= MidiIsConnected( 0, refNum);
	out= MidiIsConnected( refNum, 0);
	if( in || out)
		print ("Warning : connections not broken : in %d, out %d\n",
					(int)in, (int)out);
}

/*____________________________________________________________________*/
void Events( short isFreeMem)
{
	MidiEvPtr e, copy;
	long l, total, free, count;
	
	print ("\nEvent management :\n");
	
	print ("    MidiTotalSpace : ");flush;
	total= MidiTotalSpace();
	print ("%ld %s\n", total, OK);
	
	print ("    MidiFreeSpace : ");flush;
	free= MidiFreeSpace();
	print ("%ld %s\n", free, OK);
	
	if( isFreeMem)
	{
		print ("    MidiGrowSpace : ");flush;
		l= MidiGrowSpace( 500L);
		print ("allocated : %ld %s\n", l, OK);
		total+= l;
		l= MidiTotalSpace();
		if( (l!= total) && MidiCountAppls())
			print ("Warning : incoherent TotalSpace : %ld FreeSpace : %ld\n"
						, l, MidiFreeSpace()); 
		total= l;
	}
	
	free= MidiFreeSpace();
	print ("    MidiNewCell : ");flush;
	e= MidiNewCell();
	print ("%lx %s\n", e, OK);
	if( e && !isFreeMem)
		print ("Warning : incoherent result : %lx\n", e); 
	if( (l= MidiFreeSpace())!= free-1)
		if( isFreeMem)
			print ("Warning : incoherent FreeSpace : %ld\n", l); 
	
	if( isFreeMem)
	{
		print ("    MidiFreeCell : ");flush;
		MidiFreeCell( e);
		print ("%s\n", OK);
		if( (l= MidiFreeSpace())!= free)
			print ("Warning : incoherent FreeSpace : %ld\n", l); 
	}
	
	print ("    MidiNewEv : ");flush;
	e= MidiNewEv( typeNote);
	print ("%lx %s\n", e, OK);
	if( e && !isFreeMem)
		print ("Warning :incoherent result : %lx\n", e); 
	if( e && isFreeMem)
	{
		Date(e)= nil;
		Pitch(e)= 80;
		Chan(e)= 10;
		Vel(e)= 120;
		Dur(e)= 1000;
	
		print ("    MidiCopyEv : ");flush;
		copy= MidiCopyEv(e);
		print ("%lx %s\n", copy, OK);
		if( copy)
		{
			if( Date(copy) || (Pitch(copy)!=80) || (Chan(copy)!=10)
				|| (Vel(copy)!=120) || (Dur(copy)!=1000))
				print ("Warning : wrong copy\n");
		}
		print ("    MidiFreeEv : ");
		MidiFreeEv(e);
		print ("%s\n", OK);
		if( copy) MidiFreeEv(copy);
	}
	else if( isFreeMem)
	{
		print ("Warning : no more MidiShare events \n");
		print ("    MidiCopyEv : not tested \n");
		print ("    MidiFreeEv : not tested \n");
	}
	
	if( !isFreeMem) return ;

	e= MidiNewEv( typeSysEx);
	if( e)
	{
		print ("    MidiCountFields : ");flush;
		count= MidiCountFields(e);
		print ("%ld %s\n", count, OK);
		if( count)
			print ("Warning : wrong number of fields !\n");
		
		print ("    MidiAddField : ");flush;
		MidiAddField( e, 1L);
		print ("%s\n", OK);
		l= MidiCountFields(e);
		if( ++count!= l)
			print ("Warning : wrong number of fields !\n");
		
		print ("    MidiGetField : ");flush;
		l= MidiGetField( e, 0L);
		print ("%ld %s\n", l, OK);
		
		print ("    MidiSetField : ");flush;
		MidiSetField( e, 0L, l+1);
		print ("%s\n", OK);
		if( (l+1)!= MidiGetField( e, 0L))
			print ("Warning : field not set correctly !\n");
	}
	else
	{
		print ("    MidiSetField : not tested \n");
		print ("    MidiGetField : not tested \n");
		print ("    MidiAddField : not tested \n");
		print ("    MidiCountFields : not tested \n");
	}
}

/*____________________________________________________________________*/
static long CountOfEvents( MidiSeqPtr s)
{
	long count= 0;
	MidiEvPtr e;
	
	e= First( s);
	while( e)
	{
		count++;
		e= Link(e);
	}
	return count;
}

/*____________________________________________________________________*/
#ifdef PASCALTASKS
static pascal void ApplyProc( MidiEvPtr e )
#endif
#ifdef CTASKS
void MSALARMAPI ApplyProc( MidiEvPtr e )
#endif
{
	if( EvType(e)== typeNote)
		Pitch(e)++;
}

/*____________________________________________________________________*/
void Sequences( short isFreeMem)
{
	MidiSeqPtr seq;
	MidiEvPtr e;
	long n, free;

	print ("\nSequence management :\n");
		
	free= MidiFreeSpace();
	print ("    MidiNewSeq : ");flush;
	seq= MidiNewSeq();
	print ("%lx %s\n", seq, OK);
	if( seq && !isFreeMem)
		print ("Warning : sequence not null !\n");
	
	if( !isFreeMem) return;
	if( !seq)
	{
		print ("Warning : null sequence !\n");
		print ("    MidiAddSeq : not tested\n");
		print ("    MidiFreeSeq : not tested\n");
		print ("    MidiClearSeq : not tested\n");
		print ("    MidiApplySeq : not tested\n");
		return;
	}
		
	if( e= MidiNewEv( typeNote))
	{
		Pitch(e)= 60;
		
		print ("    MidiAddSeq : ");flush;
		MidiAddSeq( seq, e);
		print ("%s\n", OK);
		if( (n=CountOfEvents( seq))!= 1)
			print ("Warning : incoherent event number : %ld\n", n);
		
		print ("    MidiApplySeq : ");flush;
		MidiApplySeq( seq, ApplyProc);
		print ("%s\n", OK);
		if( Pitch(e)!= 61)
			print ("Warning : apply not made !\n");
	
		print ("    MidiClearSeq : ");flush;
		MidiClearSeq( seq);
		print ("%s\n", OK);
		if( n=CountOfEvents( seq))
			print ("Warning : incoherent event number : %ld\n", n);
	}
	else print ("Warning : no more MidiShare events !\n");
		
	if( e= MidiNewEv( typeNote))
	{
		MidiAddSeq( seq, e);
		print ("    MidiFreeSeq : ");flush;
		MidiFreeSeq( seq);
		print ("%s\n", OK);
		if( (n= MidiFreeSpace())!= free)
			print ("Warning : incoherent freeSpace : %ld\n", n);
		seq= nil;
	}
	else print ("Warning : no more MidiShare events !\n");
	if( seq)
	{	
		print ("    MidiFreeSeq : ");flush;
		MidiFreeSeq( seq);
		print ("%s\n", OK);
		if( (n= MidiFreeSpace())!= free)
			print ("Warning : incoherent freeSpace : %ld\n", n);
	}
}

/*____________________________________________________________________*/
void Time()
{
	print ("\nTime management :\n");
	print ("    MidiGetTime : ");flush;
	print ("%ld %s\n", MidiGetTime(), OK);
}

/*____________________________________________________________________*/
void Sending()
{
	MidiEvPtr e, copy, get;
	short r;
	unsigned long free, time, n, count;

	print ("\nSending and receiving events :\n");
	free= MidiFreeSpace();
	if( (r= MidiOpen( NewName)) < 0)
	{
		print ("Warning : impossible to open a new application !\n");
		print ("          tests not made.\n");
		return;
	}
//	MidiConnect( refNum, 0, true);
	MidiConnect( refNum, r, true);
	if( e= MidiNewEv( typeKeyOn))
	{

		Pitch(e)= 60;
		Vel(e)= 120;

		if( copy= MidiCopyEv( e))
		{
			print ("    MidiSendIm : ");flush;
			MidiSendIm( refNum, copy);
			time= MidiGetTime();
			print ("%s\n", OK);
			while( time == MidiGetTime());
			
			print ("    MidiCountEvs : ");flush;
			n= MidiCountEvs(r);
			print ("%ld %s\n", n, OK);
			if( n!= 1)
				print ("Warning : incoherent event number !\n");
			
			print ("    MidiGetEv : ");flush;
			get= MidiGetEv(r);
			print ("%lx %s\n", get, OK);
			n= MidiCountEvs(r);
			if( n)
				print ("Warning : incoherent event number : %ld\n", n);
			if( get)
				MidiFreeEv( get);
		}
		else print ("Warning : no more MidiShare events !\n");

		if( copy= MidiCopyEv( e))
		{
			print ("    MidiSend : ");flush;
			Date(copy)= time= MidiGetTime()+ 4;
			MidiSend( refNum, copy);
			while( MidiGetTime()< time);
			print ("%s\n", OK);
			n= MidiCountEvs(r);
			if( n!= 1)
				print ("Warning : incoherent number of transmited events : %ld\n", n);
			
			print ("    MidiAvailEv : ");flush;
			get= MidiAvailEv(r);
			print ("%lx %s\n", get, OK);
			if( (count= MidiCountEvs(r))!= n)
				print ("Warning : incoherent event number : %ld\n", count);
		}
		else print ("Warning : no more MidiShare events !\n");

		Date(e)= nil;
		print ("    MidiSendAt : ");flush;
		MidiSendAt( refNum, e, time= MidiGetTime()+ 4);
		print ("%s\n", OK);
		while( MidiGetTime()< time);
		n= MidiCountEvs(r);
		if( n!= 2)
			print ("Warning : incoherent number of transmited events : %ld\n", n);
			
		print ("    MidiFlushEvs : ");flush;
		MidiFlushEvs(r);
		print ("%s\n", OK);
		if( n= MidiCountEvs(r))
			print ("Warning : incoherent event number : %ld\n", n);
	}
	else print ("Warning : plus d'evt disponible !\n");
	MidiConnect( refNum, 0, false);
	MidiClose(r);
	MidiFlushEvs( refNum);
	if( (n= MidiFreeSpace())!= free)
		print ("Warning : incoherent FreeSpace : %ld (lost: %ld)\n", n, free-n);
}

/*____________________________________________________________________*/
void Mail()
{
	long a= 1; void* b;
	
	print ("\nMail boxes :\n");
	print ("    MidiReadSync : ");flush;
	b= MidiReadSync(&a);
	print ("%ld %s\n", b, OK);
	if( a)
		print ("Warning : non null value : %ld\n", a);
	
	a= nil;
	print ("    MidiWriteSync : ");flush;
	b= MidiWriteSync( &a, (void* )2);
	print ("%ld %s\n", b, OK);

	a=1;
	b= MidiWriteSync( &a, (void* )2);
	if( a!= 2)
		print ("Warning : not null variable but correctly set : %ld\n", a);
	if( b!= (void*)1)
		print ("Warning : incorrect return : %ld\n", b);
}


/*____________________________________________________________________*/
typedef struct ApplContext {
	MidiEvPtr t1;
	MidiEvPtr t2;
	Boolean res1;
	Boolean res2;
}ApplContext;

ApplContext gContext;

/*____________________________________________________________________*/
#ifdef PASCALTASKS
static pascal void MyTask( long unused1, short unused2, long a1,long unused3,long unused4)
#endif
#ifdef CTASKS
void MSALARMAPI MyTask( long unused1, short unused2, long a1,long unused3,long unused4)
#endif
{
	if( a1)
		*(long *)a1= 1;
}

/*____________________________________________________________________*/
#ifdef PASCALTASKS
static pascal void MyTask2( long unused1, short unused2, long a1,long unused3,long unused4)
#endif
#ifdef CTASKS
void MSALARMAPI MyTask2( long unused1, short unused2, long a1,long unused3,long unused4)
#endif
{
	gContext.res1 = true;
	MidiForgetTask(&gContext.t1);
	MidiForgetTask(&gContext.t2);
}

/*____________________________________________________________________*/
#ifdef PASCALTASKS
static pascal void MyTask3( long unused1, short unused2, long a1,long a2,long unused4)
#endif
#ifdef CTASKS
void MSALARMAPI MyTask3( long unused1, short unused2, long a1,long a2,long unused4)
#endif
{
	gContext.res2 = true;
}

/*____________________________________________________________________*/
#ifdef PASCALTASKS
static pascal void MyTask4( long time, short refnum, long a1,long a2,long a3)
#endif
#ifdef CTASKS
void MSALARMAPI MyTask4( long time, short refnum, long a1,long a2,long a3)
#endif
{
	gContext.res1 = true;
	gContext.t2 = MidiTask( MyTask3, time, refnum,a1, a2, a3);
	MidiForgetTask(&gContext.t1);
}

/*____________________________________________________________________*/
#ifdef PASCALTASKS
static pascal void MyDTask( long unused1, short unused2, long a1,long a2,long a3)
#endif
#ifdef CTASKS
void MSALARMAPI MyDTask( long unused1, short unused2, long a1,long a2,long a3)
#endif
{
	print ("dtask rcv %ld %ld %ld ", a1, a2, a3);
}

/*____________________________________________________________________*/
void Tasks( short isFreeMem)
{
	MidiEvPtr e;
	unsigned long time, p1=0, p2=0, p3=0;
	
	print ("\nTask Managing :\n");

#ifdef TestMidiCall			
	print ("    MidiCall : ");flush;
	MidiCall( MyTask, time= MidiGetTime(), refNum, (long)&p1, (long)&p2, (long)&p3);
	time+= 4;
	while( MidiGetTime() <= time);
	print ("%s\n", OK);
	if( (p1!= 1) && isFreeMem)
		print ("Warning : task not completed !\n");
#endif			
	print ("    MidiTask : ");flush;
	p1= 0;
	e= MidiTask( MyTask, time= MidiGetTime(), refNum, (long)&p1, (long)&p2, (long)&p3);
	time+= 50;
	while( MidiGetTime() <= time);
	print ("%lx %s\n", e, OK);
	if( (p1!= 1) && isFreeMem)
		print ("Warning : task not completed !\n");
		
	print ("    MidiForgetTask(1) : ");flush;	
	p1= 0;
	e= MidiTask( MyTask, time= MidiGetTime()+100, refNum, (long)&p1, (long)&p2, (long)&p3);
	MidiForgetTask(&e);
	print ("%s\n",OK);
	if(e) print ("Warning : MidiForgetTask does not set task address to 0 !\n");
	time+= 50;
	while( MidiGetTime() <= time);
	if( (p1!= 0) && isFreeMem)
		print ("Warning : MidiForgetTask does not forget the task!\n");
	
	
	/* test MidiForgetTask inside a task execution */
	print ("    MidiForgetTask(2) : ");flush;	
	gContext.res1 = gContext.res2 = false;
	gContext.t1 = gContext.t2 = 0;
	time = MidiGetTime()+10 ;
	gContext.t1= MidiTask( MyTask2, time, refNum, (long)&p1, (long)&p2, (long)&p3);
	gContext.t2= MidiTask( MyTask3, time, refNum, (long)&p1, (long)&p2, (long)&p3);
	print ("%s\n",OK);
	time+= 14;
	while( MidiGetTime() <= time);
	if( !gContext.res1 && isFreeMem)
		print ("Warning : task1 not completed !\n");
	if( gContext.res2 && isFreeMem)
		print ("Warning : task2 not forgeted !\n");
	if( gContext.t1 && isFreeMem)
		print ("Warning : task1 address not set to 0 !\n");
	if( gContext.t2 && isFreeMem)
		print ("Warning : task2 address not set to 0 !\n");
	
	/* test MidiForgetTask inside a task execution with late tasks */
	print ("    MidiForgetTask(3) : ");flush;	
	gContext.res1 = gContext.res2 = false;
	gContext.t1 = gContext.t2 = 0;
	time = MidiGetTime()-10 ;
	
	gContext.t1= MidiTask( MyTask2, time, refNum, (long)&p1, (long)&p2, (long)&p3);
	gContext.t2= MidiTask( MyTask3, time, refNum, (long)&p1, (long)&p2, (long)&p3);
	print ("%s\n",OK);
	
	time= MidiGetTime()+50;
	while( MidiGetTime() <= time);
	
	if( !gContext.res1 && isFreeMem)
		print ("Warning : task1 not completed !\n");
	if( gContext.res2 && isFreeMem)
		print ("Warning : task2 not forgeted !\n");
	if( gContext.t1 && isFreeMem)
		print ("Warning : task1 address not set to 0 !\n");
	if( gContext.t2 && isFreeMem)
		print ("Warning : task2 address not set to 0 !\n");
		
	/* test MidiForgetTask inside a task execution 
	/* Warning : using MidiForgetTask in the execution of the task may produce unspecified behavior.
	/* The result of this test may change on different implementations */
	
	print ("    MidiForgetTask(4) : ");flush;	
	gContext.res1 = gContext.res2 = false;
	gContext.t1 = gContext.t2 = 0;
	time = MidiGetTime() ;
	
	gContext.t1= MidiTask( MyTask4, time, refNum, (long)&p1, (long)&p2, (long)&p3);
	print ("%s\n",OK);
	
	time= MidiGetTime()+50;
	while( MidiGetTime() <= time);
	
	if( !gContext.res1 && isFreeMem)
		print ("Warning : task1 not completed !\n");
	if( !gContext.res2 && isFreeMem)
		print ("Warning : task2 not completed !\n");
	if( gContext.t1 && isFreeMem)
		print ("Warning : task1 address not set to 0 !\n");
	
			
	print ("    MidiDTask : ");flush;
	e= MidiDTask( MyDTask, time= MidiGetTime(), refNum, 1L, 2L, 3L);
	time+= 4;
	while( MidiGetTime() <= time);
	print ("%lx %s\n", e, OK);
	
	print ("    MidiCountDTasks : ");flush;
	p1= MidiCountDTasks( refNum);
	print ("%ld %s\n", p1, OK);
	if( (p1!= 1) && isFreeMem)
		print ("Warning : task not distributed !\n");

	if( !isFreeMem)
		return;
	
	print ("    MidiExec1DTask : ");flush;
	MidiExec1DTask( refNum);
	print ("%s\n", OK);
	p1= MidiCountDTasks( refNum);
	if( p1)
		print ("Warning : incorrect task number : %ld\n", p1);

	e= MidiDTask( MyDTask, time= MidiGetTime(), refNum, 1L, 2L, 3L);
	time+= 50;
	while( MidiGetTime() <= time);
	p1= MidiCountDTasks( refNum);
	if( p1!= 1)
		print ("Warning : incorrect task number : %ld\n", p1);
	print ("    MidiFlushDTasks : ");flush;
	MidiFlushDTasks( refNum);
	print ("%s\n", OK);
	p1= MidiCountDTasks( refNum);
	if( p1)
		print ("Warning : incorrect task number : %ld\n", p1);

	time= MidiGetTime()+100;
	e= MidiDTask( MyDTask, time, refNum, 1L, 2L, 3L);
	print ("    MidiForgetTask : ");flush;
	MidiForgetTask( &e);
	time+= 4;
	while( MidiGetTime() <= time);
	print ("%s\n", OK);
	p1= MidiCountDTasks( refNum);
	if( p1)
		print ("Warning : incorrect task number : %ld\n", p1);
}


/*____________________________________________________________________*/
void Synchro()
{
	TSmpteLocation loc;
	long time, tmp;
	short version = MidiGetVersion();
	
	print ("\nSmpte Synchro :\n");
	if ((version < 165) || (version > 168))
	{
		print ("    not implemented.\n");
		return;
	}
	
	time= MidiGetTime();
	print ("    MidiTime2Smpte : ");flush;
	MidiTime2Smpte( time, 1, &loc);
	print ("%s\n", OK);
	print ("        format : %d\n", (int)loc.format);
	print ("        hours  : %d\n", (int)loc.hours);
	print ("        minutes: %d\n", (int)loc.minutes);
	print ("        seconds: %d\n", (int)loc.seconds);
	print ("        frames : %d\n", (int)loc.frames);
	print ("        fracs  : %d\n", (int)loc.fracs);

	print ("    MidiSmpte2Time : ");flush;
	tmp= MidiSmpte2Time( &loc);
	print ("%s\n", OK);
	if( tmp!= time)
		print ("Warning : incorrect conversions : %ld -> %ld\n", time, tmp);
}

/*____________________________________________________________________*/
void Tolerance()
{	
	print ("\nNull pointers tolerance :\n");
	if( MidiGetVersion() < 165)
	{
		print ("    non implemented.\n");
		return;
	}

	print ("    MidiFreeCell : ");flush;
	MidiFreeCell( (MidiEvPtr)nil);
	print ("%s\n", OK);

	print ("    MidiCopyEv : ");flush;
	MidiCopyEv( (MidiEvPtr)nil);
	print ("%s\n", OK);

	print ("    MidiFreeEv : ");flush;
	MidiFreeEv( (MidiEvPtr)nil);
	print ("%s\n", OK);

#if macintosh && !defined __POWERPC__
	print ("    OldMidiSetField : ");flush;
	OldMidiSetField( (MidiEvPtr)nil, 0, 0L);
	print ("%s\n", OK);

	print ("    OldMidiGetField : ");flush;
	OldMidiGetField( (MidiEvPtr)nil, 0);
	print ("%s\n", OK);

	print ("    OldMidiCountFields : ");flush;
	OldMidiCountFields( (MidiEvPtr)nil);
	print ("%s\n", OK);
#endif

	print ("    MidiSetField : ");flush;
	MidiSetField( (MidiEvPtr)nil, 0L, 0L);
	print ("%s\n", OK);

	print ("    MidiGetField : ");flush;
	MidiGetField( (MidiEvPtr)nil, 0L);
	print ("%s\n", OK);

	print ("    MidiAddField : ");flush;
	MidiAddField( (MidiEvPtr)nil, 0L);
	print ("%s\n", OK);

	print ("    MidiCountFields : ");flush;
	MidiCountFields( (MidiEvPtr)nil);
	print ("%s\n", OK);

	print ("    MidiAddSeq : ");flush;
	MidiAddSeq( (MidiSeqPtr)nil, (MidiEvPtr)nil);
	MidiAddSeq( (MidiSeqPtr)1, (MidiEvPtr)nil);
	print ("%s\n", OK);

	print ("    MidiFreeSeq : ");flush;
	MidiFreeSeq( (MidiSeqPtr)nil);
	print ("%s\n", OK);

	print ("    MidiClearSeq : ");flush;
	MidiClearSeq( (MidiSeqPtr)nil);
	print ("%s\n", OK);

	print ("    MidiApplySeq : ");flush;
	MidiApplySeq( (MidiSeqPtr)nil, (ApplyProcPtr)nil);
	MidiApplySeq( (MidiSeqPtr)1, (ApplyProcPtr)nil);
	print ("%s\n", OK);

	print ("    MidiSendIm : ");flush;
	MidiSendIm( nil, (MidiEvPtr)nil);
	print ("%s\n", OK);

	print ("    MidiSend : ");flush;
	MidiSend( nil, (MidiEvPtr)nil);
	print ("%s\n", OK);

	print ("    MidiSendAt : ");flush;
	MidiSendAt( nil, (MidiEvPtr)nil, 0L);
	print ("%s\n", OK);
}

/*____________________________________________________________________*/
static MidiSeqPtr GetAllFreeEvs()
{
	MidiSeqPtr s;
	MidiEvPtr e;
	
	s= MidiNewSeq();
	while( e= MidiNewEv( typeNote))
		MidiAddSeq( s, e);
	return s;
}

/*____________________________________________________________________*/
void NoMem()
{
	MidiSeqPtr seq;
	long free, l;
	
	print ("\n\nLow memory tolerance :\n");
	free= MidiFreeSpace();
	seq= GetAllFreeEvs();
	l = MidiFreeSpace();
	Connections( false); flush;
	Events( false); flush;
	Sequences( false); flush;
	Tasks(false); flush;
	MidiFreeSeq( seq);
	if( free!= (l=MidiFreeSpace()))
		print ("Warning : incoherent freeSpace : %ld instead of %ld\n", l, free);
}
/*____________________________________________________________________*/
void Close()
{
	MidiClose( refNum);
}

/*____________________________________________________________________*/
main()
{
	print ("\nMidiShare functions test.\n");
	print ("================================\n");
	
	if( MidiShare()) {
		if( !Environment()) return 1;
		flush;
		OpenClose(); flush;
		ApplConfiguration(); flush;
		Connections( true); flush;
		Events( true); flush;
		Sequences( true); flush;
		Time(); flush;
		Sending(); flush;
		Mail(); flush;
		Tasks(true); flush;
		Synchro(); flush;
		Tolerance(); flush;
		NoMem(); flush;
		MidiClose( refNum);
	}
	else print ("MidiShare is not installed !\n");
	print ("\nEnd of functions test.\n");
	return 0;
}
