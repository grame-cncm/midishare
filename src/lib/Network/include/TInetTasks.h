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


#ifndef __TInetTasks__
#define __TInetTasks__

#include "InetTypes.h"
#include "MidiShareTasks.h"

enum { 	kRemoveRemoteTask=1, kCheckRemoteTask, 
		kCreateRemoteTask, kFatalErrorTask,
		kSendErrorTask, kOpenRemoteTask, 
		kCheckCompletion, kSilentCheckCompletion,
		kCnxRefusedTask };

class TUDPSocket;
class TPacket;
class TInetControler;
/*_______________________________________________________________________________*/
#ifdef macintosh
#include "OTTasks.h"
class TActiveSensing : public OTTask
#else
class TActiveSensing : public MidiShareTask
#endif
{
	public:
		enum { kDefaultRate = 100 };

				 TActiveSensing (TPacket * packet);
		virtual ~TActiveSensing () {}

		virtual void 	Initialize (TUDPSocket * sok, short rate = kDefaultRate);
		virtual void 	Run (long date, short refNum, long a1,long a2);

	protected:	
		short			fRate;
	private:	
		TUDPSocket *	fSocket;
		TPacket *		fPacket;
};

/*_______________________________________________________________________________*/
class TCPActiveSensing : public TActiveSensing
{
	public:
		enum { kStartDelay = 100, kContDelay = 200 };

				 TCPActiveSensing (TPacket * packet, IPNum ip);
		virtual ~TCPActiveSensing () {}

				void 	Start (short refNum);
				void 	ReSchedule (short refnum);

	private:	
		IPAddr		fDest;
};

/*_______________________________________________________________________________*/
class InetCtrlDTask : public MidiShareDTask
{
	public:
		enum { kDefaultRate = 500 };
		
				 InetCtrlDTask (TInetControler *ctrl, short checkRate = kDefaultRate);
		virtual ~InetCtrlDTask () {}

		virtual void 	Run	(long date, short refNum, long a1, long a2);
		virtual void 	Forget (void);

				bool _Schedule (void * a1, long type);
				bool _Schedule (long date, void * a1, long type);
				bool FatalError (const ErrString msg, long err);
				void 	SetRefNum (short refNum)	{ fRefNum = refNum; }
				TInetControler * Controler()		{ return fCtrl; }

	private:
		short	fCheckRate;
		short	fRefNum;
		long	fFatalErrCode;
		TInetControler * fCtrl;
		bool fEndFlag, fFatalPending;
};

#endif
