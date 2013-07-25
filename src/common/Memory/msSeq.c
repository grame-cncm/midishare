/*
  MidiShare Project
  Copyright (C) Grame 1999-2011

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

  modifications history:
   [08-09-99] DF - adaptation to the new memory management

*/

#include "msSeq.h"
#include "msEvents.h"

/*==============================================================================
  External MidiShare functions implementation
  ============================================================================== */
MSFunctionType(MidiSeqPtr) MSNewSeq (lifo* freelist)
{
	MidiSeqPtr seq = (MidiSeqPtr)MSNewCell(freelist);
	if( seq) {
		seq->first= seq->last= 0;
		seq->undef1= seq->undef2= 0;
	}
	return seq;
}

/*______________________________________________________________________________*/
MSFunctionType(void) MSAddSeq (MidiSeqPtr s, MidiEvPtr e)
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
MSFunctionType(void) MSFreeSeq (MidiSeqPtr s, lifo* freelist)
{
	if( s) {
		MSClearSeq (s, freelist);
		MSFreeCell (s, freelist);
	}
}

/*______________________________________________________________________________*/
MSFunctionType(void) MSClearSeq (MidiSeqPtr s, lifo* freelist)
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
MSFunctionType(void) MSApplySeq (MidiSeqPtr s, ApplyProcPtr proc)
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
