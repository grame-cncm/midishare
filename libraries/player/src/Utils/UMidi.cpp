/*

  Copyright © Grame 1996-2004

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
	
	if ((e2 = MidiCopyEv(e1))) {
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
// Replace typeNote event by  typeKeyOn and typeKeyOff events

MidiSeqPtr UMidi::TrsfNoteToKeyOn (MidiSeqPtr dest)
{
	MidiEvPtr e,e1,ei,n;

	e = First (dest);
	while (e) {
		if (EvType(e) == typeNote) {
			// A typeKeyOff ev is build and add to seq
			if (e1 = MidiCopyEv(e)){
				EvType(e1) = typeKeyOff;	// Type change
				Vel(e1) = 64;				// velocity
				Date(e1) = Date(e1) + Dur(e); // Date + Duration
				// insert in dest after  e
				ei = e;
				while (ei)
				{
					n = Link(ei);
					if(!n || Date(e1) <= Date(n))
					{
						// Insert e1 after ei and before n
						Link(ei) = e1;
						Link(e1) = n;
						break;
					}
					ei = n;
				}
				if (!n) Last (dest) = e1;

			}else {
				return 0;
			}
			// typeNote is replaced by a typeKeyOn
			EvType(e) = typeKeyOn;	// Type change
		}	
		e = Link(e);
	}
	return dest;
}

/*--------------------------------------------------------------------------*/

MidiEvPtr  UMidi::AddSeq(MidiEvPtr e1, MidiEvPtr e2)
{
	MidiEvPtr next;
	
	while((next= Link(e1)))				/* tant qu'une séquence n'est finie */
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
	if((Link(e1) = e2))			/* linke avec la fin de l'autre séquence 	*/
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
		if((firstSrc= src->first))				/* src non vide				*/
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
