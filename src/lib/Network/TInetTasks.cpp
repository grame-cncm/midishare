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
		SocketStatus err = fPacket->Send (fSocket, (IPAddrPtr)a1);
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
Boolean  InetCtrlDTask::_Schedule (long date, void * a1,long type)
{
	fEndFlag = false;
	return Schedule (date, fRefNum, (long)a1, type);
}

//_______________________________________________________________________________
Boolean  InetCtrlDTask::_Schedule (void * a1,long type)
{
	fEndFlag = false;
	return Schedule (MidiGetTime(), fRefNum, (long)a1, type);
}

//_______________________________________________________________________________
Boolean  InetCtrlDTask::FatalError (ErrString msg, long err)
{
	if (!fFatalPending) {
		fFatalErrCode = err;
		fFatalPending = _Schedule (MidiGetTime(), msg, kFatalErrorTask);
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
