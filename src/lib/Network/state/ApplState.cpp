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


#include "StatesManager.h"
#include "TInetAddress.h"

#if macintosh
#define kApplCnxID 'cnAp'
#else
#define kApplCnxID	1
#endif

//____________________________________________________________
// Application connections state
//____________________________________________________________
ApplState::ApplState (short refNum)
{
	fIn = fOut = 0;
	fMemory = 0;
	fRefNum = refNum;
	fIgnoreAlarm = false;
}

//____________________________________________________________
ApplState::~ApplState ()
{
	Reset ();
}

//____________________________________________________________
long ApplState::ID ()
{
	return kApplCnxID;
}

//____________________________________________________________
INetHandle ApplState::GetState ()
{
	if (fMemory) INetDisposeHandle (fMemory);
	long len = MemSize (fIn) + MemSize (fOut);
	fMemory = INetNewHandle (len);
	if (fMemory) {
		char * ptr = (char *)HandlePtr(fMemory);
		WriteCnx (fMemory, fIn);
		WriteCnx (fMemory, fOut);
		HandlePtr(fMemory) = ptr;
	}
	return fMemory;
}

//____________________________________________________________
void ApplState::SetState (INetHandle h)
{
	ClearCnxList (fIn);
	ClearCnxList (fOut);
	fIn = fOut = 0;
	char * ptr = (char *)HandlePtr(h);	// save handle pointer
	ReadCnx (h, &fIn);
	ReadCnx (h, &fOut);
	HandlePtr(h) = ptr;				// restore handle pointer
	RestoreState (fRefNum);
}

//____________________________________________________________
void ApplState::Alarm (short refnum, short srcRef, short changeCode)
{
	MidiName name;
	if (fIgnoreAlarm) return;
	switch (changeCode) {
		case MIDIOpenAppl:
			name = MidiGetName (srcRef);
			if (Find (name, fIn))  MidiConnect (srcRef, refnum, true);
			if (Find (name, fOut)) MidiConnect (refnum, srcRef, true);
			break;
		case MIDIChgConnect:
			ChangeCnx (refnum);
			break;
	}
}

//____________________________________________________________
void ApplState::AddCnx (short with, CnxDescPtr *dest)
{
	CnxDescPtr cnx = NewCnx (with);
	if (cnx) {
		cnx->next = *dest;
		*dest = cnx;
	}
}

//____________________________________________________________
void ApplState::RmvCnx (short with, CnxDescPtr *dest)
{
	CnxDescPtr cnx = *dest, prev = 0;
	while (cnx) {
		if (cnx->refnum == with) break;
		prev = cnx;
		cnx = cnx->next;
	}
	if (cnx) {
		if (prev) prev->next = cnx->next;
		else *dest = cnx->next;
		FreeCnx (cnx);
	}
}

//____________________________________________________________
void ApplState::ChangeCnx (short refnum)
{
	short n = MidiCountAppls ();
	for (short i=1; i<=n; i++) {
		short ref = MidiGetIndAppl (i);
		CnxDescPtr cnx = Find (ref, fOut);
		if (MidiIsConnected (refnum, ref)) {
			if (!cnx) AddCnx (ref, &fOut);
		}
		else if (cnx) RmvCnx (ref, &fOut);

		cnx = Find (ref, fIn);
		if (MidiIsConnected (ref, refnum)) {
			if (!cnx) AddCnx (ref, &fIn);
		}
		else if (cnx) RmvCnx (ref, &fIn);
	}
}

//____________________________________________________________
Boolean ApplState::Equal (MidiName name, MidiEvPtr evName)
{
	short len = *name++;
	if (MidiCountFields (evName) != len) return false;
	for (short i=0; i<len; i++) {
		 if (*name++ != MidiGetField (evName, i))
		 	return false;
	}
	return true;
}

//____________________________________________________________
CnxDescPtr ApplState::Find (MidiName name, CnxDescPtr cnx)
{
	while (cnx) {
		if (Equal (name, cnx->name)) return cnx;
		cnx = cnx->next;
	}
	return 0;
}

//____________________________________________________________
CnxDescPtr ApplState::Find (short ref, CnxDescPtr cnx)
{
	while (cnx) {
		if (cnx->refnum == ref) return cnx;
		cnx = cnx->next;
	}
	return 0;
}

//____________________________________________________________
void ApplState::RestoreState (short refNum)
{
	CnxDescPtr cnx;
	fIgnoreAlarm = true;
	cnx = fIn;
	while (cnx) {
		if (cnx->refnum >= 0) MidiConnect (cnx->refnum, refNum, true);
		cnx = cnx->next;
	}
	cnx = fOut;
	while (cnx) {
		if (cnx->refnum >= 0) MidiConnect (refNum, cnx->refnum, true);
		cnx = cnx->next;
	}
	fIgnoreAlarm = false;
}

//____________________________________________________________
void ApplState::ReadCnx (INetHandle h, CnxDescPtr *dst)
{
	short *sptr = (short *)HandlePtr(h);
	short n = *sptr++;
	char * ptr = (char *)sptr;
	while (n--) {
		CnxDescPtr cnx = NewCnx ((INetHandle)&ptr);
		if (cnx) {
			cnx->next = *dst;
			*dst = cnx;
		}
	}
	if ((long)ptr & 1) ptr++;
	HandlePtr(h) = ptr;
}

//____________________________________________________________
void ApplState::WriteCnx (INetHandle h, CnxDescPtr cnx)
{
	short *sptr = (short *)HandlePtr(h);
	*sptr++ = Count (cnx);
	MidiName ptr = (MidiName)sptr;
	while (cnx) {
		GetName (cnx, ptr);
		ptr += *ptr + 1;
		cnx = cnx->next;
	}
	if ((long)ptr & 1) ptr++;
	HandlePtr(h) = (char *)ptr;
}

//____________________________________________________________
void ApplState::GetName (CnxDescPtr cnx, MidiName name)
{
	short n = MidiCountFields (cnx->name);
	*name++ = n;
	for (short i=0; i<n; i++)
		*name++ = MidiGetField (cnx->name, i);
}

//____________________________________________________________
void ApplState::SetName (CnxDescPtr cnx, MidiName name)
{
	short n = *name++;
	while (n--) MidiAddField (cnx->name, *name++);
}

//____________________________________________________________
CnxDescPtr ApplState::NewCnx (INetHandle with)
{
	CnxDescPtr cnx = NewCnx();
	if (cnx) {
		MidiName ptr = (MidiName)HandlePtr(with);
		cnx->refnum = MidiGetNamedAppl (ptr);
		SetName (cnx, ptr);
		HandlePtr(with) = (char *)&ptr[*ptr + 1]; 
	}
	return cnx;
}

//____________________________________________________________
CnxDescPtr ApplState::NewCnx (short with)
{
	MidiName name = MidiGetName (with);
	if (!name) return 0;
	
	CnxDescPtr cnx = NewCnx();
	if (cnx) {
		cnx->refnum = with;
		SetName (cnx, name);
	}
	return cnx;
}

//____________________________________________________________
CnxDescPtr ApplState::NewCnx ()
{
	CnxDescPtr cnx = (CnxDescPtr)MidiNewCell();
	if (cnx) {
		cnx->next = 0;
		cnx->name = MidiNewEv (typeTextual);
		if (!cnx->name) {
			MidiFreeCell ((MidiEvPtr)cnx);
			cnx = 0;
		}
	}
	return cnx;
}

//____________________________________________________________
void ApplState::FreeCnx (CnxDescPtr cnx)
{
	MidiFreeEv (cnx->name);
	MidiFreeCell ((MidiEvPtr)cnx);
}

//____________________________________________________________
short ApplState::Count (CnxDescPtr cnx)
{
	short count = 0;
	while (cnx) {
		count++;
		cnx = cnx->next;
	}
	return count;
}

//____________________________________________________________
long ApplState::MemSize (CnxDescPtr cnx)
{
	long size = sizeof(short);
	while (cnx) {
		size += MidiCountFields (cnx->name) + 1;
		cnx = cnx->next;
	}
	return size & 1 ? size + 1 : size;
}

//____________________________________________________________
void ApplState::ClearCnxList (CnxDescPtr cnx)
{
	while (cnx) {
		CnxDescPtr next = cnx->next;
		FreeCnx (cnx);
		cnx = next;
	}
}

//____________________________________________________________
void ApplState::Reset ()
{
	if (fMemory) INetDisposeHandle (fMemory);
	fMemory = 0;
	ClearCnxList (fIn);
	ClearCnxList (fOut);
	fIn = fOut = 0;
}
