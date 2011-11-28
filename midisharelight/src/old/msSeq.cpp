/*

  Copyright (c) Grame 1999, 2011

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

#include "msSeq.h"
#include "msEvents.h"

#include <iostream>
using namespace std;
/*==============================================================================
  External MidiShare functions implementation
  ============================================================================== */
MidiSeqPtr MSNewSeq (lifo* freelist)
{
	MidiSeqPtr seq = (MidiSeqPtr)MSNewCell(freelist);
	if( seq) {
		seq->first= seq->last= 0;
		seq->undef1= seq->undef2= 0;
	}
	return seq;
}

/*______________________________________________________________________________*/
void MSAddSeq (MidiSeqPtr s, MidiEvPtr e)
{
	MidiEvPtr current;
	unsigned long date;
	
	if (s && e) {
		date= Date(e);
		current= s->first;
		if( !current) {							/* the sequence is empty   */
         	Link(e)= 0;
			s->first= s->last= e;
		}
		else if( date < Date( current)) {		/* insert at the beginning */
			Link(e)= current;
			s->first= e;
		}
		else if( date >= Date( s->last)) {		/* insert at the end       */
         	Link(e)= 0;
			Link(s->last)= e;
			s->last= e;
		}
		else do {								/* insert at the middle    */
			if( Date(Link(current)) > date) {
				Link(e)= Link(current);
				Link(current)= e;
				break;
			}
			current= Link(current);
		} while( current);
	}
}

/*______________________________________________________________________________*/
void MSFreeSeq (MidiSeqPtr s, lifo* freelist)
{
	if( s) {
		MSClearSeq (s, freelist);
		MSFreeCell (s, freelist);
	}
}

/*______________________________________________________________________________*/
void MSClearSeq (MidiSeqPtr s, lifo* freelist)
{
	if ( s ) {
		MidiEvPtr next, e = s->first;
		while (e) {
			next = Link(e);
			MSFreeEv (e, freelist);
			e = next;
		}
		s->first = s->last = 0;
	}
}

/*______________________________________________________________________________*/
void MSApplySeq (MidiSeqPtr s, ApplyProcPtr proc)
{
	MidiEvPtr e, next;
	if( s && proc) {
		e= s->first;
		while ( e ) {
			next = Link(e);
			(*proc)(e);
			e= next;
		}
	}
}
