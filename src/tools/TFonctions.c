/*

  Copyright � Grame 1999

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
 * Utilitaire de test et validation d'une version de MidiShare
 * Test des diff�rentes fonctions de MidiShare.
 *
 * GRAME	Lundi 7 D�cembre 1992	DF
 */

#include <stdio.h>
#include <String.h>

#ifdef PC
#	include <MidiShare.h>
#	define CNAME
#	define CTASKS
	typedef char Boolean;
#endif

#ifdef __Macintosh__
#	include "MidiShare.h"
#	define PASCALNAME
#	define PASCALTASKS
#	define unused1
#	define unused2
#	define unused3
#	define unused4
#endif

#define true	1
#define false	0
#define flush	fflush( stdout)

char *OK = "OK", *Erreur= "ERREUR";
short refNum= nil;

#ifdef PASCALNAME
MidiName ApplName 	= "\pFonctions";
MidiName NewName 	= "\pNewName";
MidiName DriverName = "\pMidiShare";
#endif

#ifdef CNAME
MidiName ApplName 	= "Fonctions";
MidiName NewName 	= "NewName";
MidiName DriverName = "MidiShare";
#endif


/*____________________________________________________________________*/
#ifdef PASCALTASKS
static pascal void RcvAlarm( short unused1)
#endif
#ifdef CTASKS
void RcvAlarm( short unused1)
#endif
{
}


/*____________________________________________________________________*/
#ifdef PASCALTASKS
static pascal void ApplAlarm( short unused1, long unused2)
#endif
#ifdef CTASKS
void ApplAlarm( short unused1, long unused2)
#endif
{
}

/*____________________________________________________________________*/
static Boolean Environment()
{
	int version, count, n;
	
	fprintf( stdout, "\nGlobal MidiShare environment :\n");
	fprintf( stdout, "    MidiGetVersion : ");flush;
	version= MidiGetVersion();
	fprintf( stdout, "%d.%d\n", version/100, version%100);

	fprintf( stdout, "    MidiCountAppls : ");flush;
	count= MidiCountAppls();
	fprintf( stdout, "%d  %s\n", count, OK);


	fprintf( stdout, "    MidiOpen : ");flush;
	if( refNum= MidiOpen( ApplName))
	{
		fprintf( stdout, "%d %s\n", (int)refNum, OK);
		
		count+= count ? 1 : 2;
		n= MidiCountAppls();
		if( n!= count)
		{
			fprintf( stdout, "    MidiCountAppls : %d\n");flush;
			fprintf( stdout, "Warning : nbre d'applications pr�sentes incoh�rent !\n", n);
		}

		fprintf( stdout, "    MidiGetIndAppl : ");flush;
		n= MidiGetIndAppl( count);
		fprintf( stdout, "%d  %s\n", n, OK);
		if( n!= refNum)
			fprintf( stdout, "Warning : mauvais index : %d renvoie %d\n", count, n);
		n= MidiGetIndAppl( 1);
		if( n!= 0)
			fprintf( stdout, "Warning : mauvais index : 1 renvoie %d\n", n);
		
		fprintf( stdout, "    MidiGetNamedAppl : ");flush;
		n= MidiGetNamedAppl( ApplName);
		fprintf( stdout, "%d  %s\n", n, OK);
		if( n!= refNum)
			fprintf( stdout, "Warning : mauvais refNum\n");
		n= MidiGetNamedAppl( DriverName);
		if( n!= 0)
			fprintf( stdout, "Warning : mauvais refNum pour %s : %d\n", DriverName, n);
	}
	else
	{
		fprintf( stdout, "MidiOpen failed\n");
		return false;
	}
	return true;
}

/*____________________________________________________________________*/
static void OpenClose()
{
	int i, n;
	short r[256];
	
	fprintf( stdout, "\nMidiOpen et MidiClose :\n");flush;
	for( i=0; i<256; i++)
	{
		if( (r[i]= MidiOpen( "\ptest")) < 0)
			break;
	}
	fprintf( stdout, "    nombre max. d'applications ouvertes : %d\n", i+2);
	n= MidiCountAppls();
	fprintf( stdout, "    nombre renvoy� par MidiShare : %d\n", n);
	for( ; i>=0; i--)
		MidiClose( r[i]);
	n= MidiCountAppls();
	if( n!= 2)
		fprintf( stdout, "Warning : toutes les appls ne sont referm�es : reste %d\n", n);
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
static void ApplConfiguration()
{
	MidiName s;
	void* info;
	MidiFilterPtr f;
	RcvAlarmPtr rcv;
	ApplAlarmPtr appl;
	
	fprintf( stdout, "\nApplication configuration :\n");
	
	fprintf( stdout, "    MidiGetName : ");flush;
	if( s= MidiGetName( refNum))
	{
#ifdef CNAME
		fprintf( stdout, "%s %s\n", s, OK);
		if( strcmp( s, ApplName))
			fprintf( stdout, "Warning : nom incoh�rent !\n");
#endif
#ifdef PASCALNAME
		s[s[0]+1]=0;
		fprintf( stdout, "%s %s\n", &s[1], OK);
		if( pascalcmp( s, ApplName))
			fprintf( stdout, "Warning : nom incoh�rent !\n");
#endif
	}
	else fprintf( stdout, "nil  %s\n", Erreur);
	
	fprintf( stdout, "    MidiSetName : ");flush;
	MidiSetName( refNum, NewName);
	fprintf( stdout, "%s : ", OK);
	if( s= MidiGetName( refNum))
	{
#ifdef CNAME
		fprintf( stdout, "%s\n", s);
		if( strcmp( s, NewName))
			fprintf( stdout, "Warning : nom incoh�rent !\n");
#endif
#ifdef PASCALNAME
		s[s[0]+1]=0;
		fprintf( stdout, "%s\n", &s[1]);
		if( pascalcmp( s, NewName))
			fprintf( stdout, "Warning : nom incoh�rent !\n");
#endif
	}
	else fprintf( stdout, "MidiGetName nil  %s\n", Erreur);
	MidiSetName( refNum, ApplName);
	
	fprintf( stdout, "    MidiGetInfo : ");flush;
	info= MidiGetInfo( refNum);
	fprintf( stdout, "%lx %s\n", info, OK);
	if( info)
		fprintf( stdout, "Warning : zone info non positionn�e et non nulle\n");
	
	fprintf( stdout, "    MidiSetInfo : ");flush;
	MidiSetInfo( refNum, (void* )s);
	fprintf( stdout, "%s\n", OK);
	if( (info= MidiGetInfo( refNum))!= s)
		fprintf( stdout, "Warning : zone info incorrecte : contient %lx au lieu de %lx\n"
						, info, s);
	MidiSetInfo( refNum, (void* )nil);
	if( info= MidiGetInfo( refNum))
		fprintf( stdout, "Warning : zone info incorrecte : contient %lx au lieu de nil\n"
						, info);
	
	fprintf( stdout, "    MidiGetFilter : ");flush;
	f= MidiGetFilter( refNum);
	fprintf( stdout, "%lx %s\n", f, OK);
	if( f)
		fprintf( stdout, "Warning : filtre non positionn� et non nul\n");
	
	fprintf( stdout, "    MidiSetFilter : ");flush;
	MidiSetFilter( refNum, (MidiFilterPtr)s);
	fprintf( stdout, "%s\n", OK);
	if( (f= MidiGetFilter( refNum))!= (MidiFilterPtr)s)
		fprintf( stdout, "Warning : filtre incorrect : %lx au lieu de %lx\n", f, s);
	MidiSetFilter( refNum, (MidiFilterPtr)nil);
	if( f= MidiGetFilter( refNum))
		fprintf( stdout, "Warning : filtre incorrect : %lx au lieu de nil\n", f);
	
	fprintf( stdout, "    MidiGetRcvAlarm : ");flush;
	rcv= MidiGetRcvAlarm( refNum);
	fprintf( stdout, "%lx %s\n", rcv, OK);
	if( rcv)
		fprintf( stdout, "Warning : alarme non positionn�e et non nulle\n");
	
	fprintf( stdout, "    MidiSetRcvAlarm : ");flush;
	MidiSetRcvAlarm( refNum, RcvAlarm);
	fprintf( stdout, "%s\n", OK);
	if( (rcv= MidiGetRcvAlarm( refNum))!= RcvAlarm)
		fprintf( stdout, "Warning : alarme incorrecte : %lx au lieu de %lx\n", rcv, RcvAlarm);
	MidiSetRcvAlarm( refNum, (RcvAlarmPtr)nil);
	if( rcv= MidiGetRcvAlarm( refNum))
		fprintf( stdout, "Warning : alarme incorrecte : %lx au lieu de nil\n", rcv);
	
	fprintf( stdout, "    MidiGetApplAlarm : ");flush;
	appl= MidiGetApplAlarm( refNum);
	fprintf( stdout, "%lx %s\n", appl, OK);
	if( appl)
		fprintf( stdout, "Warning : alarme non positionn�e et non nulle\n");
	
	fprintf( stdout, "    MidiSetApplAlarm : ");flush;
	MidiSetApplAlarm( refNum, ApplAlarm);
	fprintf( stdout, "%s\n", OK);
	if( (appl= MidiGetApplAlarm( refNum))!= ApplAlarm)
		fprintf( stdout, "Warning : alarme incorrecte : %lx au lieu de %lx\n", appl, ApplAlarm);
	MidiSetApplAlarm( refNum, (ApplAlarmPtr)nil);
	if( appl= MidiGetApplAlarm( refNum))
		fprintf( stdout, "Warning : alarme incorrecte : %lx au lieu de nil\n", appl);
}

/*____________________________________________________________________*/
static void Connections( Boolean isFreeMem)
{
	Boolean in, out;
	
	fprintf( stdout, "\nConnections :\n");
	if( MidiIsConnected( 0, refNum) || MidiIsConnected( refNum, 0))
		fprintf( stdout, "Warning : application d�j� connect�e !");
	
	fprintf( stdout, "    MidiConnect : ");flush;
	MidiConnect( 0, refNum, true);
	MidiConnect( refNum, 0, true);
	fprintf( stdout, "%s\n", OK);
	
	fprintf( stdout, "    MidiIsConnected : ");flush;
	in= MidiIsConnected( 0, refNum);
	out= MidiIsConnected( refNum, 0);
	fprintf( stdout, "%s\n", OK);
	if( !in || !out)
		if( isFreeMem)
			fprintf( stdout, "Warning : connections non �tablies : in %d, out %d\n",
					(int)in, (int)out);

	MidiConnect( 0, refNum, false);
	MidiConnect( refNum, 0, false);
	in= MidiIsConnected( 0, refNum);
	out= MidiIsConnected( refNum, 0);
	if( in || out)
		fprintf( stdout, "Warning : connections non rompues : in %d, out %d\n",
					(int)in, (int)out);
}

/*____________________________________________________________________*/
static void Events( Boolean isFreeMem)
{
	MidiEvPtr e, copy;
	long l, total, free, count;
	
	fprintf( stdout, "\nGestion des �v�nements :\n");
	
	fprintf( stdout, "    MidiTotalSpace : ");flush;
	total= MidiTotalSpace();
	fprintf( stdout, "%ld %s\n", total, OK);
	
	fprintf( stdout, "    MidiFreeSpace : ");flush;
	free= MidiFreeSpace();
	fprintf( stdout, "%ld %s\n", free, OK);
	
	if( isFreeMem)
	{
		fprintf( stdout, "    MidiGrowSpace : ");flush;
		l= MidiGrowSpace( 500L);
		fprintf( stdout, "allou�s : %ld %s\n", l, OK);
		total+= l;
		l= MidiTotalSpace();
		if( (l!= total) && MidiCountAppls())
			fprintf( stdout, "Warning : TotalSpace incoh�rent : %ld FreeSpace : %ld\n"
						, l, MidiFreeSpace()); 
		total= l;
	}
	
	free= MidiFreeSpace();
	fprintf( stdout, "    MidiNewCell : ");flush;
	e= MidiNewCell();
	fprintf( stdout, "%lx %s\n", e, OK);
	if( e && !isFreeMem)
		fprintf( stdout, "Warning : r�sultat incoh�rent : %lx\n", e); 
	if( (l= MidiFreeSpace())!= free-1)
		if( isFreeMem)
			fprintf( stdout, "Warning : FreeSpace incoh�rent : %ld\n", l); 
	
	if( isFreeMem)
	{
		fprintf( stdout, "    MidiFreeCell : ");flush;
		MidiFreeCell( e);
		fprintf( stdout, "%s\n", OK);
		if( (l= MidiFreeSpace())!= free)
			fprintf( stdout, "Warning : FreeSpace incoh�rent : %ld\n", l); 
	}
	
	fprintf( stdout, "    MidiNewEv : ");flush;
	e= MidiNewEv( typeNote);
	fprintf( stdout, "%lx %s\n", e, OK);
	if( e && !isFreeMem)
		fprintf( stdout, "Warning : r�sultat incoh�rent : %lx\n", e); 
	if( e && isFreeMem)
	{
		Date(e)= nil;
		Pitch(e)= 80;
		Chan(e)= 10;
		Vel(e)= 120;
		Dur(e)= 1000;
	
		fprintf( stdout, "    MidiCopyEv : ");flush;
		copy= MidiCopyEv(e);
		fprintf( stdout, "%lx %s\n", copy, OK);
		if( copy)
		{
			if( Date(copy) || (Pitch(copy)!=80) || (Chan(copy)!=10)
				|| (Vel(copy)!=120) || (Dur(copy)!=1000))
				fprintf( stdout, "Warning : mauvaise copie\n");
		}
		fprintf( stdout, "    MidiFreeEv : ");
		MidiFreeEv(e);
		fprintf( stdout, "%s\n", OK);
		if( copy) MidiFreeEv(copy);
	}
	else if( isFreeMem)
	{
		fprintf( stdout, "Warning : plus d'evt MidiShare\n");
		fprintf( stdout, "    MidiCopyEv : non test�\n");
		fprintf( stdout, "    MidiFreeEv : non test�\n");
	}
	
	if( !isFreeMem) return ;

	e= MidiNewEv( typeSysEx);
	if( e)
	{
		fprintf( stdout, "    MidiCountFields : ");flush;
		count= MidiCountFields(e);
		fprintf( stdout, "%ld %s\n", count, OK);
		if( count)
			fprintf( stdout, "Warning : mauvais nbre de champs !\n");
		
		fprintf( stdout, "    MidiAddField : ");flush;
		MidiAddField( e, 1L);
		fprintf( stdout, "%s\n", OK);
		l= MidiCountFields(e);
		if( ++count!= l)
			fprintf( stdout, "Warning : mauvais nbre de champs !\n");
		
		fprintf( stdout, "    MidiGetField : ");flush;
		l= MidiGetField( e, 0L);
		fprintf( stdout, "%ld %s\n", l, OK);
		
		fprintf( stdout, "    MidiSetField : ");flush;
		MidiSetField( e, 0L, l+1);
		fprintf( stdout, "%s\n", OK);
		if( (l+1)!= MidiGetField( e, 0L))
			fprintf( stdout, "Warning : champs mal positionn� !\n");
	}
	else
	{
		fprintf( stdout, "    MidiSetField : non test�\n");
		fprintf( stdout, "    MidiGetField : non test�\n");
		fprintf( stdout, "    MidiAddField : non test�\n");
		fprintf( stdout, "    MidiCountFields : non test�\n");
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
void ApplyProc( MidiEvPtr e )
#endif
{
	if( EvType(e)== typeNote)
		Pitch(e)++;
}

/*____________________________________________________________________*/
static void Sequences( Boolean isFreeMem)
{
	MidiSeqPtr seq;
	MidiEvPtr e;
	long n, free;

	fprintf( stdout, "\nGestion des s�quences :\n");
		
	free= MidiFreeSpace();
	fprintf( stdout, "    MidiNewSeq : ");flush;
	seq= MidiNewSeq();
	fprintf( stdout, "%lx %s\n", seq, OK);
	if( seq && !isFreeMem)
		fprintf( stdout, "Warning : s�quence non nulle !\n");
	
	if( !isFreeMem) return;
	if( !seq)
	{
		fprintf( stdout, "Warning : s�quence nulle !\n");
		fprintf( stdout, "    MidiAddSeq : non test�\n");
		fprintf( stdout, "    MidiFreeSeq : non test�\n");
		fprintf( stdout, "    MidiClearSeq : non test�\n");
		fprintf( stdout, "    MidiApplySeq : non test�\n");
		return;
	}
		
	if( e= MidiNewEv( typeNote))
	{
		Pitch(e)= 60;
		
		fprintf( stdout, "    MidiAddSeq : ");flush;
		MidiAddSeq( seq, e);
		fprintf( stdout, "%s\n", OK);
		if( (n=CountOfEvents( seq))!= 1)
			fprintf( stdout, "Warning : nbre d'evts incoh�rent : %ld\n", n);
		
		fprintf( stdout, "    MidiApplySeq : ");flush;
		MidiApplySeq( seq, ApplyProc);
		fprintf( stdout, "%s\n", OK);
		if( Pitch(e)!= 61)
			fprintf( stdout, "Warning : apply non effectu� !\n");
	
		fprintf( stdout, "    MidiClearSeq : ");flush;
		MidiClearSeq( seq);
		fprintf( stdout, "%s\n", OK);
		if( n=CountOfEvents( seq))
			fprintf( stdout, "Warning : nbre d'evts incoh�rent : %ld\n", n);
	}
	else fprintf( stdout, "Warning : plus d'evt disponible !\n");
		
	if( e= MidiNewEv( typeNote))
	{
		MidiAddSeq( seq, e);
		fprintf( stdout, "    MidiFreeSeq : ");flush;
		MidiFreeSeq( seq);
		fprintf( stdout, "%s\n", OK);
		if( (n= MidiFreeSpace())!= free)
			fprintf( stdout, "Warning : freeSpace incoh�rent : %ld\n", n);
		seq= nil;
	}
	else fprintf( stdout, "Warning : plus d'evt disponible !\n");
	if( seq)
	{	
		fprintf( stdout, "    MidiFreeSeq : ");flush;
		MidiFreeSeq( seq);
		fprintf( stdout, "%s\n", OK);
		if( (n= MidiFreeSpace())!= free)
			fprintf( stdout, "Warning : freeSpace incoh�rent : %ld\n", n);
	}
}

/*____________________________________________________________________*/
static void Time()
{
	fprintf( stdout, "\nGestion du temps :\n");
	fprintf( stdout, "    MidiGetTime : ");flush;
	fprintf( stdout, "%ld %s\n", MidiGetTime(), OK);
}

/*____________________________________________________________________*/
static void Sending()
{
	MidiEvPtr e, copy, get;
	short r;
	long free, time, n, count;

	fprintf( stdout, "\nSending and receiving events :\n");
	free= MidiFreeSpace();
	if( (r= MidiOpen( NewName)) < 0)
	{
		fprintf( stdout, "Warning : impossible d'ouvrir une nouvelle application !\n");
		fprintf( stdout, "          tests non effectu�s.\n");
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
			fprintf( stdout, "    MidiSendIm : ");flush;
			MidiSendIm( refNum, copy);
			time= MidiGetTime();
			fprintf( stdout, "%s\n", OK);
			while( time== MidiGetTime());
			
			fprintf( stdout, "    MidiCountEvs : ");flush;
			n= MidiCountEvs(r);
			fprintf( stdout, "%ld %s\n", n, OK);
			if( n!= 1)
				fprintf( stdout, "Warning : nbre d'evts incoh�rent !\n");
			
			fprintf( stdout, "    MidiGetEv : ");flush;
			get= MidiGetEv(r);
			fprintf( stdout, "%lx %s\n", get, OK);
			n= MidiCountEvs(r);
			if( n)
				fprintf( stdout, "Warning : nbre d'evts incoh�rent : %ld\n", n);
			if( get)
				MidiFreeEv( get);
		}
		else fprintf( stdout, "Warning : plus d'evt disponible !\n");

		if( copy= MidiCopyEv( e))
		{
			fprintf( stdout, "    MidiSend : ");flush;
			Date(copy)= time= MidiGetTime()+ 4;
			MidiSend( refNum, copy);
			while( MidiGetTime()<= time);
			fprintf( stdout, "%s\n", OK);
			n= MidiCountEvs(r);
			if( n!= 1)
				fprintf( stdout, "Warning : nbre d'evts transmis incoh�rent : %ld\n", n);
			
			fprintf( stdout, "    MidiAvailEv : ");flush;
			get= MidiAvailEv(r);
			fprintf( stdout, "%lx %s\n", get, OK);
			if( (count= MidiCountEvs(r))!= n)
				fprintf( stdout, "Warning : nbre d'evts incoh�rent : %ld\n", count);
		}
		else fprintf( stdout, "Warning : plus d'evt disponible !\n");

		Date(e)= nil;
		fprintf( stdout, "    MidiSendAt : ");flush;
		MidiSendAt( refNum, e, time= MidiGetTime()+ 4);
		fprintf( stdout, "%s\n", OK);
		while( MidiGetTime()<= time);
		n= MidiCountEvs(r);
		if( n!= 2)
			fprintf( stdout, "Warning : nbre d'evts transmis incoh�rent : %ld\n", n);
			
		fprintf( stdout, "    MidiFlushEvs : ");flush;
		MidiFlushEvs(r);
		fprintf( stdout, "%s\n", OK);
		if( n= MidiCountEvs(r))
			fprintf( stdout, "Warning : nbre d'evts incoh�rent : %ld\n", n);
	}
	else fprintf( stdout, "Warning : plus d'evt disponible !\n");
	MidiConnect( refNum, 0, false);
	MidiClose(r);
	MidiFlushEvs( refNum);
	if( (n= MidiFreeSpace())!= free)
		fprintf( stdout, "Warning : FreeSpace incoh�rent : %ld\n", n);
}

/*____________________________________________________________________*/
static void Mail()
{
	long a= 1; void* b;
	
	fprintf( stdout, "\nMail boxes :\n");
	fprintf( stdout, "    MidiReadSync : ");flush;
	b= MidiReadSync( &a);
	fprintf( stdout, "%ld %s\n", b, OK);
	if( a)
		fprintf( stdout, "Warning : contenu non nul : %ld\n", a);
	
	a= nil;
	fprintf( stdout, "    MidiWriteSync : ");flush;
	b= MidiWriteSync( &a, (void* )2);
	fprintf( stdout, "%ld %s\n", b, OK);

	a=1;
	b= MidiWriteSync( &a, (void* )2);
	if( a!= 2)
		fprintf( stdout, "Warning : variable non nulle mais positionn�e : %ld\n", a);
	if( b!= (void*)1)
		fprintf( stdout, "Warning : retour incorrect : %ld\n", b);
}

/*____________________________________________________________________*/
#ifdef PASCALTASKS
static pascal void MyTask( long unused1, short unused2, long a1,long unused3,long unused4)
#endif
#ifdef CTASKS
void MyTask( long unused1, short unused2, long a1,long unused3,long unused4)
#endif
{
	if( a1)
		*(long *)a1= 1;
}

/*____________________________________________________________________*/
#ifdef PASCALTASKS
static pascal void MyDTask( long unused1, short unused2, long a1,long a2,long a3)
#endif
#ifdef CTASKS
void MyDTask( long unused1, short unused2, long a1,long a2,long a3)
#endif
{
	fprintf( stdout, "dtask rcv %ld %ld %ld ", a1, a2, a3);
}

/*____________________________________________________________________*/
static void Tasks( Boolean isFreeMem)
{
	MidiEvPtr e;
	long time, p1=0, p2=0, p3=0;
	
	fprintf( stdout, "\nTask Managing :\n");
			
	fprintf( stdout, "    MidiCall : ");flush;
	MidiCall( MyTask, time= MidiGetTime(), refNum, (long)&p1, (long)&p2, (long)&p3);
	time+= 4;
	while( MidiGetTime() <= time);
	fprintf( stdout, "%s\n", OK);
	if( (p1!= 1) && isFreeMem)
		fprintf( stdout, "Warning : tache non effectu�e !\n");
			
	fprintf( stdout, "    MidiTask : ");flush;
	p1= 0;
	e= MidiTask( MyTask, time= MidiGetTime(), refNum, (long)&p1, (long)&p2, (long)&p3);
	time+= 4;
	while( MidiGetTime() <= time);
	fprintf( stdout, "%lx %s\n", e, OK);
	if( (p1!= 1) && isFreeMem)
		fprintf( stdout, "Warning : tache non effectu�e !\n");
			
	fprintf( stdout, "    MidiDTask : ");flush;
	e= MidiDTask( MyDTask, time= MidiGetTime(), refNum, 1L, 2L, 3L);
	time+= 4;
	while( MidiGetTime() <= time);
	fprintf( stdout, "%lx %s\n", e, OK);
	
	fprintf( stdout, "    MidiCountDTasks : ");flush;
	p1= MidiCountDTasks( refNum);
	fprintf( stdout, "%ld %s\n", p1, OK);
	if( (p1!= 1) && isFreeMem)
		fprintf( stdout, "Warning : tache non distribu�e !\n");

	if( !isFreeMem)
		return;
	
	fprintf( stdout, "    MidiExec1DTask : ");flush;
	MidiExec1DTask( refNum);
	fprintf( stdout, "%s\n", OK);
	p1= MidiCountDTasks( refNum);
	if( p1)
		fprintf( stdout, "Warning : nombre de taches incorrect : %ld\n", p1);

	e= MidiDTask( MyDTask, time= MidiGetTime(), refNum, 1L, 2L, 3L);
	time+= 4;
	while( MidiGetTime() <= time);
	p1= MidiCountDTasks( refNum);
	if( p1!= 1)
		fprintf( stdout, "Warning : nombre de taches incorrect : %ld\n", p1);
	fprintf( stdout, "    MidiFlushDTasks : ");flush;
	MidiFlushDTasks( refNum);
	fprintf( stdout, "%s\n", OK);
	p1= MidiCountDTasks( refNum);
	if( p1)
		fprintf( stdout, "Warning : nombre de taches incorrect : %ld\n", p1);

	time= MidiGetTime()+100;
	e= MidiDTask( MyDTask, time, refNum, 1L, 2L, 3L);
	fprintf( stdout, "    MidiForgetTask : ");flush;
	MidiForgetTask( &e);
	time+= 4;
	while( MidiGetTime() <= time);
	fprintf( stdout, "%s\n", OK);
	p1= MidiCountDTasks( refNum);
	if( p1)
		fprintf( stdout, "Warning : nombre de taches incorrect : %ld\n", p1);
}


/*____________________________________________________________________*/
static Synchro()
{
	TSmpteLocation loc;
	long time, tmp;
	short version = MidiGetVersion();
	
	fprintf( stdout, "\nSmpte Synchro :\n");
	if ((version < 165) || (version > 168))
	{
		fprintf( stdout, "    non impl�ment�e.\n");
		return nil;
	}
	
	time= MidiGetTime();
	fprintf( stdout, "    MidiTime2Smpte : ");flush;
	MidiTime2Smpte( time, 1, &loc);
	fprintf( stdout, "%s\n", OK);
	fprintf( stdout, "        format : %d\n", (int)loc.format);
	fprintf( stdout, "        hours  : %d\n", (int)loc.hours);
	fprintf( stdout, "        minutes: %d\n", (int)loc.minutes);
	fprintf( stdout, "        seconds: %d\n", (int)loc.seconds);
	fprintf( stdout, "        frames : %d\n", (int)loc.frames);
	fprintf( stdout, "        fracs  : %d\n", (int)loc.fracs);

	fprintf( stdout, "    MidiSmpte2Time : ");flush;
	tmp= MidiSmpte2Time( &loc);
	fprintf( stdout, "%s\n", OK);
	if( tmp!= time)
		fprintf( stdout, "Warning : conversions inexactes : %ld -> %ld\n", time, tmp);

	return true;
}

/*____________________________________________________________________*/
static void Tolerance()
{	
	fprintf( stdout, "\nTolerance aux pointeurs nuls :\n");
	if( MidiGetVersion() < 165)
	{
		fprintf( stdout, "    non impl�ment�e.\n");
		return;
	}

	fprintf( stdout, "    MidiFreeCell : ");flush;
	MidiFreeCell( (MidiEvPtr)nil);
	fprintf( stdout, "%s\n", OK);

	fprintf( stdout, "    MidiCopyEv : ");flush;
	MidiCopyEv( (MidiEvPtr)nil);
	fprintf( stdout, "%s\n", OK);

	fprintf( stdout, "    MidiFreeEv : ");flush;
	MidiFreeEv( (MidiEvPtr)nil);
	fprintf( stdout, "%s\n", OK);

	fprintf( stdout, "    OldMidiSetField : ");flush;
	OldMidiSetField( (MidiEvPtr)nil, 0, 0L);
	fprintf( stdout, "%s\n", OK);

	fprintf( stdout, "    OldMidiGetField : ");flush;
	OldMidiGetField( (MidiEvPtr)nil, 0);
	fprintf( stdout, "%s\n", OK);

	fprintf( stdout, "    OldMidiCountFields : ");flush;
	OldMidiCountFields( (MidiEvPtr)nil);
	fprintf( stdout, "%s\n", OK);

	fprintf( stdout, "    MidiSetField : ");flush;
	MidiSetField( (MidiEvPtr)nil, 0L, 0L);
	fprintf( stdout, "%s\n", OK);

	fprintf( stdout, "    MidiGetField : ");flush;
	MidiGetField( (MidiEvPtr)nil, 0L);
	fprintf( stdout, "%s\n", OK);

	fprintf( stdout, "    MidiAddField : ");flush;
	MidiAddField( (MidiEvPtr)nil, 0L);
	fprintf( stdout, "%s\n", OK);

	fprintf( stdout, "    MidiCountFields : ");flush;
	MidiCountFields( (MidiEvPtr)nil);
	fprintf( stdout, "%s\n", OK);

	fprintf( stdout, "    MidiAddSeq : ");flush;
	MidiAddSeq( (MidiSeqPtr)nil, (MidiEvPtr)nil);
	MidiAddSeq( (MidiSeqPtr)1, (MidiEvPtr)nil);
	fprintf( stdout, "%s\n", OK);

	fprintf( stdout, "    MidiFreeSeq : ");flush;
	MidiFreeSeq( (MidiSeqPtr)nil);
	fprintf( stdout, "%s\n", OK);

	fprintf( stdout, "    MidiClearSeq : ");flush;
	MidiClearSeq( (MidiSeqPtr)nil);
	fprintf( stdout, "%s\n", OK);

	fprintf( stdout, "    MidiApplySeq : ");flush;
	MidiApplySeq( (MidiSeqPtr)nil, (ApplyProcPtr)nil);
	MidiApplySeq( (MidiSeqPtr)1, (ApplyProcPtr)nil);
	fprintf( stdout, "%s\n", OK);

	fprintf( stdout, "    MidiSendIm : ");flush;
	MidiSendIm( nil, (MidiEvPtr)nil);
	fprintf( stdout, "%s\n", OK);

	fprintf( stdout, "    MidiSend : ");flush;
	MidiSend( nil, (MidiEvPtr)nil);
	fprintf( stdout, "%s\n", OK);

	fprintf( stdout, "    MidiSendAt : ");flush;
	MidiSendAt( nil, (MidiEvPtr)nil, 0L);
	fprintf( stdout, "%s\n", OK);
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
static void NoMem()
{
	MidiSeqPtr seq;
	long free, l;
	
	fprintf( stdout, "\n\nTol�rance au d�faut de m�moire :\n");
	free= MidiFreeSpace();
	seq= GetAllFreeEvs();
	l = MidiFreeSpace();
	Connections( false); fflush( stdout);
	Events( false); fflush( stdout);
	Sequences( false); fflush( stdout);
	Tasks(false); fflush( stdout);
	MidiFreeSeq( seq);
	if( free!= (l=MidiFreeSpace()))
		fprintf( stdout, "Warning : freeSpace incoh�rent : %ld au lieu de %ld\n", l, free);
}

/*____________________________________________________________________*/
main()
{
	fprintf( stdout, "\nTest des fonctions de MidiShare.\n");
	fprintf( stdout, "================================\n");
	
	if( MidiShare()) {
		if( !Environment()) return 1;
		fflush( stdout);
		OpenClose(); fflush( stdout);
		ApplConfiguration(); fflush( stdout);
		Connections( true); fflush( stdout);
		Events( true); fflush( stdout);
		Sequences( true); fflush( stdout);
		Time(); fflush( stdout);
		Sending(); fflush( stdout);
		Mail(); fflush( stdout);
		Tasks(true); fflush( stdout);
		Synchro(); fflush( stdout);
		Tolerance(); fflush( stdout);
		NoMem(); fflush( stdout);
		MidiClose( refNum);
	}
	else fprintf( stdout, "MidiShare n'est pas install� !\n");
	fprintf( stdout, "\nFin de test des fonctions.\n");
	return 0;
}
