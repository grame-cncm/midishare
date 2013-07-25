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


#include "TInetTasks.h"
#include "TWANControler.h"
#include "StrConstants.h"
#include "INetAlert.h"

//_______________________________________________________________________________
// TActiveSensing	Task
//_______________________________________________________________________________
TActiveSensing::TActiveSensing (TPacket * packet)
{
	fRate = kDefaultRate;
	fSocket = 0;
	fPacket = packet;
}

//_______________________________________________________________________________
void TActiveSensing::Initialize (TUDPSocket * sok, short rate)
{
	fRate = rate;
	fSocket = sok;
}

//_______________________________________________________________________________
void TActiveSensing::Run (long date, short refNum, long a1, long a2)
{
	if (fSocket) {
		fPacket->Send (fSocket, (IPAddrPtr)a1);
		if (a2) {
			Schedule (date + 50, refNum, a1, a2-1);
		}
		else {
			Schedule (date + fRate, refNum, a1, a2);
		}
	}
}

//_______________________________________________________________________________
// TCPActiveSensing	Task
//_______________________________________________________________________________
TCPActiveSensing::TCPActiveSensing (TPacket * packet, IPNum ip)
	: TActiveSensing (packet)
{
	IPType(fDest) = AF_INET;
	IPField(fDest) = ip;
	fRate = kStartDelay;
}

//________________________________________________________________________
void TCPActiveSensing::ReSchedule (short refnum)
{
	Forget ();
	fRate = kContDelay;
	Run (MidiGetTime(), refnum, (long)&fDest, 0L);
}

//________________________________________________________________________
void TCPActiveSensing::Start (short refNum) 
{ 
	fRate = kStartDelay;
	Run (MidiGetTime(), refNum, (long)&fDest, 0L);
}

//_______________________________________________________________________________
// InetCtrlDTask			DTask
//_______________________________________________________________________________
InetCtrlDTask::InetCtrlDTask (TInetControler *ctrl, short rate)
{
	fCheckRate = rate;
	fRefNum = -1;
	fCtrl = ctrl;
	fEndFlag = fFatalPending = false;
}

//_______________________________________________________________________________
void InetCtrlDTask::Forget ()
{
	fEndFlag = true;
	MidiShareDTask::Forget ();
}

//_______________________________________________________________________________
bool  InetCtrlDTask::_Schedule (long date, void * a1,long type)
{
	fEndFlag = false;
	return Schedule (date, fRefNum, (long)a1, type);
}

//_______________________________________________________________________________
bool  InetCtrlDTask::_Schedule (void * a1,long type)
{
	fEndFlag = false;
#if defined(macintosh) && defined(MidiSharePPC_68k)
	return Schedule (MidiGetTime(), fRefNum, (long)a1, type);
#else
	Run (MidiGetTime(), fRefNum, (long)a1, type);
	return true;
#endif
}

//_______________________________________________________________________________
bool  InetCtrlDTask::FatalError (const ErrString msg, long err)
{
	if (!fFatalPending) {
		fFatalErrCode = err;
#if defined(macintosh) && defined(MidiSharePPC_68k)
		fFatalPending = _Schedule (MidiGetTime(), msg, kFatalErrorTask);
#else
		fFatalPending = true;
		Run (MidiGetTime(), fRefNum, (long)msg, kFatalErrorTask);
#endif
	}
	return fFatalPending;
}

//_______________________________________________________________________________
void InetCtrlDTask::Run (long date, short refNum, long a1, long type)
{
	if (fEndFlag) return;
	
	INetAlert alert;
	TInetControler * ctrl = Controler ();
	if (ctrl) {
		MidiEvPtr e = (MidiEvPtr)a1;
		TRemoteMgr * mgr = (TRemoteMgr *)a1;
		TMidiRemote * remote = (TMidiRemote *)a1;
		switch (type) {
		
			case kRemoveRemoteTask:
				ctrl->RemoveRemote (a1);
				break;
				
			case kCheckRemoteTask: 
				mgr->CheckTimeOut (date);
				if (mgr->RemoteCount()) {
					Schedule (date + fCheckRate, fRefNum, a1, type);
				}
				break;

			case kCreateRemoteTask:
				ctrl->CreateRemote (e);
				break; 
				
			case kFatalErrorTask:
				ctrl->FatalError ((strPtr)a1, fFatalErrCode);
				fFatalPending = false;
				break;
			
			case kSendErrorTask:
				ctrl->RemoteFatalError (Date(e), strSockSendFailure, Tempo(e));
				MidiFreeCell (e);
				break;
			
			case kOpenRemoteTask:
				if (!remote->Open ()) {
					ctrl->CreateRemoteFailed (strMSApplFail);
					ctrl->RemoveRemote (remote->GetID());
				}
				else ctrl->OpenComplete (remote->GetID());
				break;
				
			case kCheckCompletion:
				ctrl->CheckCompletion (a1, false);
				break;
				
			case kSilentCheckCompletion:
				ctrl->CheckCompletion (a1, true);
				break;
				
			case kCnxRefusedTask:
				ctrl->CnxRefused ((strPtr)a1);
				break;
		}
	}
}
