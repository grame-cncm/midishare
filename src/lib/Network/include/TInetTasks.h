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

				Boolean _Schedule (void * a1, long type);
				Boolean _Schedule (long date, void * a1, long type);
				Boolean FatalError (ErrString msg, long err);
				void 	SetRefNum (short refNum)	{ fRefNum = refNum; }
				TInetControler * Controler()		{ return fCtrl; }

	private:
		short	fCheckRate;
		short	fRefNum;
		long	fFatalErrCode;
		TInetControler * fCtrl;
		Boolean fEndFlag, fFatalPending;
};

#endif
