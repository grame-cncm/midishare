// ===========================================================================
// The Player Library is Copyright (c) Grame, Computer Music Research Laboratory 
// 1996-1999, and is distributed as Open Source software under the Artistic License;
// see the file "Artistic" that is included in the distribution for details.
//
// Grame : Computer Music Research Laboratory
// Web : http://www.grame.fr/Research
// E-mail : MidiShare@rd.grame.fr
// ===========================================================================


// ===========================================================================
//	Umidi.cpp			    
// ===========================================================================
//
//	Midi utilities: various MidiShare sequence management functions
// 

#include "UMidi.h"

/*--------------------------------------------------------------------------*/

MidiEvPtr UMidi::NoteToKeyOff (MidiEvPtr e1)
{
	MidiEvPtr e2;
	EvType(e1) = typeKeyOn;
	
	if (e2 = MidiCopyEv(e1)) {
		EvType(e2) = typeKeyOff;
		Date(e2) = Date(e1)+MidiGetField(e1,2);
	}
	
	return e2;
}

/*--------------------------------------------------------------------------*/

MidiEvPtr UMidi::CheckEvType (MidiSeqPtr src, short type)
{
	MidiEvPtr e = FirstEv(src);

	while (e && (Date(e) == 0)){
		if (EvType(e) == type) return e;
		e = Link(e);
	}
	return 0;
}

/*--------------------------------------------------------------------------*/

MidiSeqPtr UMidi::TrsfNoteToKeyOn (MidiSeqPtr src)
{
	MidiSeqPtr tmp = MidiNewSeq();
	MidiEvPtr e,e1;

	e = First (src);
	if (tmp){
		while (e) {
			switch (EvType(e)) {
			
				case typeKeyOn:
					if (Vel(e) == 0) {
						EvType(e) = typeKeyOff;		// Type change
						Vel(e) = 64;
					}
					break;
			
				case typeNote:
					if (e1 = MidiCopyEv(e)){
						EvType(e1) = typeKeyOff;	// Type change
						Vel(e1) = 64;
						Date(e1) += Dur(e);
						MidiAddSeq(tmp,e1);
					}else {
						MidiFreeSeq(tmp);
						return 0;
					}
					EvType(e) = typeKeyOn;			// Type change
					break;
			}	
			e = Link(e);
		}
		UMidi::MixeSeq (tmp,src); // The first cell of "temp" will be desallocated in MixeSeq
		return src;
   }else
   		return 0;
  
}

/*--------------------------------------------------------------------------*/

MidiEvPtr  UMidi::AddSeq(MidiEvPtr e1, MidiEvPtr e2)
{
	MidiEvPtr next;
	
	while(next= Link(e1))				/* tant qu'une séquence n'est finie */
	{
		if(Date(next) <= Date(e2))		/* date inférieure dans la même seq */
			e1 = next;					/* rien à faire : on continue		*/
		else							/* sinon							*/
		{
			Link( e1) = e2;				/* on linke avec l'autre séquence	*/
			e1 = e2;						/* et on les inverse				*/
			e2 = next;
		}
	}
	if(Link(e1) = e2)			/* linke avec la fin de l'autre séquence 	*/
		while(Link(e2))
			e2 = Link(e2);
	return e2;					/* et renvoie le dernier evt de la séquence */
}

/*--------------------------------------------------------------------------*/

void  UMidi::MixeSeq(MidiSeqPtr src, MidiSeqPtr dest)
{
	MidiEvPtr firstSrc, firstDest;
	
	if(dest && src)							/* dest et src existent		*/
	{
		if(firstSrc= src->first)				/* src non vide				*/
		{
			if(!(firstDest = dest->first))		/* si destination est vide	*/
			{
				dest->first = firstSrc;			/* recopie du premier et	*/
				dest->last = src->last;			/* dernier evt de src		*/
			}
			else if(Date(firstSrc) < Date(firstDest))
												/* 1ier evt source précède	*/
			{									/* le 1ier evt destination	*/
				dest->first = firstSrc;			/* range dans destination	*/
				dest->last = AddSeq(firstSrc, firstDest);	/* et chainage	*/
			}
			else dest->last = AddSeq(firstDest, firstSrc);	/* et chainage	*/
		}

  		MidiFreeCell((MidiEvPtr) src);   
	}
}

/*--------------------------------------------------------------------------*/

void UMidi::FreeOneLine(MidiEvPtr cur)
{
	MidiEvPtr next;
	
	while (cur) {
		next = Link(cur);
		MidiFreeEv(cur);
		cur = next;
	}
}


/*----------------------------------------------------------------------------*/

void UMidi::SetRefnum(MidiSeqPtr src,short refnum)
{
	MidiEvPtr cur = FirstEv(src);
	
	while (cur) {
		RefNum(cur) = (Byte)refnum;
		cur = Link(cur);
	}
}

/*----------------------------------------------------------------------------*/

ULONG UMidi::LengthSeq (MidiSeqPtr s)
{
	ULONG n;
	MidiEvPtr e;

	for(e = FirstEv(s), n = 0; e; e = Link(e), n++){}
	return n;
}


/*--------------------------------------------------------------------------*/
/* 		supprime  toutes les fins de pistes 								*/
/*--------------------------------------------------------------------------*/
 void  UMidi::DelEndTrack( MidiSeqPtr seq)
{
	MidiEvPtr ev, prev, tmp;
	
	prev = 0;
	ev = FirstEv(seq);
	while(ev)
	{
		if(EvType(ev) == typeEndTrack) {			/* evt fin de piste		*/
				
			if(prev)								/* n'est pas le premier	*/
				Link(prev) = Link(ev);				/* mod. le chainage		*/
			else									/* sinon				*/
				FirstEv(seq) = Link(ev);	
				
			if (!Link(ev)) LastEv(seq) = prev;     /* si dernier, met à jour last */
				
			tmp = Link(ev);							/* sauve le suivant		*/
			MidiFreeEv(ev);							/* libère l'evt			*/
			ev = tmp;
				
		}else {
			prev = ev;
			ev = Link(ev);
		}
	}
}

/*--------------------------------------------------------------------------*/

MidiSeqPtr UMidi::BuildTrack(MidiSeqPtr s)  
{
	if (!s) return 0;
	
	ULONG len = UMidi::LengthSeq(s) * 5/2 + kMemoryLimit;
	if (MidiFreeSpace() < len) MidiGrowSpace(len);
	return (MidiFreeSpace() > len) ? UMidi::TrsfNoteToKeyOn (s): 0;
}

/*--------------------------------------------------------------------------*/

MidiSeqPtr UMidi::BuildAllTrack(MidiSeqPtr s)  
{
	if (!s) return 0;
	
	MidiSeqPtr s1;
	ULONG len = UMidi::LengthSeq(s) * 5/2 + kMemoryLimit;
	if (MidiFreeSpace() < len) MidiGrowSpace(len);
	
	if (MidiFreeSpace() > len && (s1 = UMidi::TrsfNoteToKeyOn (s))){
		DelEndTrack(s1);	
		return s1;
	}else {
		return 0;
	}
}


/*--------------------------------------------------------------------------*/

Boolean UMidi::Copy_AddSeq(MidiSeqPtr s, MidiEvPtr e) 
{
	MidiEvPtr copy = MidiCopyEv(e);
	
	if(copy) {
		Link(copy) = 0;
		MidiAddSeq(s, copy);
		return true;
	}else {
		return false;
	}
}

/*--------------------------------------------------------------------------*/

Boolean UMidi::IsEmpty(MidiSeqPtr s) { return (FirstEv(s) == 0);}

/*--------------------------------------------------------------------------*/

void UMidi::RemoveEv(MidiSeqPtr seq, MidiEvPtr e)
{
	MidiEvPtr cur, prev;
	
	prev = 0;
	cur = FirstEv(seq);
	while(cur)
	{
		if(cur == e) {			
				
			if(prev)								/* n'est pas le premier	*/
				Link(prev) = Link(cur);				/* mod. le chainage		*/
			else									/* sinon				*/
				FirstEv(seq) = Link(cur);	
				
			if (!Link(cur))	LastEv(seq) = prev;     /* si dernier, met à jour last */
			
			return;	
				
		}else {
			prev = cur;
			cur = Link(cur);
		}
	}
}
