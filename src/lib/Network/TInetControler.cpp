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

#include <stdio.h>

#include "TimeTools.h"
#include "TInetControler.h"
#include "StrConstants.h"
#include "INetAlert.h"
#include "MidiTools.h"

extern Boolean doneFlag;
//____________________________________________________________
TInetControler::TInetControler (NetConfigPtr net, MidiName name, short mode) 
	: fNetInfos (net->port), fSocket (net->port)
{
	fDrvName = name;
	fState = kSleeping;
	fMode = mode;
	fNetConfig = *net;
	fActiveSensing = new TActiveSensing (&fIDPacket);
	fDTasks = new InetCtrlDTask (this);
	needWakeup = needSleep = false;
	fSocket.SetListener (this);
	if (!fNetInfos.Available())
		FatalError (strNetInfos, fNetInfos.Error());
	if (fMode == kDriverMode) {
		fAppl = new TInetDriver (this);
		Open ();
	}
	else fAppl = new MidiShareAppl ();
}

//____________________________________________________________
TInetControler::~TInetControler ()
{
	USleep ();
	delete fAppl;
}

//____________________________________________________________
void TInetControler::RemoveRemote (IPNum id)
{
	fRemoteMgr.Remove (id);
}

//____________________________________________________________
void TInetControler::SetMode (short mode)
{
	INetAlert alert;
	if (fMode == mode) return;
	
	MidiShareAppl * appl = fAppl;
	if (fDTasks) fDTasks->Forget ();
	if (mode == kDriverMode) {
		fAppl = new TInetDriver (this);
		Open ();
		if (!Sleeping())
			fRemoteMgr.SetMode (mode, GetRefNum());
	}
	else {
		fAppl = new MidiShareAppl ();
		if (!Sleeping()) {
			Open ();
			fRemoteMgr.SetMode (mode, 0);
		}
	}
	if (fUDPMode) {
		fActiveSensing->Forget ();
		fActiveSensing->Run (MidiGetTime(), GetRefNum(), (long)fNetInfos.BroadcastAddress(), 0);
	}
	delete appl;
	fMode = mode;
}

//____________________________________________________________
Boolean TInetControler::Open ()
{
	if (!fAppl || !fDTasks || !fActiveSensing) return false;
	if (!fAppl->Open (fDrvName)) {
		INetAlert alert;
		alert.FatalError (fDrvName, strWakeUpFailure, 
			(fMode == kDriverMode) ? strDrvRegFailure : strMSApplFail, 0L);
		return false;
	}
	fDTasks->SetRefNum(GetRefNum());
	return true;
}

//____________________________________________________________
void TInetControler::Close ()
{ 
	if (fActiveSensing) delete fActiveSensing;
	fActiveSensing = 0;
	if (fDTasks) delete fDTasks;
	fDTasks = 0;
	if (fAppl) fAppl->Close ();
}

//____________________________________________________________
void TInetControler::ExecDTasks ()
{
	long n = CountDTasks ();
	while (n-- && !doneFlag) {
		Exec1DTask ();
	}
}

//____________________________________________________________
void TInetControler::DoIdle ()
{
	if (fMode == kApplMode) {
		if (Sleeping()) {
			if (MidiCountAppls ()) UWakeup ();
		}
		else if ((MidiCountAppls() - fRemoteMgr.RemoteCount()) == 2) {
			USleep ();
		}
		else {
			ExecDTasks ();
		}
	}
	else {
		if (needWakeup) {
			UWakeup ();
			needWakeup = false;
		}
		else if (needSleep) {
			USleep ();
			needSleep = false;
		}
		else ExecDTasks ();
	}
}

//____________________________________________________________
void TInetControler::WakeUp (short r)	{ UWakeup (); }
void TInetControler::Sleep  (short r)	{ USleep(); }

//____________________________________________________________
Boolean TInetControler::UWakeup (Boolean udpMode)
{
	INetAlert alert;
	if (!Sleeping()) return true;
	fState = kWakeup;
	SocketStatus err = fSocket.Open (udpMode);
	if (err != noErr) {
		alert.FatalError (fDrvName, strWakeUpFailure, strSockOpenFailure, err);
		return false;
	}
	if ((fMode == kApplMode) && !Open ()) {
		return false;
	}
	if (udpMode) {
		if (!fActiveSensing) return false;
		fActiveSensing->Initialize (&fSocket);
		fActiveSensing->Run (MidiGetTime(), GetRefNum(), (long)fNetInfos.BroadcastAddress(), 5);
	}
	fUDPMode = udpMode;
	return true;
}

//____________________________________________________________
void TInetControler::Bye ()
{
	TByePacket bye;
	Boolean forget = MidiCountAppls() > 0;

	if (forget) {
		if (fActiveSensing) fActiveSensing->Forget ();
		if (fDTasks) fDTasks->Forget ();
		MidiFlushDTasks (GetRefNum());
	}
	else {
		if (fActiveSensing) fActiveSensing->Clear ();
		if (fDTasks) fDTasks->Clear ();
	}
	bye.Send (&fSocket, fNetInfos.BroadcastAddress());
}

//____________________________________________________________
void TInetControler::USleep ()
{
	if (!Sleeping()) {
		fState = kSleeping;
		Bye ();
		fSocket.Close ();
		fRemoteMgr.RemoveAll();
		Close ();
	}
}

//____________________________________________________________
void TInetControler::RcvAlarm (short refnum)
{
	if (fMode == kApplMode) MidiFlushEvs (refnum);
	else {
		short prevSlot  = -1;
		MidiEvPtr e = MidiGetEv (refnum);
		TMidiRemote * remote = 0;
		while (e) {
			
			if (Port(e) != prevSlot) {
				prevSlot = Port(e);
				remote = fRemoteMgr.FindOutSlot (Port(e));
			}
			if (remote) {
				remote->Put(e);
			}
			e = MidiGetEv (refnum);
		}
		fRemoteMgr.RcvAlarmLoop (refnum);
	}
}

//____________________________________________________________
void TInetControler::RcvPacket (IPAddrPtr from, GenericPacketPtr p, short len)
{
	if (Sleeping()) {
		return;
	}

	if (TPacket::Check (&p->header)) {	// check the packet header
		switch (p->header.type) {
			case kMidi:		DispatchMidi (PIPField(from), p, len);
				break;
			case kHostID:	RemoteID (from, (IDPacketPtr)p, len);
				break;
			case kBye:		CallRemoveRemote (PIPField(from));
				break;
			case kEcho:		Reply (from, p, len);
				break;
			case kEchoReply:
				break;
			default:
				MidiTools::Print ("unknown packet", GetRefNum());
				MidiTools::Print (p->header.type, GetRefNum());
		}
	}
	else {
		MidiTools::Print ("packet bad header", GetRefNum());
		MidiTools::Print (p->header.type, GetRefNum());
	}
}

//____________________________________________________________
void TInetControler::DispatchTCP (IPNum ip, GenericPacketPtr p, short len)
{
	unsigned long t = MidiGetTime();
	TMidiRemote * r = FindUDPRemote (ip);
	if (r) {
		r->RcvPacket (t, (MidiPacketPtr)p, true);
	}
}

//____________________________________________________________
void TInetControler::DispatchMidi (IPNum ip, GenericPacketPtr p, short len)
{
	unsigned long t = MidiGetTime();
	TMidiRemote * r = FindUDPRemote (ip);
	if (r) {
		r->RcvPacket (t, (MidiPacketPtr)p);
	}
}

//____________________________________________________________
Boolean TInetControler::CreateRemote (MidiEvPtr param)
{
	TMidiRemote * r; INetAlert alert;
	IPNum ip; PeerTimes times; Boolean ret = false;
	MidiEvPtr name = EvToRemote (param, &ip, &times);

	// check params and if remote exists
	if (!name || FindUDPRemote (ip)) return false;

	UPDParams parms; RemoteCtrlParams rp;
	parms.ip = ip;
	parms.groupTime = fNetConfig.groupTime;
	parms.latency = fNetConfig.maxlat;
	parms.MTU = LocalMTU();
	parms.udp = &fSocket;
	parms.specialIP = GetSpecialIP ();
	parms.special = GetSpecialSocket (ip);
	
	rp.ctrlTask = fDTasks;
	rp.initTimes = times;
	rp.idPacket = &fIDPacket;
	rp.name = name;

	r = fRemoteMgr.AddRemote (&parms, &rp, (fMode == kDriverMode) ? GetRefNum() : 0);
	if (r) {
		if (fRemoteMgr.RemoteCount() == 1) 	// first remote client
			CallCheckRemote (&fRemoteMgr);
		ret = true;
	}
	else CreateRemoteFailed (strMemFail);
	MidiFreeEv (name);
	return ret;
}

//____________________________________________________________
MidiEvPtr TInetControler::RemoteToEv (IPNum ip, char * name, PeerTimesPtr times)
{
	MidiEvPtr e = MidiNewEv (typePrivate);
	if (e) {
		MidiEvPtr ename = MidiTools::Text2Event (name);
		if (ename) {
			Date(e) = times->local;
			MidiSetField (e, 0, ip);
			MidiSetField (e, 1, (long)ename);
			MidiSetField (e, 2, times->remote);
		}
		else {
			MidiFreeEv (e);
			e = 0;
		}
	}
	return e;
}

//____________________________________________________________
MidiEvPtr TInetControler::EvToRemote (MidiEvPtr e, IPNum *ip, PeerTimesPtr times)
{
	if (!e) return 0;
	MidiEvPtr ename = (MidiEvPtr)MidiGetField (e, 1);
	times->local = Date(e);
	times->remote = MidiGetField (e, 2);
	*ip = MidiGetField (e, 0);
	MidiFreeEv (e);
	return ename;
}

//____________________________________________________________
void TInetControler::RemoteID (IPAddrPtr addr, IDPacketPtr id, short len)
{
	PeerTimes times;
	times.local = MidiGetTime();
	times.remote = ntohl(id->time);
	IPNum ip = PIPField(addr);
	
	if (!fRemoteMgr.Refresh (ip, &times)) {
		// cannot refresh an existing remote, needs to be created
		char * name = RemoteName (ip, id);
		if (!name) return;

		MidiEvPtr e = RemoteToEv (ip, name, &times);
		if (e) CallCreateRemote (e);
		else CallFatalError (strMSMemFail, 0);
	}
}

//____________________________________________________________
void TInetControler::Reply (IPAddrPtr to, GenericPacketPtr p, short len)
{
	p->header.type = kEchoReply;
	SocketStatus err = fSocket.Send (to, p, &len);
	if (err != noErr) SockError (err);
}

//____________________________________________________________
void TInetControler::RcvEvent  (UPDEvent ev, SockResult result, void * infos)
{
}

//____________________________________________________________
void TInetControler::SockError (SocketStatus err)
{
//	CallFatalError (strSockFailure, err);
	MidiTools::Print ("TIC SockError", GetRefNum());
	MidiTools::Print (err, GetRefNum());
}

//____________________________________________________________
void TInetControler::FatalError (ErrString msg, SocketStatus err)
{
	INetAlert alert;
	USleep ();
	alert.FatalError (fDrvName, strNetControl, msg, err);
}

//____________________________________________________________
void TInetControler::RemoteFatalError (IPNum id, ErrString msg, SocketStatus err)
{
	TMidiRemote * r = FindUDPRemote (id);
	if (r) {
		INetAlert alert;
		alert.Report (fDrvName, r->GetName(), msg, err);
		RemoveRemote (id);
	}
}
