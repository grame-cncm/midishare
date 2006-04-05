/*

  Copyright © Grame 1996-2006

  This library is free software; you can redistribute it and modify it under 
  the terms of the GNU Library General Public License as published by the 
  Free Software Foundation version 2 of the License, or any later version.

  This library is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public License 
  for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

  Grame Research Laboratory, 9, rue du Garet 69001 Lyon - France
  research@grame.fr

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
 * interface des fonctions de lecture et d'écriture de fichier
 * au format MidiFile.
 *
 * ce fichier doit etre linke avec la librairie MidiFile
 *
 * les fonctions d'interface sont les suivantes :
 *		int MidiFileSave(StringPtr filename, ScorePtr sc, short format, short timeDef, short nTicks)
 *      int MidiFileLoad(StringPtr filename, ScorePtr sc, short *format, short *timeDef, short *nTicks)
 *
 * lors de la sauvegarde, et pour un format 1 ou 2, le refNum des événements sert 
 * à les dispatcher sur les différentes pistes (soit donc 256 pistes possibles). 
 * Si ce numéro de référence ne correspond pas au numéro de la piste courante,
 * il est stocké dans un événement de type SeqName (créé s'il n'existe pas déjà).
 * Ce numéro de référence est restauré lors de la relecture du fichier.
 *
 * Spécifications particulières au format 1:
 * - un numéro de référence null envoie les événements dans la tempo map.
 * - quelque soit leur numéro de référence, les événements suivants sont
 *   également envoyés dans la tempo map :
 *							typeCopyright
 *							typeMarker
 *							typeTempo
 *							typeSMPTEOffset
 *							typeTimeSign
 *							typeKeySign
 *  à la relecture, il n'est donc pas possible de restaurer leur numéro de référence
 *  si celui-ci diffère de 0.
 *	
 * Ces fonctions retournent nil si tout s'est bien passé, un code d'erreur négatif
 * si l'erreur concerne MidiShare ou le format MidiFile, un code d'erreur positif
 * si l'erreur concerne la lecture, l'écriture du fichier ou le système.
 *
 * GRAME Friday, March 27, 1992 D.F.
 *
/*----------------------------------------------------------------------------*/

#include "Tmtrack.h"

#ifdef __Macintosh__
	#include <CType.h>
	#ifdef __MacOS9__
		#include <Files.h>
	#endif
	#include <StdLib.h>
	#include <String.h>
#endif

#ifdef __Windows__
	#include <StdLib.h>
	#include <String.h>
#endif

#ifdef __linux__
	#include <string.h>
#endif

/*----------------------------------------------------------------------------*/

char *Player= "PLAYER ref:";

/*--------------- les types stockés dans la tempo map ----------------------*/

#define IsTempoMap(t) ((t)==typeCopyright || (t)==typeMarker || ((t)>=typeTempo && (t)<=typeKeySign))

/*--------------- les variables globales -----------------------------------*/

char trackListe[maxTrack];					/* liste des pistes à écrire 	*/

/*--------------------------------------------------------------------------*/
/*																			*/
/*   fonctions qui positionnent le numéro de référence des événement en		*/
/*   fonction du numéro de piste ou des infos contenues dans l'événement	*/
/*   SeqName de la piste.													*/
/*																			*/
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
 void SetSeqRef(MidiSeqPtr seq,  short refNum)
{
	 MidiEvPtr ev;
	
	ev= seq->first;
	while (ev)
	{
		RefNum(ev)= refNum;
		ev= Link(ev);
	}
}

/*--------------------------------------------------------------------------*/
 char *GetBeginKey(char *buff)
{
	 char *start= nil;
	 long k, l;
	
	k= *(long *)Player;
	l= strlen(Player);
	while (*buff)
	{
		if (*(long *)buff == k && !strncmp(buff, Player, l))
		{
			start= buff;
			break;
		}
		else buff++;
	}
	return start;
}

/*--------------------------------------------------------------------------*/
 MidiEvPtr RestoreSeqName(char *buff)
{
	MidiEvPtr ev= nil;
	 char *tmp;
	
	if (tmp= GetBeginKey(buff))			/* recherche le début de la clé */
	{
		if (tmp!= buff && *(tmp-1) == ' ')		/* si un espace le précède	*/
			tmp--;								/* on pointe un char avant	*/
		*tmp = 0;								/* fin de chaine			*/
		tmp= buff;								/* pointe le début			*/
		if (ev= MidiNewEv(typeSeqName))		/* nouvel événement			*/
		{
			while (*tmp)						/* recopie de l'ancien		*/
				MidiAddField(ev, *tmp++);		/* octet par octet			*/
		}
	}
	return ev;
}

/*--------------------------------------------------------------------------*/
/* renvoie le premier événement de type SeqName ou TrackName de la séquence */
 MidiEvPtr GetTrackName(MidiSeqPtr seq, MidiEvPtr *prec)
{
	 MidiEvPtr ev, p= nil;
	
	ev= seq->first;
	while (ev)
	{
		if (EvType(ev) == typeSeqName)
		{
			if (prec) *prec= p;
			break;
		}
		p= ev;
		ev= Link(ev);
	}
	return ev;
}

/*--------------------------------------------------------------------------*/
/* renvoie le numéro de référence d'un evt SeqName transféré dans un buffer */
 short GetEvRef(char *buff, int keyLen, short numPiste)	
{
	short refNum, i = 0;
	char *tmp;
	char strRef[4];
	
	refNum = numPiste;
	if (tmp = GetBeginKey(buff))			/* recherche le début de la clé */
	{
		tmp += keyLen;							/* tmp pte le numéro de port*/		
		while (isdigit(*tmp) && i<3)
			strRef[i++] = *tmp++;				/* on le recopie 			*/
		strRef[i] = 0;							/* fin de chaine 			*/
		refNum= atoi(strRef);					/* conversion en numérique	*/
	}
	return refNum;
}

/*--------------------------------------------------------------------------*/
/*   	renvoie le refNum d'une seq	(par défaut: numéro de piste)			*/
 short GetSeqRef(MidiSeqPtr seq, short numPiste)
{
	MidiEvPtr ev, ori;
	short refNum;
	MidiEvPtr prec= nil;
	int i, n, l;
	char buff[512];

	refNum = numPiste;
	if (ev = GetTrackName(seq, &prec))
	{
		n= MidiCountFields(ev);				/* nbre de champs de l'evt	*/
		l= strlen(Player);						/* longueur + que minimale	*/
		if (n < 511)
		{
			for(i = 0; i < n; i++)					/* lit le contenu de l'evt	*/
				buff[i] = MidiGetField(ev, i);
			buff[i] = 0;									/* fin de chaine	*/
			refNum= GetEvRef(buff, l, numPiste);
			if (!strncmp(buff, Player,l) && !prec)		/* evt ajouté		*/
			{
				seq->first= Link(ev);			/* supprime l'evt de la seq */
				if (!seq->first)
					seq->last = nil;
				MidiFreeEv(ev);				/* et le libère				*/
			}
			else if (ori= RestoreSeqName(buff))		/* evt modifié		*/
			{
				Date(ori) = Date(ev);
				Link(ori) = Link(ev);			/* on restaure l'original	*/
				if (prec) Link(prec)= ori;		/* on le remplace dans		*/
				else seq->first = ori;			/* la séquence				*/
				MidiFreeEv(ev);				/* libère l'evt modifié		*/
			}
		}
	}
	return refNum;
}

/*--------------------------------------------------------------------------*/
/* 		supprime les fins de pistes qui sont au milieu de la séquence		*/
/*--------------------------------------------------------------------------*/
 void DelEndTrack(MidiSeqPtr seq)
{
	 MidiEvPtr ev, prev, tmp;
	
	prev= nil;
	ev= seq->first;
	while (ev)
	{
		if (EvType(ev) == typeEndTrack && Link(ev))	/* evt fin de piste		*/
		{											/* n'est pas le dernier */
			if (prev)								/* n'est pas le premier	*/
				Link(prev) = Link(ev);				/* mod. le chainage		*/
			else									/* sinon				*/
				seq->firs t= Link(ev);				/* suivant= premier		*/
			tmp = Link(ev);							/* sauve le suivant		*/
			MidiFreeEv(ev);							/* libère l'evt			*/
			ev = tmp;								/* evt courant=suivant	*/
		}
		else 
		{
			prev = ev;
			ev = Link(ev);
		}
	}
}

/*--------------------------------------------------------------------------*/
/*    					fonctions de mixage des séquences					*/
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
 MidiEvPtr AddSeq(MidiEvPtr e1, MidiEvPtr e2)
{
	 MidiEvPtr next;
	
	while (next = Link(e1))				/* tant qu'une séquence n'est finie */
	{
		if (Date(next) <= Date(e2))		/* date inférieure dans la même seq */
			e1 = next;					/* rien à faire : on continue		*/
		else							/* sinon							*/
		{
			Link(e1) = e2;				/* on linke avec l'autre séquence	*/
			e1 = e2;						/* et on les inverse				*/
			e2 = next;
		}
	}
	if (Link(e1) = e2)			/* linke avec la fin de l'autre séquence 	*/
		while (Link(e2))
			e2 = Link(e2);
	return e2;					/* et renvoie le dernier evt de la séquence */
}

/*--------------------------------------------------------------------------*/
 void MixeSeq(MidiSeqPtr src, MidiSeqPtr dest)
{
	 MidiEvPtr firstSrc, firstDest;
	
	if (dest && src)							/* dest et src existent		*/
	{
		if (firstSrc = src->first)				/* src non vide				*/
		{
			if (!(firstDest = dest->first))		/* si destination est vide	*/
			{
				dest->first = firstSrc;			/* recopie du premier et	*/
				dest->last = src->last;			/* dernier evt de src		*/
			}
			else if (Date(firstSrc) < Date(firstDest))
												/* 1ier evt source précède	*/
			{									/* le 1ier evt destination	*/
				dest->first = firstSrc;			/* range dans destination	*/
				dest->last = AddSeq(firstSrc, firstDest);	/* et chainage	*/
			}
			else dest->last = AddSeq(firstDest, firstSrc);	/* et chainage	*/
		}
	}
}

/*----------------------------------------------------------------------------
 Replace typeNote event by  typeKeyOn and typeKeyOff events
 Necessary to insert typePort event to prefix typekeyOff event.
 On return, true if succes, false if MidiShare memory error.
-----------------------------------------------------------------------------*/
static Boolean TrsfNoteToKeyOn (MidiSeqPtr dest)
{
	MidiEvPtr e,e1,ei,n;

	e = First (dest);
	while (e) {
			if (EvType(e) == typeNote) {
				// A typeKeyOff ev is build and add to seq
				if (e1 = MidiCopyEv(e)) {
					EvType(e1) = typeKeyOff;	// Type change
					Vel(e1) = 64;			// velocity
					Date(e1) = Date(e1) + Dur(e); // Date + Duration
					// insert in dest after  e
					ei = e;
					while (ei)
					{
						n = Link(ei);
						if (!n || Date(e1) <= Date(n))
						{
							// Insert e1 after ei and before n
							Link(ei) = e1;
							Link(e1) = n;
							break;
						}
						ei = n;
					}
					if (!n) Last (dest) = e1;

				} else {
					return false;
				}
				// typeNote is replaced by a typeKeyOn
				EvType(e) = typeKeyOn;			// Type change
			}	
			e = Link(e);
	}
	return true;
}


/*--------------------------------------------------------------------------*/
/*				Codage de tempo et TimeSign 								*/
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
static MidiEvPtr CheckEvType (MidiSeqPtr src, short type)
{
	MidiEvPtr e = FirstEv(src);

	while (e && (Date(e) == 0)) {
		if (EvType(e) == type) return e;
		e = Link(e);
	}
	return 0;
}

/*--------------------------------------------------------------------------*/
static MidiSeqPtr WriteTempoAndTimeSign(MidiSeqPtr seq) 
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
		} else {
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
		} else {
			return 0;
		}
	}
	
	return seq;
}


/*--------------------------------------------------------------------------*/
/*				Codage de l'info de port									*/
/*--------------------------------------------------------------------------*/
static void SetSeqPort(MidiSeqPtr seq)
{
	MidiEvPtr ev, prev, tmp;
	int port = 0;
	prev = nil;
	ev = seq->first;
	
	while (ev)
	{
		if (EvType(ev) == typePortPrefix && Link(ev))	/* evt port 			*/
		{												/* n'est pas le dernier */
			port = PortPrefix(ev);
			if (prev)									/* n'est pas le premier	*/
				Link(prev) = Link(ev);					/* mod. le chainage		*/
			else										/* sinon				*/
				seq->first = Link(ev);					/* suivant= premier		*/
			tmp = Link(ev);								/* sauve le suivant		*/
			MidiFreeEv(ev);								/* libere l'evt			*/
			ev = tmp;									/* evt courant=suivant	*/
		}
		else 
		{	Port(ev) = port;  							/* attribue le port courant */
			prev = ev;
			ev = Link(ev);
		}
	}
}

/*--------------------------------------------------------------------------*/
void UseTrack(MidiSeqPtr seq, MidiSeqPtr dest, int i)
{
	SetSeqRef(seq, GetSeqRef(seq, i));	/* restitue le refnum		*/
	SetSeqPort(seq);					/* restitue le Port         */
	MixeSeq(seq, dest);				/* mixe avec la seq. dest	*/
	seq->first = seq->last = nil;			/* sauvegarde des evts		*/
	MidiFreeSeq(seq);					/* libère la sequence		*/
}

/*--------------------------------------------------------------------------*/
int TryToReadTrack(midiFILE *fd, MidiSeqPtr dest, int i)
{
	MidiSeqPtr seq;
	int ret= 0;

	if (seq = MidiFileReadTrack(fd)) {		/* lecture de la piste		*/
		UseTrack(seq, dest, i);
	} else {
		if (MidiFile_errno == MidiFileErrEvs) {
			while ((seq == 0) && (MidiGrowSpace(10000) > 0)) {
				MidiFileCloseTrack(fd);
				MidiFileChooseTrack(fd,i);
				seq = MidiFileReadTrack(fd);
		
			}
			if (seq) {UseTrack(seq, dest, i);}
		}	
			
	if (!seq)
		if (MidiFile_errno != MidiFileNoErr)
			ret = MidiFile_errno;	// Erreur MidiShare
		else		
			ret = ErrRead;		// Erreur de lecture
	}
	return ret;
}

/*--------------------------------------------------------------------------*/
 int ReadTracks(midiFILE *fd, MidiSeqPtr dest)
{
	int i, ret= 0;
	
	for (i = 0; i< fd->ntrks && !ret; i++)
	{
		ret = TryToReadTrack(fd, dest,i);
	}
	return ret;
}


/*--------------------------------------------------------------------------*/
/*																			*/
/*			les fonctions d'analyse et d'écriture d'une score				*/
/*																			*/
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
 void InitTrackListe(void)
{
	int i;
	char *tmp;
	
	tmp = trackListe;
	for(i = 0; i < maxTrack; i++)
		*tmp++= false;
}

/*--------------------------------------------------------------------------*/
 Boolean WriteEndTrack(midiFILE *fd, MidiEvPtr prev)
{
	MidiEvPtr ev;
	Boolean ret = false;
	MidiSeqPtr seq;
	
	if (ev= MidiNewEv(typeEndTrack))		/* alloue un evt fin de piste	*/
	{
		Link(ev)= nil;
		if ((seq=fd->keyOff) && seq->first)	/* il reste des keyOff à écrire */
		{
			Date(ev)= Date(seq->last);
			Link(seq->last)= ev;			/* on place l'evt endTrack au 	*/
			seq->last= ev;					/* bout de la séquence			*/
			ret= true;
		}
		else
		{
			if (prev)						/* s'il y a un événement avant	*/
			{
				Date(ev)= Date(prev);		/* positionne sa date			*/
				if (EvType(prev)== typeNote)/* si le précédent est une note */
					Date(ev)+= Dur(prev);	/* rajoute la durée de la note	*/
			}
			else Date(ev)= 0;
			ret= MidiFileWriteEv(fd, ev);	/* écrit l'événement			*/
			MidiFreeEv(ev);				/* et le libère					*/
		}
	}
	else MidiFile_errno= MidiFileErrEvs;
	return ret;
}

/*--------------------------------------------------------------------------*/
 Boolean WriteSeqName(midiFILE *fd, MidiEvPtr ev, short ref, short numPiste)
{
	MidiEvPtr name;
	Boolean ret = false;
	char *tmp;
	char buff[6];
	
	if (ref == numPiste)						/* refNum égal au num de piste	*/
		ret = MidiFileWriteEv(fd, ev);		/* on écrit l'événement tel quel*/
	else if (name = MidiCopyEv(ev))			/* sinon on le copie			*/
	{
		/* ajoute espace si l'ev n'est pas vide */
		if (MidiCountFields(ev)) MidiAddField(name, ' ');			
		tmp = Player;
		while (*tmp)						/* ajoute la clé				*/
			MidiAddField(name, *tmp++);
		sprintf(buff, "%d", (int)ref);		/* convertit le refNum			*/
		tmp = buff;
		while (*tmp)						/* ajoute le refNum				*/
			MidiAddField(name, *tmp++);
		ret = MidiFileWriteEv(fd, name);	/* on écrit l'événement			*/
		MidiFreeEv(name);					/* et on le libère				*/
	}
	else MidiFile_errno = MidiFileErrEvs;
	return ret;
}

/*--------------------------------------------------------------------------*/
 Boolean WriteTrackFormat2(midiFILE *fd, MidiSeqPtr seq,
								   short ref, short numPiste)
{
	Boolean firstName= true, ret = true;
	MidiEvPtr ev, lastWrite = nil;

	ev = seq->first;
	while (ev && ret)
	{
		if (RefNum(ev) == ref)
		{
			if (firstName && EvType(ev)== typeSeqName)
			{
				ret = WriteSeqName(fd, ev, ref, numPiste);
				firstName = false;
			}
			else ret = MidiFileWriteEv(fd, ev);
			lastWrite = ev;
		}
		ev = Link(ev);
	}
	if (ret)
		if (!lastWrite || EvType(lastWrite) != typeEndTrack)
			ret= WriteEndTrack(fd, lastWrite);
	return ret;
}

/*--------------------------------------------------------------------------*/
 Boolean WriteTrackFormat1(midiFILE *fd, MidiSeqPtr seq,
								   short ref, short numPiste)
{
	Boolean firstName = true, ret = true;
	MidiEvPtr ev, lastWrite = nil;
	short type;
	ev = seq->first;
	
	while (ev && ret)
	{
		if (RefNum(ev)== ref)
		{
			type= EvType(ev);
			if (!IsTempoMap(type))		/* n'appartient pas à la tempo map	*/
			{
				if (firstName && EvType(ev)== typeSeqName)
				{
					ret= WriteSeqName(fd, ev, ref, numPiste);
					firstName= false;
				}
				else ret= MidiFileWriteEv(fd, ev);
				lastWrite= ev;
			}
		}
		ev= Link(ev);
	}
	if (ret)
		if (!lastWrite || EvType(lastWrite)!= typeEndTrack)
			ret= WriteEndTrack(fd, lastWrite);
	return ret;
}

/*--------------------------------------------------------------------------*/
 Boolean WriteTempoMap(midiFILE *fd, MidiSeqPtr seq)
{
	MidiEvPtr ev, lastWrite = nil;
	short type;
	Boolean ret = true;
	ev = seq->first;
	
	while (ev && ret)
	{
		type= EvType(ev);
		if (IsTempoMap(type))				/* c'est un evt de la tempo map	*/
		{
			ret = MidiFileWriteEv(fd, ev);	/* on l'écrit		*/
			lastWrite = ev;
		}
		else if (!RefNum(ev))				/* sinon, si son refNum est 0	*/
		{
			ret = MidiFileWriteEv(fd, ev);	/* on l'écrit		*/
			lastWrite = ev;
		}
		ev= Link(ev);
	}
	if (ret)
		if (!lastWrite || EvType(lastWrite) != typeEndTrack)
			ret = WriteEndTrack(fd, lastWrite);
	return ret;
}

/*--------------------------------------------------------------------------*/
 void AnalyseSeq(midiFILE *fd, MidiSeqPtr seq)
{
	MidiEvPtr ev;
	short type;
	ev = seq->first;
	
	if (fd->format== midifile1)			/* c'est un format 1	*/
	{
		while (ev)
		{
			type= EvType(ev);
			if (!IsTempoMap(type))		/* n'appartient pas à la tempo map	*/
				trackListe[ RefNum(ev)]= true;
			ev= Link(ev);
		}
	}
	else								/* c'est un format 2	*/
	{
		while (ev)
		{
			trackListe[ RefNum(ev)]= true;
			ev= Link(ev);
		}
	}
}


/*--------------------------------------------------------------------------*/
static Boolean AddPortPrefix(MidiSeqPtr seq)
{
	MidiEvPtr ev1,ev2,prev;
	short portTable[maxTrack];
    int i;
    ev 1= seq->first;
    prev = 0;
    
    for (i = 0; i<maxTrack; i++) portTable[i] = 0;
    
    if (MidiGetVersion() >= 185) 
    {
    	while (ev1)
    	{
		   	/* ecrit un changement de port */
			if (Port(ev1) != portTable[RefNum(ev1)]) {
				portTable[RefNum(ev1)] = Port(ev1);
				
				if  ((ev2= MidiNewEv(typePortPrefix))) {
					PortPrefix(ev2) = Port(ev1);
					Port(ev2) = Port(ev1);
					RefNum(ev2) = RefNum(ev1); 	/* sur la meme piste */
					Date(ev2) = Date(ev1);
					if (prev) 
						Link(prev) = ev2;
					else
						seq->first = ev2;
					Link(ev2) = ev1;
				}else
					return false;
			}
			
			prev = ev1;
       		ev1= Link(ev1);
		}
	}
  
  	return true;
}

/*--------------------------------------------------------------------------*/
 void pCopy(char *dest,  char * src)
{
	short i;
	
	i = *src++;
	while (i--)
		*dest++= *src++;
	*dest= 0;
}

/*--------------------------------------------------------------------------*/
 void cCopy(char *dest,  char * src)
{
	short i = 0;
	
	while (src[i] != 0) {
		dest[i] = src[i];
		i++;
	}
	dest[i] = 0;	
}

/*--------------------------------------------------------------------------*/
 Boolean WriteTracks(midiFILE *fd, MidiSeqPtr seq)
{
	 short i = 0, numPiste = 0;
	 Boolean ret = true;
	
	if (fd->format == midifile1)
	{
		if (MidiFileNewTrack(fd))
		{
			ret= WriteTempoMap(fd, seq); 
			if (!MidiFileCloseTrack(fd) || !ret)
				ret = false;
			numPiste++;
			i++;
		}
		else ret= false;
	}
	for(i; i< maxTrack && ret; i++)
	{
		if (trackListe[i])
		{
			if (MidiFileNewTrack(fd))
			{
				if (fd->format== midifile1)
					ret= WriteTrackFormat1(fd, seq, i, numPiste);
				else
					ret= WriteTrackFormat2(fd, seq, i, numPiste);
				numPiste++;
				if (!MidiFileCloseTrack(fd) || !ret)
					ret= false;
			}
			else ret= false;
		}
	}
	return ret;
}

/*--------------------------------------------------------------------------*/

 void SetLoadDates(MidiFileInfosPtr infos, MidiSeqPtr s)
{
	MidiEvPtr e;
	
	if (infos->timedef )  				  /* temps SMPTE */
	{
		if (e = MidiNewEv(typeTempo)) {   /* ajout d'un ev tempo (conversion au format Midi) */
			Tempo(e) = 500000000 / (infos->timedef * infos->clicks);
			Date(e) = 0;
			MidiAddSeq (s, e);
			infos->clicks = 500;
			infos->timedef = 0;  /* format midi */
		}
	}
}

/*--------------------------------------------------------------------------*/
/* 						Les fonctions d'interface Player						*/
/* 			attention : on récupère des chaines pascal de Player				*/
/*--------------------------------------------------------------------------*/

int  EXPORT MidiFileSave(char * name, MidiSeqPtr seq, MidiFileInfosPtr infos)
{
	char Cname[256];
	midiFILE *fd;
	int ret= 0;        /* code de retour de la fonction	*/
	Boolean t;

	if (infos->format < 0 || infos->format > 2)
		return ErrMidiFileFormat;

	seq = WriteTempoAndTimeSign(seq);	/* Add Tempo and TimeSign info if not present */
	if (!seq) return MidiFileErrEvs;    /* plus d'evs MidiShare */

	InitTrackListe();					  /* init la liste des pistes à nil	*/
	cCopy (Cname, name);
	
	/* codage des ports */
	if (!TrsfNoteToKeyOn(seq)) {		/* transformation des notes an couple KeyOn/KeyOff 	*/
		return MidiFileErrEvs;
	}
	if (!AddPortPrefix(seq)) {			/* ajout d'evenements PortPrefix a chaque changement de port */
		return MidiFileErrEvs;
	}
 
	if (fd= MidiFileCreate(Cname, infos->format, infos->timedef, infos->clicks))
	{
		if (infos->format)						/* format 1 ou 2			*/
		{
			AnalyseSeq(fd, seq);				/* analyse de la séquence 	*/
			t= WriteTracks(fd, seq);			/* écrit les pistes			*/
		}
		else t= MidiFileWriteTrack(fd, seq);	/* format 0					*/
		if (!t)									/* il y a eu une erreur		*/
		{
			if (MidiFile_errno!= MidiFileNoErr)
				ret= MidiFile_errno;
			else ret= ErrWrite;
		}
		MidiFileClose(fd);						/* referme le fichier		*/
	}
	else if (MidiFile_errno!= MidiFileNoErr)
		ret= MidiFile_errno;
	else ret= ErrOpen;
	return ret;
}


/*--------------------------------------------------------------------------*/
 void ReturnTimeInfos(midiFILE *fd, MidiFileInfosPtr infos)
{
	int t;
	
	if (fd->time & 0x8000)					/* temps smpte	*/
	{
		t = fd->time & 0x7fff;
		infos->timedef = t >> 8;				/* timedef		*/
		infos->clicks = t & 0xff;			/* clicks		*/
	}
	else									/* temps midi	*/
	{
		infos->timedef = 0;
		infos->clicks = fd->time;
	}
}

/*--------------------------------------------------------------------------*/
int  EXPORT MidiFileLoad(char * name, MidiSeqPtr seq, MidiFileInfosPtr infos)
{
	char Cname[256];
	midiFILE *fd;					/* descripteur du fichier MidiFile	*/
	int ret = 0; 					/* code de retour de la fonction	*/
	
	cCopy(Cname, name);
	
	if (fd = MidiFileOpen(Cname, MidiFileRead))	/* ouvre le fichier 		*/
	{
				
		seq->first = seq->last = nil;			/* init de la séquence  	*/
		if (ret = ReadTracks(fd, seq))			/* lit les pistes			*/
			MidiClearSeq(seq);					/* vide la seq si erreur	*/
		else									/* sinon					*/
		{
			DelEndTrack(seq);					/* supprime fins de pistes	*/
			if (infos)
			{
				infos->format= fd->format;
				ReturnTimeInfos(fd, infos);
				SetLoadDates(infos, seq);
				infos->tracks = fd->ntrks;     
			}
		}
		MidiFileClose(fd);						/* referme le fichier   	*/
	}
	else if (MidiFile_errno != MidiFileNoErr)
		ret = MidiFile_errno;
	else ret = ErrOpen;
	return ret;
}

