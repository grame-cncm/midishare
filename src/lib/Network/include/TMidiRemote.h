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


#ifndef __TMidiRemote__
#define __TMidiRemote__

#include "InetTypes.h"
#include "TInetTasks.h"
#include "MidiShareAppl.h"
#include "MidiShareTasks.h"
#include "TMidiOutStream.h"
#include "TMidiInStream.h"
#include "TRemote.h"
#include "TRemoteSlot.h"
#include "SkewControler.h"

class TUDPSocket;
class TTCPSocket;

typedef struct {
	UDPLinearizeMthTbl	outMethods;
	UDPParseMethodTbl	inMethods;
} StreamMethods, * StreamMethodsPtr;

typedef struct {
	IPNum 	ip;
	short	groupTime;
	short	latency;
	short	MTU;
	TUDPSocket * udp;
	IPNum 	specialIP;
	TSocket * special;
} UPDParams, * UPDParamsPtr;

typedef struct {
	StreamMethodsPtr mth;
	InetCtrlDTask *	ctrlTask;
	PeerTimes 		initTimes;
	TIDPacket *		idPacket;
	MidiEvPtr		name;
} RemoteCtrlParams, * RemoteCtrlParamsPtr;

//____________________________________________________________
#ifdef macintosh
#include "OTTasks.h"
class TGroupTask : public OTTask
#else
class TGroupTask : public MidiShareTask
#endif
{
	public:
		void 	Run (long date, short refNum, long a1,long a2);
};

class InetCtrlDTask;
//____________________________________________________________
class TMidiRemote : public TRemote
{
	public:
			enum { kSpecialPort = 255, kSpecialRefNum = 255 };
	
			 	 TMidiRemote (UPDParamsPtr params, RemoteCtrlParams *p, short refNum=0);
		virtual ~TMidiRemote ();
	
				bool Open ()			{ return Open (fName); }
				bool Opened ()		{ return fAppl ?  fAppl->Opened() : false; }
				void 	Close ();
				IPNum	GetID ()		{ return fIP; }
				short	GroupTime ()	{ return fGroupTime; }
				short	MaxLatency ()	{ return fUDPIn.MaxLatency(); }
				short	MaxLatencyOver(){ return fUDPIn.MaxLatencyOver(); }
				short	CurLatency ()	{ return fCurLatency; }
				short	MTU ()			{ return fUDPOut.MTU(); }
				short	InThroughput (short * overflow)	{ return fUDPIn.Throughput(overflow); }
				short	OutThroughput (short * overflow){ return fUDPOut.Throughput(overflow); }

				void	UDPSetup (short latency, short groupTime);
		unsigned long	Missing ()		{ return fUDPIn.Missed(); }

				short	OutSlotRef ()	{ return ((TRemoteSlot *)fAppl)->OutSlotRef(); }
				void 	SetRcvAlarm  	(bool state) { fAppl->SetRcvAlarm(state); }

				void 	Put (MidiEvPtr e);
				void 	CheckLatency  	(PeerTimesPtr times);
				void 	RcvAlarm  		(short refnum);
				void	CollectEvents 	(unsigned long date, short refNum);
				void	RcvPacket 		(long date, MidiPacketPtr p, bool fromtcp = false);

				void	SetListener  	(TRemoteListener * l)	{ fListener = l; }
				bool SetMode 	 	(short mode, short refNum);				
				MidiName GetName 		();
				short 	GetRefNum 		() { return fAppl ? fAppl->GetRefNum() : -1; }
				bool	CallRcvAlarm	() { return fRcvAlarmState && fPutEvs; }
				
	protected:
		short			fGroupTime;
		InetCtrlDTask *	fCtrlTask;

	private:
					bool Open (MidiName name);
					void SendError (SocketStatus err);

		SkewControler *	fSkew;
		PeerTimes		fTimeInit;
		long			fTimesOffset, fCurLatency, fLocalInit;
		MidiShareAppl *	fAppl;
		TMidiOutStream	fUDPOut, 	fSpecialOut;
		TMidiInStream	fUDPIn,		fTCPIn;
		IPNum			fIP;
		NameStr 		fName;
		
		TRemoteListener * fListener;
		TGroupTask		fGroupTask;
		bool			fRcvAlarmState, fPutEvs, fLocked;
};

#endif
