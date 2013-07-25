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


#ifndef __StatesManager__
#define __StatesManager__

#include "InetTypes.h"
#include "MidiShare.h"
#include "Handle.h"

//____________________________________________________________
typedef struct CnxDesc CnxDesc, * CnxDescPtr;
struct CnxDesc {
	CnxDescPtr	next;
	short		refnum;
	MidiEvPtr 	name;
};

//____________________________________________________________
class RemoteState
{
	public:
			 	 RemoteState () {}
		virtual ~RemoteState () {}
	
		virtual INetHandle	GetState () = 0;
		virtual void 	SetState (INetHandle h) = 0;
		virtual void 	Alarm 	 (short refnum, short srcref, short changeCode) = 0;
		virtual long 	ID 	 	 () = 0;
};

//____________________________________________________________
class StatesManager
{
	public:
			 	 StatesManager () { fState = 0; }
		virtual ~StatesManager ();
	
		void SetState  (RemoteState * state)  { fState = state; }
		void SaveState (IPNum id);
		void LoadState (IPNum id);
				
	private:
		RemoteState * 	fState;
};

//____________________________________________________________
class ApplState : public RemoteState
{
	public:
			 	 ApplState (short refNum);
		virtual ~ApplState ();
	
		INetHandle	GetState ();
		void	SetState (INetHandle h);
		void 	Alarm 	 (short refnum, short srcref, short changeCode);
		long 	ID 	 	 ();
				
	private:
		void		ReadCnx (INetHandle h, CnxDescPtr *dest);
		void		WriteCnx (INetHandle h, CnxDescPtr cnx);

		void		RestoreState (short refNum);

		void		ChangeCnx (short refnum);

		void		GetName (CnxDescPtr cnx, MidiName name);
		void		SetName (CnxDescPtr cnx, MidiName name);
		CnxDescPtr	Find 	(MidiName name, CnxDescPtr cnx);
		CnxDescPtr	Find 	(short ref, CnxDescPtr cnx);
		
		bool		Equal 	(MidiName name, MidiEvPtr evName);
		void		AddCnx 	(short with, CnxDescPtr *dest);
		void		RmvCnx 	(short with, CnxDescPtr *dest);
		CnxDescPtr	NewCnx 	(short with);
		CnxDescPtr	NewCnx 	(INetHandle with);
		CnxDescPtr	NewCnx 	();
		void		FreeCnx (CnxDescPtr cnx);
		long		MemSize (CnxDescPtr list);
		short		Count 	(CnxDescPtr list);
		void		ClearCnxList (CnxDescPtr list);
		void		Reset ();

		bool		fIgnoreAlarm;
		INetHandle 	fMemory;
		CnxDescPtr	fIn, fOut;
		short		fRefNum;
};

class TRemoteSlot;
//____________________________________________________________
class SlotState : public RemoteState
{
	public:
			 	 SlotState (TRemoteSlot * slots);
		virtual ~SlotState ();
	
		INetHandle	GetState ();
		void 	SetState (INetHandle h);
		void 	Alarm 	 (short refnum, short srcref, short changeCode);
		long 	ID 	 	 ();

	private:
		void RestoreSlot  (SlotRefNum slot, char *cnx);

		INetHandle 		fMemory;
		TRemoteSlot * 	fSlots;
};

#endif
