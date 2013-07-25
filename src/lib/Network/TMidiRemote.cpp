/*
  MidiShare Project
  Copyright (C) Grame 2001
  Copyright (C) Mil Productions 2001

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/


#include "MidiTools.h"
#include "TNetInfos.h"
#include "TMidiRemote.h"
#include "TRemoteAppl.h"

#include "TUDPSocket.h"
#include "TTCPSocket.h"

//____________________________________________________________
// TGroupTask
//____________________________________________________________
void TGroupTask::Run (long date, short refNum, long a1,long a2)
{
	TMidiRemote * remote = (TMidiRemote *)a1;
	remote->CollectEvents (date, refNum);
}

//____________________________________________________________
// TMidiRemote
//____________________________________________________________
TMidiRemote::TMidiRemote (UPDParamsPtr udp, RemoteCtrlParams * rp, short refNum)
	: 	fUDPOut(rp->mth->outMethods, udp->udp, udp->ip, udp->MTU),
		fSpecialOut(rp->mth->outMethods, udp->special, udp->specialIP, udp->MTU),
		fUDPIn(rp->mth->inMethods, &fTimeInit, udp->latency),
		fTCPIn(rp->mth->inMethods, &fTimeInit, udp->latency)
{
	TNetInfos inf (0);
	fListener	= 0;
	
	fTimeInit 	= rp->initTimes;
	fLocalInit  = fTimeInit.local;
	fTimesOffset= rp->initTimes.local - rp->initTimes.remote;
	fCtrlTask 	= rp->ctrlTask;
	MidiTools::Event2Text (rp->name, fName, kMaxHostNameLen);

	fIP 		= udp->ip;
	fGroupTime 	= udp->groupTime;

	fRcvAlarmState = true;
	fPutEvs = 0;
	fLocked = false;
	fCurLatency = 0;

	if (refNum) fAppl = new TRemoteSlot (fIP, refNum);
	else fAppl = new TRemoteAppl (this);

	if (GetID() != inf.LocalIP()) {
		AEFTMAParams ftma; SkewParams p;
		
		SkewControler::GetFTMAParams (udp->latency <= 10, &ftma);
		p.times	= &fTimeInit;
		p.ip		= udp->ip;
		p.socket	= udp->udp;
		p.packet	= rp->idPacket;
		fSkew = new SkewControler (&p, &ftma);
	}
	else {
		fSkew = 0;
		fLocked = true;
		fCtrlTask->_Schedule (MidiGetTime(), this, kOpenRemoteTask);
	}
}

//____________________________________________________________
TMidiRemote::~TMidiRemote ()
{
	Close ();
	if (fAppl) {
		delete fAppl;
		fAppl = 0;
	}
	if (fSkew) {
		delete fSkew;
		fSkew = 0;
	}
}

//____________________________________________________________
bool TMidiRemote::SetMode (short mode, short refNum)
{
	MidiShareAppl * appl = fAppl;
	fGroupTask.Forget ();
	if (mode) fAppl = new TRemoteSlot (fIP, refNum);
	else fAppl = new TRemoteAppl (this);
	delete appl;
	return Open ();
}				

//____________________________________________________________
bool TMidiRemote::Open (MidiName name)
{
	if (fAppl && fAppl->Open (name)) {
		return true;
	}
	return false;
}

//____________________________________________________________
void TMidiRemote::Close ()
{
	fGroupTask.Forget ();
	if (fAppl) fAppl->Close();
}

//____________________________________________________________
MidiName TMidiRemote::GetName ()
{
	return fAppl ? fAppl->GetName () : 0;
}

//____________________________________________________________
void TMidiRemote::Put (MidiEvPtr e)
{
	((TRemoteSlot *)fAppl)->Put(e);
	fPutEvs++;
}

//____________________________________________________________
void TMidiRemote::CheckLatency (PeerTimesPtr times)
{
	long offset = times->local - times->remote - fTimesOffset;
	if (fSkew && fSkew->Put (times->local, offset) && !fLocked) {
			fLocalInit = fTimeInit.local = times->local;
			fTimeInit.remote = times->remote;
//			fTimesOffset = times->local - times->remote;
			fCurLatency = 0;
		fLocked = true;
		fCtrlTask->_Schedule (this, kOpenRemoteTask);
	}
	else fCurLatency = offset - (fTimeInit.local - fLocalInit);
}

//____________________________________________________________
void TMidiRemote::RcvAlarm (short refnum)
{
	fRcvAlarmState = false;
	fAppl->SetRcvAlarm (false);		// switch to task mode
	fGroupTask.Schedule (MidiGetTime() + fGroupTime, refnum, (long)this, 0);
	if (fListener) fListener->SetRcvAlarm (refnum, false);
}

//____________________________________________________________
void TMidiRemote::UDPSetup (short latency, short groupTime)
{
	fUDPIn.SetLatency (latency);
	fGroupTime = groupTime;
}

//____________________________________________________________
void TMidiRemote::SendError (SocketStatus err)
{
	MidiEvPtr e = MidiNewCell();
	if (e) {
		Date(e) = GetID();
		Tempo(e) = err;
		if (!fCtrlTask->_Schedule (e, kSendErrorTask))
			MidiFreeCell (e);
	}
}

//____________________________________________________________
void TMidiRemote::CollectEvents (unsigned long date, short refnum)
{
	SocketStatus err = noErr; unsigned long packDate, offset;
	MidiEvPtr e = fAppl->GetEv ();
	if (!e) {						// not any event received
		fAppl->SetRcvAlarm (true);	// switch to receive alarm mode
		fPutEvs = 0;
		fRcvAlarmState = true;
		if (fListener) fListener->SetRcvAlarm (refnum, true);
		return;
	}
	
	packDate = date;
	offset = date - fGroupTime;
	fUDPOut.Set (packDate);
	fSpecialOut.Set (packDate);
	while (e && (err == noErr)) {
		Date(e) -= offset;
		if ((Port(e) == kSpecialPort) && (RefNum(e) != kSpecialRefNum)) {
			if (fSpecialOut.IP()) RefNum(e) = kSpecialRefNum;
			err = fSpecialOut.Put (e);
		}
		else 
			err = fUDPOut.Put (e);
		e = fAppl->GetEv ();
	}
	if (err == noErr)
		err = fUDPOut.Flush();
	if (err == noErr)
		err = fSpecialOut.Flush();
	if (err != noErr)
		SendError (err);
	else fGroupTask.Schedule (date + fGroupTime, refnum, (long)this, 0);
}

//____________________________________________________________
void TMidiRemote::RcvPacket (long date, MidiPacketPtr p, bool fromtcp)
{
	TMidiInStream * in = fromtcp ? &fTCPIn : &fUDPIn;
	if (fListener) {
		PeerTimes times;
		times.remote = ntohl(p->midi.date);
		if (times.remote) {
			times.local = date;
			CheckLatency (&times);
		}
	}
	if (in->Put (p, date)) {
		while (true) {
			MidiEvPtr e = in->GetEv();
			if (e) {
				fAppl->Send (e);
			}
			else break;
		}
	}
	else {
		// only for debug purpose
		in->Reset ();
		RcvPacket (date, p, fromtcp);
	}
}
