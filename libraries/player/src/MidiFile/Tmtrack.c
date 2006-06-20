/* ===========================================================================
* The Player Library is Copyright (c) Grame, Computer Music Research Laboratory 
* 1996-1999, and is distributed as Open Source software under the Artistic License;
* see the file "Artistic" that is included in the distribution for details.
*
* Grame : Computer Music Research Laboratory
* Web : http://www.grame.fr/Research
* E-mail : MidiShare@rd.grame.fr
* ===========================================================================
*/

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* TMtrack.cpp : gestion des MidiFile et des tracks
/*
/* History
/*
/* 3/05/96: version 1.00
/*
/*
/*----------------------------------------------------------------------------*/
 /*
 * interface des fonctions de lecture et d'�criture de fichier
 * au format MidiFile.
 *
 * ce fichier doit etre linke avec la librairie MidiFile
 *
 * les fonctions d'interface sont les suivantes :
 *		int MidiFileSave( StringPtr filename, ScorePtr sc, short format, short timeDef, short nTicks)
 *      int MidiFileLoad( StringPtr filename, ScorePtr sc, short *format, short *timeDef, short *nTicks)
 *
 * lors de la sauvegarde, et pour un format 1 ou 2, le refNum des �v�nements sert 
 * � les dispatcher sur les diff�rentes pistes (soit donc 256 pistes possibles). 
 * Si ce num�ro de r�f�rence ne correspond pas au num�ro de la piste courante,
 * il est stock� dans un �v�nement de type SeqName (cr�� s'il n'existe pas d�j�).
 * Ce num�ro de r�f�rence est restaur� lors de la relecture du fichier.
 *
 * Sp�cifications particuli�res au format 1:
 * - un num�ro de r�f�rence null envoie les �v�nements dans la tempo map.
 * - quelque soit leur num�ro de r�f�rence, les �v�nements suivants sont
 *   �galement envoy�s dans la tempo map :
 *							typeCopyright
 *							typeMarker
 *							typeTempo
 *							typeSMPTEOffset
 *							typeTimeSign
 *							typeKeySign
 *  � la relecture, il n'est donc pas possible de restaurer leur num�ro de r�f�rence
 *  si celui-ci diff�re de 0.
 *	
 * Ces fonctions retournent nil si tout s'est bien pass�, un code d'erreur n�gatif
 * si l'erreur concerne MidiShare ou le format MidiFile, un code d'erreur positif
 * si l'erreur concerne la lecture, l'�criture du fichier ou le syst�me.
 *
 * GRAME Friday, March 27, 1992 D.F.
 *
/*----------------------------------------------------------------------------*/

#include "Tmtrack.h"


#ifdef __Macintosh__
	#include <CType.h>
//	#include <Files.h>
	#include <StdLib.h>
	#include <String.h>
#endif

#ifdef __Windows__
	#include <StdLib.h>
	#include <String.h>
#endif

/*----------------------------------------------------------------------------*/

char *Player= "PLAYER ref:";

/*--------------- les types stock�s dans la tempo map ----------------------*/

#define IsTempoMap( t) ( (t)==typeCopyright || (t)==typeMarker || ((t)>=typeTempo && (t)<=typeKeySign))

/*--------------- les variables globales -----------------------------------*/

char trackListe[maxTrack];					/* liste des pistes � �crire 	*/

/*--------------------------------------------------------------------------*/
/*																			*/
/*   fonctions qui positionnent le num�ro de r�f�rence des �v�nement en		*/
/*   fonction du num�ro de piste ou des infos contenues dans l'�v�nement	*/
/*   SeqName de la piste.													*/
/*																			*/
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
 void SetSeqRef( MidiSeqPtr seq, register short refNum)
{
	register MidiEvPtr ev;
	
	ev= seq->first;
	while( ev)
	{
		RefNum(ev)= refNum;
		ev= Link(ev);
	}
}

/*--------------------------------------------------------------------------*/
 char *GetBeginKey( register char *buff)
{
	register char *start= nil;
	register long k, l;
	
	k= *(long *)Player;
	l= strlen(Player);
	while( *buff)
	{
		if( *(long *)buff== k && !strncmp( buff, Player, l))
		{
			start= buff;
			break;
		}
		else buff++;
	}
	return start;
}

/*--------------------------------------------------------------------------*/
 MidiEvPtr RestoreSeqName( char *buff)
{
	MidiEvPtr ev= nil;
	register char *tmp;
	
	if( tmp= GetBeginKey( buff))			/* recherche le d�but de la cl� */
	{
		if( tmp!= buff && *(tmp-1)== ' ')		/* si un espace le pr�c�de	*/
			tmp--;								/* on pointe un char avant	*/
		*tmp= 0;								/* fin de chaine			*/
		tmp= buff;								/* pointe le d�but			*/
		if( ev= MidiNewEv( typeSeqName))		/* nouvel �v�nement			*/
		{
			while( *tmp)						/* recopie de l'ancien		*/
				MidiAddField( ev, *tmp++);		/* octet par octet			*/
		}
	}
	return ev;
}

/*--------------------------------------------------------------------------*/
/* renvoie le premier �v�nement de type SeqName ou TrackName de la s�quence */
 MidiEvPtr GetTrackName( MidiSeqPtr seq, MidiEvPtr *prec)
{
	register MidiEvPtr ev, p= nil;
	
	ev= seq->first;
	while( ev)
	{
		if( EvType(ev)== typeSeqName)
		{
			if( prec) *prec= p;
			break;
		}
		p= ev;
		ev= Link(ev);
	}
	return ev;
}

/*--------------------------------------------------------------------------*/
/* renvoie le num�ro de r�f�rence d'un evt SeqName transf�r� dans un buffer */
 short GetEvRef( char *buff, int keyLen, short numPiste)	
{
	//register short refNum, i=nil;
	register short refNum, i=0;
	register char *tmp;
	char strRef[4];
	
	refNum= numPiste;
	if( tmp= GetBeginKey( buff))			/* recherche le d�but de la cl� */
	{
		tmp+= keyLen;							/* tmp pte le num�ro de port*/		
		while( isdigit(*tmp) && i<3)
			strRef[i++]= *tmp++;				/* on le recopie 			*/
		strRef[i]=0;							/* fin de chaine 			*/
		refNum= atoi( strRef);					/* conversion en num�rique	*/
	}
	return refNum;
}

/*--------------------------------------------------------------------------*/
/*   	renvoie le refNum d'une seq	(par d�faut: num�ro de piste)			*/
 short GetSeqRef( MidiSeqPtr seq, short numPiste)
{
	register MidiEvPtr ev, ori;
	register short refNum;
	MidiEvPtr prec= nil;
	register int i, n, l;
	char buff[512];

	refNum= numPiste;
	if( ev= GetTrackName( seq, &prec))
	{
		n= MidiCountFields( ev);				/* nbre de champs de l'evt	*/
		l= strlen( Player);						/* longueur + que minimale	*/
		if( n < 511)
		{
			for( i=0; i<n; i++)					/* lit le contenu de l'evt	*/
				buff[i]= MidiGetField( ev, i);
			buff[i]= 0;									/* fin de chaine	*/
			if( !strncmp( buff, Player,l) && !prec)		/* evt ajout�		*/
			{
				seq->first= Link(ev);			/* supprime l'evt de la seq */
				if( !seq->first)
					seq->last= nil;
				MidiFreeEv( ev);				/* et le lib�re				*/
			}
			else if( ori= RestoreSeqName( buff))		/* evt modifi�		*/
			{
				Date(ori)= Date(ev);
				Link(ori)= Link(ev);			/* on restaure l'original	*/
				if( prec) Link(prec)= ori;		/* on le remplace dans		*/
				else seq->first= ori;			/* la s�quence				*/
				MidiFreeEv( ev);				/* lib�re l'evt modifi�		*/
			}
			refNum= GetEvRef( buff, l, numPiste);
		}
	}
	return refNum;
}

/*--------------------------------------------------------------------------*/
/* 		supprime les fins de pistes qui sont au milieu de la s�quence		*/
/*--------------------------------------------------------------------------*/
 void DelEndTrack( MidiSeqPtr seq)
{
	register MidiEvPtr ev, prev, tmp;
	
	prev= nil;
	ev= seq->first;
	while( ev)
	{
		if( EvType(ev)== typeEndTrack && Link(ev))	/* evt fin de piste		*/
		{											/* n'est pas le dernier */
			if( prev)								/* n'est pas le premier	*/
				Link(prev)= Link(ev);				/* mod. le chainage		*/
			else									/* sinon				*/
				seq->first= Link(ev);				/* suivant= premier		*/
			tmp= Link(ev);							/* sauve le suivant		*/
			MidiFreeEv(ev);							/* lib�re l'evt			*/
			ev= tmp;								/* evt courant=suivant	*/
		}
		else 
		{
			prev= ev;
			ev= Link(ev);
		}
	}
}

/*--------------------------------------------------------------------------*/
/*    					fonctions de mixage des s�quences					*/
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
 MidiEvPtr AddSeq( register MidiEvPtr e1, register MidiEvPtr e2)
{
	register MidiEvPtr next;
	
	while( next= Link(e1))				/* tant qu'une s�quence n'est finie */
	{
		if( Date(next) <= Date(e2))		/* date inf�rieure dans la m�me seq */
			e1= next;					/* rien � faire : on continue		*/
		else							/* sinon							*/
		{
			Link( e1)= e2;				/* on linke avec l'autre s�quence	*/
			e1= e2;						/* et on les inverse				*/
			e2= next;
		}
	}
	if( Link(e1)= e2)			/* linke avec la fin de l'autre s�quence 	*/
		while( Link(e2))
			e2= Link(e2);
	return e2;					/* et renvoie le dernier evt de la s�quence */
}

/*--------------------------------------------------------------------------*/
 void MixeSeq( MidiSeqPtr src, MidiSeqPtr dest)
{
	register MidiEvPtr firstSrc, firstDest;
	
	if( dest && src)							/* dest et src existent		*/
	{
		if( firstSrc= src->first)				/* src non vide				*/
		{
			if( !(firstDest= dest->first))		/* si destination est vide	*/
			{
				dest->first= firstSrc;			/* recopie du premier et	*/
				dest->last= src->last;			/* dernier evt de src		*/
			}
			else if( Date(firstSrc) < Date(firstDest))
												/* 1ier evt source pr�c�de	*/
			{									/* le 1ier evt destination	*/
				dest->first= firstSrc;			/* range dans destination	*/
				dest->last= AddSeq( firstSrc, firstDest);	/* et chainage	*/
			}
			else dest->last= AddSeq( firstDest, firstSrc);	/* et chainage	*/
		}
	}
}


/*--------------------------------------------------------------------------*/
/*				Codage de tempo et TimeSign 								*/
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/

MidiEvPtr CheckEvType (MidiSeqPtr src, short type)
{
	MidiEvPtr e = FirstEv(src);

	while (e && (Date(e) == 0)){
		if (EvType(e) == type) return e;
		e = Link(e);
	}
	return 0;
}

/*--------------------------------------------------------------------------*/

MidiSeqPtr WriteTempoAndTimeSign (MidiSeqPtr seq) 
{
	MidiEvPtr e1, e2;

	e1 = CheckEvType(seq,typeTempo);  		// check Tempo at Date 0
	e2 = CheckEvType(seq,typeTimeSign);		// check TimeSign at Date 0
	
	if (e1 == 0) {
		e1 = MidiNewEv(typeTempo); 
		if (e1) {
			Tempo(e1) = 500000;
			Date(e1) = 0;
			RefNum(e1) = 0;
			Link(e1) = FirstEv(seq);
			FirstEv(seq) = e1;
			if (Last(seq) == 0) Last(seq) = e1; // if seq empty
		}else {
			return 0;
		}
	}
	
	if (e2 == 0) {
		e2 = MidiNewEv(typeTimeSign); 
		if (e2) {
			Date(e2) = 0;
			TSNum(e2) = 4;
			TSDenom(e2) = 2;
			TSClocks (e2) = 0;
			TS32nd (e2) = 0;
			RefNum(e2) = 0;
			Link(e2) = First(seq);
			First(seq) = e2;
			if (Last(seq) == 0) Last(seq) = e2; // if seq empty
		}else {
			return 0;
		}
	}
	
	return seq;
}


/*--------------------------------------------------------------------------*/
/*				Codage de l'info de port									*/
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
// Rend le premier ev sur chaque piste
void BuildTable(MidiSeqPtr seq, MidiEvPtr* table) {

	MidiEvPtr cur = FirstEv(seq); 

	while (cur ){
		if(table[RefNum(cur)] == 0) table[RefNum(cur)]  = cur;
		cur = Link(cur);
	}
}

/*--------------------------------------------------------------------------*/

MidiSeqPtr WritePortInfo (MidiSeqPtr seq) 
{
	MidiEvPtr e1, e2;
	MidiSeqPtr tmp;
	char buff[32] ;
	short i,j;
    MidiEvPtr table[256];

	// Initialisation
	for (i = 0; i< 256 ; i++) { table[i] = 0;}
	tmp = MidiNewSeq();
 	if (!tmp) return 0;

	BuildTable(seq,table);

	// Pour chaque piste SAUF la TempoMap
	for (i = 1; i< 256 ; i++) {  
		
		if ((e1 =  table[i]) && IsNormalEv(e1)) {    // S'il faut coder le port
			if (e2 = MidiNewEv (typeInstrName)) {

				// Ecriture de l'ev de codage de port � la date 0	
				Date(e2) = 0;
			   	Chan(e2) = Chan(e1);  // ne sert � rien ???
				Port(e2) = 0;
	           	RefNum(e2) = RefNum(e1);

	        	sprintf(buff, "Port %ld",Port(e1));
				for (j=0 ;buff[j]; j++) { MidiAddField(e2 ,buff[j]);}
				
				//Ajoute toujours au d�but
				Link(e2) = FirstEv(seq);
				FirstEv(seq) = e2;
			
			}else 
				return 0;
		}
	}

	return seq;
}

/*--------------------------------------------------------------------------*/

Boolean IsNormalEv(MidiEvPtr e) 
{
	return ((EvType(e) != typeInstrName) || !CheckPort(e));
}

/*--------------------------------------------------------------------------*/

Boolean CheckPort(MidiEvPtr e)
{
	char name[256];
	short i, n = MidiCountFields(e);

	for (i= 0; i< n ; i++) {name[i] = MidiGetField(e,i);}
	name[n] = 0;
	
	return ((strcmp(name, "Modem") == 0)  
			|| (strcmp(name, "Printer") == 0) 
			|| ((strncmp(name,"Port", 4) == 0)));
}

/*--------------------------------------------------------------------------*/

short GetPortFromString(MidiEvPtr e)
{
	char name[256];
	short port, i, n = MidiCountFields(e);

	for (i= 0; i< n ; i++) {name[i] = MidiGetField(e,i);}
	name[n] = 0;
	
	if (strcmp(name, "Modem") == 0) 
		return ModemPort;
	else if (strcmp(name, "Printer") == 0)  
		return PrinterPort;
	else if (strncmp(name,"Port", 4) == 0) {
		port = atol(&name[4]);
		return ((port >= 0) && (port < 256)) ? port : ModemPort;
	}else 
		return ModemPort;
}

/*--------------------------------------------------------------------------*/

void SetSeqPort (MidiSeqPtr seq)
{
	MidiEvPtr e = FirstEv (seq);
	short port = ModemPort;

	while (e) { 
		if ((EvType(e) == typeInstrName) || (EvType(e) == typeMarker)) port = GetPortFromString(e);
		Port(e) = port;
		e = Link(e);
	}
}

/*--------------------------------------------------------------------------*/

void UseTrack (MidiSeqPtr seq, MidiSeqPtr dest , int i)
{
	SetSeqRef( seq, GetSeqRef(seq, i));	/* restitue le refnum		*/
	SetSeqPort (seq);					/* restitue le Port         SL le 16/01/98 */
	MixeSeq( seq, dest);				/* mixe avec la seq. dest	*/
	seq->first= seq->last= nil;			/* sauvegarde des evts		*/
	MidiFreeSeq( seq);					/* lib�re la sequence		*/
}

/*--------------------------------------------------------------------------*/

int TryToReadTrack ( register midiFILE *fd, MidiSeqPtr dest, int i)
{
	register MidiSeqPtr seq;
	register ret= 0;


	if( seq = MidiFileReadTrack(fd)){		/* lecture de la piste		*/
		UseTrack (seq, dest, i);
	}else{
		if (MidiFile_errno == MidiFileErrEvs) {
			while ((seq == 0) && (MidiGrowSpace(10000) > 0)) {
				MidiFileCloseTrack(fd);
				MidiFileChooseTrack(fd,i);
				seq =  MidiFileReadTrack(fd);
		
			}
			if (seq) {
				UseTrack (seq, dest, i);
			}else 
				if( MidiFile_errno!= MidiFileNoErr)
					ret= MidiFile_errno;
				else 
					ret= ErrRead;
		}
	}
	return ret;
}

/*--------------------------------------------------------------------------*/
 int ReadTracks( register midiFILE *fd, MidiSeqPtr dest)
{
	register int i, ret= 0;
	
	for (i=0; i< fd->ntrks && !ret; i++)
	{
		ret = TryToReadTrack (fd, dest,i);
	}
	return ret;
}


/*--------------------------------------------------------------------------*/
/*																			*/
/*			les fonctions d'analyse et d'�criture d'une score				*/
/*																			*/
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
 void InitTrackListe( void)
{
	register int i;
	register char *tmp;
	
	tmp= trackListe;
	for( i= 0; i< maxTrack; i++)
		*tmp++= false;
}

/*--------------------------------------------------------------------------*/
 Boolean WriteEndTrack( register midiFILE *fd, register MidiEvPtr prev)
{
	register MidiEvPtr ev;
	register Boolean ret= false;
	MidiSeqPtr seq;
	
	if( ev= MidiNewEv( typeEndTrack))		/* alloue un evt fin de piste	*/
	{
		Link(ev)= nil;
		if( (seq=fd->keyOff) && seq->first)	/* il reste des keyOff � �crire */
		{
			Date(ev)= Date(seq->last);
			Link(seq->last)= ev;			/* on place l'evt endTrack au 	*/
			seq->last= ev;					/* bout de la s�quence			*/
			ret= true;
		}
		else
		{
			if( prev)						/* s'il y a un �v�nement avant	*/
			{
				Date(ev)= Date(prev);		/* positionne sa date			*/
				if( EvType(prev)== typeNote)/* si le pr�c�dent est une note */
					Date(ev)+= Dur(prev);	/* rajoute la dur�e de la note	*/
			}
			else Date(ev)= 0;
			ret= MidiFileWriteEv( fd, ev);	/* �crit l'�v�nement			*/
			MidiFreeEv( ev);				/* et le lib�re					*/
		}
	}
	else MidiFile_errno= MidiFileErrEvs;
	return ret;
}

/*--------------------------------------------------------------------------*/
 Boolean WriteSeqName( midiFILE *fd, MidiEvPtr ev, short ref, short numPiste)
{
	register MidiEvPtr name;
	register Boolean ret= false;
	register char *tmp;
	char buff[6];
	
	if( ref== numPiste)						/* refNum �gal au num de piste	*/
		ret= MidiFileWriteEv( fd, ev);		/* on �crit l'�v�nement tel quel*/
	else if( name= MidiCopyEv( ev))			/* sinon on le copie			*/
	{
		MidiAddField( name, ' ');			/* ajoute un espace				*/
		tmp= Player;
		while( *tmp)						/* ajoute la cl�				*/
			MidiAddField( name, *tmp++);
		sprintf( buff, "%d", (int)ref);		/* convertit le refNum			*/
		tmp= buff;
		while( *tmp)						/* ajoute le refNum				*/
			MidiAddField( name, *tmp++);
		ret= MidiFileWriteEv( fd, name);	/* on �crit l'�v�nement			*/
		MidiFreeEv( name);					/* et on le lib�re				*/
	}
	else MidiFile_errno= MidiFileErrEvs;
	return ret;
}

/*--------------------------------------------------------------------------*/
 Boolean WriteTrackFormat2( register midiFILE *fd, MidiSeqPtr seq,
								  register short ref, short numPiste)
{
	register Boolean firstName= true, ret= true;
	register MidiEvPtr ev, lastWrite= nil;

	ev= seq->first;
	while( ev && ret)
	{
		if( RefNum(ev)== ref)
		{
			if( firstName && EvType(ev)== typeSeqName)
			{
				ret= WriteSeqName( fd, ev, ref, numPiste);
				firstName= false;
			}
			else ret= MidiFileWriteEv( fd, ev);
			lastWrite= ev;
		}
		ev= Link(ev);
	}
	if( ret)
		if( !lastWrite || EvType(lastWrite)!= typeEndTrack)
			ret= WriteEndTrack( fd, lastWrite);
	return ret;
}

/*--------------------------------------------------------------------------*/
 Boolean WriteTrackFormat1( register midiFILE *fd, MidiSeqPtr seq,
								  register short ref, short numPiste)
{
	register Boolean firstName= true, ret= true;
	register MidiEvPtr ev, lastWrite= nil;
	register short type;

	ev= seq->first;
	while( ev && ret)
	{
		if( RefNum(ev)== ref)
		{
			type= EvType(ev);
			if( !IsTempoMap( type))		/* n'appartient pas � la tempo map	*/
			{
				if( firstName && EvType(ev)== typeSeqName)
				{
					ret= WriteSeqName( fd, ev, ref, numPiste);
					firstName= false;
				}
				else ret= MidiFileWriteEv( fd, ev);
				lastWrite= ev;
			}
		}
		ev= Link(ev);
	}
	if( ret)
		if( !lastWrite || EvType(lastWrite)!= typeEndTrack)
			ret= WriteEndTrack( fd, lastWrite);
	return ret;
}

/*--------------------------------------------------------------------------*/
 Boolean WriteTempoMap( register midiFILE *fd, MidiSeqPtr seq)
{
	register MidiEvPtr ev, lastWrite= nil;
	register short type;
	register Boolean ret= true;

	ev= seq->first;
	while( ev && ret)
	{
		type= EvType(ev);
		if( IsTempoMap( type))				/* c'est un evt de la tempo map	*/
		{
				ret= MidiFileWriteEv( fd, ev);			/* on l'�crit		*/
				lastWrite= ev;
		}
		else if( !RefNum(ev))				/* sinon, si son refNum est 0	*/
		{
			ret= MidiFileWriteEv( fd, ev);				/* on l'�crit		*/
			lastWrite= ev;
		}
		ev= Link(ev);
	}
	if( ret)
		if( !lastWrite || EvType(lastWrite)!= typeEndTrack)
			ret= WriteEndTrack( fd, lastWrite);
	return ret;
}

/*--------------------------------------------------------------------------*/
 void AnalyseSeq( midiFILE *fd, MidiSeqPtr seq)
{
	register MidiEvPtr ev;
	register short type;

	ev= seq->first;
	if( fd->format== midifile1)						/* c'est un format 1	*/
	{
		while( ev)
		{
			type= EvType(ev);
			if( !IsTempoMap( type))		/* n'appartient pas � la tempo map	*/
				trackListe[ RefNum(ev)]= true;
			if (ev == Link (ev)){
				ev = 0;
			}
			else ev= Link(ev);

		}
	}
	else											/* c'est un format 2	*/
	{
		while( ev)
		{
			trackListe[ RefNum(ev)]= true;
			ev= Link(ev);
		}
	}
}

/*--------------------------------------------------------------------------*/
 void pCopy( register char *dest, register char * src)
{
	register short i;
	
	i= *src++;
	while( i--)
		*dest++= *src++;
	*dest= 0;
}

/*--------------------------------------------------------------------------*/
 void cCopy( register char *dest, register char * src)
{
	register short i = 0;
	
	while (src[i] != 0) {
		dest[i] = src[i];
		i++;
	}
	dest[i] = 0;	
}

/*--------------------------------------------------------------------------*/
 Boolean WriteTracks( register midiFILE *fd, register MidiSeqPtr seq)
{
	register short i=0, numPiste= 0;
	register Boolean ret= true;
	
	if( fd->format== midifile1)
	{
		if( MidiFileNewTrack( fd))
		{
			ret= WriteTempoMap( fd, seq); 
			if( !MidiFileCloseTrack && ret)
				ret= false;
			numPiste++;
			i++;
		}
		else ret= false;
	}
	for( i; i< maxTrack && ret; i++)
	{
		if( trackListe[i])
		{
			if( MidiFileNewTrack( fd))
			{
				if( fd->format== midifile1)
					ret= WriteTrackFormat1( fd, seq, i, numPiste);
				else
					ret= WriteTrackFormat2( fd, seq, i, numPiste);
				numPiste++;
				if( !MidiFileCloseTrack( fd) && ret)
					ret= false;
			}
			else ret= false;
		}
	}
	return ret;
}

/*--------------------------------------------------------------------------*/

 void SetLoadDates( MidiFileInfosPtr infos, MidiSeqPtr s)
{
	MidiEvPtr e;
	
	if( infos->timedef )  				/* temps SMPTE */
	{
		if (e = MidiNewEv(typeTempo)) {   // ajout d'un ev tempo (conversion au format Midi)
			Tempo(e) = infos->timedef * infos->clicks * 2000;
			Date(e) = 0;
			MidiAddSeq (s, e);
			infos->clicks = 500;
			infos->timedef = 0;  // format midi
		}
	}
}

/*--------------------------------------------------------------------------*/
/* 						Les fonctions d'interface Player						*/
/* 			attention : on r�cup�re des chaines pascal de Player				*/
/*--------------------------------------------------------------------------*/

int  EXPORT MidiFileSave( char * name, MidiSeqPtr seq, MidiFileInfosPtr infos)

{
	char Cname[256];
	midiFILE *fd;
	int ret= 0;        /* code de retour de la fonction	*/
	Boolean t;

	if( infos->format < 0 || infos->format > 2)
		return ErrMidiFileFormat;

	seq = WriteTempoAndTimeSign(seq);	/* Add Tempo and TimeSign info if not present */
	if (!seq) return MidiFileErrEvs;    /* plus d'evs MidiShare */

	//seq = WritePortInfo(seq);           /* Codage des Ports  SL 19/01/98 */
	//if (!seq) return MidiFileErrEvs;    /* plus d'evs MidiShare */

	InitTrackListe();					  /* init la liste des pistes � nil	*/
	cCopy (Cname, name);
 
	if( fd= MidiFileCreate( Cname, infos->format, infos->timedef, infos->clicks))
	{
		if( infos->format)						/* format 1 ou 2			*/
		{
			AnalyseSeq( fd, seq);				/* analyse de la s�quence 	*/
			t= WriteTracks( fd, seq);			/* �crit les pistes			*/
		}
		else t= MidiFileWriteTrack( fd, seq);	/* format 0					*/
		if( !t)									/* il y a eu une erreur		*/
		{
			if( MidiFile_errno!= MidiFileNoErr)
				ret= MidiFile_errno;
			else ret= ErrWrite;
		}
		MidiFileClose( fd);						/* referme le fichier		*/
	}
	else if( MidiFile_errno!= MidiFileNoErr)
		ret= MidiFile_errno;
	else ret= ErrOpen;
	return ret;
}


/*--------------------------------------------------------------------------*/
 void ReturnTimeInfos( register midiFILE *fd, MidiFileInfosPtr infos)
{
	register int t;
	
	if( fd->time & 0x8000)					/* temps smpte	*/
	{
		t= fd->time & 0x7fff;
		infos->timedef= t >> 8;				/* timedef		*/
		infos->clicks= t & 0xff;			/* clicks		*/
	}
	else									/* temps midi	*/
	{
		infos->timedef= 0;
		infos->clicks= fd->time;
	}
}

/*--------------------------------------------------------------------------*/
int  EXPORT MidiFileLoad( char * name, MidiSeqPtr seq, MidiFileInfosPtr infos)

{
	char Cname[256];
	register midiFILE *fd;					/* descripteur du fichier MidiFile	*/
	int ret= 0; 							/* code de retour de la fonction	*/
	
	cCopy (Cname, name);
	
	if( fd= MidiFileOpen( Cname, MidiFileRead))	/* ouvre le fichier 		*/
	{
				
		seq->first= seq->last= nil;				/* init de la s�quence  	*/
		if( ret= ReadTracks( fd, seq))			/* lit les pistes			*/
			MidiClearSeq( seq);					/* vide la seq si erreur	*/
		else									/* sinon					*/
		{
			DelEndTrack( seq);					/* supprime fins de pistes	*/
			if( infos)
			{
				infos->format= fd->format;
				ReturnTimeInfos( fd, infos);
				SetLoadDates( infos, seq);
				infos->tracks = fd->ntrks;     
			}
		}
		MidiFileClose( fd);						/* referme le fichier   	*/
	}
	else if( MidiFile_errno!= MidiFileNoErr)
		ret= MidiFile_errno;
	else ret= ErrOpen;
	return ret;
}

