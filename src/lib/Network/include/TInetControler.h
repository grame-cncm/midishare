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


#ifndef __TInetControler__
#define __TInetControler__

#include "INetAlert.h"
#include "MidiShare.h"
#include "InetTypes.h"
#include "StrConstants.h"
#include "TInetAddress.h"
#include "TInetTasks.h"
#include "MidiShareDriver.h"
#include "TNetInfos.h"
#include "TRemoteMgr.h"
#include "TUDPSocket.h"

enum { kLocalPort=4950, kWanPort=4951};

class TInetControler;
//_________________________________________________________________________________
class TInetDriver : public MidiShareDriver
{
	public:		
				 TInetDriver (TInetControler *ctrl) : MidiShareDriver(100) { fControler = ctrl;}
		virtual ~TInetDriver () {}

				void	RcvAlarm (short refnum);
				void	WakeUp (short r);
				void	Sleep  (short r);
	private:
		TInetControler * 	fControler;	
};

//_________________________________________________________________________________
class TInetControler : public TUDPListener
{
	friend class InetCtrlDTask;

	public:
		enum { kApplMode = 0, kDriverMode };

			 	 TInetControler (NetConfigPtr net, MidiName name, short mode=kApplMode);
		virtual ~TInetControler ();
	
		virtual void 	DoIdle ();
		virtual void 	Quit ()	{ USleep(); }
		virtual void	RemoveRemote  (IPNum id);
		virtual void	OpenComplete  (IPNum id) {}
		virtual void	CheckCompletion  (IPNum id, bool silently) {}
		virtual void	CnxRefused  (strPtr host) {}
	
				short 	RemoteCount ()		{ return fRemoteMgr.RemoteCount(); }
				void 	SetMode  (short mode);
				bool Sleeping ()			{ return fState == kSleeping; }
				void 	RcvAlarm (short refnum);
				NetConfig * Net ()			{ return &fNetConfig; }
				TUDPSocket * GetSocket ()	{ return &fSocket; }
				TIDPacket *  GetIDPacket ()	{ return &fIDPacket; }

				// TUDPListener methods
				void	RcvPacket (IPAddrPtr from, GenericPacketPtr p, short len);
				void	RcvEvent  (UPDEvent ev, SockResult result, void * infos);
				void	SockError (SocketStatus err);

				// midishare application methods
				bool Open ();
				void 	Close ();
				short 	GetRefNum ()	{ return fAppl ? fAppl->GetRefNum() : -1; }
				long	CountDTasks ()	{ return fAppl ? fAppl->CountDTasks () : 0; }
				void	Exec1DTask ()	{ fAppl->Exec1DTask (); }
				
				// driver specific methods
				void	WakeUp (short r);
				void	Sleep  (short r);

				void	CallFatalError 	 (const ErrString msg, long err);
				void	CreateRemoteFailed (const strPtr reason); 
				void	DispatchMidi(IPNum ip, GenericPacketPtr p, short len);
				void	DispatchTCP(IPNum ip, GenericPacketPtr p, short len);
				void	CallCnxRefused  (strPtr host); 

	
	protected:
		enum { 	kWakeup, kSleeping, kDefaultLatency = 10 };
		virtual bool UWakeup (bool udpMode = true); // deferred wake up
		virtual void 	USleep ();  					  // deferred sleep
		virtual void 	Bye ();
		virtual char *	RemoteName (IPNum ip, IDPacketPtr id)	{ return id->name; }
		virtual TSocket * GetSpecialSocket (IPNum ip)			{ return &fSocket; }
		virtual IPNum 	GetSpecialIP ()							{ return fNetInfos.BroadcastIP (); }
		virtual short	LocalMTU ()								{ return kMaxPacketBuff; }
		virtual void	CallCheckRemote  (TRemoteMgr *mgr); 
		virtual bool	CreateRemote	 (MidiEvPtr param);
		
		TMidiRemote * 	FindUDPRemote (IPNum ip)	{ return fRemoteMgr.FindRemote (ip); }
				void 	ExecDTasks ();
				void	CallRemoveRemote (IPNum ip); 
				void	CallCheckCompletion (IPNum ip, unsigned long timeout, bool silently); 

		TRemoteMgr		fRemoteMgr;
		NetConfig		fNetConfig;
		short			fMode;
		bool 		fState, fUDPMode;
		TNetInfos 		fNetInfos;
		strPtr			fDrvName;

		InetCtrlDTask * fDTasks;
	
	private:
		MidiEvPtr	RemoteToEv	 (IPNum ip, char * name, PeerTimesPtr times);
		MidiEvPtr	EvToRemote	 (MidiEvPtr ev, IPNum *ip, PeerTimesPtr times);
		void 	RemoteFatalError (IPNum id, const ErrString msg, SocketStatus err);				
		void 	FatalError 		 (const ErrString msg, SocketStatus err);				

		void	RemoteID 	(IPAddrPtr ip, IDPacketPtr id, short len);
		void	Reply 		(IPAddrPtr to, GenericPacketPtr p, short len);

		void	CallCreateRemote (MidiEvPtr param); 

		bool 		needWakeup, needSleep;
		MidiShareAppl *	fAppl;
		TIDPacket		fIDPacket;
		TUDPSocket		fSocket;

		TActiveSensing * fActiveSensing;
};

//_________________________________________________________________________________
inline void TInetControler::CreateRemoteFailed (const strPtr reason) 
				{ INetAlert alert; alert.Report (fDrvName, strRemCreateFailure, reason, 0L); }
				
inline void TInetControler::CallFatalError (const ErrString msg, long err) 
				{ fDTasks->FatalError (msg, err); }
inline void TInetControler::CallCreateRemote (MidiEvPtr param) 
				{ fDTasks->_Schedule (param, kCreateRemoteTask); }
inline void TInetControler::CallCheckRemote (TRemoteMgr *mgr) 
				{ fDTasks->_Schedule (mgr, kCheckRemoteTask); }
inline void TInetControler::CallCnxRefused (strPtr host) 
				{ fDTasks->_Schedule (host, kCnxRefusedTask); }
inline void TInetControler::CallRemoveRemote (IPNum ip) 
				{ fDTasks->_Schedule ((void *)ip, kRemoveRemoteTask); }
inline void TInetControler::CallCheckCompletion (IPNum ip, unsigned long timeout, bool silently) 
				{ fDTasks->_Schedule (MidiGetTime()+timeout, (void *)ip, 
				  silently ? kSilentCheckCompletion : kCheckCompletion); }

//_________________________________________________________________________________
inline void TInetDriver::WakeUp (short r)	{ fControler->WakeUp (r); }
inline void TInetDriver::Sleep  (short r)	{ fControler->Sleep (r); }
inline void	TInetDriver::RcvAlarm (short r)	{ fControler->RcvAlarm (r); }

#endif
