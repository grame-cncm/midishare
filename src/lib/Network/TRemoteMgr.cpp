/*

  Copyright © Grame 2001
  Copyright © Mil Productions 2001

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
  grame@grame.fr

*/

#if !macintosh
#include <stdlib.h>
#include <stdio.h>
#endif

#include "TRemoteMgr.h"
#include "TMidiClient.h"

//____________________________________________________________
// UDP Remote management
//____________________________________________________________
TRemoteMgr::TRemoteMgr ()
{
	UDPStreamInitMthTbl (fMethods.outMethods);
	UDPParseInitMthTbl  (fMethods.inMethods);
}

//____________________________________________________________
TMidiRemote * TRemoteMgr::AddRemote (UPDParamsPtr params, RemoteCtrlParams * rp, short drvRef)
{
	rp->mth = &fMethods;
	TMidiRemote * r = new TMidiRemote (params, rp, drvRef);
	if (r) {
		RemotePtr cl = Add (r);
		if (cl) {
			cl->timeOut = MidiGetTime () + kInitTTL;
			return r;
		}
		delete r;
	}
	return 0;
}

//____________________________________________________________
short TRemoteMgr::RemoteCount ()
{ 
	short count = 0;
	RemotePtr r = Head();
	while (r) {
		TMidiRemote * m = (TMidiRemote *)r->remote;
		if (m->Opened()) count++;
		r = r->next;
	}
	return count; 
};

//____________________________________________________________
void TRemoteMgr::RcvAlarmLoop (short refNum)
{
	RemotePtr r = Head();
	while (r) {
		TMidiRemote * m = (TMidiRemote *)r->remote;
		if (m->CallRcvAlarm()) {
			m->RcvAlarm (refNum);
		}
		r = r->next;
	}
}				

//____________________________________________________________
void TRemoteMgr::SetMode (short mode, short refNum)
{
	RemotePtr r = Head();
	while (r) {
		TMidiRemote * m = (TMidiRemote *)r->remote;
		m->SetMode (mode, refNum);
		r = r->next;
	}
}

//____________________________________________________________
void TRemoteMgr::CheckTimeOut (unsigned long date)
{
	RemotePtr r = Head();
	RemotePtr prev = 0, next;
	while (r) {
		next = r->next;
		if (r->timeOut < date) {
			Remove (r->remote->GetID());
		}
		else prev = r;
		r = next;
	}
}

//____________________________________________________________
Boolean TRemoteMgr::Refresh (IPNum id, PeerTimesPtr times)
{
	RemotePtr r = Head();
	while (r) {
		if (r->remote->GetID() == id) {
			TMidiRemote * m = (TMidiRemote *)r->remote;
			r->timeOut = times->local + kRemoteTTL;
			m->CheckLatency (times);
			return true;
		}
		r = r->next;
	}
	return false;
}

//____________________________________________________________
TMidiRemote * TRemoteMgr::FindOutSlot (short slotRef)
{
	RemotePtr r = Head();
	while (r) {
		TMidiRemote * m = (TMidiRemote *)r->remote;
		if (m->OutSlotRef() == slotRef) return m;
		r = r->next;
	}
	return 0;
}

//____________________________________________________________
// Generic Remote List management
//____________________________________________________________
TRemote * TRemoteList::Find (IPNum id)
{
	RemotePtr prev, r = Find (id, &prev);
	return r ? r->remote : 0;
}

//____________________________________________________________
void TRemoteList::Remove (IPNum id)
{
// this method is not completely safe: the final CAS call
// don't check that r->next is still a valid pointer
// however, it should not be a problem in the context of the
// method invocation: only the application Idle thread should
// remove remote clients
	RemotePtr prev, r, * adr;
	do {
		r = Find(id, &prev);
		if (!r) break;
		adr = prev ? &prev->next : (RemotePtr *)&fRemote.top;
	} while (!CAS (adr, r, r->next));
	if (r) {
		Delete (r);
	}
}

//____________________________________________________________
RemotePtr TRemoteList::Add (TRemote * remote)
{
	RemotePtr cl = Allocate ();
	if (cl) {
		cl->remote = remote;
		lfpush (&fRemote, (cell *)cl);
	}
	return cl;
}

//____________________________________________________________
void TRemoteList::Clear ()
{
	RemotePtr cl = (RemotePtr)lfpop (&fRemote);
	while (cl) {
		Delete (cl);
		cl = (RemotePtr)lfpop (&fRemote);
	}
}

//____________________________________________________________
void TRemoteList::Delete (RemotePtr r)
{
	delete r->remote;
	Free (r);
}

//____________________________________________________________
RemotePtr TRemoteList::Find (IPNum id, RemotePtr *previous)
{
	RemotePtr r = Head();
	*previous = 0;
	while (r) {
		if (r->remote->GetID() == id) return r;
		*previous = r;
		r = r->next;
	}
	return 0;
}

//____________________________________________________________
void TRemoteList::Free (RemotePtr r)	
{ 
#if macintosh
	DisposePtr ((Ptr)r);
#else
	free (r);
#endif 
}

//____________________________________________________________
RemotePtr TRemoteList::Allocate() 
{
#if macintosh
	return (RemotePtr)NewPtr (sizeof(RemoteList)); 
#else
	return (RemotePtr)malloc (sizeof(RemoteList)); 
#endif 
}
